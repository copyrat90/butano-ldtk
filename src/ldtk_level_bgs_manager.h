#pragma once

#include "ldtk_gen_idents_fwd.h"

#include <bn_fixed.h>
#include <bn_fixed_point_fwd.h>
#include <bn_optional.h>
#include <bn_size.h>

#include <cstdint>

namespace bn
{

enum class green_swap_mode : std::uint8_t;

class camera_ptr;
class window;

} // namespace bn

namespace ldtk
{

class level;
class level_bgs_builder;

namespace level_bgs_manager
{

using id_t = void*;

void init();

[[nodiscard]] auto create(level_bgs_builder&& builder) -> id_t;

[[nodiscard]] auto create_optional(level_bgs_builder&& builder) -> id_t;

void increase_usages(id_t id);

void decrease_usages(id_t id);

void set_level(id_t id, const level& level);

void set_level(id_t id, level_bgs_builder&& builder);

auto has_background(id_t id, gen::layer_ident layer_identifier) -> bool;

[[nodiscard]] auto dimensions(id_t id) -> const bn::size&;

[[nodiscard]] auto position(id_t id) -> const bn::fixed_point&;

void set_x(id_t id, bn::fixed x);

void set_y(id_t id, bn::fixed y);

void set_position(id_t id, const bn::fixed_point& position);

[[nodiscard]] auto priority(id_t id, gen::layer_ident layer_identifier) -> int;

void set_priority(id_t id, int priority);

void set_priority(id_t id, int priority, gen::layer_ident layer_identifier);

[[nodiscard]] auto z_order(id_t id, gen::layer_ident layer_identifier) -> int;

void set_z_order(id_t id, int z_order);

void set_z_order(id_t id, int z_order, gen::layer_ident layer_identifier);

void put_above(id_t id);

void put_above(id_t id, gen::layer_ident layer_identifier);

void put_below(id_t id);

void put_below(id_t id, gen::layer_ident layer_identifier);

[[nodiscard]] auto mosaic_enabled(id_t id, gen::layer_ident layer_identifier) -> bool;

void set_mosaic_enabled(id_t id, bool mosaic_enabled);

void set_mosaic_enabled(id_t id, bool mosaic_enabled, gen::layer_ident layer_identifier);

[[nodiscard]] auto green_swap_mode(id_t id, gen::layer_ident layer_identifier) -> bn::green_swap_mode;

void set_green_swap_mode(id_t id, bn::green_swap_mode green_swap_mode);

void set_green_swap_mode(id_t id, bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier);

[[nodiscard]] auto blending_top_enabled(id_t id, gen::layer_ident layer_identifier) -> bool;

void set_blending_top_enabled(id_t id, bool blending_top_enabled);

void set_blending_top_enabled(id_t id, bool blending_top_enabled, gen::layer_ident layer_identifier);

[[nodiscard]] auto blending_bottom_enabled(id_t id, gen::layer_ident layer_identifier) -> bool;

void set_blending_bottom_enabled(id_t id, bool blending_bottom_enabled);

void set_blending_bottom_enabled(id_t id, bool blending_bottom_enabled, gen::layer_ident layer_identifier);

[[nodiscard]] auto visible(id_t id, gen::layer_ident layer_identifier) -> bool;

void set_visible(id_t id, bool visible);

void set_visible(id_t id, bool visible, gen::layer_ident layer_identifier);

[[nodiscard]] auto visible_in_window(id_t id, const bn::window& window, gen::layer_ident layer_identifier) -> bool;

void set_visible_in_window(id_t id, bool visible, bn::window& window);

void set_visible_in_window(id_t id, bool visible, bn::window& window, gen::layer_ident layer_identifier);

[[nodiscard]] auto camera(id_t id) -> const bn::optional<bn::camera_ptr>&;

void set_camera(id_t id, bn::camera_ptr&& camera);

void remove_camera(id_t id);

} // namespace level_bgs_manager

} // namespace ldtk
