#pragma once

#include <bn_assert.h>
#include <bn_optional.h>
#include <bn_type_id.h>

#include <type_traits>

namespace ldtk
{

/// @brief Enum value that's stored in the `field` with `bn::type_id_t`
class typed_enum
{
private:
    bn::type_id_t _type_id;
    int _number;

    /// @cond DO_NOT_DOCUMENT
public:
    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    constexpr typed_enum(Enum raw) : _type_id(bn::type_id<Enum>()), _number(static_cast<decltype(_number)>(raw))
    {
    }
    /// @endcond

public:
    /// @brief Extract the concrete enum from this typed enum.
    /// @tparam Enum Type of the enum to be extracted.
    /// @note If you provide an incorrect type argument to `Enum`, it will error out. \n
    /// If you don't like that, you can use `get_optional()` instead.
    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    [[nodiscard]] constexpr auto get() const -> Enum
    {
        BN_ASSERT(bn::type_id<Enum>() == _type_id, "Invalid Enum type");

        return static_cast<Enum>(_number);
    }

    /// @brief Extract the concrete enum from this typed enum.
    /// @tparam Enum Type of the enum to be extracted.
    /// @note If you provide an incorrect type argument to `Enum`, it will return `bn::nullopt`. \n
    /// If you don't like that, you can use `get()` instead.
    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    [[nodiscard]] constexpr auto get_optional() const -> bn::optional<Enum>
    {
        if (bn::type_id<Enum>() != _type_id)
            return bn::nullopt;

        return static_cast<Enum>(_number);
    }

public:
    /// @brief Type id of this enum
    [[nodiscard]] constexpr auto type_id() const -> bn::type_id_t
    {
        return _type_id;
    }

    /// @brief Numeric value of the enum
    [[nodiscard]] constexpr auto number() const -> decltype(_number)
    {
        return _number;
    }
};

} // namespace ldtk
