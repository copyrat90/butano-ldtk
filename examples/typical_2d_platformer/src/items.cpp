// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "items.h"

#include "ldtk_entity.h"
#include "ldtk_layer.h"
#include "ldtk_level.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

namespace t2p
{

void items::load_from_level(const ldtk::level& level, const bn::camera_ptr& camera)
{
    _items.clear();

    // Compile-time check to ensure there are up to `MAX_ITEMS_COUNT` items in all levels
    static_assert(
        [] {
            for (const ldtk::level& lv : ldtk::gen::gen_project.levels())
            {
                const ldtk::layer& ent_layer = lv.get_layer(ldtk::gen::layer_ident::entities);
                int item_count = 0;

                for (const ldtk::entity& ent : ent_layer.entity_instances())
                {
                    if (ent.identifier() == ldtk::gen::entity_ident::item)
                    {
                        if (++item_count > MAX_ITEMS_COUNT)
                            return false;
                    }
                }
            }
            return true;
        }(),
        "Too many items exist in a level");

    const ldtk::layer& entities_layer = level.get_layer(ldtk::gen::layer_ident::entities);
    for (const ldtk::entity& entity : entities_layer.entity_instances())
    {
        if (entity.identifier() != ldtk::gen::entity_ident::item)
            continue;

        // Get the item kind `ldtk::field` from the entity
        const ldtk::field& item_kind_field =
            entity.get_field(ldtk::gen::entity_field_ident::ENTITY_item_FIELD_item_kind);

        // Extract the generated `item_kind` enum from it.
        //
        // `ldtk::field::get<T>()` checks type mismatch, and if `T` is incorrent, errors out.
        // (NOTE! If this field were nullable, you would have checked `item_kind_field.has_value()` beforehand!)
        ldtk::gen::item_kind item_kind = item_kind_field.get<ldtk::gen::item_kind>();

        _items.emplace_back(item_kind, entity.px(), camera);
    }
}

void items::update()
{
    for (auto& item : _items)
        item.update();
}

} // namespace t2p
