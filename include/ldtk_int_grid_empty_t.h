#pragma once

#include "ldtk_int_grid_base.h"

#include <bn_assert.h>

namespace ldtk
{

class int_grid_empty_t : public int_grid_base
{
public:
    constexpr int_grid_empty_t(int c_width, int c_height) : int_grid_base(c_width, c_height)
    {
    }

public:
    [[nodiscard]] constexpr auto cell_int(int grid_x, int grid_y) const -> int override
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_x >= 0 && grid_x < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        return 0;
    }
};

} // namespace ldtk
