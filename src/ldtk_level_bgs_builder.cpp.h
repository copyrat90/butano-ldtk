// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_level_bgs_builder.h"

#include "ldtk_level.h"

#include "bn_top_left_utils.h"

#include <bn_assert.h>
#include <bn_bgs.h>

namespace ldtk
{

/// @brief Constructor.
/// @param level `level` containing the required information to generate the level backgrounds.
level_bgs_builder::level_bgs_builder(const ldtk::level& level) : _level(level)
{
    const auto& layer_instances = level.layer_instances();

    // BG generation order: Bottom -> Top
    for (auto iter = layer_instances.rbegin(); iter != layer_instances.rend(); ++iter)
    {
        auto& layer = *iter;

        if (layer.auto_layer_tiles() || layer.grid_tiles())
        {
            _bgs_attrs.emplace_back(layer, layer.visible(), layer.opacity() != bn::fixed(1));
        }
    }
}

auto level_bgs_builder::has_background(gen::layer_ident layer_identifier) const -> bool
{
    return bg_attr_nullable(layer_identifier) != nullptr;
}

auto level_bgs_builder::top_left_x() const -> bn::fixed
{
    return bn::to_top_left_x(x(), dimensions().width());
}

auto level_bgs_builder::set_top_left_x(bn::fixed top_left_x) -> level_bgs_builder&
{
    return set_x(bn::from_top_left_x(top_left_x, dimensions().width()));
}

auto level_bgs_builder::top_left_y() const -> bn::fixed
{
    return bn::to_top_left_y(y(), dimensions().height());
}

auto level_bgs_builder::set_top_left_y(bn::fixed top_left_y) -> level_bgs_builder&
{
    return set_y(bn::from_top_left_y(top_left_y, dimensions().height()));
}

auto level_bgs_builder::top_left_position() const -> bn::fixed_point
{
    return bn::to_top_left_position(position(), dimensions());
}

auto level_bgs_builder::set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y) -> level_bgs_builder&
{
    return set_position(bn::from_top_left_position(bn::fixed_point(top_left_x, top_left_y), dimensions()));
}

auto level_bgs_builder::set_top_left_position(const bn::fixed_point& top_left_position) -> level_bgs_builder&
{
    return set_position(bn::from_top_left_position(top_left_position, dimensions()));
}

auto level_bgs_builder::priority(gen::layer_ident layer_identifier) const -> int
{
    return bg_attr(layer_identifier).priority;
}

auto level_bgs_builder::set_priority(int priority) -> level_bgs_builder&
{
    BN_ASSERT(priority >= bn::bgs::min_priority() && priority <= bn::bgs::max_priority(),
              "Invalid priority: ", priority);

    for (auto& attr : _bgs_attrs)
        attr.priority = priority;
    return *this;
}

auto level_bgs_builder::set_priority(int priority, gen::layer_ident layer_identifier) -> level_bgs_builder&
{
    BN_ASSERT(priority >= bn::bgs::min_priority() && priority <= bn::bgs::max_priority(),
              "Invalid priority: ", priority);

    bg_attr(layer_identifier).priority = priority;
    return *this;
}

auto level_bgs_builder::z_order(gen::layer_ident layer_identifier) const -> int
{
    return bg_attr(layer_identifier).z_order;
}

auto level_bgs_builder::set_z_order(int z_order) -> level_bgs_builder&
{
    BN_ASSERT(z_order >= bn::bgs::min_z_order() && z_order <= bn::bgs::max_z_order(), "Invalid z order: ", z_order);

    for (auto& attr : _bgs_attrs)
        attr.z_order = z_order;
    return *this;
}

auto level_bgs_builder::set_z_order(int z_order, gen::layer_ident layer_identifier) -> level_bgs_builder&
{
    BN_ASSERT(z_order >= bn::bgs::min_z_order() && z_order <= bn::bgs::max_z_order(), "Invalid z order: ", z_order);

    bg_attr(layer_identifier).z_order = z_order;
    return *this;
}

auto level_bgs_builder::mosaic_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return bg_attr(layer_identifier).mosaic_enabled;
}

auto level_bgs_builder::set_mosaic_enabled(bool mosaic_enabled) -> level_bgs_builder&
{
    for (auto& attr : _bgs_attrs)
        attr.mosaic_enabled = mosaic_enabled;
    return *this;
}

auto level_bgs_builder::set_mosaic_enabled(bool mosaic_enabled, gen::layer_ident layer_identifier) -> level_bgs_builder&
{
    bg_attr(layer_identifier).mosaic_enabled = mosaic_enabled;
    return *this;
}

auto level_bgs_builder::blending_top_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return bg_attr(layer_identifier).blending_top_enabled;
}

auto level_bgs_builder::set_blending_top_enabled(bool blending_top_enabled) -> level_bgs_builder&
{
    for (auto& attr : _bgs_attrs)
        attr.blending_top_enabled = blending_top_enabled;
    return *this;
}

auto level_bgs_builder::set_blending_top_enabled(bool blending_top_enabled, gen::layer_ident layer_identifier)
    -> level_bgs_builder&
{
    bg_attr(layer_identifier).blending_top_enabled = blending_top_enabled;
    return *this;
}

auto level_bgs_builder::blending_bottom_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return bg_attr(layer_identifier).blending_bottom_enabled;
}

auto level_bgs_builder::set_blending_bottom_enabled(bool blending_bottom_enabled) -> level_bgs_builder&
{
    for (auto& attr : _bgs_attrs)
        attr.blending_bottom_enabled = blending_bottom_enabled;
    return *this;
}

auto level_bgs_builder::set_blending_bottom_enabled(bool blending_bottom_enabled, gen::layer_ident layer_identifier)
    -> level_bgs_builder&
{
    bg_attr(layer_identifier).blending_bottom_enabled = blending_bottom_enabled;
    return *this;
}

auto level_bgs_builder::green_swap_mode(gen::layer_ident layer_identifier) const -> bn::green_swap_mode
{
    return bg_attr(layer_identifier).green_swap_mode;
}

auto level_bgs_builder::set_green_swap_mode(bn::green_swap_mode green_swap_mode) -> level_bgs_builder&
{
    for (auto& attr : _bgs_attrs)
        attr.green_swap_mode = green_swap_mode;
    return *this;
}

auto level_bgs_builder::set_green_swap_mode(bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier)
    -> level_bgs_builder&
{
    bg_attr(layer_identifier).green_swap_mode = green_swap_mode;
    return *this;
}

auto level_bgs_builder::visible(gen::layer_ident layer_identifier) const -> bool
{
    return bg_attr(layer_identifier).visible;
}

auto level_bgs_builder::set_visible(bool visible) -> level_bgs_builder&
{
    for (auto& attr : _bgs_attrs)
        attr.visible = visible;
    return *this;
}

auto level_bgs_builder::set_visible(bool visible, gen::layer_ident layer_identifier) -> level_bgs_builder&
{
    bg_attr(layer_identifier).visible = visible;
    return *this;
}

auto level_bgs_builder::release_camera() -> bn::optional<bn::camera_ptr>
{
    bn::optional<bn::camera_ptr> result = std::move(_camera);
    _camera.reset();
    return result;
}

auto level_bgs_builder::build() const -> level_bgs_ptr
{
    return level_bgs_ptr::create(*this);
}

auto level_bgs_builder::release_build() -> level_bgs_ptr
{
    return level_bgs_ptr::create(std::move(*this));
}

auto level_bgs_builder::build_optional() const -> bn::optional<level_bgs_ptr>
{
    return level_bgs_ptr::create_optional(*this);
}

auto level_bgs_builder::release_build_optional() -> bn::optional<level_bgs_ptr>
{
    return level_bgs_ptr::create_optional(std::move(*this));
}

auto level_bgs_builder::dimensions() const -> const bn::size&
{
    return _level.px_size();
}

auto level_bgs_builder::bg_attr(gen::layer_ident layer_identifier) -> bg_unique_attributes&
{
    auto* attr = bg_attr_nullable(layer_identifier);
    BN_ASSERT(attr, "Layer of (gen::layer_ident)", (int)layer_identifier,
              " doesn't have a background associated with it");

    return *attr;
}

auto level_bgs_builder::bg_attr(gen::layer_ident layer_identifier) const -> const bg_unique_attributes&
{
    const auto* attr = bg_attr_nullable(layer_identifier);
    BN_ASSERT(attr, "Layer of (gen::layer_ident)", (int)layer_identifier,
              " doesn't have a background associated with it");

    return *attr;
}

auto level_bgs_builder::bg_attr_nullable(gen::layer_ident layer_identifier) -> bg_unique_attributes*
{
    BN_ASSERT((int)layer_identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)layer_identifier);
    BN_ASSERT((int)layer_identifier < _level.layer_instances().size(), "Out of bound identifier (gen::layer_ident)",
              (int)layer_identifier);

    for (auto& bg_attr : _bgs_attrs)
    {
        if (bg_attr.layer_instance.identifier() == layer_identifier)
            return &bg_attr;
    }

    return nullptr;
}

auto level_bgs_builder::bg_attr_nullable(gen::layer_ident layer_identifier) const -> const bg_unique_attributes*
{
    BN_ASSERT((int)layer_identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)layer_identifier);
    BN_ASSERT((int)layer_identifier < _level.layer_instances().size(), "Out of bound identifier (gen::layer_ident)",
              (int)layer_identifier);

    for (const auto& bg_attr : _bgs_attrs)
    {
        if (bg_attr.layer_instance.identifier() == layer_identifier)
            return &bg_attr;
    }

    return nullptr;
}

} // namespace ldtk
