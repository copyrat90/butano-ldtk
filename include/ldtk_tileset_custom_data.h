// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_tile_index.h"

#include <bn_string_view.h>

namespace ldtk
{

class tileset_custom_data
{
    /// @cond DO_NOT_DOCUMENT
public:
    constexpr tileset_custom_data(const bn::string_view& data, tile_index tile_id) : _data(data), _tile_id(tile_id)
    {
    }
    /// @endcond

public:
    [[nodiscard]] constexpr auto data() const -> const bn::string_view&
    {
        return _data;
    }

    [[nodiscard]] constexpr auto tile_id() const -> tile_index
    {
        return _tile_id;
    }

private:
    bn::string_view _data;
    tile_index _tile_id;
};

} // namespace ldtk
