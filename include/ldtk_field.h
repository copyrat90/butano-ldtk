#pragma once

#include "ldtk_entity_ref.h"
#include "ldtk_field_type.h"
#include "ldtk_gen_ident_fwd.h"
#include "ldtk_typed_enum.h"

#include <bn_assert.h>
#include <bn_color.h>
#include <bn_fixed.h>
#include <bn_optional.h>
#include <bn_point.h>
#include <bn_span.h>
#include <bn_string_view.h>

#include <memory>
#include <type_traits>

#define LDTK_FIELD_GETTER_TYPE_ASSERT(expected) \
    BN_ASSERT(_type == expected, "Field type is not ", #expected, " - it was (field_type)", (int)_type)

#define LDTK_FIELD_GETTER_NULL_CHECK BN_ASSERT(has_value(), "Field was null")

namespace ldtk
{

class field
{
public:
    constexpr field(gen::ident identifier, int value)
        : _identifier(identifier), _type(field_type::INT), _has_value(true)
    {
        std::construct_at(&_value.integral, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<int>& value)
        : _identifier(identifier), _type(field_type::INT), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.integral, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, bn::fixed value)
        : _identifier(identifier), _type(field_type::FIXED), _has_value(true)
    {
        std::construct_at(&_value.fixed, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<bn::fixed>& value)
        : _identifier(identifier), _type(field_type::FIXED), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.fixed, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, bool value)
        : _identifier(identifier), _type(field_type::BOOL), _has_value(true)
    {
        std::construct_at(&_value.flag, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<bool>& value)
        : _identifier(identifier), _type(field_type::BOOL), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.flag, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, const bn::string_view& value)
        : _identifier(identifier), _type(field_type::STRING), _has_value(true)
    {
        std::construct_at(&_value.str, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<bn::string_view>& value)
        : _identifier(identifier), _type(field_type::STRING), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.str, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, bn::color value)
        : _identifier(identifier), _type(field_type::COLOR), _has_value(true)
    {
        std::construct_at(&_value.color, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<bn::color>& value)
        : _identifier(identifier), _type(field_type::COLOR), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.color, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, const typed_enum& value)
        : _identifier(identifier), _type(field_type::TYPED_ENUM), _has_value(true)
    {
        std::construct_at(&_value.t_enum, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<typed_enum>& value)
        : _identifier(identifier), _type(field_type::TYPED_ENUM), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.t_enum, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    // constexpr field(gen::ident identifier, const tile& value)
    //     : _identifier(identifier), _type(field_type::TILE), _has_value(true)
    // {
    //     std::construct_at(&_value.tile, value);
    // }

    // constexpr field(gen::ident identifier, const bn::optional<tile>& value)
    //     : _identifier(identifier), _type(field_type::TILE), _has_value(value.has_value())
    // {
    //     if (value.has_value())
    //         std::construct_at(&_value.tile , value.value());
    //     else
    //         std::construct_at(&_value.empty, 0);
    // }

    constexpr field(gen::ident identifier, const entity_ref& value)
        : _identifier(identifier), _type(field_type::ENTITY_REF), _has_value(true)
    {
        std::construct_at(&_value.ent_ref, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<entity_ref>& value)
        : _identifier(identifier), _type(field_type::ENTITY_REF), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.ent_ref, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, const bn::point& value)
        : _identifier(identifier), _type(field_type::POINT), _has_value(true)
    {
        std::construct_at(&_value.point, value);
    }

    constexpr field(gen::ident identifier, const bn::optional<bn::point>& value)
        : _identifier(identifier), _type(field_type::POINT), _has_value(value.has_value())
    {
        if (value.has_value())
            std::construct_at(&_value.point, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(gen::ident identifier, const bn::span<const int>& value)
        : _identifier(identifier), _type(field_type::INT_SPAN), _has_value(true)
    {
        std::construct_at(&_value.int_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::fixed>& value)
        : _identifier(identifier), _type(field_type::FIXED_SPAN), _has_value(true)
    {
        std::construct_at(&_value.fixed_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bool>& value)
        : _identifier(identifier), _type(field_type::BOOL_SPAN), _has_value(true)
    {
        std::construct_at(&_value.flag_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::string_view>& value)
        : _identifier(identifier), _type(field_type::STRING_SPAN), _has_value(true)
    {
        std::construct_at(&_value.str_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::color>& value)
        : _identifier(identifier), _type(field_type::COLOR_SPAN), _has_value(true)
    {
        std::construct_at(&_value.color_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const typed_enum>& value)
        : _identifier(identifier), _type(field_type::TYPED_ENUM_SPAN), _has_value(true)
    {
        std::construct_at(&_value.t_enum_span, value);
    }

    // constexpr field(gen::ident identifier, const bn::span<const tile>& value)
    //     : _identifier(identifier), _type(field_type::TILE_SPAN), _has_value(true)
    // {
    //     std::construct_at(&_value.tile_span , value);
    // }

    constexpr field(gen::ident identifier, const bn::span<const entity_ref>& value)
        : _identifier(identifier), _type(field_type::ENTITY_REF_SPAN), _has_value(true)
    {
        std::construct_at(&_value.ent_ref_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::point>& value)
        : _identifier(identifier), _type(field_type::POINT_SPAN), _has_value(true)
    {
        std::construct_at(&_value.point_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<int>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_INT_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_int_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<bn::fixed>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_FIXED_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_fixed_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<bn::string_view>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_STRING_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_str_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<typed_enum>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_TYPED_ENUM_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_t_enum_span, value);
    }

    // constexpr field(gen::ident identifier, const bn::span<const bn::optional<tile>>& value)
    //     : _identifier(identifier), _type(field_type::OPTIONAL_TILE_SPAN), _has_value(true)
    // {
    //     std::construct_at(&_value.opt_tile_span, value);
    // }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<entity_ref>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_ENTITY_REF_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_ent_ref_span, value);
    }

    constexpr field(gen::ident identifier, const bn::span<const bn::optional<bn::point>>& value)
        : _identifier(identifier), _type(field_type::OPTIONAL_POINT_SPAN), _has_value(true)
    {
        std::construct_at(&_value.opt_point_span, value);
    }

    constexpr ~field()
    {
        if (_has_value)
        {
            switch (_type)
            {
            case field_type::INT:
                std::destroy_at(&_value.integral);
                break;
            case field_type::FIXED:
                std::destroy_at(&_value.fixed);
                break;
            case field_type::BOOL:
                std::destroy_at(&_value.flag);
                break;
            case field_type::STRING:
                std::destroy_at(&_value.str);
                break;
            case field_type::COLOR:
                std::destroy_at(&_value.color);
                break;
            case field_type::TYPED_ENUM:
                std::destroy_at(&_value.t_enum);
                break;
            // case field_type::TILE:
            //     std::destroy_at(&_value.tile);
            //     break;
            case field_type::ENTITY_REF:
                std::destroy_at(&_value.ent_ref);
                break;
            case field_type::POINT:
                std::destroy_at(&_value.point);
                break;

            case field_type::INT_SPAN:
                std::destroy_at(&_value.int_span);
                break;
            case field_type::FIXED_SPAN:
                std::destroy_at(&_value.fixed_span);
                break;
            case field_type::BOOL_SPAN:
                std::destroy_at(&_value.flag_span);
                break;
            case field_type::STRING_SPAN:
                std::destroy_at(&_value.str_span);
                break;
            case field_type::COLOR_SPAN:
                std::destroy_at(&_value.color_span);
                break;
            case field_type::TYPED_ENUM_SPAN:
                std::destroy_at(&_value.t_enum_span);
                break;
            // case field_type::TILE_SPAN:
            //     std::destroy_at(&_value.tile_span);
            //     break;
            case field_type::ENTITY_REF_SPAN:
                std::destroy_at(&_value.ent_ref_span);
                break;
            case field_type::POINT_SPAN:
                std::destroy_at(&_value.point_span);
                break;

            case field_type::OPTIONAL_INT_SPAN:
                std::destroy_at(&_value.opt_int_span);
                break;
            case field_type::OPTIONAL_FIXED_SPAN:
                std::destroy_at(&_value.opt_fixed_span);
                break;
            case field_type::OPTIONAL_STRING_SPAN:
                std::destroy_at(&_value.opt_str_span);
                break;
            case field_type::OPTIONAL_TYPED_ENUM_SPAN:
                std::destroy_at(&_value.opt_t_enum_span);
                break;
            // case field_type::OPTIONAL_TILE_SPAN:
            //     std::destroy_at(&_value.opt_tile_span);
            //     break;
            case field_type::OPTIONAL_ENTITY_REF_SPAN:
                std::destroy_at(&_value.opt_ent_ref_span);
                break;
            case field_type::OPTIONAL_POINT_SPAN:
                std::destroy_at(&_value.opt_point_span);
                break;
            default:
                break;
            }
        }
        else
        {
            std::destroy_at(&_value.empty);
        }
    }

public:
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    [[nodiscard]] constexpr auto type() const -> field_type
    {
        return _type;
    }

    [[nodiscard]] constexpr auto has_value() const -> bool
    {
        return _has_value;
    }

public:
    template <typename T>
    [[nodiscard]] constexpr auto get() const -> const T&
    {
        if constexpr (std::is_same_v<T, int>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::INT);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.integral;
        }
        else if constexpr (std::is_same_v<T, bn::fixed>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::FIXED);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.fixed;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::BOOL);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.flag;
        }
        else if constexpr (std::is_same_v<T, bn::string_view>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::STRING);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.str;
        }
        else if constexpr (std::is_same_v<T, bn::color>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::COLOR);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.color;
        }
        else if constexpr (std::is_same_v<T, typed_enum>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::TYPED_ENUM);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.t_enum;
        }
        // else if constexpr (std::is_same_v<T, tile>)
        // {
        //     LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::TILE);
        //     LDTK_FIELD_GETTER_NULL_CHECK;
        //     return _value.tile;
        // }
        else if constexpr (std::is_same_v<T, entity_ref>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::ENTITY_REF);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.ent_ref;
        }
        else if constexpr (std::is_same_v<T, bn::point>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::POINT);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.point;
        }
        else if constexpr (std::is_same_v<T, bn::span<const int>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::INT_SPAN);
            return _value.int_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::fixed>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::FIXED_SPAN);
            return _value.fixed_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bool>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::BOOL_SPAN);
            return _value.flag_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::string_view>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::STRING_SPAN);
            return _value.str_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::color>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::COLOR_SPAN);
            return _value.color_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const typed_enum>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::TYPED_ENUM_SPAN);
            return _value.t_enum_span;
        }
        // else if constexpr (std::is_same_v<T, bn::span<const tile>>)
        // {
        //     LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::TILE_SPAN);
        //     return _value.tile_span;
        // }
        else if constexpr (std::is_same_v<T, bn::span<const entity_ref>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::ENTITY_REF_SPAN);
            return _value.ent_ref_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::point>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::POINT_SPAN);
            return _value.point_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<int>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_INT_SPAN);
            return _value.opt_int_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::fixed>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_FIXED_SPAN);
            return _value.opt_fixed_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::string_view>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_STRING_SPAN);
            return _value.opt_str_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<typed_enum>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_TYPED_ENUM_SPAN);
            return _value.opt_t_enum_span;
        }
        // else if constexpr (std::is_same_v<T, bn::span<const bn::optional<tile>>>)
        // {
        //     LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_TILE_SPAN);
        //     return _value.opt_tile_span;
        // }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<entity_ref>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_ENTITY_REF_SPAN);
            return _value.opt_ent_ref_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::point>>>)
        {
            LDTK_FIELD_GETTER_TYPE_ASSERT(field_type::OPTIONAL_POINT_SPAN);
            return _value.opt_point_span;
        }
        else
        {
            static_assert(false, "Invalid type parameter T provided");
        }
    }

private:
    gen::ident _identifier;

    field_type _type;

    // `false` is possible only for non-spans;
    // spans are always initialized, thus always `true`.
    bool _has_value;

    union data_t {
        char empty;

        int integral;
        bn::fixed fixed;
        bool flag;
        bn::string_view str;
        bn::color color;
        typed_enum t_enum;
        // tile tile;
        entity_ref ent_ref;
        bn::point point;

        bn::span<const int> int_span;
        bn::span<const bn::fixed> fixed_span;
        bn::span<const bool> flag_span;
        bn::span<const bn::string_view> str_span;
        bn::span<const bn::color> color_span;
        bn::span<const typed_enum> t_enum_span;
        // bn::span<const tile> tile_span;
        bn::span<const entity_ref> ent_ref_span;
        bn::span<const bn::point> point_span;

        bn::span<const bn::optional<int>> opt_int_span;
        bn::span<const bn::optional<bn::fixed>> opt_fixed_span;
        bn::span<const bn::optional<bn::string_view>> opt_str_span;
        bn::span<const bn::optional<typed_enum>> opt_t_enum_span;
        // bn::span<const bn::optional<tile>> opt_tile_span;
        bn::span<const bn::optional<entity_ref>> opt_ent_ref_span;
        bn::span<const bn::optional<bn::point>> opt_point_span;

        constexpr data_t()
        {
        }

        constexpr ~data_t()
        {
        }
    } _value;
};

} // namespace ldtk
