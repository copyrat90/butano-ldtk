#pragma once

#include <bn_assert.h>
#include <bn_optional.h>
#include <bn_type_id.h>

#include <type_traits>

namespace ldtk
{

class typed_enum
{
private:
    bn::type_id_t _type_id;
    int _number;

public:
    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    constexpr typed_enum(Enum raw) : _type_id(bn::type_id<Enum>()), _number(static_cast<decltype(_number)>(raw))
    {
    }

public:
    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    [[nodiscard]] constexpr auto get() const -> Enum
    {
        BN_ASSERT(bn::type_id<Enum>() == _type_id, "Invalid Enum type");

        return static_cast<Enum>(_number);
    }

    template <typename Enum>
        requires(std::is_enum_v<Enum> && sizeof(Enum) <= sizeof(_number))
    [[nodiscard]] constexpr auto get_optional() const -> bn::optional<Enum>
    {
        if (bn::type_id<Enum>() != _type_id)
            return bn::nullopt;

        return static_cast<Enum>(_number);
    }

public:
    [[nodiscard]] constexpr auto type_id() const -> bn::type_id_t
    {
        return _type_id;
    }

    [[nodiscard]] constexpr auto number() const -> decltype(_number)
    {
        return _number;
    }
};

} // namespace ldtk
