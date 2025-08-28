#pragma once

#include "ldtk_field_definition.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_tags_fwd.h"

#include <bn_fixed_point.h>
#include <bn_size.h>
#include <bn_span.h>

namespace ldtk
{

class entity_definition
{
public:
    constexpr entity_definition(const bn::size& size, gen::entity_ident identifier, bn::fixed_point pivot, int uid,
                                const bn::span<const field_definition>& field_defs,
                                const bn::span<const gen::entity_tag>& tags)
        : _size(size), _identifier(identifier), _pivot(pivot), _uid(uid), _field_defs(field_defs), _tags(tags)
    {
    }

public:
    /// @brief Pixel size
    [[nodiscard]] constexpr auto size() const -> const bn::size&
    {
        return _size;
    }

    /// @brief Pixel width
    [[nodiscard]] constexpr auto width() const -> int
    {
        return size().width();
    }

    /// @brief Pixel height
    [[nodiscard]] constexpr auto height() const -> int
    {
        return size().height();
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::entity_ident
    {
        return _identifier;
    }

    /// @brief Pivot coordinate (from 0 to 1.0)
    [[nodiscard]] constexpr auto pivot() const -> const bn::fixed_point&
    {
        return _pivot;
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

    /// @brief Array of field definitions
    [[nodiscard]] constexpr auto field_defs() const -> const bn::span<const field_definition>&
    {
        return _field_defs;
    }

    /// @brief An array of strings that classifies this entity
    [[nodiscard]] constexpr auto tags() const -> const bn::span<const gen::entity_tag>&
    {
        return _tags;
    }

private:
    bn::size _size;
    gen::entity_ident _identifier;
    bn::fixed_point _pivot;
    int _uid;
    bn::span<const field_definition> _field_defs;
    bn::span<const gen::entity_tag> _tags;
};

} // namespace ldtk
