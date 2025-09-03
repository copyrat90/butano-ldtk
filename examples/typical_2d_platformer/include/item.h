// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "configs.h"

#include "ldtk_gen_enums.h"
#include "ldtk_gen_idents.h"

#include <bn_fixed_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

#include <algorithm>

namespace bn
{
class camera_ptr;
}

namespace t2p
{

class item final
{
public:
    static constexpr int ANIM_FRAMES = [] {
        return std::max({
            ITEM_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_acorn_graphics_indexes)
                .def()
                .array_max_length(),
            ITEM_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_cherry_graphics_indexes)
                .def()
                .array_max_length(),
            ITEM_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_item_configs_FIELD_gem_graphics_indexes)
                .def()
                .array_max_length(),
        });
    }();

    using item_animate_action_t = bn::sprite_animate_action<ANIM_FRAMES>;

public:
    item(ldtk::gen::item_kind item_kind, const bn::fixed_point& position, const bn::camera_ptr&);

    void update();

private:
    bn::sprite_ptr _sprite;
    item_animate_action_t _animate_action;
};

} // namespace t2p
