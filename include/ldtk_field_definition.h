// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_field_type.h"
#include "ldtk_gen_idents_fwd.h"

#include <bn_optional.h>
#include <bn_type_id.h>

#include <type_traits>

namespace ldtk
{

class field_definition
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr field_definition(field_type type, bool is_array, const bn::optional<bn::type_id_t>& enum_type,
                               bool can_be_null, gen::level_field_ident identifier, int uid, int array_min_length,
                               int array_max_length)
        : _type(type), _is_array(is_array), _can_be_null(can_be_null), _enum_type(enum_type),
          _identifier((int)identifier), _uid(uid), _array_min_length(array_min_length),
          _array_max_length(array_max_length)
    {
    }

    constexpr field_definition(field_type type, bool is_array, const bn::optional<bn::type_id_t>& enum_type,
                               bool can_be_null, gen::entity_field_ident identifier, int uid, int array_min_length,
                               int array_max_length)
        : _type(type), _is_array(is_array), _can_be_null(can_be_null), _enum_type(enum_type),
          _identifier((int)identifier), _uid(uid), _array_min_length(array_min_length),
          _array_max_length(array_max_length)
    {
    }
    /// @endcond

public:
    /// @brief Value type.
    [[nodiscard]] constexpr auto type() const -> field_type
    {
        return _type;
    }

    /// @brief Whether the field is array or not.
    [[nodiscard]] constexpr auto is_array() const -> bool
    {
        return _is_array;
    }

    /// @brief Enum type.
    [[nodiscard]] constexpr auto enum_type() const -> const bn::optional<bn::type_id_t>&
    {
        return _enum_type;
    }

    /// @brief `true` if the value can be null.
    /// For arrays, `true` means it can contain null values
    /// (exception: array of Points can't have null values).
    [[nodiscard]] constexpr auto can_be_null() const -> bool
    {
        return _can_be_null;
    }

    /// @brief User defined unique identifier
    template <typename Ident>
        requires(std::is_same_v<Ident, gen::level_field_ident> || std::is_same_v<Ident, gen::entity_field_ident>)
    [[nodiscard]] constexpr auto identifier() const -> Ident
    {
        return _identifier;
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

    /// @brief Minimum length of this array field.
    /// @note If no limit is provided, it returns `-1`.
    [[nodiscard]] constexpr auto array_min_length() const -> int
    {
        return _array_min_length;
    }

    /// @brief Maximum length of this array field.
    /// @note If no limit is provided, it returns `-1`.
    [[nodiscard]] constexpr auto array_max_length() const -> int
    {
        return _array_max_length;
    }

private:
    field_type _type;
    bool _is_array;
    bool _can_be_null;
    bn::optional<bn::type_id_t> _enum_type;
    int _identifier;
    int _uid;
    int _array_min_length;
    int _array_max_length;
};

} // namespace ldtk
