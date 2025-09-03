// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_entity.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include <algorithm>

namespace t2p
{

inline constexpr const ldtk::entity& PLAYER_CONFIGS = [] constexpr -> const ldtk::entity& {
    const ldtk::layer& configs_layer =
        ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::your_typical_2d_platformer)
            .get_layer(ldtk::gen::layer_ident::entities);

    const auto iter = std::ranges::find_if(configs_layer.entity_instances(), [](const ldtk::entity& config_entity) {
        return config_entity.identifier() == ldtk::gen::entity_ident::player_configs;
    });

    return *iter; // It should exist
}();

inline constexpr const ldtk::entity& ITEM_CONFIGS = [] constexpr -> const ldtk::entity& {
    const ldtk::layer& configs_layer =
        ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::your_typical_2d_platformer)
            .get_layer(ldtk::gen::layer_ident::entities);

    const auto iter = std::ranges::find_if(configs_layer.entity_instances(), [](const ldtk::entity& config_entity) {
        return config_entity.identifier() == ldtk::gen::entity_ident::item_configs;
    });

    return *iter; // It should exist
}();

inline constexpr const ldtk::entity& MOB_CONFIGS = [] constexpr -> const ldtk::entity& {
    const ldtk::layer& configs_layer =
        ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::your_typical_2d_platformer)
            .get_layer(ldtk::gen::layer_ident::entities);

    const auto iter = std::ranges::find_if(configs_layer.entity_instances(), [](const ldtk::entity& config_entity) {
        return config_entity.identifier() == ldtk::gen::entity_ident::mob_configs;
    });

    return *iter; // It should exist
}();

} // namespace t2p
