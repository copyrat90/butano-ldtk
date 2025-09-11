// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_tile_index.h"

#include <bn_size.h>

namespace ldtk
{

class tile_grid_base
{
public:
    struct tile_info
    {
        /// @brief Index of the tile.
        tile_index index;
        /// @brief Whether the tile is flipped horizontally or not.
        bool x_flip;
        /// @brief Whether the tile is flipped vertically or not.
        bool y_flip;
    };

public:
    /// @cond DO_NOT_DOCUMENT
    constexpr virtual ~tile_grid_base() = default;

    constexpr tile_grid_base(int c_width, int c_height) : _c_size(c_width, c_height)
    {
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr tile_grid_base(const tile_grid_base&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr tile_grid_base& operator=(const tile_grid_base&) = delete;

    /// @brief Defaulted move constructor.
    constexpr tile_grid_base(tile_grid_base&&) = default;

    /// @brief Defaulted move assignment operator.
    constexpr tile_grid_base& operator=(tile_grid_base&&) = default;

    /// @brief Get whether the cell storage is bloated (`u16`) or not (`u8`).
    [[nodiscard]] constexpr virtual auto bloated() const -> bool = 0;

public:
    /// @brief Get the cell tile info with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_info(int grid_x, int grid_y) const -> tile_info = 0;

    /// @brief Get the cell tile index with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_index(int grid_x, int grid_y) const -> tile_index = 0;

    /// @brief Get the cell tile X flip with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_x_flip(int grid_x, int grid_y) const -> bool = 0;

    /// @brief Get the cell tile Y flip with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_y_flip(int grid_x, int grid_y) const -> bool = 0;

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
