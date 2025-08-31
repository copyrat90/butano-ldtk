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
        tile_index index;
        bool x_flip;
        bool y_flip;
    };

public:
    constexpr virtual ~tile_grid_base() = default;

    constexpr tile_grid_base(int c_width, int c_height) : _c_size(c_width, c_height)
    {
    }

    constexpr virtual auto bloated() const -> bool = 0;

public:
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
