// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <bn_size.h>

namespace ldtk
{

class int_grid_base
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr virtual ~int_grid_base() = default;

    constexpr int_grid_base(int c_width, int c_height) : _c_size(c_width, c_height)
    {
    }
    /// @endcond

    /// @brief Get the size of bytes used for cell storage. \n
    /// (empty: 0, `u8`: 1, `u16`: 2, `u32`: 4)
    [[nodiscard]] constexpr virtual auto cell_storage_size() const -> int = 0;

public:
    /// @brief Get the cell int value with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_int(int grid_x, int grid_y) const -> int = 0;

public:
    /// @brief Grid-based size
    [[nodiscard]] constexpr auto c_size() const -> const bn::size&
    {
        return _c_size;
    }

    /// @brief Grid-based width
    [[nodiscard]] constexpr auto c_width() const -> int
    {
        return c_size().width();
    }

    /// @brief Grid-based height
    [[nodiscard]] constexpr auto c_height() const -> int
    {
        return c_size().height();
    }

private:
    bn::size _c_size;
};

} // namespace ldtk
