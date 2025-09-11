// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_gen_idents_fwd.h"

#include <bn_optional.h>

namespace ldtk
{

class int_grid_value_group_info
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr int_grid_value_group_info(const bn::optional<gen::layer_int_grid_value_group_ident>& identifier, int uid)
        : _identifier(identifier), _uid(uid)
    {
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr int_grid_value_group_info(const int_grid_value_group_info&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr int_grid_value_group_info& operator=(const int_grid_value_group_info&) = delete;

    /// @brief Defaulted move constructor.
    constexpr int_grid_value_group_info(int_grid_value_group_info&&) = default;

    /// @brief Defaulted move assignment operator.
    constexpr int_grid_value_group_info& operator=(int_grid_value_group_info&&) = default;

public:
    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> const bn::optional<gen::layer_int_grid_value_group_ident>&
    {
        return _identifier;
    }

    /// @brief Group unique ID
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

private:
    bn::optional<gen::layer_int_grid_value_group_ident> _identifier;
    int _uid;
};

} // namespace ldtk
