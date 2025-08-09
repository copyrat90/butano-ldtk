#pragma once

namespace ldtk
{

class tile_grid_base
{
public:
    constexpr virtual ~tile_grid_base() = default;

    constexpr tile_grid_base(int c_width, int c_height) : _c_width(c_width), _c_height(c_height)
    {
    }

public:
    // @brief Get the cell tile index with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_index(int grid_x, int grid_y) const -> int = 0;

    // @brief Get the cell tile X flip with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_x_flip(int grid_x, int grid_y) const -> bool = 0;

    // @brief Get the cell tile Y flip with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_tile_y_flip(int grid_x, int grid_y) const -> bool = 0;

public:
    /// @brief Grid-based width
    [[nodiscard]] constexpr auto c_width() const -> int
    {
        return _c_width;
    }

    /// @brief Grid-based height
    [[nodiscard]] constexpr auto c_height() const -> int
    {
        return _c_height;
    }

private:
    int _c_width;
    int _c_height;
};

} // namespace ldtk
