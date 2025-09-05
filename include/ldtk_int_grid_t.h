// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_int_grid_base.h"

#include <bn_assert.h>
#include <bn_span.h>

#include <concepts>

namespace ldtk
{

template <std::unsigned_integral Int>
class int_grid_t final : public int_grid_base
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr int_grid_t(int c_width, int c_height, const bn::span<const Int>& grid)
        : int_grid_base(c_width, c_height), _grid(grid)
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());
    }
    /// @endcond

    /// @brief Get the size of bytes used for cell storage. \n
    /// (empty: 0, `u8`: 1, `u16`: 2, `u32`: 4)
    [[nodiscard]] constexpr auto cell_storage_size() const -> int override final
    {
        return sizeof(Int);
    }

public:
    /// @brief Get the cell int value with the grid coordinate
    /// @note If you know the concrete type, you can use `cell_int_no_virtual()` instead
    /// to avoid virtual function call overhead.
    [[nodiscard]] constexpr auto cell_int(int grid_x, int grid_y) const -> int override final
    {
        return cell_int_no_virtual(grid_x, grid_y);
    }

public:
    /// @brief Get the cell int value with the grid coordinate
    /// @note This is provided to avoid virtual function call overhead when you know the concrete type.
    [[nodiscard]] constexpr auto cell_int_no_virtual(int grid_x, int grid_y) const -> int
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        return static_cast<int>(_grid[grid_y * c_width() + grid_x]);
    }

private:
    bn::span<const Int> _grid;
};

} // namespace ldtk
