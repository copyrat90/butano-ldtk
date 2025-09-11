// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_tile_index.h"

#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class tileset_enum_tag
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr tileset_enum_tag(const bn::span<const tile_index>& tile_ids) : _tile_ids(tile_ids)
    {
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr tileset_enum_tag(const tileset_enum_tag&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr tileset_enum_tag& operator=(const tileset_enum_tag&) = delete;

    /// @brief Defaulted move constructor.
    constexpr tileset_enum_tag(tileset_enum_tag&&) = default;

    /// @brief Defaulted move assignment operator.
    constexpr tileset_enum_tag& operator=(tileset_enum_tag&&) = default;

public:
    [[nodiscard]] constexpr auto has_tile_id(tile_index id) const -> bool
    {
        return std::ranges::contains(_tile_ids, id);
    }

public:
    [[nodiscard]] constexpr auto tile_ids() const -> const bn::span<const tile_index>&
    {
        return _tile_ids;
    }

private:
    bn::span<const tile_index> _tile_ids;
};

} // namespace ldtk
