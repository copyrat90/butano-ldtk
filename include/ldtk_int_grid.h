// SPDX-FileCopyrightText: Copyright 2025-2026 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <bn_assert.h>
#include <bn_size.h>
#include <bn_span.h>

#include <cstdint>
#include <memory>
#include <utility>

namespace ldtk
{

class int_grid final
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr int_grid(int c_width, int c_height) : _c_size(c_width, c_height), _cell_storage_size(0)
    {
        std::construct_at(&_grid.empty, 0);
    }

    constexpr int_grid(int c_width, int c_height, const bn::span<const std::uint8_t>& grid)
        : _c_size(c_width, c_height), _cell_storage_size(sizeof(std::uint8_t))
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());

        std::construct_at(&_grid.u8_span, grid);
    }

    constexpr int_grid(int c_width, int c_height, const bn::span<const std::uint16_t>& grid)
        : _c_size(c_width, c_height), _cell_storage_size(sizeof(std::uint16_t))
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());

        std::construct_at(&_grid.u16_span, grid);
    }

    constexpr int_grid(int c_width, int c_height, const bn::span<const std::uint32_t>& grid)
        : _c_size(c_width, c_height), _cell_storage_size(sizeof(std::uint32_t))
    {
        BN_ASSERT(c_width * c_height == grid.size(), "Invalid grid dimensions: ", c_width, "x", c_height,
                  ", != ", grid.size());

        std::construct_at(&_grid.u32_span, grid);
    }

    constexpr ~int_grid()
    {
        switch (_cell_storage_size)
        {
        case 0:
            std::destroy_at(&_grid.empty);
            break;
        case 1:
            std::destroy_at(&_grid.u8_span);
            break;
        case 2:
            std::destroy_at(&_grid.u16_span);
            break;
        case 4:
            std::destroy_at(&_grid.u32_span);
            break;
        default:
            BN_ERROR("Incorrect cell storage size");
        }
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr int_grid(const int_grid&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr int_grid& operator=(const int_grid&) = delete;

    /// @brief Defaulted move constructor.
    constexpr int_grid(int_grid&&) = default;

public:
    /// @brief Get the cell int value with the grid coordinate
    [[nodiscard]] constexpr auto cell_int(int grid_x, int grid_y) const -> int
    {
        BN_ASSERT(grid_x >= 0 && grid_x < c_width(), "Invalid grid_x: ", grid_x, " [0..", c_width(), ")");
        BN_ASSERT(grid_y >= 0 && grid_y < c_height(), "Invalid grid_y: ", grid_y, " [0..", c_height(), ")");

        switch (_cell_storage_size)
        {
        case 0:
            return 0;
        case 1:
            return static_cast<int>(_grid.u8_span[grid_y * c_width() + grid_x]);
        case 2:
            return static_cast<int>(_grid.u16_span[grid_y * c_width() + grid_x]);
        case 4:
            return static_cast<int>(_grid.u32_span[grid_y * c_width() + grid_x]);
        default:
            break;
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
    const std::uint8_t _cell_storage_size;

    union grid_t {
        char empty;

        bn::span<const std::uint8_t> u8_span;
        bn::span<const std::uint16_t> u16_span;
        bn::span<const std::uint32_t> u32_span;

        constexpr grid_t()
        {
        }

        constexpr ~grid_t()
        {
        }
    } _grid;
};

} // namespace ldtk
