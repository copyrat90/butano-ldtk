#pragma once

#include "ldtk_field_type.h"
#include "ldtk_gen_ident_fwd.h"

namespace ldtk
{

class field_definition
{
public:
    constexpr field_definition(field_type type, bool can_be_null, gen::ident identifier, int uid)
        : _type(type), _can_be_null(can_be_null), _identifier(identifier), _uid(uid)
    {
    }

public:
    /// @brief Value type.
    [[nodiscard]] constexpr auto type() const -> field_type
    {
        return _type;
    }

    /// @brief TRUE if the value can be null.
    /// For arrays, TRUE means it can contain null values
    /// (exception: array of Points can't have null values).
    [[nodiscard]] constexpr auto can_be_null() const -> bool
    {
        return _can_be_null;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

private:
    field_type _type;
    bool _can_be_null;
    gen::ident _identifier;
    int _uid;
};

} // namespace ldtk
