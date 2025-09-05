// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_level_bgs_ptr.h"

#include "ldtk_level_bgs_builder.h"
#include "ldtk_level_bgs_manager.h"

#include "bn_top_left_utils.h"

#include <utility>

namespace ldtk
{

auto level_bgs_ptr::create(const level& level) -> level_bgs_ptr
{
    return level_bgs_ptr(level_bgs_manager::create(level_bgs_builder(level)));
}

auto level_bgs_ptr::create(const bn::fixed_point& position, const level& level) -> level_bgs_ptr
{
    level_bgs_builder builder(level);
    builder.set_position(position);
    return level_bgs_ptr(level_bgs_manager::create(std::move(builder)));
}

auto level_bgs_ptr::create(bn::fixed x, bn::fixed y, const level& level) -> level_bgs_ptr
{
    level_bgs_builder builder(level);
    builder.set_position(bn::fixed_point(x, y));
    return level_bgs_ptr(level_bgs_manager::create(std::move(builder)));
}

auto level_bgs_ptr::create(const level_bgs_builder& builder) -> level_bgs_ptr
{
    return level_bgs_ptr(level_bgs_manager::create(level_bgs_builder(builder)));
}

auto level_bgs_ptr::create(level_bgs_builder&& builder) -> level_bgs_ptr
{
    return level_bgs_ptr(level_bgs_manager::create(std::move(builder)));
}

auto level_bgs_ptr::create_optional(const level& level) -> bn::optional<level_bgs_ptr>
{
    bn::optional<level_bgs_ptr> result;

    if (handle_t handle = level_bgs_manager::create_optional(level_bgs_builder(level)))
    {
        result = level_bgs_ptr(handle);
    }

    return result;
}

auto level_bgs_ptr::create_optional(const bn::fixed_point& position, const level& level) -> bn::optional<level_bgs_ptr>
{
    bn::optional<level_bgs_ptr> result;
    level_bgs_builder builder(level);
    builder.set_position(position);

    if (handle_t handle = level_bgs_manager::create_optional(std::move(builder)))
    {
        result = level_bgs_ptr(handle);
    }

    return result;
}

auto level_bgs_ptr::create_optional(bn::fixed x, bn::fixed y, const level& level) -> bn::optional<level_bgs_ptr>
{
    bn::optional<level_bgs_ptr> result;
    level_bgs_builder builder(level);
    builder.set_position(bn::fixed_point(x, y));

    if (handle_t handle = level_bgs_manager::create_optional(std::move(builder)))
    {
        result = level_bgs_ptr(handle);
    }

    return result;
}

auto level_bgs_ptr::create_optional(const level_bgs_builder& builder) -> bn::optional<level_bgs_ptr>
{
    bn::optional<level_bgs_ptr> result;

    if (handle_t handle = level_bgs_manager::create_optional(level_bgs_builder(builder)))
    {
        result = level_bgs_ptr(handle);
    }

    return result;
}

auto level_bgs_ptr::create_optional(level_bgs_builder&& builder) -> bn::optional<level_bgs_ptr>
{
    bn::optional<level_bgs_ptr> result;

    if (handle_t handle = level_bgs_manager::create_optional(std::move(builder)))
    {
        result = level_bgs_ptr(handle);
    }

    return result;
}

level_bgs_ptr::level_bgs_ptr(const level_bgs_ptr& other) : level_bgs_ptr(other._handle)
{
    level_bgs_manager::increase_usages(_handle);
}

auto level_bgs_ptr::operator=(const level_bgs_ptr& other) -> level_bgs_ptr&
{
    if (_handle != other._handle)
    {
        if (_handle)
        {
            level_bgs_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        level_bgs_manager::increase_usages(_handle);
    }

    return *this;
}

level_bgs_ptr::~level_bgs_ptr()
{
    if (_handle)
    {
        level_bgs_manager::decrease_usages(_handle);
    }
}

auto level_bgs_ptr::has_background(gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::has_background(_handle, layer_identifier);
}

void level_bgs_ptr::set_level(const level& level)
{
    level_bgs_manager::set_level(_handle, level);
}

void level_bgs_ptr::set_level(const level_bgs_builder& builder)
{
    level_bgs_manager::set_level(_handle, level_bgs_builder(builder));
}

void level_bgs_ptr::set_level(level_bgs_builder&& builder)
{
    level_bgs_manager::set_level(_handle, std::move(builder));
}

auto level_bgs_ptr::dimensions() const -> bn::size
{
    return level_bgs_manager::dimensions(_handle);
}

auto level_bgs_ptr::x() const -> bn::fixed
{
    return position().x();
}

void level_bgs_ptr::set_x(bn::fixed x)
{
    level_bgs_manager::set_x(_handle, x);
}

auto level_bgs_ptr::y() const -> bn::fixed
{
    return position().y();
}

void level_bgs_ptr::set_y(bn::fixed y)
{
    level_bgs_manager::set_y(_handle, y);
}

auto level_bgs_ptr::position() const -> const bn::fixed_point&
{
    return level_bgs_manager::position(_handle);
}

void level_bgs_ptr::set_position(bn::fixed x, bn::fixed y)
{
    level_bgs_manager::set_position(_handle, bn::fixed_point(x, y));
}

void level_bgs_ptr::set_position(const bn::fixed_point& position)
{
    level_bgs_manager::set_position(_handle, position);
}

auto level_bgs_ptr::top_left_x() const -> bn::fixed
{
    return bn::to_top_left_x(position().x(), dimensions().width());
}

void level_bgs_ptr::set_top_left_x(bn::fixed top_left_x)
{
    set_x(bn::from_top_left_x(top_left_x, dimensions().width()));
}

auto level_bgs_ptr::top_left_y() const -> bn::fixed
{
    return bn::to_top_left_y(position().y(), dimensions().height());
}

void level_bgs_ptr::set_top_left_y(bn::fixed top_left_y)
{
    set_y(bn::from_top_left_y(top_left_y, dimensions().height()));
}

auto level_bgs_ptr::top_left_position() const -> bn::fixed_point
{
    return bn::to_top_left_position(position(), dimensions());
}

void level_bgs_ptr::set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y)
{
    set_position(bn::from_top_left_position(bn::fixed_point(top_left_x, top_left_y), dimensions()));
}

void level_bgs_ptr::set_top_left_position(const bn::fixed_point& top_left_position)
{
    set_position(bn::from_top_left_position(top_left_position, dimensions()));
}

auto level_bgs_ptr::priority(gen::layer_ident layer_identifier) const -> int
{
    return level_bgs_manager::priority(_handle, layer_identifier);
}

void level_bgs_ptr::set_priority(int priority)
{
    level_bgs_manager::set_priority(_handle, priority);
}

void level_bgs_ptr::set_priority(int priority, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_priority(_handle, priority, layer_identifier);
}

auto level_bgs_ptr::z_order(gen::layer_ident layer_identifier) const -> int
{
    return level_bgs_manager::z_order(_handle, layer_identifier);
}

void level_bgs_ptr::set_z_order(int z_order)
{
    level_bgs_manager::set_z_order(_handle, z_order);
}

void level_bgs_ptr::set_z_order(int z_order, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_z_order(_handle, z_order, layer_identifier);
}

void level_bgs_ptr::put_above()
{
    level_bgs_manager::put_above(_handle);
}

void level_bgs_ptr::put_above(gen::layer_ident layer_identifier)
{
    level_bgs_manager::put_above(_handle, layer_identifier);
}

void level_bgs_ptr::put_below()
{
    level_bgs_manager::put_below(_handle);
}

void level_bgs_ptr::put_below(gen::layer_ident layer_identifier)
{
    level_bgs_manager::put_below(_handle, layer_identifier);
}

auto level_bgs_ptr::mosaic_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::mosaic_enabled(_handle, layer_identifier);
}

void level_bgs_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    level_bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

void level_bgs_ptr::set_mosaic_enabled(bool mosaic_enabled, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled, layer_identifier);
}

auto level_bgs_ptr::blending_top_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::blending_top_enabled(_handle, layer_identifier);
}

void level_bgs_ptr::set_blending_top_enabled(bool blending_top_enabled)
{
    level_bgs_manager::set_blending_top_enabled(_handle, blending_top_enabled);
}

void level_bgs_ptr::set_blending_top_enabled(bool blending_top_enabled, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_blending_top_enabled(_handle, blending_top_enabled, layer_identifier);
}

auto level_bgs_ptr::blending_bottom_enabled(gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::blending_bottom_enabled(_handle, layer_identifier);
}

void level_bgs_ptr::set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    level_bgs_manager::set_blending_bottom_enabled(_handle, blending_bottom_enabled);
}

void level_bgs_ptr::set_blending_bottom_enabled(bool blending_bottom_enabled, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_blending_bottom_enabled(_handle, blending_bottom_enabled, layer_identifier);
}

auto level_bgs_ptr::green_swap_mode(gen::layer_ident layer_identifier) const -> bn::green_swap_mode
{
    return level_bgs_manager::green_swap_mode(_handle, layer_identifier);
}

void level_bgs_ptr::set_green_swap_mode(bn::green_swap_mode green_swap_mode)
{
    level_bgs_manager::set_green_swap_mode(_handle, green_swap_mode);
}

void level_bgs_ptr::set_green_swap_mode(bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_green_swap_mode(_handle, green_swap_mode, layer_identifier);
}

auto level_bgs_ptr::visible(gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::visible(_handle, layer_identifier);
}

void level_bgs_ptr::set_visible(bool visible)
{
    level_bgs_manager::set_visible(_handle, visible);
}

void level_bgs_ptr::set_visible(bool visible, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_visible(_handle, visible, layer_identifier);
}

auto level_bgs_ptr::visible_in_window(const bn::window& window, gen::layer_ident layer_identifier) const -> bool
{
    return level_bgs_manager::visible_in_window(_handle, window, layer_identifier);
}

void level_bgs_ptr::set_visible_in_window(bool visible, bn::window& window)
{
    level_bgs_manager::set_visible_in_window(_handle, visible, window);
}

void level_bgs_ptr::set_visible_in_window(bool visible, bn::window& window, gen::layer_ident layer_identifier)
{
    level_bgs_manager::set_visible_in_window(_handle, visible, window, layer_identifier);
}

auto level_bgs_ptr::camera() const -> const bn::optional<bn::camera_ptr>&
{
    return level_bgs_manager::camera(_handle);
}

void level_bgs_ptr::set_camera(const bn::camera_ptr& camera)
{
    level_bgs_manager::set_camera(_handle, bn::camera_ptr(camera));
}

void level_bgs_ptr::set_camera(bn::camera_ptr&& camera)
{
    level_bgs_manager::set_camera(_handle, std::move(camera));
}

void level_bgs_ptr::set_camera(const bn::optional<bn::camera_ptr>& camera)
{
    if (const bn::camera_ptr* camera_ref = camera.get())
    {
        level_bgs_manager::set_camera(_handle, bn::camera_ptr(*camera_ref));
    }
    else
    {
        level_bgs_manager::remove_camera(_handle);
    }
}

void level_bgs_ptr::set_camera(bn::optional<bn::camera_ptr>&& camera)
{
    if (bn::camera_ptr* camera_ref = camera.get())
    {
        level_bgs_manager::set_camera(_handle, std::move(*camera_ref));
    }
    else
    {
        level_bgs_manager::remove_camera(_handle);
    }
}

void level_bgs_ptr::remove_camera()
{
    level_bgs_manager::remove_camera(_handle);
}

} // namespace ldtk
