// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "mob.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include "bn_sprite_items_ant.h"
#include "bn_sprite_items_bat.h"
#include "bn_sprite_items_bear.h"
#include "bn_sprite_items_bettle.h"
#include "bn_sprite_items_opossum.h"

#include <bn_assert.h>
#include <bn_display.h>
#include <bn_span.h>
#include <bn_sprite_builder.h>

#include <cstdint>

namespace t2p
{

namespace
{

constexpr int GRID_SIZE = ldtk::gen::gen_project.defs().get_layer_def(ldtk::gen::layer_ident::collisions).grid_size();

constexpr bn::fixed_point TILE_FEET_DIFF(GRID_SIZE / 2, GRID_SIZE);

// Mob entity pivot point is set to the center of the feet in LDtk
constexpr bn::fixed_point PIVOT_POINT =
    ldtk::gen::gen_project.defs().get_entity_def(ldtk::gen::entity_ident::mob).pivot();

constexpr const bn::sprite_item* MOB_SPR_ITEMS[] = {
    &bn::sprite_items::ant,    &bn::sprite_items::bat,     &bn::sprite_items::bear,
    &bn::sprite_items::bettle, &bn::sprite_items::opossum,
};
constexpr bn::span<const bn::sprite_item* const> MOB_SPR_ITEMS_SPAN(MOB_SPR_ITEMS);

auto create_mob_sprite(ldtk::gen::mob_kind mob_kind, const bn::fixed_point& position, const bn::camera_ptr& camera)
    -> bn::sprite_ptr
{
    BN_ASSERT((int)mob_kind < MOB_SPR_ITEMS_SPAN.size(), "Invalid mob kind: ", (int)mob_kind);
    const bn::sprite_item& spr_item = *MOB_SPR_ITEMS[(int)mob_kind];

    bn::sprite_builder builder(spr_item);
    builder.set_top_left_position(position - bn::fixed_point(spr_item.shape_size().width() * PIVOT_POINT.x(),
                                                             spr_item.shape_size().height() * PIVOT_POINT.y()));
    builder.set_camera(camera);
    return builder.release_build();
}

auto get_anim_action(bn::sprite_ptr& sprite, ldtk::gen::entity_field_ident wait_updates_ident,
                     const bn::sprite_tiles_item& tiles_item, ldtk::gen::entity_field_ident graphics_indexes_ident)
    -> mob::mob_animate_action_t
{
    std::uint16_t wait_updates = MOB_CONFIGS.get_field(wait_updates_ident).get<std::uint16_t>();
    bn::span<const std::uint16_t> graphics_indexes =
        MOB_CONFIGS.get_field(graphics_indexes_ident).get<bn::span<const std::uint16_t>>();

    BN_ASSERT(graphics_indexes.size() > 1);
    return mob::mob_animate_action_t::forever(sprite, wait_updates, tiles_item, graphics_indexes);
}

auto create_mob_anim_action(bn::sprite_ptr& sprite, ldtk::gen::mob_kind mob_kind) -> mob::mob_animate_action_t
{
    switch (mob_kind)
    {
        using kind = ldtk::gen::mob_kind;

    case kind::ant:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_ant_wait_updates,
                               bn::sprite_items::ant.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_ant_graphics_indexes);

    case kind::bat:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bat_wait_updates,
                               bn::sprite_items::bat.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bat_graphics_indexes);

    case kind::bear:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bear_wait_updates,
                               bn::sprite_items::bear.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bear_graphics_indexes);

    case kind::bettle:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bettle_wait_updates,
                               bn::sprite_items::bettle.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bettle_graphics_indexes);

    case kind::opossum:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_opossum_wait_updates,
                               bn::sprite_items::opossum.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_opossum_graphics_indexes);

    default:
        BN_ERROR("Invalid mob kind: ", (int)mob_kind);
    }

    std::unreachable();
}

auto create_mob_patrol_action(bn::sprite_ptr& sprite, ldtk::gen::mob_kind mob_kind,
                              const bn::optional<bn::point>& patrol, int patrol_duration_updates)
    -> bn::optional<bn::sprite_move_loop_action>
{
    BN_ASSERT((int)mob_kind < MOB_SPR_ITEMS_SPAN.size(), "Invalid mob kind: ", (int)mob_kind);

    bn::optional<bn::sprite_move_loop_action> result;

    if (patrol.has_value())
    {
        const bn::sprite_item& spr_item = *MOB_SPR_ITEMS[(int)mob_kind];

        // This is level top-left (0, 0) coordinates
        const bn::fixed_point level_destination{
            patrol->x() * GRID_SIZE + TILE_FEET_DIFF.x(),
            patrol->y() * GRID_SIZE + TILE_FEET_DIFF.y() - spr_item.shape_size().height() / 2,
        };

        // But we need the Butano center coordinates for this
        const bn::fixed_point destination{
            level_destination.x() - bn::display::width() / 2,
            level_destination.y() - bn::display::height() / 2,
        };

        result = bn::sprite_move_loop_action(sprite, patrol_duration_updates, destination);
    }

    return result;
}

} // namespace

mob::mob(ldtk::gen::mob_kind mob_kind, const bn::span<const ldtk::gen::item_kind>& loots,
         const bn::optional<bn::point>& patrol, int patrol_duration_updates, const bn::fixed_point& position,
         const bn::camera_ptr& camera)
    : _mob_sprite(create_mob_sprite(mob_kind, position, camera)),
      _mob_anim_action(create_mob_anim_action(_mob_sprite, mob_kind)),
      _mob_patrol_action(create_mob_patrol_action(_mob_sprite, mob_kind, patrol, patrol_duration_updates))
{
    // Exercise for the reader: Show loots sprites above the mob head with `loots`
    //
    // You could reuse some code from `item.cpp` to achieve that.
    ((void)loots);
}

void mob::update()
{
    if (!_mob_anim_action.done())
        _mob_anim_action.update();

    if (_mob_patrol_action.has_value())
    {
        const bn::fixed_point prev_pos = _mob_sprite.position();

        _mob_patrol_action->update();

        const bool moving_left = _mob_sprite.position().x() < prev_pos.x();
        _mob_sprite.set_horizontal_flip(moving_left);
    }
}

} // namespace t2p
