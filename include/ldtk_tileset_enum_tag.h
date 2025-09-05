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
    /// @cond DO_NOT_DOCUMENT
public:
    constexpr tileset_enum_tag(const bn::span<const tile_index>& tile_ids) : _tile_ids(tile_ids)
    {
    }
    /// @endcond

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
