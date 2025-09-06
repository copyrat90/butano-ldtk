// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <cstdint>

namespace ldtk
{

enum class field_type : std::uint8_t
{
    UINT_8,
    INT_8,
    UINT_16,
    INT_16,
    INT_32,
    FIXED,
    BOOL,
    STRING,
    COLOR,
    TYPED_ENUM,
    // TILE,
    ENTITY_REF,
    POINT,

    UINT_8_SPAN,
    INT_8_SPAN,
    UINT_16_SPAN,
    INT_16_SPAN,
    INT_32_SPAN,
    FIXED_SPAN,
    BOOL_SPAN,
    STRING_SPAN,
    COLOR_SPAN,
    TYPED_ENUM_SPAN,
    // TILE_SPAN,
    ENTITY_REF_SPAN,
    POINT_SPAN,

    OPTIONAL_UINT_8_SPAN,
    OPTIONAL_INT_8_SPAN,
    OPTIONAL_UINT_16_SPAN,
    OPTIONAL_INT_16_SPAN,
    OPTIONAL_INT_32_SPAN,
    OPTIONAL_FIXED_SPAN,
    OPTIONAL_STRING_SPAN,
    OPTIONAL_TYPED_ENUM_SPAN,
    // OPTIONAL_TILE_SPAN,
    OPTIONAL_ENTITY_REF_SPAN,
    OPTIONAL_POINT_SPAN,
};

} // namespace ldtk
