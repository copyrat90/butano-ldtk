// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_entity_definition.h"
#include "ldtk_field_definition.h"
#include "ldtk_layer_definition.h"
#include "ldtk_tileset_definition.h"

#include "ldtk_gen_idents_fwd.h"

#include <bn_span.h>

namespace ldtk
{

class definitions
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr definitions(const bn::span<const entity_definition>& entities,
                          const bn::span<const layer_definition>& layers,
                          const bn::span<const field_definition>& level_fields,
                          const bn::span<const tileset_definition>& tilesets)
        : _entities(entities), _layers(layers), _level_fields(level_fields), _tilesets(tilesets)
    {
    }
    /// @endcond

public:
    /// @brief Looks up a entity definition with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the entity to search for.
    /// @return Reference to the entity definition.
    [[nodiscard]] constexpr auto get_entity_def(gen::entity_ident identifier) const -> const entity_definition&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::entity_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _entities.size(), "Out of bound identifier (gen::entity_ident)", (int)identifier);

        return _entities.data()[(int)identifier];
    }

    /// @brief Looks up a layer definition with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the layer to search for.
    /// @return Reference to the layer definition.
    [[nodiscard]] constexpr auto get_layer_def(gen::layer_ident identifier) const -> const layer_definition&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _layers.size(), "Out of bound identifier (gen::layer_ident)", (int)identifier);

        return _layers.data()[(int)identifier];
    }

    /// @brief Looks up a level field definition with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the level field to search for.
    /// @return Reference to the level field definition.
    [[nodiscard]] constexpr auto get_level_field_def(gen::level_field_ident identifier) const -> const field_definition&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::level_field_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _level_fields.size(), "Out of bound identifier (gen::level_field_ident)",
                  (int)identifier);

        return _level_fields.data()[(int)identifier];
    }

    /// @brief Looks up a tileset definition with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the tileset to search for.
    /// @return Reference to the tileset definition.
    [[nodiscard]] constexpr auto get_tileset_def(gen::tileset_ident identifier) const -> const tileset_definition&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::tileset_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _tilesets.size(), "Out of bound identifier (gen::tileset_ident)", (int)identifier);

        return _tilesets.data()[(int)identifier];
    }

public:
    /// @brief All entities definitions, including their custom fields
    [[nodiscard]] constexpr auto entities() const -> const bn::span<const entity_definition>&
    {
        return _entities;
    }

    /// @brief All layer definitions
    [[nodiscard]] constexpr auto layers() const -> const bn::span<const layer_definition>&
    {
        return _layers;
    }

    /// @brief All custom fields available to all levels.
    [[nodiscard]] constexpr auto level_fields() const -> const bn::span<const field_definition>&
    {
        return _level_fields;
    }

    /// @brief All tilesets
    [[nodiscard]] constexpr auto tilesets() const -> const bn::span<const tileset_definition>&
    {
        return _tilesets;
    }

private:
    bn::span<const entity_definition> _entities;
    bn::span<const layer_definition> _layers;
    bn::span<const field_definition> _level_fields;
    bn::span<const tileset_definition> _tilesets;
};

} // namespace ldtk
