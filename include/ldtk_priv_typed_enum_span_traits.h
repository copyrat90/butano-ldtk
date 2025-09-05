// Written with a help of Microsoft Copilot

#pragma once

#include <type_traits>

#include <bn_optional.h>
#include <bn_span.h>

/// @cond DO_NOT_DOCUMENT

namespace ldtk::priv
{

template <typename T>
struct is_span_of_const_scoped_enum : std::false_type
{
};

template <typename ScopedEnum>
struct is_span_of_const_scoped_enum<bn::span<const ScopedEnum>> : std::bool_constant<std::is_scoped_enum_v<ScopedEnum>>
{
};

template <typename T>
inline constexpr bool is_span_of_const_scoped_enum_v = is_span_of_const_scoped_enum<T>::value;

template <typename T>
struct extract_scoped_enum_from_const_span;

template <typename ScopedEnum>
struct extract_scoped_enum_from_const_span<bn::span<const ScopedEnum>>
{
    using type = ScopedEnum;
};

template <typename T>
using extract_scoped_enum_from_const_span_t = extract_scoped_enum_from_const_span<T>::type;

template <typename T>
struct is_span_of_const_optional_scoped_enum : std::false_type
{
};

template <typename ScopedEnum>
struct is_span_of_const_optional_scoped_enum<bn::span<const bn::optional<ScopedEnum>>>
    : std::bool_constant<std::is_scoped_enum_v<ScopedEnum>>
{
};

template <typename T>
inline constexpr bool is_span_of_const_optional_scoped_enum_v = is_span_of_const_optional_scoped_enum<T>::value;

template <typename T>
struct extract_scoped_enum_from_const_optional_span;

template <typename ScopedEnum>
struct extract_scoped_enum_from_const_optional_span<bn::span<const bn::optional<ScopedEnum>>>
{
    using type = ScopedEnum;
};

template <typename T>
using extract_scoped_enum_from_const_optional_span_t = extract_scoped_enum_from_const_optional_span<T>::type;

} // namespace ldtk::priv

/// @endcond
