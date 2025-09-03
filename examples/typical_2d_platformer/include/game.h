// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_level_bgs_ptr.h"

#include <bn_camera_ptr.h>

#include "items.h"
#include "mobs.h"
#include "player.h"

namespace ldtk
{
class level;
class entity;
} // namespace ldtk

namespace t2p
{

class game
{
public:
    game();

    void update();

    void next_level();

private:
    void update_camera();
    void move_camera_to_level_bounds();

private:
    auto init_level_bgs() -> ldtk::level_bgs_ptr;

    auto get_player_spawn_position() const -> bn::fixed_point;

    auto get_entity_spawn_position(const ldtk::entity& entity) const -> bn::fixed_point;

private:
    int _level_index;
    const ldtk::level* _level;

    bn::camera_ptr _camera;
    player _player;
    ldtk::level_bgs_ptr _level_bgs;

    items _items;
    mobs _mobs;
};

} // namespace t2p
