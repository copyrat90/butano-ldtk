// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_level_bgs_manager.h"

#include "ldtk_level_bgs_builder.cpp.h"
#include "ldtk_level_bgs_ptr.cpp.h"

#include "ldtk_gen_idents_fwd.h"
#include "ldtk_layer.h"
#include "ldtk_tile_grid_t.h"
#include "ldtk_tileset_definition.h"

#include <bn_assert.h>
#include <bn_bgs.h>
#include <bn_common.h>
#include <bn_config_bgs.h>
#include <bn_core.h>
#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_math.h>
#include <bn_point.h>
#include <bn_pool.h>
#include <bn_regular_bg_builder.h>
#include <bn_regular_bg_map_cell.h>
#include <bn_regular_bg_map_cell_info.h>
#include <bn_regular_bg_map_item.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>

#include "ldtk_div_utils.h"

#include <algorithm>
#include <cstddef>
#include <new>
#include <utility>

namespace ldtk::level_bgs_manager
{

namespace
{

struct bg_t
{
    static constexpr int COLUMNS = 32;
    static constexpr int ROWS = 32;

    const level& lv;
    const layer& layer_instance;
    const tile_grid_base& grid;

    alignas(int) bn::regular_bg_map_cell cells[ROWS * COLUMNS];
    bn::regular_bg_map_item map_item;
    bn::regular_bg_ptr bg_ptr;
    bn::regular_bg_map_ptr map_ptr;
    bool next_visible;
    bool grid_bloated;

    bg_t(const level& lv_, const layer& layer_, const bn::fixed_point& cam_applied_pos, const level_bgs_builder&);

    void update(const bn::fixed_point& next_cam_applied_pos, const bn::fixed_point& prev_cam_applied_pos);

private:
    void update_camera_applied_position(const bn::fixed_point& cam_applied_pos);

    void update_all_cells(const bn::fixed_point& cam_applied_pos);
    void update_part_cells(const bn::fixed_point& next_cam_applied_pos, const bn::fixed_point& prev_cam_applied_pos);

    void reset_all_cells(const bn::fixed_point& final_pos);
    void reset_part_cells(const bn::fixed_point& next_final_pos, const bn::fixed_point& prev_final_pos);

    void reset_rows(const int level_8x8_first_y, const int level_8x8_last_y, const int level_8x8_first_x,
                    const int level_8x8_last_x);
    void reset_columns(const int level_8x8_first_y, const int level_8x8_last_y, const int level_8x8_first_x,
                       const int level_8x8_last_x);

    auto init_bg_ptr(const layer& layer_, const bn::fixed_point& cam_applied_pos, const level_bgs_builder&)
        -> bn::regular_bg_ptr;

    auto apply_layer_diff(const bn::fixed_point& cam_applied_pos) const -> bn::fixed_point
    {
        return bn::fixed_point{
            (cam_applied_pos.x() + layer_instance.px_total_offset_x()) * (1 - layer_instance.def().parallax_factor_x()),
            (cam_applied_pos.y() + layer_instance.px_total_offset_y()) * (1 - layer_instance.def().parallax_factor_y()),
        };
    }
};

struct lv_t
{
    unsigned usages = 1;

    const level* lv;
    bn::fixed_point prev_cam_applied_pos;
    bn::fixed_point cur_raw_pos; // camera not applied
    bn::optional<bn::camera_ptr> cam;
    bn::vector<bg_t*, BN_CFG_BGS_MAX_ITEMS> bgs;

    lv_t(level_bgs_builder&& builder);
    ~lv_t();

    void reset_bgs();
    void set_level(const level& level_);
    void set_level(level_bgs_builder&& builder);

    auto get_bg(gen::layer_ident layer_identifier) -> bg_t&;
    auto get_bg(gen::layer_ident layer_identifier) const -> const bg_t&;
    auto get_bg_nullable(gen::layer_ident layer_identifier) -> bg_t*;
    auto get_bg_nullable(gen::layer_ident layer_identifier) const -> const bg_t*;
};

struct static_data
{
    bn::core::update_callback_type previous_callback;

    bn::pool<bg_t, BN_CFG_BGS_MAX_ITEMS> bgs_pool;
    bn::pool<lv_t, BN_CFG_BGS_MAX_ITEMS> levels_pool;
    bn::vector<lv_t*, BN_CFG_BGS_MAX_ITEMS> levels_vector;

    static_data(bn::core::update_callback_type prev_callback) : previous_callback(prev_callback)
    {
    }
};

alignas(static_data) BN_DATA_EWRAM_BSS std::byte data_buffer[sizeof(static_data)];

auto data_ref() -> static_data&
{
    return *std::launder(reinterpret_cast<static_data*>(data_buffer));
}

void update_callback()
{
    if (auto previous_callback = data_ref().previous_callback)
        previous_callback();

    static_data& data = data_ref();

    for (auto* level : data.levels_vector)
    {
        const bn::fixed_point next_cam_applied_pos =
            level->cur_raw_pos - (level->cam ? level->cam->position() : bn::fixed_point(0, 0));

        for (auto* bg : level->bgs)
        {
            bg->update(next_cam_applied_pos, level->prev_cam_applied_pos);
        }

        level->prev_cam_applied_pos = next_cam_applied_pos;
    }
}

bg_t::bg_t(const level& lv_, const layer& layer_, const bn::fixed_point& cam_applied_pos,
           const level_bgs_builder& builder)
    : lv(lv_), layer_instance(layer_),
      grid(layer_.auto_layer_tiles() ? *layer_.auto_layer_tiles() : *layer_.grid_tiles()),
      map_item(cells[0], bn::size(COLUMNS, ROWS)), bg_ptr(init_bg_ptr(layer_, cam_applied_pos, builder)),
      map_ptr(bg_ptr.map()), next_visible(bg_ptr.visible()), grid_bloated(grid.bloated())
{
}

void bg_t::update(const bn::fixed_point& next_cam_applied_pos, const bn::fixed_point& prev_cam_applied_pos)
{
    if (next_visible)
    {
        if (!bg_ptr.visible())
        {
            update_all_cells(next_cam_applied_pos);
        }
        // Update cells when level position changed
        else if (next_cam_applied_pos != prev_cam_applied_pos)
        {
            // Only update cells that needs to be changed
            update_part_cells(next_cam_applied_pos, prev_cam_applied_pos);
        }
    }

    update_camera_applied_position(next_cam_applied_pos);

    bg_ptr.set_visible(next_visible);
}

void bg_t::update_camera_applied_position(const bn::fixed_point& cam_applied_pos)
{
    static constexpr bn::fixed_point CANVAS_SIZE(COLUMNS * 8, ROWS * 8);
    static constexpr bn::fixed_point HALF_CANVAS_SIZE(CANVAS_SIZE / 2);
    const bn::fixed_point level_size(lv.px_width(), lv.px_height());
    const bn::fixed_point half_level_size(level_size / 2);

    // Everything is top-left coordinate, (0, 0) being top-left of the level
    bg_ptr.set_position(apply_layer_diff(cam_applied_pos) - half_level_size + HALF_CANVAS_SIZE);
}

void bg_t::update_all_cells(const bn::fixed_point& cam_applied_pos)
{
    reset_all_cells(apply_layer_diff(cam_applied_pos));
    map_ptr.reload_cells_ref();
}

void bg_t::update_part_cells(const bn::fixed_point& next_cam_applied_pos, const bn::fixed_point& prev_cam_applied_pos)
{
    reset_part_cells(apply_layer_diff(next_cam_applied_pos), apply_layer_diff(prev_cam_applied_pos));
    map_ptr.reload_cells_ref();
}

void bg_t::reset_all_cells(const bn::fixed_point& final_pos)
{
    // Everything is top-left coordinate, (0, 0) being top-left of the level
    static constexpr bn::point SCREEN_CELLS(bn::display::width() / 8, bn::display::height() / 8);
    static constexpr bn::fixed_point HALF_SCREEN_SIZE(bn::display::width() / 2, bn::display::height() / 2);
    const bn::fixed_point half_level_size(lv.px_width() / 2, lv.px_height() / 2);

    const bn::fixed_point screen_top_left = -final_pos - HALF_SCREEN_SIZE + half_level_size;

    const bn::point level_8x8_first((screen_top_left.x() / 8).floor_integer(),
                                    (screen_top_left.y() / 8).floor_integer());
    const bn::point level_8x8_last(level_8x8_first + SCREEN_CELLS);

    reset_rows(level_8x8_first.y(), level_8x8_last.y(), level_8x8_first.x(), level_8x8_last.x());
}

void bg_t::reset_part_cells(const bn::fixed_point& next_final_pos, const bn::fixed_point& prev_final_pos)
{
    // Everything is top-left coordinate, (0, 0) being top-left of the level
    static constexpr bn::point SCREEN_CELLS(bn::display::width() / 8, bn::display::height() / 8);
    static constexpr bn::fixed_point HALF_SCREEN_SIZE(bn::display::width() / 2, bn::display::height() / 2);
    const bn::fixed_point half_level_size(lv.px_width() / 2, lv.px_height() / 2);

    const bn::fixed_point next_screen_top_left = -next_final_pos - HALF_SCREEN_SIZE + half_level_size;
    const bn::fixed_point prev_screen_top_left = -prev_final_pos - HALF_SCREEN_SIZE + half_level_size;

    const bn::point level_8x8_next_top_left((next_screen_top_left.x() / 8).floor_integer(),
                                            (next_screen_top_left.y() / 8).floor_integer());
    const bn::point level_8x8_next_bottom_right(level_8x8_next_top_left + SCREEN_CELLS);
    const bn::point level_8x8_prev_top_left((prev_screen_top_left.x() / 8).floor_integer(),
                                            (prev_screen_top_left.y() / 8).floor_integer());
    const bn::point level_8x8_prev_bottom_right(level_8x8_prev_top_left + SCREEN_CELLS);

    const int up_diff = -level_8x8_next_top_left.y() + level_8x8_prev_top_left.y();
    const int down_diff = -up_diff;
    const int left_diff = -level_8x8_next_top_left.x() + level_8x8_prev_top_left.x();
    const int right_diff = -left_diff;

    // If `diff` is over the screen size, you can actually overwrite the same cell twice.
    // If we're moving left or up, the last overwritting one will be the wrong source tile.
    //
    // I'm doing a cheap fix to just full reload for that case.
    if (bn::abs(up_diff) >= SCREEN_CELLS.y() || bn::abs(left_diff) >= SCREEN_CELLS.x())
    {
        reset_all_cells(next_final_pos);
    }
    else
    {
        if (up_diff > 0)
            reset_rows(level_8x8_next_top_left.y(), level_8x8_next_top_left.y() + (up_diff - 1),
                       level_8x8_next_top_left.x(), level_8x8_next_bottom_right.x());
        else if (down_diff > 0)
            reset_rows(level_8x8_next_bottom_right.y() - (down_diff - 1), level_8x8_next_bottom_right.y(),
                       level_8x8_next_top_left.x(), level_8x8_next_bottom_right.x());

        if (left_diff > 0)
            reset_columns(level_8x8_next_top_left.y() + (up_diff > 0 ? up_diff : 0),
                          level_8x8_next_bottom_right.y() - (down_diff > 0 ? down_diff : 0),
                          level_8x8_next_top_left.x(), level_8x8_next_top_left.x() + (left_diff - 1));
        else if (right_diff > 0)
            reset_columns(level_8x8_next_top_left.y() + (up_diff > 0 ? up_diff : 0),
                          level_8x8_next_bottom_right.y() - (down_diff > 0 ? down_diff : 0),
                          level_8x8_next_bottom_right.x() - (right_diff - 1), level_8x8_next_bottom_right.x());
    }
}

void bg_t::reset_rows(const int level_8x8_first_y, const int level_8x8_last_y, const int level_8x8_first_x,
                      const int level_8x8_last_x)
{
    const int m_tile_cnt = layer_instance.grid_size() >> 3;
    const int m_tile_cnt_squared = m_tile_cnt * m_tile_cnt;

    // Optimization: accumulate divmod instead of recalculate every time
    int my = py_div(level_8x8_first_y, m_tile_cnt);
    int my_rnd_cnt = py_mod(level_8x8_first_y, m_tile_cnt);

    int cy = py_mod(level_8x8_first_y, ROWS);

    int mx_init = py_div(level_8x8_first_x, m_tile_cnt);
    int mx_rnd_cnt_init = py_mod(level_8x8_first_x, m_tile_cnt);

    int cx_init = py_mod(level_8x8_first_x, COLUMNS);

    for (int ly = level_8x8_first_y; ly <= level_8x8_last_y; ++ly)
    {
        int mx = mx_init;
        int mx_rnd_cnt = mx_rnd_cnt_init;
        int cx = cx_init;

        for (int lx = level_8x8_first_x; lx <= level_8x8_last_x; ++lx)
        {
            tile_grid_base::tile_info m_tile_info;

            if (mx < 0 || mx >= grid.c_width() || my < 0 || my >= grid.c_height())
            {
                m_tile_info.index = 0;
                m_tile_info.x_flip = false;
                m_tile_info.y_flip = false;
            }
            else
            {
                // Optimization: no virtual function call
                m_tile_info = grid_bloated
                                  ? static_cast<const tile_grid_t<true>&>(grid).cell_tile_info_no_virtual(mx, my)
                                  : static_cast<const tile_grid_t<false>&>(grid).cell_tile_info_no_virtual(mx, my);
            }

            const int tx = m_tile_info.x_flip ? m_tile_cnt - 1 - mx_rnd_cnt : mx_rnd_cnt;
            const int ty = m_tile_info.y_flip ? m_tile_cnt - 1 - my_rnd_cnt : my_rnd_cnt;
            const bn::regular_bg_map_cell raw_cell =
                layer_instance.tileset_def()
                    ->bg_item()
                    .map_item()
                    .cells_ptr()[(m_tile_info.index * m_tile_cnt_squared) + (ty * m_tile_cnt) + tx];

            bn::regular_bg_map_cell_info cell_info(raw_cell);
            if (m_tile_info.x_flip)
                cell_info.set_horizontal_flip(!cell_info.horizontal_flip());
            if (m_tile_info.y_flip)
                cell_info.set_vertical_flip(!cell_info.vertical_flip());

            cells[map_item.cell_index(cx, cy)] = cell_info.cell();

            if (++mx_rnd_cnt == m_tile_cnt)
            {
                ++mx;
                mx_rnd_cnt = 0;
            }
            if (++cx == COLUMNS)
            {
                cx = 0;
            }
        }

        if (++my_rnd_cnt == m_tile_cnt)
        {
            ++my;
            my_rnd_cnt = 0;
        }
        if (++cy == ROWS)
        {
            cy = 0;
        }
    }
}

void bg_t::reset_columns(const int level_8x8_first_y, const int level_8x8_last_y, const int level_8x8_first_x,
                         const int level_8x8_last_x)
{
    const int m_tile_cnt = layer_instance.grid_size() >> 3;
    const int m_tile_cnt_squared = m_tile_cnt * m_tile_cnt;

    // Optimization: accumulate divmod instead of recalculate every time
    int mx = py_div(level_8x8_first_x, m_tile_cnt);
    int mx_rnd_cnt = py_mod(level_8x8_first_x, m_tile_cnt);

    int cx = py_mod(level_8x8_first_x, COLUMNS);

    int my_init = py_div(level_8x8_first_y, m_tile_cnt);
    int my_rnd_cnt_init = py_mod(level_8x8_first_y, m_tile_cnt);

    int cy_init = py_mod(level_8x8_first_y, ROWS);

    for (int lx = level_8x8_first_x; lx <= level_8x8_last_x; ++lx)
    {
        int my = my_init;
        int my_rnd_cnt = my_rnd_cnt_init;
        int cy = cy_init;

        for (int ly = level_8x8_first_y; ly <= level_8x8_last_y; ++ly)
        {
            tile_grid_base::tile_info m_tile_info;

            if (mx < 0 || mx >= grid.c_width() || my < 0 || my >= grid.c_height())
            {
                m_tile_info.index = 0;
                m_tile_info.x_flip = false;
                m_tile_info.y_flip = false;
            }
            else
            {
                // Optimization: no virtual function call
                m_tile_info = grid_bloated
                                  ? static_cast<const tile_grid_t<true>&>(grid).cell_tile_info_no_virtual(mx, my)
                                  : static_cast<const tile_grid_t<false>&>(grid).cell_tile_info_no_virtual(mx, my);
            }

            const int tx = m_tile_info.x_flip ? m_tile_cnt - 1 - mx_rnd_cnt : mx_rnd_cnt;
            const int ty = m_tile_info.y_flip ? m_tile_cnt - 1 - my_rnd_cnt : my_rnd_cnt;
            const bn::regular_bg_map_cell raw_cell =
                layer_instance.tileset_def()
                    ->bg_item()
                    .map_item()
                    .cells_ptr()[(m_tile_info.index * m_tile_cnt_squared) + (ty * m_tile_cnt) + tx];

            bn::regular_bg_map_cell_info cell_info(raw_cell);
            if (m_tile_info.x_flip)
                cell_info.set_horizontal_flip(!cell_info.horizontal_flip());
            if (m_tile_info.y_flip)
                cell_info.set_vertical_flip(!cell_info.vertical_flip());

            cells[map_item.cell_index(cx, cy)] = cell_info.cell();

            if (++my_rnd_cnt == m_tile_cnt)
            {
                ++my;
                my_rnd_cnt = 0;
            }
            if (++cy == ROWS)
            {
                cy = 0;
            }
        }

        if (++mx_rnd_cnt == m_tile_cnt)
        {
            ++mx;
            mx_rnd_cnt = 0;
        }
        if (++cx == COLUMNS)
        {
            cx = 0;
        }
    }
}

auto bg_t::init_bg_ptr(const layer& layer_, const bn::fixed_point& cam_applied_pos, const level_bgs_builder& lv_builder)
    -> bn::regular_bg_ptr
{
    BN_BASIC_ASSERT(layer_.tileset_def());

    const bn::fixed_point final_pos = apply_layer_diff(cam_applied_pos);

    // Initialize the cells first, before creating bg
    reset_all_cells(final_pos);

    bn::regular_bg_item bg_item(layer_.tileset_def()->bg_item().tiles_item(),
                                layer_.tileset_def()->bg_item().palette_item(), map_item);
    bn::regular_bg_builder builder(bg_item);

    // Apply initial bg settings
    builder.set_position(final_pos);
    builder.set_priority(lv_builder.priority(layer_.identifier()));
    builder.set_z_order(lv_builder.z_order(layer_.identifier()));
    builder.set_mosaic_enabled(lv_builder.mosaic_enabled(layer_.identifier()));
    builder.set_blending_top_enabled(lv_builder.blending_top_enabled(layer_.identifier()));
    builder.set_blending_bottom_enabled(lv_builder.blending_bottom_enabled(layer_.identifier()));
    builder.set_green_swap_mode(lv_builder.green_swap_mode(layer_.identifier()));
    builder.set_visible(lv_builder.visible(layer_.identifier()));

    return builder.release_build();
}

lv_t::lv_t(level_bgs_builder&& builder)
{
    set_level(std::move(builder));
}

lv_t::~lv_t()
{
    reset_bgs();
}

void lv_t::reset_bgs()
{
    for (bg_t* bg : bgs)
        data_ref().bgs_pool.destroy(*bg);

    bgs.clear();
}

void lv_t::set_level(const level& level_)
{
    level_bgs_builder builder(level_);
    builder.set_position(cur_raw_pos);
    builder.set_camera(cam);

    set_level(std::move(builder));
}

void lv_t::set_level(level_bgs_builder&& builder)
{
    reset_bgs();

    lv = &builder.level();
    prev_cam_applied_pos =
        builder.position() - (builder.camera() ? builder.camera()->position() : bn::fixed_point(0, 0));
    cur_raw_pos = builder.position();
    cam = builder.release_camera();

    const auto& layer_instances = builder.level().layer_instances();

    // BG generation order: Bottom -> Top
    for (auto iter = layer_instances.rbegin(); iter != layer_instances.rend(); ++iter)
    {
        auto& layer = *iter;

        if (layer.auto_layer_tiles() || layer.grid_tiles())
        {
            BN_BASIC_ASSERT(!data_ref().bgs_pool.full(), "No more BG items available");

            bgs.push_back(&data_ref().bgs_pool.create(*lv, layer, prev_cam_applied_pos, builder));
        }
    }
}

auto lv_t::get_bg(gen::layer_ident layer_identifier) -> bg_t&
{
    bg_t* bg = get_bg_nullable(layer_identifier);
    BN_ASSERT(bg, "Layer of (gen::layer_ident)", (int)layer_identifier,
              " doesn't have a background associated with it");

    return *bg;
}

auto lv_t::get_bg(gen::layer_ident layer_identifier) const -> const bg_t&
{
    const bg_t* bg = get_bg_nullable(layer_identifier);
    BN_ASSERT(bg, "Layer of (gen::layer_ident)", (int)layer_identifier,
              " doesn't have a background associated with it");

    return *bg;
}

auto lv_t::get_bg_nullable(gen::layer_ident layer_identifier) -> bg_t*
{
    BN_ASSERT((int)layer_identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)layer_identifier);
    BN_ASSERT((int)layer_identifier < lv->layer_instances().size(), "Out of bound identifier (gen::layer_ident)",
              (int)layer_identifier);

    for (bg_t* bg : bgs)
    {
        if (bg->layer_instance.identifier() == layer_identifier)
            return bg;
    }

    return nullptr;
}

auto lv_t::get_bg_nullable(gen::layer_ident layer_identifier) const -> const bg_t*
{
    BN_ASSERT((int)layer_identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)layer_identifier);
    BN_ASSERT((int)layer_identifier < lv->layer_instances().size(), "Out of bound identifier (gen::layer_ident)",
              (int)layer_identifier);

    for (const bg_t* bg : bgs)
    {
        if (bg->layer_instance.identifier() == layer_identifier)
            return bg;
    }

    return nullptr;
}

} // namespace

void init()
{
    ::new (static_cast<void*>(data_buffer)) static_data(bn::core::update_callback());

    bn::core::set_update_callback(update_callback);
}

auto create(level_bgs_builder&& builder) -> id_t
{
    static_data& data = data_ref();
    BN_BASIC_ASSERT(!data.levels_vector.full(), "No more level items available");

    lv_t& lv = data.levels_pool.create(std::move(builder));
    data.levels_vector.push_back(&lv);

    return &lv;
}

auto create_optional(level_bgs_builder&& builder) -> id_t
{
    static_data& data = data_ref();
    if (data.levels_vector.full())
        return nullptr;

    // Check BG count (required <= remaining)
    int bg_count = 0;
    for (const auto& layer : builder.level().layer_instances())
        bg_count += (layer.auto_layer_tiles() || layer.grid_tiles());
    if (bn::bgs::available_items_count() + bg_count > BN_CFG_BGS_MAX_ITEMS)
        return nullptr;

    lv_t& lv = data.levels_pool.create(std::move(builder));
    data.levels_vector.push_back(&lv);

    return &lv;
}

void increase_usages(id_t id)
{
    auto lv = static_cast<lv_t*>(id);
    ++lv->usages;
}

void decrease_usages(id_t id)
{
    auto lv = static_cast<lv_t*>(id);
    --lv->usages;

    if (!lv->usages) [[likely]]
    {
        static_data& data = data_ref();

        bn::erase(data.levels_vector, lv);
        data.levels_pool.destroy(*lv);
    }
}

void set_level(id_t id, const level& level)
{
    auto lv = static_cast<lv_t*>(id);
    lv->set_level(level);
}

void set_level(id_t id, level_bgs_builder&& builder)
{
    auto lv = static_cast<lv_t*>(id);
    lv->set_level(std::move(builder));
}

auto has_background(id_t id, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg_nullable(layer_identifier) != nullptr;
}

auto dimensions(id_t id) -> const bn::size&
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->lv->px_size();
}

auto position(id_t id) -> const bn::fixed_point&
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->cur_raw_pos;
}

void set_x(id_t id, bn::fixed x)
{
    auto lv = static_cast<lv_t*>(id);
    lv->cur_raw_pos.set_x(x);
}

void set_y(id_t id, bn::fixed y)
{
    auto lv = static_cast<lv_t*>(id);
    lv->cur_raw_pos.set_y(y);
}

void set_position(id_t id, const bn::fixed_point& position)
{
    auto lv = static_cast<lv_t*>(id);
    lv->cur_raw_pos = position;
}

auto priority(id_t id, gen::layer_ident layer_identifier) -> int
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.priority();
}

void set_priority(id_t id, int priority)
{
    BN_ASSERT(priority >= bn::bgs::min_priority() && priority <= bn::bgs::max_priority(),
              "Invalid priority: ", priority);

    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_priority(priority);
}

void set_priority(id_t id, int priority, gen::layer_ident layer_identifier)
{
    BN_ASSERT(priority >= bn::bgs::min_priority() && priority <= bn::bgs::max_priority(),
              "Invalid priority: ", priority);

    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_priority(priority);
}

auto z_order(id_t id, gen::layer_ident layer_identifier) -> int
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.z_order();
}

void set_z_order(id_t id, int z_order)
{
    BN_ASSERT(z_order >= bn::bgs::min_z_order() && z_order <= bn::bgs::max_z_order(), "Invalid z order: ", z_order);

    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_z_order(z_order);
}

void set_z_order(id_t id, int z_order, gen::layer_ident layer_identifier)
{
    BN_ASSERT(z_order >= bn::bgs::min_z_order() && z_order <= bn::bgs::max_z_order(), "Invalid z order: ", z_order);

    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_z_order(z_order);
}

void put_above(id_t id)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.put_above();
}

void put_above(id_t id, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.put_above();
}

void put_below(id_t id)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.put_below();
}

void put_below(id_t id, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.put_below();
}

auto mosaic_enabled(id_t id, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.mosaic_enabled();
}

void set_mosaic_enabled(id_t id, bool mosaic_enabled)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_mosaic_enabled(mosaic_enabled);
}

void set_mosaic_enabled(id_t id, bool mosaic_enabled, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_mosaic_enabled(mosaic_enabled);
}

auto green_swap_mode(id_t id, gen::layer_ident layer_identifier) -> bn::green_swap_mode
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.green_swap_mode();
}

void set_green_swap_mode(id_t id, bn::green_swap_mode green_swap_mode)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_green_swap_mode(green_swap_mode);
}

void set_green_swap_mode(id_t id, bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_green_swap_mode(green_swap_mode);
}

auto blending_top_enabled(id_t id, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.blending_top_enabled();
}

void set_blending_top_enabled(id_t id, bool blending_top_enabled)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_blending_top_enabled(blending_top_enabled);
}

void set_blending_top_enabled(id_t id, bool blending_top_enabled, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_blending_top_enabled(blending_top_enabled);
}

auto blending_bottom_enabled(id_t id, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.blending_bottom_enabled();
}

void set_blending_bottom_enabled(id_t id, bool blending_bottom_enabled)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_blending_bottom_enabled(blending_bottom_enabled);
}

void set_blending_bottom_enabled(id_t id, bool blending_bottom_enabled, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_blending_bottom_enabled(blending_bottom_enabled);
}

auto visible(id_t id, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).next_visible;
}

void set_visible(id_t id, bool visible)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->next_visible = visible;
}

void set_visible(id_t id, bool visible, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).next_visible = visible;
}

auto visible_in_window(id_t id, const bn::window& window, gen::layer_ident layer_identifier) -> bool
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->get_bg(layer_identifier).bg_ptr.visible_in_window(window);
}

void set_visible_in_window(id_t id, bool visible, bn::window& window)
{
    auto lv = static_cast<lv_t*>(id);
    for (auto* bg : lv->bgs)
        bg->bg_ptr.set_visible_in_window(visible, window);
}

void set_visible_in_window(id_t id, bool visible, bn::window& window, gen::layer_ident layer_identifier)
{
    auto lv = static_cast<lv_t*>(id);
    lv->get_bg(layer_identifier).bg_ptr.set_visible_in_window(visible, window);
}

auto camera(id_t id) -> const bn::optional<bn::camera_ptr>&
{
    auto lv = static_cast<const lv_t*>(id);
    return lv->cam;
}

void set_camera(id_t id, bn::camera_ptr&& camera)
{
    auto lv = static_cast<lv_t*>(id);
    lv->cam = std::move(camera);
}

void remove_camera(id_t id)
{
    auto lv = static_cast<lv_t*>(id);
    lv->cam.reset();
}

} // namespace ldtk::level_bgs_manager
