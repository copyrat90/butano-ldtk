#pragma once

#include <cstdint>

namespace ldtk
{

enum class field_type : std::uint8_t
{
    INT,
    FIXED,
    BOOL,
    STRING,
    COLOR,
    TYPED_ENUM,
    // TILE,
    ENTITY_REF,
    POINT,

    INT_SPAN,
    FIXED_SPAN,
    BOOL_SPAN,
    STRING_SPAN,
    COLOR_SPAN,
    TYPED_ENUM_SPAN,
    // TILE_SPAN,
    ENTITY_REF_SPAN,
    POINT_SPAN,

    OPTIONAL_INT_SPAN,
    OPTIONAL_FIXED_SPAN,
    OPTIONAL_STRING_SPAN,
    OPTIONAL_TYPED_ENUM_SPAN,
    // OPTIONAL_TILE_SPAN,
    OPTIONAL_ENTITY_REF_SPAN,
    OPTIONAL_POINT_SPAN,
};

} // namespace ldtk
