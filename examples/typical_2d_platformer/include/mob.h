// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "configs.h"

#include "ldtk_gen_enums.h"
#include "ldtk_gen_idents.h"

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_point.h>
#include <bn_span_fwd.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

#include <algorithm>

namespace bn
{
class camera_ptr;
}

namespace t2p
{

class mob final
{
public:
    static constexpr int ANIM_FRAMES = [] {
        return std::max({
            MOB_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_ant_graphics_indexes)
                .def()
                .array_max_length(),
            MOB_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bat_graphics_indexes)
                .def()
                .array_max_length(),
            MOB_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bear_graphics_indexes)
                .def()
                .array_max_length(),
            MOB_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_bettle_graphics_indexes)
                .def()
                .array_max_length(),
            MOB_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_mob_configs_FIELD_opossum_graphics_indexes)
                .def()
                .array_max_length(),
        });
    }();

    using mob_animate_action_t = bn::sprite_animate_action<ANIM_FRAMES>;

public:
    mob(ldtk::gen::mob_kind mob_kind, const bn::span<const ldtk::gen::item_kind>& loots,
        const bn::optional<bn::point>& patrol, int patrol_duration_updates, const bn::fixed_point& position,
        const bn::camera_ptr& camera);

    void update();

private:
    bn::sprite_ptr _mob_sprite;
    mob_animate_action_t _mob_anim_action;
    bn::optional<bn::sprite_move_loop_action> _mob_patrol_action;
};

} // namespace t2p
