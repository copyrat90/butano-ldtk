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
    [[nodiscard]] constexpr auto get_int() const -> int
    {
        BN_ASSERT(_type == field_type::INT, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.integral;
    }

    [[nodiscard]] constexpr auto get_fixed() const -> bn::fixed
    {
        BN_ASSERT(_type == field_type::FIXED, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.fixed;
    }

    [[nodiscard]] constexpr auto get_bool() const -> bool
    {
        BN_ASSERT(_type == field_type::BOOL, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.flag;
    }

    [[nodiscard]] constexpr auto get_string() const -> const bn::string_view&
    {
        BN_ASSERT(_type == field_type::STRING, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.str;
    }

    [[nodiscard]] constexpr auto get_color() const -> bn::color
    {
        BN_ASSERT(_type == field_type::COLOR, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.color;
    }

    [[nodiscard]] constexpr auto get_typed_enum() const -> const typed_enum&
    {
        BN_ASSERT(_type == field_type::TYPED_ENUM, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.t_enum;
    }

    // [[nodiscard]] constexpr auto get_tile() const -> const tile&
    // {
    //     BN_ASSERT(_type == field_type::TILE, "Invalid field type, was ", (int)_type);
    //     BN_ASSERT(has_value(), "Field was null");

    //     return _value.tile;
    // }

    [[nodiscard]] constexpr auto get_entity_ref() const -> const entity_ref&
    {
        BN_ASSERT(_type == field_type::ENTITY_REF, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.ent_ref;
    }

    [[nodiscard]] constexpr auto get_point() const -> const bn::point&
    {
        BN_ASSERT(_type == field_type::POINT, "Invalid field type, was ", (int)_type);
        BN_ASSERT(has_value(), "Field was null");

        return _value.point;
    }

    [[nodiscard]] constexpr auto get_int_span() const -> const bn::span<const int>&
    {
        BN_ASSERT(_type == field_type::INT_SPAN, "Invalid field type, was ", (int)_type);

        return _value.int_span;
    }

    [[nodiscard]] constexpr auto get_fixed_span() const -> const bn::span<const bn::fixed>&
    {
        BN_ASSERT(_type == field_type::FIXED_SPAN, "Invalid field type, was ", (int)_type);

        return _value.fixed_span;
    }

    [[nodiscard]] constexpr auto get_bool_span() const -> const bn::span<const bool>&
    {
        BN_ASSERT(_type == field_type::BOOL_SPAN, "Invalid field type, was ", (int)_type);

        return _value.flag_span;
    }

    [[nodiscard]] constexpr auto get_string_span() const -> const bn::span<const bn::string_view>&
    {
        BN_ASSERT(_type == field_type::STRING_SPAN, "Invalid field type, was ", (int)_type);

        return _value.str_span;
    }

    [[nodiscard]] constexpr auto get_color_span() const -> const bn::span<const bn::color>&
    {
        BN_ASSERT(_type == field_type::COLOR_SPAN, "Invalid field type, was ", (int)_type);

        return _value.color_span;
    }

    [[nodiscard]] constexpr auto get_typed_enum_span() const -> const bn::span<const typed_enum>&
    {
        BN_ASSERT(_type == field_type::TYPED_ENUM_SPAN, "Invalid field type, was ", (int)_type);

        return _value.t_enum_span;
    }

    // [[nodiscard]] constexpr auto get_tile_span() const -> const bn::span<const tile>&
    // {
    //     BN_ASSERT(_type == field_type::TILE_SPAN, "Invalid field type, was ", (int)_type);

    //     return _value.tile_span;
    // }

    [[nodiscard]] constexpr auto get_entity_ref_span() const -> const bn::span<const entity_ref>&
    {
        BN_ASSERT(_type == field_type::ENTITY_REF_SPAN, "Invalid field type, was ", (int)_type);

        return _value.ent_ref_span;
    }

    [[nodiscard]] constexpr auto get_point_span() const -> const bn::span<const bn::point>&
    {
        BN_ASSERT(_type == field_type::POINT_SPAN, "Invalid field type, was ", (int)_type);

        return _value.point_span;
    }

    [[nodiscard]] constexpr auto get_optional_int_span() const -> const bn::span<const bn::optional<int>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_INT_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_int_span;
    }

    [[nodiscard]] constexpr auto get_optional_fixed_span() const -> const bn::span<const bn::optional<bn::fixed>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_FIXED_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_fixed_span;
    }

    [[nodiscard]] constexpr auto get_optional_string_span() const
        -> const bn::span<const bn::optional<bn::string_view>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_STRING_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_str_span;
    }

    [[nodiscard]] constexpr auto get_optional_typed_enum_span() const -> const bn::span<const bn::optional<typed_enum>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_TYPED_ENUM_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_t_enum_span;
    }

    // [[nodiscard]] constexpr auto get_optional_tile_span() const -> const bn::span<const bn::optional<tile>>&
    // {
    //     BN_ASSERT(_type == field_type::OPTIONAL_TILE_SPAN, "Invalid field type, was ", (int)_type);

    //     return _value.opt_tile_span;
    // }

    [[nodiscard]] constexpr auto get_optional_entity_ref_span() const -> const bn::span<const bn::optional<entity_ref>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_ENTITY_REF_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_ent_ref_span;
    }

    [[nodiscard]] constexpr auto get_optional_point_span() const -> const bn::span<const bn::optional<bn::point>>&
    {
        BN_ASSERT(_type == field_type::OPTIONAL_POINT_SPAN, "Invalid field type, was ", (int)_type);

        return _value.opt_point_span;
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
