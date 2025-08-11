#pragma once

#include "ldtk_gen_ident_fwd.h"

#include <bn_color.h>
#include <bn_optional.h>

namespace ldtk
{

class int_grid_value_info
{
public:
    constexpr int_grid_value_info(bn::color color, int group_uid, const bn::optional<gen::ident>& identifier, int value)
        : _color(color), _group_uid(group_uid), _identifier(identifier), _value(value)
    {
    }

public:
    [[nodiscard]] constexpr auto color() const -> bn::color
    {
        return _color;
    }

    /// @brief Parent group identifier (0 if none)
    [[nodiscard]] constexpr auto group_uid() const -> int
    {
        return _group_uid;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> const bn::optional<gen::ident>&
    {
        return _identifier;
    }

    /// @brief The IntGrid value itself
    [[nodiscard]] constexpr auto value() const -> int
    {
        return _value;
    }

private:
    bn::color _color;
    int _group_uid;
    bn::optional<gen::ident> _identifier;
    int _value;
};

} // namespace ldtk
