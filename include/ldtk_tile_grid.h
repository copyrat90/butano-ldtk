// SPDX-FileCopyrightText: Copyright 2025-2026 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_tile_index.h"

#include <bn_assert.h>
#include <bn_size.h>
#include <bn_span.h>

#include <cstdint>
#include <memory>
#include <utility>

namespace ldtk
{

class tile_grid final
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

private:
    using compact_cell = std::uint8_t;

    union compact_tile {
        compact_cell data;
        struct
        {
            compact_cell index : 6;
            bool x_flip : 1;
            bool y_flip : 1;
        } fields;
    };

    using bloated_cell = std::uint16_t;

    union bloated_tile {
        bloated_cell data;
        struct
        {
            bloated_cell index : 14;
            bool x_flip : 1;
            bool y_flip : 1;
        } fields;
    };

public:
    /// @cond DO_NOT_DOCUMENT
    constexpr tile_grid(int c_width, int c_height, const bn::span<const compact_cell>& grid)
        : _c_size(c_width, c_height), _bloated(false)
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());

        std::construct_at(&_grid.compact_span, grid);
    }

    constexpr tile_grid(int c_width, int c_height, const bn::span<const bloated_cell>& grid)
        : _c_size(c_width, c_height), _bloated(true)
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());

        std::construct_at(&_grid.bloated_span, grid);
    }

    constexpr ~tile_grid()
    {
        if (_bloated)
            std::destroy_at(&_grid.bloated_span);
        else
            std::destroy_at(&_grid.compact_span);
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr tile_grid(const tile_grid&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr tile_grid& operator=(const tile_grid&) = delete;

    /// @brief Defaulted move constructor.
    constexpr tile_grid(tile_grid&&) = default;

public:
    /// @brief Get the cell tile info with the grid coordinate
    [[nodiscard]] constexpr auto cell_tile_info(int grid_x, int grid_y) const -> tile_info
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        if (_bloated)
        {
            const bloated_tile tile{_grid.bloated_span[grid_y * c_width() + grid_x]};
            return tile_info{
                .index = tile.fields.index,
                .x_flip = tile.fields.x_flip,
                .y_flip = tile.fields.y_flip,
            };
        }
        else
        {
            const compact_tile tile{_grid.compact_span[grid_y * c_width() + grid_x]};
            return tile_info{
                .index = tile.fields.index,
                .x_flip = tile.fields.x_flip,
                .y_flip = tile.fields.y_flip,
            };
        }

        std::unreachable();
    }

    /// @brief Get the cell tile index with the grid coordinate
    [[nodiscard]] constexpr auto cell_tile_index(int grid_x, int grid_y) const -> tile_index
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        if (_bloated)
        {
            const bloated_tile tile{_grid.bloated_span[grid_y * c_width() + grid_x]};
            return tile.fields.index;
        }
        else
        {
            const compact_tile tile{_grid.compact_span[grid_y * c_width() + grid_x]};
            return tile.fields.index;
        }

        std::unreachable();
    }

    /// @brief Get the cell tile X flip with the grid coordinate
    [[nodiscard]] constexpr auto cell_tile_x_flip(int grid_x, int grid_y) const -> bool
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        if (_bloated)
        {
            const bloated_tile tile{_grid.bloated_span[grid_y * c_width() + grid_x]};
            return tile.fields.x_flip;
        }
        else
        {
            const compact_tile tile{_grid.compact_span[grid_y * c_width() + grid_x]};
            return tile.fields.x_flip;
        }

        std::unreachable();
    }

    /// @brief Get the cell tile Y flip with the grid coordinate
    [[nodiscard]] constexpr auto cell_tile_y_flip(int grid_x, int grid_y) const -> bool
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        if (_bloated)
        {
            const bloated_tile tile{_grid.bloated_span[grid_y * c_width() + grid_x]};
            return tile.fields.y_flip;
        }
        else
        {
            const compact_tile tile{_grid.compact_span[grid_y * c_width() + grid_x]};
            return tile.fields.y_flip;
        }

        std::unreachable();
    }

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
    const bn::size _c_size;
    const bool _bloated;

    union grid_t {
        char empty;

        bn::span<const compact_cell> compact_span;
        bn::span<const bloated_cell> bloated_span;

        constexpr grid_t()
        {
        }

        constexpr ~grid_t()
        {
        }
    } _grid;
};

} // namespace ldtk
