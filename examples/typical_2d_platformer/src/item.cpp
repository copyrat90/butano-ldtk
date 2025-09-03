// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "item.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include "bn_sprite_items_acorn.h"
#include "bn_sprite_items_cherry.h"
#include "bn_sprite_items_gem.h"

#include <bn_assert.h>
#include <bn_camera_ptr.h>
#include <bn_sprite_builder.h>

#include <cstdint>
#include <utility>

namespace t2p
{

namespace
{

constexpr bn::fixed_point PIVOT_POINT =
    ldtk::gen::gen_project.defs().get_entity_def(ldtk::gen::entity_ident::item).pivot();

auto create_item_sprite(ldtk::gen::item_kind item_kind, const bn::fixed_point& position, const bn::camera_ptr& camera)
    -> bn::sprite_ptr
{
    static constexpr const bn::sprite_item* SPR_ITEMS[] = {
        &bn::sprite_items::acorn,
        &bn::sprite_items::cherry,
        &bn::sprite_items::gem,
    };
    static constexpr bn::span<const bn::sprite_item* const> SPR_ITEMS_SPAN(SPR_ITEMS);

    BN_ASSERT((int)item_kind < SPR_ITEMS_SPAN.size(), "Invalid item kind: ", (int)item_kind);
    const bn::sprite_item& spr_item = *SPR_ITEMS[(int)item_kind];

    bn::sprite_builder builder(spr_item);
    builder.set_top_left_position(position - bn::fixed_point(spr_item.shape_size().width() * PIVOT_POINT.x(),
                                                             spr_item.shape_size().height() * PIVOT_POINT.y()));
    builder.set_camera(camera);
    return builder.release_build();
}

auto get_anim_action(bn::sprite_ptr& sprite, ldtk::gen::entity_field_ident wait_updates_ident,
                     const bn::sprite_tiles_item& tiles_item, ldtk::gen::entity_field_ident graphics_indexes_ident)
    -> item::item_animate_action_t
{
    std::uint16_t wait_updates = ITEM_CONFIGS.get_field(wait_updates_ident).get<std::uint16_t>();
    bn::span<const std::uint16_t> graphics_indexes =
        ITEM_CONFIGS.get_field(graphics_indexes_ident).get<bn::span<const std::uint16_t>>();

    BN_ASSERT(graphics_indexes.size() > 1);
    return item::item_animate_action_t::forever(sprite, wait_updates, tiles_item, graphics_indexes);
}

auto create_item_anim_action(bn::sprite_ptr& sprite, ldtk::gen::item_kind item_kind) -> item::item_animate_action_t
{
    switch (item_kind)
    {
        using kind = ldtk::gen::item_kind;

    case kind::acorn:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_acorn_wait_updates,
                               bn::sprite_items::acorn.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_acorn_graphics_indexes);

    case kind::cherry:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_cherry_wait_updates,
                               bn::sprite_items::cherry.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_cherry_graphics_indexes);

    case kind::gem:
        return get_anim_action(sprite, ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_gem_wait_updates,
                               bn::sprite_items::gem.tiles_item(),
                               ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_gem_graphics_indexes);

    default:
        BN_ERROR("Invalid item kind: ", (int)item_kind);
    }

    std::unreachable();
}

} // namespace

item::item(ldtk::gen::item_kind item_kind, const bn::fixed_point& position, const bn::camera_ptr& camera)
    : _sprite(create_item_sprite(item_kind, position, camera)),
      _animate_action(create_item_anim_action(_sprite, item_kind))
{
}

void item::update()
{
    if (!_animate_action.done())
        _animate_action.update();
}

} // namespace t2p
