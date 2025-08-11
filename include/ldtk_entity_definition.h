#pragma once

#include "ldtk_field_definition.h"
#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_tag_fwd.h"

#include <bn_fixed_point.h>
#include <bn_span.h>

namespace ldtk
{

class entity_definition
{
public:
    constexpr entity_definition(int height, gen::ident identifier, bn::fixed_point pivot, int uid,
                                int width, const bn::span<const field_definition>& field_defs,
                                const bn::span<const gen::tag>& tags)
        : _height(height), _identifier(identifier), _pivot(pivot), _uid(uid), _width(width),
          _field_defs(field_defs), _tags(tags)
    {
    }

public:
    /// @brief Pixel height
    [[nodiscard]] constexpr auto height() const -> int
    {
        return _height;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief Pivot coordinate (from 0 to 1.0)
    [[nodiscard]] constexpr auto pivot() const -> bn::fixed_point
    {
        return _pivot;
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

    /// @brief Pixel width
    [[nodiscard]] constexpr auto width() const -> int
    {
        return _width;
    }

    /// @brief Array of field definitions
    [[nodiscard]] constexpr auto field_defs() const -> const bn::span<const field_definition>&
    {
        return _field_defs;
    }

    /// @brief An array of strings that classifies this entity
    [[nodiscard]] constexpr auto tags() const -> const bn::span<const gen::tag>&
    {
        return _tags;
    }

private:
    int _height;
    gen::ident _identifier;
    bn::fixed_point _pivot;
    int _uid;
    int _width;
    bn::span<const field_definition> _field_defs;
    bn::span<const gen::tag> _tags;
};

} // namespace ldtk
