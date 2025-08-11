#pragma once

#include "ldtk_gen_ident_fwd.h"
#include "ldtk_int_grid_value_group_info.h"
#include "ldtk_int_grid_value_info.h"
#include "ldtk_layer_type.h"

#include <bn_fixed.h>
#include <bn_span.h>

namespace ldtk
{

class layer_definition
{
public:
    constexpr layer_definition(layer_type type, int grid_size, gen::ident identifier,
                               const bn::span<const int_grid_value_info>& int_grid_values,
                               const bn::span<const int_grid_value_group_info>& int_grid_value_groups,
                               bn::fixed parallax_factor_x, bn::fixed parallax_factor_y, int uid)
        : _type(type), _grid_size(grid_size), _identifier(identifier), _int_grid_values(int_grid_values),
          _int_grid_value_groups(int_grid_value_groups), _parallax_factor_x(parallax_factor_x),
          _parallax_factor_y(parallax_factor_y), _uid(uid)
    {
    }

public:
    /// @brief (Only *IntGrid layer*) Gets the info associated with the IntGrid value for this layer.
    /// @param int_grid_value Actual IntGrid value.
    /// @return Pointer to the info, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto get_int_grid_value_info(int int_grid_value) const -> const int_grid_value_info*
    {
        if (int_grid_value <= 0 || int_grid_value > _int_grid_values.size())
            return nullptr;

        return &*(_int_grid_values.begin() + int_grid_value - 1);
    }

    /// @brief (Only *IntGrid layer*) Gets the info associated with the IntGrid value group for this layer.
    /// @param group_uid IntGrid value group UID.
    /// @return Pointer to the info, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto get_int_grid_value_group_info(int group_uid) const -> const int_grid_value_group_info*
    {
        if (group_uid <= 0 || group_uid > _int_grid_value_groups.size())
            return nullptr;

        return &*(_int_grid_value_groups.begin() + group_uid - 1);
    }

public:
    /// @brief Type of the layer
    [[nodiscard]] constexpr auto type() const -> layer_type
    {
        return _type;
    }

    /// @brief Width and height of the grid in pixels
    [[nodiscard]] constexpr auto grid_size() const -> int
    {
        return _grid_size;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief (Only *IntGrid layer*) An array that defines extra optional info for each IntGrid value.
    /// @note Unlike in JSON specs, the array order is sorted with IntGrid values.
    [[nodiscard]] constexpr auto int_grid_values() const -> const bn::span<const int_grid_value_info>&
    {
        return _int_grid_values;
    }

    /// @brief (Only *IntGrid layer*) Group informations for IntGrid values
    /// @note Unlike in JSON specs, the array order is sorted with IntGrid value group UIDs.
    [[nodiscard]] constexpr auto int_grid_value_groups() const -> const bn::span<const int_grid_value_group_info>&
    {
        return _int_grid_value_groups;
    }

    /// @brief Parallax horizontal factor (from -1 to 1, defaults to 0)
    /// which affects the scrolling speed of this layer, creating a fake 3D (parallax) effect.
    [[nodiscard]] constexpr auto parallax_factor_x() const -> bn::fixed
    {
        return _parallax_factor_x;
    }

    /// @brief Parallax vertical factor (from -1 to 1, defaults to 0)
    /// which affects the scrolling speed of this layer, creating a fake 3D (parallax) effect.
    [[nodiscard]] constexpr auto parallax_factor_y() const -> bn::fixed
    {
        return _parallax_factor_y;
    }

    /// @brief Unique Int Intidentifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

private:
    layer_type _type;
    int _grid_size;
    gen::ident _identifier;
    bn::span<const int_grid_value_info> _int_grid_values;
    bn::span<const int_grid_value_group_info> _int_grid_value_groups;
    bn::fixed _parallax_factor_x;
    bn::fixed _parallax_factor_y;
    int _uid;
};

} // namespace ldtk
