// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "mobs.h"

#include "ldtk_entity.h"
#include "ldtk_layer.h"
#include "ldtk_level.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include <cstdint>

namespace t2p
{

void mobs::load_from_level(const ldtk::level& level, const bn::camera_ptr& camera)
{
    _mobs.clear();

    // Compile-time check to ensure there are up to `MAX_MOBS_COUNT` mobs in all levels
    static_assert(
        [] {
            for (const ldtk::level& lv : ldtk::gen::gen_project.levels())
            {
                const ldtk::layer& ent_layer = lv.get_layer(ldtk::gen::layer_ident::entities);
                int item_count = 0;

                for (const ldtk::entity& ent : ent_layer.entity_instances())
                {
                    if (ent.identifier() == ldtk::gen::entity_ident::mob)
                    {
                        if (++item_count > MAX_MOBS_COUNT)
                            return false;
                    }
                }
            }
            return true;
        }(),
        "Too many mobs exist in a level");

    const ldtk::layer& entities_layer = level.get_layer(ldtk::gen::layer_ident::entities);
    for (const ldtk::entity& entity : entities_layer.entity_instances())
    {
        if (entity.identifier() != ldtk::gen::entity_ident::mob)
            continue;

        // See `ldtk::field::get<T>()` documentation for more info about extracting objects from `ldtk::field`.

        // Extract the generated `mob_kind` from its field.
        ldtk::gen::mob_kind mob_kind =
            entity.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_FIELD_mob_kind).get<ldtk::gen::mob_kind>();

        // Extract the generated `loots` span from its field.
        bn::span<const ldtk::gen::item_kind> loots =
            entity.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_FIELD_loots)
                .get<bn::span<const ldtk::gen::item_kind>>();

        // Extract the generated `patrol` from its field.
        bn::optional<bn::point> patrol;
        const ldtk::field& patrol_field = entity.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_FIELD_patrol);
        // `patrol` field can be null, so we need to check `has_value()` first.
        if (patrol_field.has_value())
            patrol = patrol_field.get<bn::point>();

        // Extract the generated `patrol_duration_updates` from its field.
        int patrol_duration_updates =
            entity.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_FIELD_patrol_duration_updates)
                .get<std::uint16_t>();

        _mobs.emplace_back(mob_kind, loots, patrol, patrol_duration_updates, entity.px(), camera);
    }
}

void mobs::update()
{
    for (auto& mob : _mobs)
        mob.update();
}

} // namespace t2p
