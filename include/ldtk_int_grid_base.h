#pragma once

namespace ldtk
{

class int_grid_base
{
public:
    constexpr virtual ~int_grid_base() = default;

    constexpr int_grid_base(int c_width, int c_height) : _c_width(c_width), _c_height(c_height)
    {
    }

public:
    /// @brief Get the cell int value with the grid coordinate
    [[nodiscard]] constexpr virtual auto cell_int(int grid_x, int grid_y) const -> int = 0;

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
