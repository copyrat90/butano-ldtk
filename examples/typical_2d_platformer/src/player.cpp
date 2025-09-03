// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player.h"

#include <bn_camera_ptr.h>

namespace t2p
{

player::player(const bn::fixed_point& position, const bn::camera_ptr& camera)
    : _position(position), _velocity(0, 0), _animation(position, camera), _state_machine(*this)
{
}

void player::update(const ldtk::level& level)
{
    _state_machine.handle_input(*this);
    _animation.handle_input(*this);
    _physics.update(*this, level);
    _state_machine.update(*this, level);
    _animation.update();
}

auto player::position() const -> const bn::fixed_point&
{
    return _position;
}

void player::set_position(const bn::fixed_point& position)
{
    _position = position;
    _animation.set_position(position);
}

auto player::x() const -> bn::fixed
{
    return _position.x();
}

void player::set_x(bn::fixed x)
{
    _position.set_x(x);
    _animation.set_x(x);
}

auto player::y() const -> bn::fixed
{
    return _position.y();
}

void player::set_y(bn::fixed y)
{
    _position.set_y(y);
    _animation.set_y(y);
}

auto player::velocity() const -> const bn::fixed_point&
{
    return _velocity;
}

void player::set_velocity(const bn::fixed_point& velocity)
{
    _velocity = velocity;
}

auto player::vel_x() const -> bn::fixed
{
    return _velocity.x();
}

void player::set_vel_x(bn::fixed x)
{
    _velocity.set_x(x);
}

auto player::vel_y() const -> bn::fixed
{
    return _velocity.y();
}

void player::set_vel_y(bn::fixed y)
{
    _velocity.set_y(y);
}

auto player::animation() -> player_animation&
{
    return _animation;
}

auto player::animation() const -> const player_animation&
{
    return _animation;
}

auto player::physics() -> player_physics&
{
    return _physics;
}

auto player::physics() const -> const player_physics&
{
    return _physics;
}

auto player::state_machine() -> player_state_machine&
{
    return _state_machine;
}

auto player::state_machine() const -> const player_state_machine&
{
    return _state_machine;
}

} // namespace t2p
