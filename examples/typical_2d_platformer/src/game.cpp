// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "game.h"

#include "ldtk_layer.h"
#include "ldtk_level.h"
#include "ldtk_level_bgs_builder.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include <bn_backdrop.h>
#include <bn_blending.h>
#include <bn_display.h>
#include <bn_keypad.h>

#include <algorithm>

namespace t2p
{

game::game()
    : _level_index(0), _level(ldtk::gen::gen_project.levels().data() + _level_index),
      _camera(bn::camera_ptr::create(get_player_spawn_position())), _player(_camera.position(), _camera),
      _level_bgs(init_level_bgs())
{
    bn::blending::set_transparency_alpha(ldtk::gen::gen_project.opacity());
    bn::backdrop::set_color(_level->bg_color());

    _items.load_from_level(*_level, _camera);
    _mobs.load_from_level(*_level, _camera);
    _player.animation().put_above();
}

void game::update()
{
    _player.update(*_level);
    _items.update();
    _mobs.update();

    update_camera();
}

void game::next_level()
{
    _level_index = (_level_index + 1) % ldtk::gen::gen_project.levels().size();
    _level = ldtk::gen::gen_project.levels().data() + _level_index;

    _player.set_position(get_player_spawn_position());
    update_camera();
    _level_bgs.set_level(*_level);
    _level_bgs.set_top_left_position(0, 0); // top left for the level bgs

    _items.load_from_level(*_level, _camera);
    _mobs.load_from_level(*_level, _camera);
    _player.animation().put_above();
}

void game::update_camera()
{
    static constexpr bn::fixed_point CAM_DIFF(-bn::display::width() / 2, -20 - bn::display::height() / 2);

    _camera.set_position(_player.position() + CAM_DIFF);

    move_camera_to_level_bounds();
}

void game::move_camera_to_level_bounds()
{
    // Use top-left coordinates for the level boundaries
    const bn::fixed level_left_bound = 0;
    const bn::fixed level_right_bound = _level->px_width() - bn::display::width();
    const bn::fixed level_top_bound = 0;
    const bn::fixed level_bottom_bound = _level->px_height() - bn::display::height();

    const bn::fixed left_overshoot = level_left_bound - _camera.x();
    const bn::fixed right_overshoot = _camera.x() - level_right_bound;
    const bn::fixed up_overshoot = level_top_bound - _camera.y();
    const bn::fixed down_overshoot = _camera.y() - level_bottom_bound;

    if (left_overshoot > 0)
        _camera.set_x(_camera.x() + left_overshoot);
    if (right_overshoot > 0)
        _camera.set_x(_camera.x() - right_overshoot);
    if (up_overshoot > 0)
        _camera.set_y(_camera.y() + up_overshoot);
    if (down_overshoot > 0)
        _camera.set_y(_camera.y() - down_overshoot);
}

auto game::init_level_bgs() -> ldtk::level_bgs_ptr
{
    ldtk::level_bgs_builder builder(*_level);

    move_camera_to_level_bounds();
    builder.set_camera(_camera);
    builder.set_top_left_position(0, 0);

    return builder.release_build();
}

auto game::get_player_spawn_position() const -> bn::fixed_point
{
    const ldtk::layer& entity_layer = _level->get_layer(ldtk::gen::layer_ident::entities);

    // Get the player from this level's "entity" layer
    auto player_iter = std::ranges::find_if(entity_layer.entity_instances(), [](const ldtk::entity& entity) {
        return entity.identifier() == ldtk::gen::entity_ident::player;
    });

    // Compile-time check to ensure every level has a player entity
    static_assert(
        [] {
            for (const ldtk::level& level : ldtk::gen::gen_project.levels())
            {
                const ldtk::layer& ent_layer = level.get_layer(ldtk::gen::layer_ident::entities);

                auto iter = std::ranges::find_if(ent_layer.entity_instances(), [](const ldtk::entity& entity) {
                    return entity.identifier() == ldtk::gen::entity_ident::player;
                });

                if (iter == ent_layer.entity_instances().cend())
                    return false;
            }

            return true;
        }(),
        "Player not found in a level");

    // Now that we know every level has a player entity,
    // it's safe to dereference the iterator
    const ldtk::entity& player = *player_iter;

    return get_entity_spawn_position(player);
}

auto game::get_entity_spawn_position(const ldtk::entity& entity) const -> bn::fixed_point
{
    // Get the pixel coordinate of the entity
    return entity.px();
}

} // namespace t2p
