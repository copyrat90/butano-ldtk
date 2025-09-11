// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_gen_idents_fwd.h"

#include <bn_color.h>
#include <bn_optional.h>

namespace ldtk
{

class int_grid_value_info
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr int_grid_value_info(bn::color color, int group_uid,
                                  const bn::optional<gen::layer_int_grid_value_ident>& identifier, int value)
        : _color(color), _group_uid(group_uid), _identifier(identifier), _value(value)
    {
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr int_grid_value_info(const int_grid_value_info&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr int_grid_value_info& operator=(const int_grid_value_info&) = delete;

    /// @brief Defaulted move constructor.
    constexpr int_grid_value_info(int_grid_value_info&&) = default;

    /// @brief Defaulted move assignment operator.
    constexpr int_grid_value_info& operator=(int_grid_value_info&&) = default;

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
    [[nodiscard]] constexpr auto identifier() const -> const bn::optional<gen::layer_int_grid_value_ident>&
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
    bn::optional<gen::layer_int_grid_value_ident> _identifier;
    int _value;
};

} // namespace ldtk
