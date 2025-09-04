#pragma once

#include "ldtk_tile_grid_base.h"

#include <bn_assert.h>
#include <bn_span.h>

#include <cstdint>
#include <type_traits>

namespace ldtk
{

/// @cond DO_NOT_DOCUMENT

struct compact_tile_traits
{
    using cell = std::uint8_t;

    union tile {
        cell data;
        struct
        {
            cell index : 6;
            bool x_flip : 1;
            bool y_flip : 1;
        } fields;
    };
};

struct bloated_tile_traits
{
    using cell = std::uint16_t;

    union tile {
        cell data;
        struct
        {
            cell index : 14;
            bool x_flip : 1;
            bool y_flip : 1;
        } fields;
    };
};

/// @endcond

template <bool Bloated>
class tile_grid_t final : public tile_grid_base,
                          private std::conditional_t<Bloated, bloated_tile_traits, compact_tile_traits>
{
private:
    using tile_traits = std::conditional_t<Bloated, bloated_tile_traits, compact_tile_traits>;

    using typename tile_traits::cell;
    using typename tile_traits::tile;

public:
    /// @cond DO_NOT_DOCUMENT
    constexpr tile_grid_t(int c_width, int c_height, const bn::span<const cell>& grid)
        : tile_grid_base(c_width, c_height), _grid(grid)
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());
    }
    /// @endcond

    /// @brief Get whether the cell storage is bloated (`u16`) or not (`u8`).
    [[nodiscard]] constexpr auto bloated() const -> bool override final
    {
        return Bloated;
    }

public:
    /// @brief Get the cell tile info with the grid coordinate
    /// @note If you know the concrete type, you can use `cell_tile_info_no_virtual()` instead
    /// to avoid virtual function call overhead.
    [[nodiscard]] constexpr auto cell_tile_info(int grid_x, int grid_y) const -> tile_info override final
    {
        return cell_tile_info_no_virtual(grid_x, grid_y);
    }

    /// @brief Get the cell tile index with the grid coordinate
    /// @note If you know the concrete type, you can use `cell_tile_index_no_virtual()` instead
    /// to avoid virtual function call overhead.
    [[nodiscard]] constexpr auto cell_tile_index(int grid_x, int grid_y) const -> tile_index override final
    {
        return cell_tile_index_no_virtual(grid_x, grid_y);
    }

    /// @brief Get the cell tile X flip with the grid coordinate
    /// @note If you know the concrete type, you can use `cell_tile_x_flip_no_virtual()` instead
    /// to avoid virtual function call overhead.
    [[nodiscard]] constexpr auto cell_tile_x_flip(int grid_x, int grid_y) const -> bool override final
    {
        return cell_tile_x_flip_no_virtual(grid_x, grid_y);
    }

    /// @brief Get the cell tile Y flip with the grid coordinate
    /// @note If you know the concrete type, you can use `cell_tile_y_flip_no_virtual()` instead
    /// to avoid virtual function call overhead.
    [[nodiscard]] constexpr auto cell_tile_y_flip(int grid_x, int grid_y) const -> bool override final
    {
        return cell_tile_y_flip_no_virtual(grid_x, grid_y);
    }

public:
    /// @brief Get the cell tile info with the grid coordinate
    /// @note This is provided to avoid virtual function call overhead when you know the concrete type.
    [[nodiscard]] constexpr auto cell_tile_info_no_virtual(int grid_x, int grid_y) const -> tile_info
    {
        tile t = get_tile(grid_x, grid_y);

        return tile_info{
            .index = t.fields.index,
            .x_flip = t.fields.x_flip,
            .y_flip = t.fields.y_flip,
        };
    }

    /// @brief Get the cell tile index with the grid coordinate
    /// @note This is provided to avoid virtual function call overhead when you know the concrete type.
    [[nodiscard]] constexpr auto cell_tile_index_no_virtual(int grid_x, int grid_y) const -> tile_index
    {
        return get_tile(grid_x, grid_y).fields.index;
    }

    /// @brief Get the cell tile X flip with the grid coordinate
    /// @note This is provided to avoid virtual function call overhead when you know the concrete type.
    [[nodiscard]] constexpr auto cell_tile_x_flip_no_virtual(int grid_x, int grid_y) const -> bool
    {
        return get_tile(grid_x, grid_y).fields.x_flip;
    }

    /// @brief Get the cell tile Y flip with the grid coordinate
    /// @note This is provided to avoid virtual function call overhead when you know the concrete type.
    [[nodiscard]] constexpr auto cell_tile_y_flip_no_virtual(int grid_x, int grid_y) const -> bool
    {
        return get_tile(grid_x, grid_y).fields.y_flip;
    }

private:
    [[nodiscard]] constexpr auto get_tile(int grid_x, int grid_y) const -> tile
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        return tile{_grid[grid_y * c_width() + grid_x]};
    }

private:
    bn::span<const cell> _grid;
};

} // namespace ldtk
