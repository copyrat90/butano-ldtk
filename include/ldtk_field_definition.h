#pragma once

#include "ldtk_field_type.h"
#include "ldtk_gen_ident_fwd.h"

#include <bn_optional.h>
#include <bn_type_id.h>

namespace ldtk
{

class field_definition
{
public:
    constexpr field_definition(field_type type, const bn::optional<bn::type_id_t>& enum_type, bool can_be_null,
                               gen::ident identifier, int uid)
        : _type(type), _can_be_null(can_be_null), _enum_type(enum_type), _identifier(identifier), _uid(uid)
    {
    }

public:
    /// @brief Value type.
    [[nodiscard]] constexpr auto type() const -> field_type
    {
        return _type;
    }

    [[nodiscard]] constexpr auto enum_type() const -> const bn::optional<bn::type_id_t>&
    {
        return _enum_type;
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
    bn::optional<bn::type_id_t> _enum_type;
    gen::ident _identifier;
    int _uid;
};

} // namespace ldtk
