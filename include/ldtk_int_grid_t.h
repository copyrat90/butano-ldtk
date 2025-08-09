#pragma once

#include "ldtk_int_grid_base.h"

#include <bn_assert.h>
#include <bn_span.h>

#include <concepts>

namespace ldtk
{

template <std::unsigned_integral Int>
class int_grid_t : public int_grid_base
{
public:
    constexpr int_grid_t(int c_width, int c_height, const bn::span<const Int>& grid)
        : int_grid_base(c_width, c_height), _grid(grid)
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());
    }

public:
    [[nodiscard]] constexpr auto cell_int(int grid_x, int grid_y) const -> int override
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_x >= 0 && grid_x < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        return static_cast<int>(_grid[grid_y * c_width() + grid_x]);
    }

private:
    bn::span<const Int> _grid;
};

} // namespace ldtk
