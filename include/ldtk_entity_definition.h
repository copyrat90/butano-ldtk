// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_field_definition.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_tags_fwd.h"

#include <bn_fixed_point.h>
#include <bn_size.h>
#include <bn_span.h>

#include <cstdint>

namespace ldtk
{

class entity_definition
{
public:
    /// @brief How the `max_count()` limits the number of max entities
    enum class limit_scope_kind : std::uint8_t
    {
        PER_LAYER,
        PER_LEVEL,
        PER_WORLD
    };

public:
    /// @cond DO_NOT_DOCUMENT
    constexpr entity_definition(const bn::size& size, gen::entity_ident identifier, bn::fixed_point pivot, int uid,
                                const bn::span<const field_definition>& field_defs,
                                const bn::span<const gen::entity_tag>& tags, std::uint16_t max_count,
                                limit_scope_kind limit_scope)
        : _size(size), _identifier(identifier), _pivot(pivot), _uid(uid), _field_defs(field_defs), _tags(tags),
          _max_count(max_count), _limit_scope(limit_scope)
    {
    }
    /// @endcond

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

    /// @brief Max entities count. The scope is indicated via `limit_scope()`
    /// @note `0` means it's unlimited.
    [[nodiscard]] constexpr auto max_count() const -> int
    {
        return _max_count;
    }

    /// @brief Indicates how the `max_count()` limits the number of max entities
    [[nodiscard]] constexpr auto limit_scope() const -> limit_scope_kind
    {
        return _limit_scope;
    }

private:
    bn::size _size;
    gen::entity_ident _identifier;
    bn::fixed_point _pivot;
    int _uid;
    bn::span<const field_definition> _field_defs;
    bn::span<const gen::entity_tag> _tags;
    std::uint16_t _max_count;
    limit_scope_kind _limit_scope;
};

} // namespace ldtk
