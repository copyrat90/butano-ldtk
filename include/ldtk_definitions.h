#pragma once

#include "ldtk_entity_definition.h"
#include "ldtk_field_definition.h"
#include "ldtk_layer_definition.h"
#include "ldtk_tileset_definition.h"

#include <bn_span.h>

namespace ldtk
{

class definitions
{
public:
    constexpr definitions(const bn::span<const entity_definition>& entities,
                          const bn::span<const layer_definition>& layers,
                          const bn::span<const field_definition>& level_fields,
                          const bn::span<const tileset_definition>& tilesets)
        : _entities(entities), _layers(layers), _level_fields(level_fields), _tilesets(tilesets)
    {
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
