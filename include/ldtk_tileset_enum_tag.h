#pragma once

#include "ldtk_tile_index.h"

#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class tileset_enum_tag
{
public:
    constexpr tileset_enum_tag(int enum_value_id, const bn::span<const tile_index>& tile_ids)
        : _enum_value_id(enum_value_id), _tile_ids(tile_ids)
    {
    }

public:
    [[nodiscard]] constexpr auto has_tile_id(tile_index id) const -> bool
    {
        return std::ranges::contains(_tile_ids, id);
    }

public:
    [[nodiscard]] constexpr auto enum_value_id() const -> int
    {
        return _enum_value_id;
    }

    [[nodiscard]] constexpr auto tile_ids() const -> const bn::span<const tile_index>&
    {
        return _tile_ids;
    }

private:
    int _enum_value_id;
    bn::span<const tile_index> _tile_ids;
};

} // namespace ldtk
