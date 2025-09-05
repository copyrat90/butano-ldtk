// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <concepts>
#include <type_traits>

#include <bn_optional.h>
#include <bn_span.h>

/// @cond DO_NOT_DOCUMENT

namespace ldtk::priv
{

template <typename T>
concept span_of_const_optional_scoped_enum =
    std::same_as<T, bn::span<const bn::optional<typename T::value_type::value_type>>> &&
    std::is_scoped_enum_v<typename T::value_type::value_type>;

template <typename T>
concept span_of_const_scoped_enum =
    std::same_as<T, bn::span<const typename T::value_type>> && std::is_scoped_enum_v<typename T::value_type>;

} // namespace ldtk::priv

/// @endcond
