#pragma once

#include <bn_assert.h>
#include <bn_optional.h>
#include <bn_span.h>
#include <bn_type_id.h>

#include <type_traits>

/// @cond DO_NOT_DOCUMENT

namespace ldtk::priv
{

/// @brief Enum span that's stored in the `field` with `bn::type_id_t`
class optional_typed_enum_span
{
private:
    bn::type_id_t _type_id;
    const void* _optional_span_begin;
    const void* _optional_span_end;

public:
    template <typename Enum>
        requires std::is_scoped_enum_v<Enum>
    constexpr optional_typed_enum_span(bn::span<const bn::optional<Enum>> span)
        : _type_id(bn::type_id<Enum>()), _optional_span_begin(span.data()),
          _optional_span_end(span.data() + span.size())
    {
    }

public:
    /// @brief Extract the concrete optional enum span from this optional typed enum span.
    /// @tparam Enum Type of the enum to be extracted.
    /// @note If you provide an incorrect type argument to `Enum`, it will error out. \n
    /// If you don't like that, you can use `get_optional()` instead.
    template <typename Enum>
        requires std::is_scoped_enum_v<Enum>
    [[nodiscard]] constexpr auto get() const -> bn::span<const bn::optional<Enum>>
    {
        BN_ASSERT(bn::type_id<Enum>() == _type_id, "Invalid Enum type");

        return bn::span<const bn::optional<Enum>>(static_cast<const bn::optional<Enum>*>(_optional_span_begin),
                                                  static_cast<const bn::optional<Enum>*>(_optional_span_end));
    }

    /// @brief Extract the concrete optional enum span from this optional typed enum span.
    /// @tparam Enum Type of the enum to be extracted.
    /// @note If you provide an incorrect type argument to `Enum`, it will return `bn::nullopt`. \n
    /// If you don't like that, you can use `get()` instead.
    template <typename Enum>
        requires std::is_scoped_enum_v<Enum>
    [[nodiscard]] constexpr auto get_optional() const -> bn::optional<bn::span<const bn::optional<Enum>>>
    {
        if (bn::type_id<Enum>() != _type_id)
            return bn::nullopt;

        return bn::span<const bn::optional<Enum>>(static_cast<const bn::optional<Enum>*>(_optional_span_begin),
                                                  static_cast<const bn::optional<Enum>*>(_optional_span_end));
    }

public:
    /// @brief Type id of this enum span
    [[nodiscard]] constexpr auto type_id() const -> bn::type_id_t
    {
        return _type_id;
    }
};

} // namespace ldtk::priv

/// @endcond
