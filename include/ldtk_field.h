// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_field_definition.h"

#include "ldtk_entity_ref.h"
#include "ldtk_field_type.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_priv_concepts.h"
#include "ldtk_priv_optional_typed_enum_span.h"
#include "ldtk_priv_typed_enum.h"
#include "ldtk_priv_typed_enum_span.h"

#include <bn_assert.h>
#include <bn_color.h>
#include <bn_fixed.h>
#include <bn_optional.h>
#include <bn_point.h>
#include <bn_span.h>
#include <bn_string_view.h>
#include <bn_type_id.h>

#include <cstdint>
#include <memory>
#include <type_traits>

/// @cond DO_NOT_DOCUMENT
#define LDTK_FIELD_TYPE_ASSERT(expected) \
    BN_ASSERT(type() == expected, "Field type is not ", #expected, " - it was (field_type)", (int)type())
#define LDTK_FIELD_ENUM_TYPE_ASSERT(expected) \
    BN_ASSERT(enum_type().has_value() && enum_type() == expected, "Incorrect field enum type");

#define LDTK_FIELD_GETTER_NULL_CHECK BN_ASSERT(has_value(), "Field was null")
/// @endcond

namespace ldtk
{

class field
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr field(const field_definition& def, std::uint8_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_8);

        std::construct_at(&_value.u8, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::uint8_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_8);

        if (value.has_value())
            std::construct_at(&_value.u8, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::int8_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_8);

        std::construct_at(&_value.s8, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::int8_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_8);

        if (value.has_value())
            std::construct_at(&_value.s8, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::uint16_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_16);

        std::construct_at(&_value.u16, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::uint16_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_16);

        if (value.has_value())
            std::construct_at(&_value.u16, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::int16_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_16);

        std::construct_at(&_value.s16, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::int16_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_16);

        if (value.has_value())
            std::construct_at(&_value.s16, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::uint32_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_32);

        std::construct_at(&_value.u32, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::uint32_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_32);

        if (value.has_value())
            std::construct_at(&_value.u32, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::int32_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_32);

        std::construct_at(&_value.s32, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::int32_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_32);

        if (value.has_value())
            std::construct_at(&_value.s32, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::uint64_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_64);

        std::construct_at(&_value.u64, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::uint64_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_64);

        if (value.has_value())
            std::construct_at(&_value.u64, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, std::int64_t value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_64);

        std::construct_at(&_value.s64, value);
    }

    constexpr field(const field_definition& def, const bn::optional<std::int64_t>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_64);

        if (value.has_value())
            std::construct_at(&_value.s64, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, bn::fixed value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::FIXED);

        std::construct_at(&_value.fixed, value);
    }

    constexpr field(const field_definition& def, const bn::optional<bn::fixed>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::FIXED);

        if (value.has_value())
            std::construct_at(&_value.fixed, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, bool value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::BOOL);

        std::construct_at(&_value.flag, value);
    }

    constexpr field(const field_definition& def, const bn::optional<bool>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::BOOL);

        if (value.has_value())
            std::construct_at(&_value.flag, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, const bn::string_view& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::STRING);

        std::construct_at(&_value.str, value);
    }

    constexpr field(const field_definition& def, const bn::optional<bn::string_view>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::STRING);

        if (value.has_value())
            std::construct_at(&_value.str, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, bn::color value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::COLOR);

        std::construct_at(&_value.color, value);
    }

    constexpr field(const field_definition& def, const bn::optional<bn::color>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::COLOR);

        if (value.has_value())
            std::construct_at(&_value.color, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    template <typename Enum>
        requires std::is_scoped_enum_v<Enum>
    constexpr field(const field_definition& def, Enum value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::TYPED_ENUM);
        LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<Enum>());

        std::construct_at(&_value.t_enum, value);
    }

    template <typename Enum>
        requires std::is_scoped_enum_v<Enum>
    constexpr field(const field_definition& def, const bn::optional<Enum>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::TYPED_ENUM);

        if (value.has_value())
            std::construct_at(&_value.t_enum, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    // constexpr field(const field_definition& def, const tile& value)
    //     : _def(def), _has_value(true)
    // {
    //     LDTK_FIELD_TYPE_ASSERT(field_type::TILE);
    //
    //     std::construct_at(&_value.tile, value);
    // }

    // constexpr field(const field_definition& def, const bn::optional<tile>& value)
    //     : _def(def), _has_value(value.has_value())
    // {
    //     LDTK_FIELD_TYPE_ASSERT(field_type::TILE);
    //
    //     if (value.has_value())
    //         std::construct_at(&_value.tile , value.value());
    //     else
    //         std::construct_at(&_value.empty, 0);
    // }

    constexpr field(const field_definition& def, const entity_ref& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::ENTITY_REF);

        std::construct_at(&_value.ent_ref, value);
    }

    constexpr field(const field_definition& def, const bn::optional<entity_ref>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::ENTITY_REF);

        if (value.has_value())
            std::construct_at(&_value.ent_ref, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, const bn::point& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::POINT);

        std::construct_at(&_value.point, value);
    }

    constexpr field(const field_definition& def, const bn::optional<bn::point>& value)
        : _def(def), _has_value(value.has_value())
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::POINT);

        if (value.has_value())
            std::construct_at(&_value.point, value.value());
        else
            std::construct_at(&_value.empty, 0);
    }

    constexpr field(const field_definition& def, const bn::span<const std::uint8_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_8_SPAN);

        std::construct_at(&_value.u8_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::int8_t>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_8_SPAN);

        std::construct_at(&_value.s8_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::uint16_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_16_SPAN);

        std::construct_at(&_value.u16_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::int16_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_16_SPAN);

        std::construct_at(&_value.s16_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::uint32_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_32_SPAN);

        std::construct_at(&_value.u32_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::int32_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_32_SPAN);

        std::construct_at(&_value.s32_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::uint64_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::UINT_64_SPAN);

        std::construct_at(&_value.u64_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const std::int64_t>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::INT_64_SPAN);

        std::construct_at(&_value.s64_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::fixed>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::FIXED_SPAN);

        std::construct_at(&_value.fixed_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bool>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::BOOL_SPAN);

        std::construct_at(&_value.flag_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::string_view>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::STRING_SPAN);

        std::construct_at(&_value.str_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::color>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::COLOR_SPAN);

        std::construct_at(&_value.color_span, value);
    }

    template <typename Enum>
        requires(std::is_scoped_enum_v<Enum>)
    constexpr field(const field_definition& def, const bn::span<const Enum>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::TYPED_ENUM_SPAN);
        LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<Enum>());

        std::construct_at(&_value.t_enum_span, value);
    }

    // constexpr field(const field_definition& def, const bn::span<const tile>& value)
    //     : _def(def), _has_value(true)
    // {
    //     LDTK_FIELD_TYPE_ASSERT(field_type::TILE_SPAN);
    //
    //     std::construct_at(&_value.tile_span, value);
    // }

    constexpr field(const field_definition& def, const bn::span<const entity_ref>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::ENTITY_REF_SPAN);

        std::construct_at(&_value.ent_ref_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::point>& value) : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::POINT_SPAN);

        std::construct_at(&_value.point_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::uint8_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_8_SPAN);

        std::construct_at(&_value.opt_u8_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::int8_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_8_SPAN);

        std::construct_at(&_value.opt_s8_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::uint16_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_16_SPAN);

        std::construct_at(&_value.opt_u16_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::int16_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_16_SPAN);

        std::construct_at(&_value.opt_s16_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::uint32_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_32_SPAN);

        std::construct_at(&_value.opt_u32_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::int32_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_32_SPAN);

        std::construct_at(&_value.opt_s32_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::uint64_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_64_SPAN);

        std::construct_at(&_value.opt_u64_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<std::int64_t>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_64_SPAN);

        std::construct_at(&_value.opt_s64_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<bn::fixed>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_FIXED_SPAN);

        std::construct_at(&_value.opt_fixed_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<bn::string_view>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_STRING_SPAN);

        std::construct_at(&_value.opt_str_span, value);
    }

    template <typename Enum>
        requires(std::is_scoped_enum_v<Enum>)
    constexpr field(const field_definition& def, const bn::span<const bn::optional<Enum>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_TYPED_ENUM_SPAN);
        LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<Enum>());

        std::construct_at(&_value.opt_t_enum_span, value);
    }

    // constexpr field(const field_definition& def, const bn::span<const bn::optional<tile>>& value)
    //     : _def(def), _has_value(true)
    // {
    //     LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_TILE_SPAN);
    //
    //     std::construct_at(&_value.opt_tile_span, value);
    // }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<entity_ref>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_ENTITY_REF_SPAN);

        std::construct_at(&_value.opt_ent_ref_span, value);
    }

    constexpr field(const field_definition& def, const bn::span<const bn::optional<bn::point>>& value)
        : _def(def), _has_value(true)
    {
        LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_POINT_SPAN);

        std::construct_at(&_value.opt_point_span, value);
    }

    constexpr ~field()
    {
        if (_has_value)
        {
            switch (type())
            {
            case field_type::UINT_8:
                std::destroy_at(&_value.u8);
                break;
            case field_type::INT_8:
                std::destroy_at(&_value.s8);
                break;
            case field_type::UINT_16:
                std::destroy_at(&_value.u16);
                break;
            case field_type::INT_16:
                std::destroy_at(&_value.s16);
                break;
            case field_type::UINT_32:
                std::destroy_at(&_value.u32);
                break;
            case field_type::INT_32:
                std::destroy_at(&_value.s32);
                break;
            case field_type::UINT_64:
                std::destroy_at(&_value.u64);
                break;
            case field_type::INT_64:
                std::destroy_at(&_value.s64);
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

            case field_type::UINT_8_SPAN:
                std::destroy_at(&_value.u8_span);
                break;
            case field_type::INT_8_SPAN:
                std::destroy_at(&_value.s8_span);
                break;
            case field_type::UINT_16_SPAN:
                std::destroy_at(&_value.u16_span);
                break;
            case field_type::INT_16_SPAN:
                std::destroy_at(&_value.s16_span);
                break;
            case field_type::UINT_32_SPAN:
                std::destroy_at(&_value.u32_span);
                break;
            case field_type::INT_32_SPAN:
                std::destroy_at(&_value.s32_span);
                break;
            case field_type::UINT_64_SPAN:
                std::destroy_at(&_value.u64_span);
                break;
            case field_type::INT_64_SPAN:
                std::destroy_at(&_value.s64_span);
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

            case field_type::OPTIONAL_UINT_8_SPAN:
                std::destroy_at(&_value.opt_u8_span);
                break;
            case field_type::OPTIONAL_INT_8_SPAN:
                std::destroy_at(&_value.opt_s8_span);
                break;
            case field_type::OPTIONAL_UINT_16_SPAN:
                std::destroy_at(&_value.opt_u16_span);
                break;
            case field_type::OPTIONAL_INT_16_SPAN:
                std::destroy_at(&_value.opt_s16_span);
                break;
            case field_type::OPTIONAL_UINT_32_SPAN:
                std::destroy_at(&_value.opt_u32_span);
                break;
            case field_type::OPTIONAL_INT_32_SPAN:
                std::destroy_at(&_value.opt_s32_span);
                break;
            case field_type::OPTIONAL_UINT_64_SPAN:
                std::destroy_at(&_value.opt_u64_span);
                break;
            case field_type::OPTIONAL_INT_64_SPAN:
                std::destroy_at(&_value.opt_s64_span);
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
    /// @endcond

public:
    /// @brief Reference to the Field definition
    [[nodiscard]] constexpr auto def() const -> const field_definition&
    {
        return _def;
    }

    /// @brief Field definition identifier
    /// @tparam Ident Either `gen::level_field_ident` or `gen::entity_field_ident`, depending on where this field
    /// belongs
    template <typename Ident>
        requires(std::is_same_v<Ident, gen::level_field_ident> || std::is_same_v<Ident, gen::entity_field_ident>)
    [[nodiscard]] constexpr auto identifier() const -> Ident
    {
        return def().identifier<Ident>();
    }

    /// @brief Type of the field
    [[nodiscard]] constexpr auto type() const -> field_type
    {
        return def().type();
    }

    /// @brief Enum type of the field
    [[nodiscard]] constexpr auto enum_type() const -> const bn::optional<bn::type_id_t>&
    {
        return def().enum_type();
    }

    /// @brief If this field has a value or not.
    /// @note You @b must check this for the non-array types that are set to `Can be null`/`Is optional`. (See `get()`)
    /// \n If this field is an array (span), this always returns `true`.
    [[nodiscard]] constexpr auto has_value() const -> bool
    {
        return _has_value;
    }

public:
    // clang-format off

    /// @brief Extract the concrete object from the field.
    /// @tparam T Type of the field to be extracted.
    /// @details Type parameter `T` depends on your LDtk level/entity field type.
    ///
    /// If it is a non-array type, you use one of these type:
    /// | LDtk non-array type | Butano type `T`                        |
    /// | ------------------- | -------------------------------------- |
    /// | Integer             | INT (Various integer types, see below) |
    /// | Float               | `bn::fixed`                            |
    /// | Boolean             | `bool`                                 |
    /// | String              | `bn::string_view`                      |
    /// | Multilines          | `bn::string_view`                      |
    /// | Color               | `bn::color`                            |
    /// | Enum (`enum_name`)  | `ldtk::gen::enum_name`                 |
    /// | Entity ref          | `ldtk::entity_ref`                     |
    /// | Point               | `bn::point`                            |
    ///
    /// If the field is set to `Can be null`/`Is optional` in the LDtk field value specifications, (See the image below) \n
    /// you @b must check `has_value()` before calling `get()`.
    ///
    /// INT is determined by the limits you set in the LDtk field value specifications. (See the image below) \n
    /// By default, if you don't set any limit, it uses `std::int32_t`. \n
    /// If you set the `Min` and/or `Max`, it uses other integer types:
    /// | Limits `[Min..Max]`                           | INT                     |
    /// | --------------------------------------------- | ----------------------- |
    /// | `[0..255]`                                    | `std::uint8_t`          |
    /// | `[-128..127]`                                 | `std::int8_t`           |
    /// | `[0..65535]`                                  | `std::uint16_t`         |
    /// | `[-32768..32767]`                             | `std::int16_t`          |
    /// | `[0..4294967295]`                             | `std::uint32_t`         |
    /// | `[-2147483648..2147483647]`                   | `std::int32_t`          |
    /// | `[0..18446744073709551615]`                   | `std::uint64_t`         |
    /// | `[-9223372036854775808..9223372036854775807]` | `std::int64_t`          |
    ///
    /// * Unsigned always takes precedence over the same size of signed integer type, whenever possible.
    ///    * `[0..127]` will be `std::uint8_t`, @b not signed.
    /// * Unspecified limits are assumed as `-2147483648` for `Min` and `2147483647` for `Max`.
    /// * LDtk 1.5.3 has a [bug](https://github.com/deepnight/ldtk/issues/1192) that if you don't specify the limits, it limits putting the value in `[-32768..32767]` range.
    ///    * You can expand this by setting the limits yourself.
    ///    * Still, it [overflows for the limits out of `[-2147483..2147483]`](https://github.com/deepnight/ldtk/issues/1191), so there's no way to use `std::uint64_t` and `std::int64_t` at the moment.
    ///
    /// ![](docs/images/field_limits_and_null.png)
    ///
    /// If it is an array type, you also need to consider if it is set to `Can contain nulls` in the LDtk field value specifications:
    /// | LDtk array type     | `T` when CAN'T contain nulls           | `T` when CAN contain nulls                           |
    /// | ------------------- | -------------------------------------- | ---------------------------------------------------- |
    /// | Integer             | `bn::span<const INT>`                  | `bn::span<const bn::optional<INT>>`                  |
    /// | Float               | `bn::span<const bn::fixed>`            | `bn::span<const bn::optional<bn::fixed>>`            |
    /// | Boolean             | `bn::span<const bool>`                 | `bn::span<const bn::optional<bool>>`                 |
    /// | String              | `bn::span<const bn::string_view>`      | `bn::span<const bn::optional<bn::string_view>>`      |
    /// | Multilines          | `bn::span<const bn::string_view>`      | `bn::span<const bn::optional<bn::string_view>>`      |
    /// | Color               | `bn::span<const bn::color>`            | `bn::span<const bn::optional<bn::color>>`            |
    /// | Enum (`enum_name`)  | `bn::span<const ldtk::gen::enum_name>` | `bn::span<const bn::optional<ldtk::gen::enum_name>>` |
    /// | Entity ref          | `bn::span<const ldtk::entity_ref>`     | `bn::span<const bn::optional<ldtk::entity_ref>>`     |
    /// | Point               | `bn::span<const bn::point>`            | `bn::span<const bn::optional<bn::point>>`            |
    /// For arrays, checking `has_value()` is not necessary, as it always returns `true`.
    ///
    /// @note If you provide an incorrect type argument to `T`, it will error out. \n\n
    /// If it is a non-array type, and the field is set to `Can be null`/`Is optional`, \n
    /// you @b must check `has_value()` first to ensure if it contains a value or not.
    template <typename T>
    [[nodiscard]] constexpr auto get() const -> T
    {
        // clang-format on
        if constexpr (std::is_same_v<T, std::uint8_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_8);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.u8;
        }
        else if constexpr (std::is_same_v<T, std::int8_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_8);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.s8;
        }
        else if constexpr (std::is_same_v<T, std::uint16_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_16);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.u16;
        }
        else if constexpr (std::is_same_v<T, std::int16_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_16);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.s16;
        }
        else if constexpr (std::is_same_v<T, std::uint32_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_32);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.u32;
        }
        else if constexpr (std::is_same_v<T, std::int32_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_32);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.s32;
        }
        else if constexpr (std::is_same_v<T, std::uint64_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_64);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.u64;
        }
        else if constexpr (std::is_same_v<T, std::int64_t>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_64);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.s64;
        }
        else if constexpr (std::is_same_v<T, bn::fixed>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::FIXED);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.fixed;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::BOOL);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.flag;
        }
        else if constexpr (std::is_same_v<T, bn::string_view>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::STRING);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.str;
        }
        else if constexpr (std::is_same_v<T, bn::color>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::COLOR);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.color;
        }
        else if constexpr (std::is_scoped_enum_v<T>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::TYPED_ENUM);
            LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<T>());
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.t_enum.get<T>();
        }
        // else if constexpr (std::is_same_v<T, tile>)
        // {
        //     LDTK_FIELD_TYPE_ASSERT(field_type::TILE);
        //     LDTK_FIELD_GETTER_NULL_CHECK;
        //     return _value.tile;
        // }
        else if constexpr (std::is_same_v<T, entity_ref>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::ENTITY_REF);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.ent_ref;
        }
        else if constexpr (std::is_same_v<T, bn::point>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::POINT);
            LDTK_FIELD_GETTER_NULL_CHECK;
            return _value.point;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::uint8_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_8_SPAN);
            return _value.u8_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::int8_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_8_SPAN);
            return _value.s8_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::uint16_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_16_SPAN);
            return _value.u16_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::int16_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_16_SPAN);
            return _value.s16_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::uint32_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_32_SPAN);
            return _value.u32_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::int32_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_32_SPAN);
            return _value.s32_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::uint64_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::UINT_64_SPAN);
            return _value.u64_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const std::int64_t>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::INT_64_SPAN);
            return _value.s64_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::fixed>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::FIXED_SPAN);
            return _value.fixed_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bool>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::BOOL_SPAN);
            return _value.flag_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::string_view>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::STRING_SPAN);
            return _value.str_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::color>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::COLOR_SPAN);
            return _value.color_span;
        }
        else if constexpr (priv::span_of_const_scoped_enum<T>)
        {
            using Enum = std::remove_const_t<typename T::value_type>;

            LDTK_FIELD_TYPE_ASSERT(field_type::TYPED_ENUM_SPAN);
            LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<Enum>());
            return _value.t_enum_span.get<Enum>();
        }
        // else if constexpr (std::is_same_v<T, bn::span<const tile>>)
        // {
        //     LDTK_FIELD_TYPE_ASSERT(field_type::TILE_SPAN);
        //     return _value.tile_span;
        // }
        else if constexpr (std::is_same_v<T, bn::span<const entity_ref>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::ENTITY_REF_SPAN);
            return _value.ent_ref_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::point>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::POINT_SPAN);
            return _value.point_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::uint8_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_8_SPAN);
            return _value.opt_u8_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::int8_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_8_SPAN);
            return _value.opt_s8_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::uint16_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_16_SPAN);
            return _value.opt_u16_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::int16_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_16_SPAN);
            return _value.opt_s16_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::uint32_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_32_SPAN);
            return _value.opt_u32_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::int32_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_32_SPAN);
            return _value.opt_s32_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::uint64_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_UINT_64_SPAN);
            return _value.opt_u64_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<std::int64_t>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_INT_64_SPAN);
            return _value.opt_s64_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::fixed>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_FIXED_SPAN);
            return _value.opt_fixed_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::string_view>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_STRING_SPAN);
            return _value.opt_str_span;
        }
        else if constexpr (priv::span_of_const_optional_scoped_enum<T>)
        {
            using Enum = std::remove_const_t<typename T::value_type::value_type>;

            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_TYPED_ENUM_SPAN);
            LDTK_FIELD_ENUM_TYPE_ASSERT(bn::type_id<Enum>());
            return _value.opt_t_enum_span.get<Enum>();
        }
        // else if constexpr (std::is_same_v<T, bn::span<const bn::optional<tile>>>)
        // {
        //     LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_TILE_SPAN);
        //     return _value.opt_tile_span;
        // }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<entity_ref>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_ENTITY_REF_SPAN);
            return _value.opt_ent_ref_span;
        }
        else if constexpr (std::is_same_v<T, bn::span<const bn::optional<bn::point>>>)
        {
            LDTK_FIELD_TYPE_ASSERT(field_type::OPTIONAL_POINT_SPAN);
            return _value.opt_point_span;
        }
        else
        {
            static_assert(false, "Invalid type parameter T provided");
        }
    }

private:
    const field_definition& _def;

    // `false` is possible only for non-spans;
    // spans are always initialized, thus always `true`.
    bool _has_value;

    /// @cond DO_NOT_DOCUMENT
    union data_t {
        char empty;

        std::uint8_t u8;
        std::int8_t s8;
        std::uint16_t u16;
        std::int16_t s16;
        std::uint32_t u32;
        std::int32_t s32;
        std::uint64_t u64;
        std::int64_t s64;
        bn::fixed fixed;
        bool flag;
        bn::string_view str;
        bn::color color;
        priv::typed_enum t_enum;
        // tile tile;
        entity_ref ent_ref;
        bn::point point;

        bn::span<const std::uint8_t> u8_span;
        bn::span<const std::int8_t> s8_span;
        bn::span<const std::uint16_t> u16_span;
        bn::span<const std::int16_t> s16_span;
        bn::span<const std::uint32_t> u32_span;
        bn::span<const std::int32_t> s32_span;
        bn::span<const std::uint64_t> u64_span;
        bn::span<const std::int64_t> s64_span;
        bn::span<const bn::fixed> fixed_span;
        bn::span<const bool> flag_span;
        bn::span<const bn::string_view> str_span;
        bn::span<const bn::color> color_span;
        priv::typed_enum_span t_enum_span;
        // bn::span<const tile> tile_span;
        bn::span<const entity_ref> ent_ref_span;
        bn::span<const bn::point> point_span;

        bn::span<const bn::optional<std::uint8_t>> opt_u8_span;
        bn::span<const bn::optional<std::int8_t>> opt_s8_span;
        bn::span<const bn::optional<std::uint16_t>> opt_u16_span;
        bn::span<const bn::optional<std::int16_t>> opt_s16_span;
        bn::span<const bn::optional<std::uint32_t>> opt_u32_span;
        bn::span<const bn::optional<std::int32_t>> opt_s32_span;
        bn::span<const bn::optional<std::uint64_t>> opt_u64_span;
        bn::span<const bn::optional<std::int64_t>> opt_s64_span;
        bn::span<const bn::optional<bn::fixed>> opt_fixed_span;
        bn::span<const bn::optional<bn::string_view>> opt_str_span;
        priv::optional_typed_enum_span opt_t_enum_span;
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
    /// @endcond
};

} // namespace ldtk
