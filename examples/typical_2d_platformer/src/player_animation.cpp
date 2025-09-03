// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player_animation.h"

#include "player.h"

#include "ldtk_gen_project.h"

#include "bn_sprite_items_foxy.h"

#include <bn_assert.h>
#include <bn_span.h>
#include <bn_sprite_builder.h>

#include <cstdint>
#include <utility>

namespace t2p
{

namespace
{

constexpr const auto& foxy = bn::sprite_items::foxy;

// Player entity pivot point is set to the center of the feet in LDtk
constexpr bn::fixed_point PIVOT_POINT =
    ldtk::gen::gen_project.defs().get_entity_def(ldtk::gen::entity_ident::player).pivot();

// Sprites should end up on the center of the feet this way
constexpr bn::fixed_point SPR_DIFF(-foxy.shape_size().width() * PIVOT_POINT.x(),
                                   -foxy.shape_size().height() * PIVOT_POINT.y());

auto create_player_sprite(const bn::fixed_point& position, const bn::camera_ptr& camera) -> bn::sprite_ptr
{
    bn::sprite_builder builder(bn::sprite_items::foxy);
    builder.set_top_left_position(position + SPR_DIFF);
    builder.set_camera(camera);
    return builder.release_build();
}

auto set_first_frame_and_get_anim_action(bn::sprite_ptr& sprite, ldtk::gen::entity_field_ident forever_ident,
                                         ldtk::gen::entity_field_ident wait_updates_ident,
                                         ldtk::gen::entity_field_ident graphics_indexes_ident)
    -> bn::optional<player_animation::player_animate_action_t>
{
    bool forever = PLAYER_CONFIGS.get_field(forever_ident).get<bool>();
    std::uint16_t wait_updates = PLAYER_CONFIGS.get_field(wait_updates_ident).get<std::uint16_t>();
    bn::span<const std::uint16_t> graphics_indexes =
        PLAYER_CONFIGS.get_field(graphics_indexes_ident).get<bn::span<const std::uint16_t>>();

    // Sets the sprite to first frame of the animation
    BN_ASSERT(graphics_indexes.size() > 0);
    sprite.set_tiles(foxy.tiles_item(), graphics_indexes[0]);

    // Get the animate action
    if (graphics_indexes.size() > 1)
    {
        if (forever)
            return player_animation::player_animate_action_t::forever(sprite, wait_updates, foxy.tiles_item(),
                                                                      graphics_indexes);
        return player_animation::player_animate_action_t::once(sprite, wait_updates, foxy.tiles_item(),
                                                               graphics_indexes);
    }

    return bn::nullopt;
}

auto create_player_anim_action(bn::sprite_ptr& sprite, player_animation::kind anim_kind)
    -> bn::optional<player_animation::player_animate_action_t>
{
    switch (anim_kind)
    {
        using kind = player_animation::kind;

    case kind::IDLE:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_idle_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_idle_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_idle_graphics_indexes);

    case kind::RUN:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_run_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_run_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_run_graphics_indexes);

    case kind::JUMP:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_jump_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_jump_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_jump_graphics_indexes);

    case kind::FALL:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_fall_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_fall_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_fall_graphics_indexes);

    case kind::LOOK_UP:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_look_up_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_look_up_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_look_up_graphics_indexes);

    case kind::CROUCH:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_crouch_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_crouch_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_crouch_graphics_indexes);

    case kind::WALL_GRAB:
        return set_first_frame_and_get_anim_action(
            sprite, ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_wall_grab_forever,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_wall_grab_wait_updates,
            ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_wall_grab_graphics_indexes);

    default:
        BN_ERROR("Invalid player animation kind: ", (int)anim_kind);
    }

    std::unreachable();
}

} // namespace

player_animation::player_animation(const bn::fixed_point& position, const bn::camera_ptr& camera)
    : _sprite(create_player_sprite(position, camera)), _anim_kind(kind::IDLE),
      _anim_action(create_player_anim_action(_sprite, _anim_kind))
{
}

void player_animation::handle_input(const player& player_)
{
    if (player_.vel_x() != 0)
        _sprite.set_horizontal_flip(player_.vel_x() < 0);
}

void player_animation::update()
{
    if (_anim_action && !_anim_action->done())
        _anim_action->update();
}

auto player_animation::animation_kind() const -> player_animation::kind
{
    return _anim_kind;
}

void player_animation::start(player_animation::kind anim_kind)
{
    _anim_kind = anim_kind;
    _anim_action = create_player_anim_action(_sprite, anim_kind);
}

auto player_animation::flip() const -> bool
{
    return _sprite.horizontal_flip();
}

void player_animation::set_flip(bool flip)
{
    _sprite.set_horizontal_flip(flip);
}

void player_animation::put_above()
{
    _sprite.put_above();
}

auto player_animation::position() const -> bn::fixed_point
{
    return _sprite.top_left_position() - SPR_DIFF;
}

void player_animation::set_position(const bn::fixed_point& position)
{
    _sprite.set_top_left_position(position + SPR_DIFF);
}

auto player_animation::x() const -> bn::fixed
{
    return _sprite.top_left_x() - SPR_DIFF.x();
}

void player_animation::set_x(bn::fixed x)
{
    _sprite.set_top_left_x(x + SPR_DIFF.x());
}

auto player_animation::y() const -> bn::fixed
{
    return _sprite.top_left_y() - SPR_DIFF.y();
}

void player_animation::set_y(bn::fixed y)
{
    _sprite.set_top_left_y(y + SPR_DIFF.y());
}

} // namespace t2p
