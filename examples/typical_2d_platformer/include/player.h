// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "player_animation.h"
#include "player_physics.h"
#include "player_state_machine.h"

#include <bn_fixed_point.h>

namespace ldtk
{
class level;
}

namespace bn
{
class camera_ptr;
}

namespace t2p
{

class player final
{
public:
    player(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const ldtk::level&);

public:
    auto position() const -> const bn::fixed_point&;
    void set_position(const bn::fixed_point& position);
    auto x() const -> bn::fixed;
    void set_x(bn::fixed x);
    auto y() const -> bn::fixed;
    void set_y(bn::fixed y);

    auto velocity() const -> const bn::fixed_point&;
    void set_velocity(const bn::fixed_point& velocity);
    auto vel_x() const -> bn::fixed;
    void set_vel_x(bn::fixed x);
    auto vel_y() const -> bn::fixed;
    void set_vel_y(bn::fixed y);

public:
    auto animation() -> player_animation&;
    auto animation() const -> const player_animation&;

    auto physics() -> player_physics&;
    auto physics() const -> const player_physics&;

    auto state_machine() -> player_state_machine&;
    auto state_machine() const -> const player_state_machine&;

private:
    bn::fixed_point _position;
    bn::fixed_point _velocity;

    player_animation _animation;
    player_physics _physics;
    player_state_machine _state_machine;
};

} // namespace t2p
