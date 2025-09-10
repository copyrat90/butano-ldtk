// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_gen_project.h"

#include <bn_math.h>

static constexpr bn::fixed EPSILON = bn::fixed(0.001);

[[nodiscard]] constexpr auto zero_approx(bn::fixed f1, bn::fixed f2) -> bool
{
    return bn::abs(f1 - f2) < EPSILON;
}

static_assert([] {
    const auto& level = ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::level_0);
    const auto& entities_layer = level.get_layer(ldtk::gen::layer_ident::entities);
    const auto& black_sq = entities_layer.find_entity(ldtk::gen::entity_iid::_fa264e90_8560_11f0_8a9b_53b5d9f4a711);
    const auto& green_sq = entities_layer.find_entity(ldtk::gen::entity_iid::_fefb7d50_8560_11f0_8a9b_6da874eeb8b3);

    using ef_id = ldtk::gen::entity_field_ident;

    if ((0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_int).get<std::int32_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u8).get<std::uint8_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s8).get<std::int8_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u16).get<std::uint16_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s16).get<std::int16_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u32).get<std::uint32_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s32).get<std::int32_t>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_float).get<bn::fixed>()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean).get<bool>()) ||
        ("" != black_sq.get_field(ef_id::ENTITY_entity_FIELD_string).get<bn::string_view>()) ||
        ("" != black_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines).get<bn::string_view>()) ||
        (bn::color(0, 0, 0) != black_sq.get_field(ef_id::ENTITY_entity_FIELD_color).get<bn::color>()) ||
        (ldtk::gen::my_enum::ev_0 !=
         black_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum).get<ldtk::gen::my_enum>()) ||
        (ldtk::gen::level_ident::level_0 !=
             black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().level_ident() ||
         ldtk::gen::layer_ident::entities !=
             black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().layer_ident() ||
         ldtk::gen::entity_iid::_fefb7d50_8560_11f0_8a9b_6da874eeb8b3 !=
             black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().entity_iid()) ||
        (bn::point(2, 5) != black_sq.get_field(ef_id::ENTITY_entity_FIELD_point).get<bn::point>()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_int_arr).get<bn::span<const std::int32_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_arr).get<bn::span<const std::uint8_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_arr).get<bn::span<const std::uint16_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_arr).get<bn::span<const std::uint32_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_arr).get<bn::span<const std::int32_t>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean_arr).get<bn::span<const bool>>().size()) ||
        (0 !=
         black_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>().size()) ||
        (0 !=
         black_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_color_arr).get<bn::span<const bn::color>>().size()) ||
        (0 !=
         black_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr).get<bn::span<const ldtk::gen::my_enum>>().size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_arr)
                  .get<bn::span<const ldtk::entity_ref>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_point_arr).get<bn::span<const bn::point>>().size()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt).has_value()) ||
        (false != black_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt).has_value()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                  .get<bn::span<const bn::optional<std::int32_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt_arr)
                  .get<bn::span<const bn::optional<std::uint8_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt_arr)
                  .get<bn::span<const bn::optional<std::int8_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt_arr)
                  .get<bn::span<const bn::optional<std::uint16_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                  .get<bn::span<const bn::optional<std::int16_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt_arr)
                  .get<bn::span<const bn::optional<std::uint32_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt_arr)
                  .get<bn::span<const bn::optional<std::int32_t>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                  .get<bn::span<const bn::optional<bn::fixed>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt_arr)
                  .get<bn::span<const bn::optional<bn::string_view>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt_arr)
                  .get<bn::span<const bn::optional<bn::string_view>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt_arr)
                  .get<bn::span<const bn::optional<ldtk::gen::my_enum>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt_arr)
                  .get<bn::span<const bn::optional<ldtk::entity_ref>>>()
                  .size()) ||
        (0 != black_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                  .get<bn::span<const bn::optional<bn::point>>>()
                  .size()))
        return false;

    if ((32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int).get<std::int32_t>()) ||
        (255 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8).get<std::uint8_t>()) ||
        (-128 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8).get<std::int8_t>()) ||
        (65535 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16).get<std::uint16_t>()) ||
        (-32768 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16).get<std::int16_t>()) ||
        (2147483u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32).get<std::uint32_t>()) ||
        (-2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32).get<std::int32_t>()) ||
        (!zero_approx(bn::fixed(3.1415), green_sq.get_field(ef_id::ENTITY_entity_FIELD_float).get<bn::fixed>())) ||
        (true != green_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean).get<bool>()) ||
        ("Hello, world!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string).get<bn::string_view>()) ||
        ("Goodbye,\nworld!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines).get<bn::string_view>()) ||
        (bn::color(10, 20, 7) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_color).get<bn::color>()) ||
        (ldtk::gen::my_enum::ev_3 !=
         green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum).get<ldtk::gen::my_enum>()) ||
        (level.identifier() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().level_ident() ||
         entities_layer.identifier() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().layer_ident() ||
         black_sq.iid() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref).get<ldtk::entity_ref>().entity_iid()) ||
        (bn::point(8, 5) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point).get<bn::point>()) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_arr).get<bn::span<const std::int32_t>>().size() ||
         -32768 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_arr).get<bn::span<const std::int32_t>>()[0] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_arr).get<bn::span<const std::int32_t>>()[1] ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_arr).get<bn::span<const std::int32_t>>()[2]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_arr).get<bn::span<const std::uint8_t>>().size() ||
         255 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_arr).get<bn::span<const std::uint8_t>>()[0] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_arr).get<bn::span<const std::uint8_t>>()[1] ||
         127 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_arr).get<bn::span<const std::uint8_t>>()[2]) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>().size() ||
         127 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>()[0] ||
         -128 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>()[1] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>()[2] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_arr).get<bn::span<const std::int8_t>>()[3]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_arr).get<bn::span<const std::uint16_t>>().size() ||
         65535 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_arr).get<bn::span<const std::uint16_t>>()[0] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_arr).get<bn::span<const std::uint16_t>>()[1] ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_arr).get<bn::span<const std::uint16_t>>()[2]) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>().size() ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>()[0] ||
         -32768 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>()[1] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>()[2] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_arr).get<bn::span<const std::int16_t>>()[3]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_arr).get<bn::span<const std::uint32_t>>().size() ||
         0u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_arr).get<bn::span<const std::uint32_t>>()[0] ||
         0u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_arr).get<bn::span<const std::uint32_t>>()[1] ||
         2147483u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_arr).get<bn::span<const std::uint32_t>>()[2]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_arr).get<bn::span<const std::int32_t>>().size() ||
         2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_arr).get<bn::span<const std::int32_t>>()[0] ||
         -2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_arr).get<bn::span<const std::int32_t>>()[1] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_arr).get<bn::span<const std::int32_t>>()[2]) ||
        (6 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>().size() ||
         !zero_approx(bn::fixed(0),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[0]) ||
         !zero_approx(bn::fixed(3.1415),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[1]) ||
         !zero_approx(bn::fixed(31.4159),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[2]) ||
         !zero_approx(bn::fixed(314.1592),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[3]) ||
         !zero_approx(bn::fixed(3141.5926),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[4]) ||
         !zero_approx(bn::fixed(31415.9265),
                      green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_arr).get<bn::span<const bn::fixed>>()[5])) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean_arr).get<bn::span<const bool>>().size() ||
         false != green_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean_arr).get<bn::span<const bool>>()[0] ||
         true != green_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean_arr).get<bn::span<const bool>>()[1] ||
         false != green_sq.get_field(ef_id::ENTITY_entity_FIELD_boolean_arr).get<bn::span<const bool>>()[2]) ||
        (5 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>().size() ||
         "" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>()[0] ||
         "Hello, world!" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>()[1] ||
         "안녕, 세상아!" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>()[2] ||
         "'" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>()[3] ||
         "\"" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_arr).get<bn::span<const bn::string_view>>()[4]) ||
        (5 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr)
                  .get<bn::span<const bn::string_view>>()
                  .size() ||
         "" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>()[0] ||
         "Goodbye,\nworld!" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>()[1] ||
         "\n\nLine 3" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>()[2] ||
         "Line 1\n\n" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>()[3] ||
         "\nLine 2\n" !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_arr).get<bn::span<const bn::string_view>>()[4]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_color_arr).get<bn::span<const bn::color>>().size() ||
         bn::color(31, 0, 0) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_color_arr).get<bn::span<const bn::color>>()[0] ||
         bn::color(0, 25, 31) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_color_arr).get<bn::span<const bn::color>>()[1] ||
         bn::color(31, 31, 0) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_color_arr).get<bn::span<const bn::color>>()[2]) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr)
                  .get<bn::span<const ldtk::gen::my_enum>>()
                  .size() ||
         ldtk::gen::my_enum::ev_2 !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr).get<bn::span<const ldtk::gen::my_enum>>()[0] ||
         ldtk::gen::my_enum::ev_1 !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr).get<bn::span<const ldtk::gen::my_enum>>()[1] ||
         ldtk::gen::my_enum::ev_3 !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr).get<bn::span<const ldtk::gen::my_enum>>()[2] ||
         ldtk::gen::my_enum::ev_0 !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_arr).get<bn::span<const ldtk::gen::my_enum>>()[3]) ||
        (1 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_arr)
                  .get<bn::span<const ldtk::entity_ref>>()
                  .size() ||
         level.identifier() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_arr)
                                   .get<bn::span<const ldtk::entity_ref>>()[0]
                                   .level_ident() ||
         entities_layer.identifier() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_arr)
                                            .get<bn::span<const ldtk::entity_ref>>()[0]
                                            .layer_ident() ||
         black_sq.iid() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_arr)
                               .get<bn::span<const ldtk::entity_ref>>()[0]
                               .entity_iid()) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_arr).get<bn::span<const bn::point>>().size() ||
         bn::point(10, 0) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_arr).get<bn::span<const bn::point>>()[0] ||
         bn::point(12, 0) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_arr).get<bn::span<const bn::point>>()[1] ||
         bn::point(14, 0) !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_arr).get<bn::span<const bn::point>>()[2]) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt).has_value() ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt).get<std::int32_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt).has_value() ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt).get<std::uint8_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt).has_value() ||
         127 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt).get<std::int8_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt).has_value() ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt).get<std::uint16_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt).has_value() ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt).get<std::int16_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt).has_value() ||
         2147483u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt).get<std::uint32_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt).has_value() ||
         2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt).get<std::int32_t>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt).has_value() ||
         !zero_approx(bn::fixed(3.1415), green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt).get<bn::fixed>())) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt).has_value() ||
         "Hello, world!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt).get<bn::string_view>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt).has_value() ||
         "Goodbye,\nworld!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt).get<bn::string_view>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt).has_value() ||
         ldtk::gen::my_enum::ev_1 !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt).get<ldtk::gen::my_enum>()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt).has_value() ||
         level.identifier() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt).get<ldtk::entity_ref>().level_ident() ||
         entities_layer.identifier() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt).get<ldtk::entity_ref>().layer_ident() ||
         black_sq.iid() !=
             green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt).get<ldtk::entity_ref>().entity_iid()) ||
        (!green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt).has_value() ||
         bn::point(10, 2) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt).get<bn::point>()) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                  .get<bn::span<const bn::optional<std::int32_t>>>()
                  .size() ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                            .get<bn::span<const bn::optional<std::int32_t>>>()[0] ||
         -32768 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                       .get<bn::span<const bn::optional<std::int32_t>>>()[1] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                            .get<bn::span<const bn::optional<std::int32_t>>>()[2] ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_int_opt_arr)
                      .get<bn::span<const bn::optional<std::int32_t>>>()[3]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt_arr)
                  .get<bn::span<const bn::optional<std::uint8_t>>>()
                  .size() ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt_arr)
                            .get<bn::span<const bn::optional<std::uint8_t>>>()[0] ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt_arr)
                  .get<bn::span<const bn::optional<std::uint8_t>>>()[1] ||
         255 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u8_opt_arr)
                    .get<bn::span<const bn::optional<std::uint8_t>>>()[2]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt_arr)
                  .get<bn::span<const bn::optional<std::int8_t>>>()
                  .size() ||
         -128 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt_arr)
                     .get<bn::span<const bn::optional<std::int8_t>>>()[0] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt_arr)
                            .get<bn::span<const bn::optional<std::int8_t>>>()[1] ||
         127 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s8_opt_arr)
                    .get<bn::span<const bn::optional<std::int8_t>>>()[2]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt_arr)
                  .get<bn::span<const bn::optional<std::uint16_t>>>()
                  .size() ||
         0 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt_arr)
                  .get<bn::span<const bn::optional<std::uint16_t>>>()[0] ||
         65535 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt_arr)
                      .get<bn::span<const bn::optional<std::uint16_t>>>()[1] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u16_opt_arr)
                            .get<bn::span<const bn::optional<std::uint16_t>>>()[2]) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                  .get<bn::span<const bn::optional<std::int16_t>>>()
                  .size() ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                            .get<bn::span<const bn::optional<std::int16_t>>>()[0] ||
         -32768 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                       .get<bn::span<const bn::optional<std::int16_t>>>()[1] ||
         32767 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                      .get<bn::span<const bn::optional<std::int16_t>>>()[2] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s16_opt_arr)
                            .get<bn::span<const bn::optional<std::int16_t>>>()[3]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt_arr)
                  .get<bn::span<const bn::optional<std::uint32_t>>>()
                  .size() ||
         0u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt_arr)
                   .get<bn::span<const bn::optional<std::uint32_t>>>()[0] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt_arr)
                            .get<bn::span<const bn::optional<std::uint32_t>>>()[1] ||
         2147483u != green_sq.get_field(ef_id::ENTITY_entity_FIELD_u32_opt_arr)
                         .get<bn::span<const bn::optional<std::uint32_t>>>()[2]) ||
        (2 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt_arr)
                  .get<bn::span<const bn::optional<std::int32_t>>>()
                  .size() ||
         -2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt_arr)
                         .get<bn::span<const bn::optional<std::int32_t>>>()[0] ||
         2147483 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_s32_opt_arr)
                        .get<bn::span<const bn::optional<std::int32_t>>>()[1]) ||
        (6 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                  .get<bn::span<const bn::optional<bn::fixed>>>()
                  .size() ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                            .get<bn::span<const bn::optional<bn::fixed>>>()[0] ||
         !zero_approx(bn::fixed(3.1415), *green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                                              .get<bn::span<const bn::optional<bn::fixed>>>()[1]) ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                            .get<bn::span<const bn::optional<bn::fixed>>>()[2] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                            .get<bn::span<const bn::optional<bn::fixed>>>()[3] ||
         !zero_approx(bn::fixed(31415.9265), *green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                                                  .get<bn::span<const bn::optional<bn::fixed>>>()[4]) ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_float_opt_arr)
                            .get<bn::span<const bn::optional<bn::fixed>>>()[5]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt_arr)
                  .get<bn::span<const bn::optional<bn::string_view>>>()
                  .size() ||
         "Goodbye, world!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt_arr)
                                  .get<bn::span<const bn::optional<bn::string_view>>>()[0] ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt_arr)
                            .get<bn::span<const bn::optional<bn::string_view>>>()[1] ||
         "잘있어, 세상아!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_string_opt_arr)
                                  .get<bn::span<const bn::optional<bn::string_view>>>()[2]) ||
        (3 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt_arr)
                  .get<bn::span<const bn::optional<bn::string_view>>>()
                  .size() ||
         bn::nullopt != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt_arr)
                            .get<bn::span<const bn::optional<bn::string_view>>>()[0] ||
         "Hello,\nworld!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt_arr)
                                 .get<bn::span<const bn::optional<bn::string_view>>>()[1] ||
         "안녕,\n세상아!" != green_sq.get_field(ef_id::ENTITY_entity_FIELD_multilines_opt_arr)
                                 .get<bn::span<const bn::optional<bn::string_view>>>()[2]) ||
        (1 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt_arr)
                  .get<bn::span<const bn::optional<ldtk::gen::my_enum>>>()
                  .size() ||
         ldtk::gen::my_enum::ev_2 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_my_enum_opt_arr)
                                         .get<bn::span<const bn::optional<ldtk::gen::my_enum>>>()[0]) ||
        (1 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt_arr)
                  .get<bn::span<const bn::optional<ldtk::entity_ref>>>()
                  .size() ||
         level.identifier() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt_arr)
                                   .get<bn::span<const bn::optional<ldtk::entity_ref>>>()[0]
                                   ->level_ident() ||
         entities_layer.identifier() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt_arr)
                                            .get<bn::span<const bn::optional<ldtk::entity_ref>>>()[0]
                                            ->layer_ident() ||
         black_sq.iid() != green_sq.get_field(ef_id::ENTITY_entity_FIELD_entity_ref_opt_arr)
                               .get<bn::span<const bn::optional<ldtk::entity_ref>>>()[0]
                               ->entity_iid()) ||
        (4 != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                  .get<bn::span<const bn::optional<bn::point>>>()
                  .size() ||
         bn::point(10, 4) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                                 .get<bn::span<const bn::optional<bn::point>>>()[0] ||
         bn::point(12, 4) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                                 .get<bn::span<const bn::optional<bn::point>>>()[1] ||
         bn::point(14, 4) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                                 .get<bn::span<const bn::optional<bn::point>>>()[2] ||
         bn::point(14, 6) != green_sq.get_field(ef_id::ENTITY_entity_FIELD_point_opt_arr)
                                 .get<bn::span<const bn::optional<bn::point>>>()[3]))
        return false;

    return true;
}());
