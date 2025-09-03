// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <bn_fixed_point_fwd.h>
#include <bn_top_left_fixed_rect_fwd.h>

#include "wall_grab_direction.h"

namespace ldtk
{
class level;
}

namespace t2p
{

class player;

class player_physics final
{
public:
    void update(player&, const ldtk::level&);

    auto grounded() const -> bool;
    auto wall_grabbed_direction() const -> wall_grab_direction;

    static auto detect_point_collision_with_level_walls(const bn::fixed_point& point, const ldtk::level&) -> bool;

    static auto detect_upside_collision(const bn::top_left_fixed_rect& collision, const ldtk::level&) -> bool;

private:
    void update_y_fall_speed(player&);
    void update_position(player&, const ldtk::level&);
    void move_and_collide_with_level_walls(player&, const ldtk::level&);

    void apply_x_friction(player&);

private:
    bool _grounded = false;
    wall_grab_direction _wall_grabbed_direction = wall_grab_direction::NONE;
};

} // namespace t2p
