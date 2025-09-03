// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "configs.h"

#include "ldtk_gen_idents.h"

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

#include <algorithm>

namespace bn
{
class camera_ptr;
}

namespace t2p
{

class player;

class player_animation final
{
public:
    enum class kind
    {
        IDLE,
        RUN,
        JUMP,
        FALL,
        LOOK_UP,
        CROUCH,
        WALL_GRAB
    };

    static constexpr int ANIM_FRAMES = [] {
        return std::max({
            PLAYER_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_idle_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_run_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_jump_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_fall_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS
                .get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_look_up_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS.get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_crouch_graphics_indexes)
                .def()
                .array_max_length(),
            PLAYER_CONFIGS
                .get_field(ldtk::gen::entity_field_ident::ENTITY_player_configs_FIELD_wall_grab_graphics_indexes)
                .def()
                .array_max_length(),
        });
    }();

    using player_animate_action_t = bn::sprite_animate_action<ANIM_FRAMES>;

public:
    player_animation(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void handle_input(const player&);

    void update();

public:
    auto animation_kind() const -> player_animation::kind;
    void start(player_animation::kind anim_kind);

    auto flip() const -> bool;
    void set_flip(bool flip);

    void put_above();

public:
    auto position() const -> bn::fixed_point;
    void set_position(const bn::fixed_point& position);
    auto x() const -> bn::fixed;
    void set_x(bn::fixed x);
    auto y() const -> bn::fixed;
    void set_y(bn::fixed y);

private:
    bn::sprite_ptr _sprite;
    kind _anim_kind;
    bn::optional<player_animate_action_t> _anim_action;
};

} // namespace t2p
