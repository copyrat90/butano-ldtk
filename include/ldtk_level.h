// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_field.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_iids_fwd.h"
#include "ldtk_layer.h"
#include "ldtk_level_bgs_ptr.h"

#include <bn_assert.h>
#include <bn_color.h>
#include <bn_fixed_point.h>
#include <bn_point.h>
#include <bn_size.h>
#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class level
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr level(bn::color bg_color, const bn::span<const field>& field_instances, gen::level_ident identifier,
                    gen::level_iid iid, const bn::span<const layer>& layer_instances, const bn::size& px_size, int uid,
                    int world_depth, const bn::point& world_coord)
        : _bg_color(bg_color), _field_instances(field_instances), _identifier(identifier), _iid(iid),
          _layer_instances(layer_instances), _px_size(px_size), _uid(uid), _world_depth(world_depth),
          _world_coord(world_coord)
    {
    }
    /// @endcond

public:
    /// @brief Looks up a layer with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the layer to search for.
    /// @return Reference to the layer.
    [[nodiscard]] constexpr auto get_layer(gen::layer_ident identifier) const -> const layer&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::layer_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _layer_instances.size(), "Out of bound identifier (gen::layer_ident)",
                  (int)identifier);

        return _layer_instances.data()[(int)identifier];
    }

    /// @brief Linear searches a layer with its Instance id.
    /// @note If you know the identifier, prefer `get_layer()` instead, as that's O(1). \n
    /// Not finding the layer errors out;
    /// You should @b never use layer IIDs that's for other level.
    /// @param iid Instance id of the layer to search for.
    /// @return Reference to the found layer.
    [[deprecated("Use `get_layer()` instead. Linear searching a layer with `iid` shouldn't be "
                 "necessary.")]] [[nodiscard]] constexpr auto
    find_layer(gen::layer_iid iid) const -> const layer&
    {
        auto iter = std::ranges::find_if(_layer_instances, [iid](const layer& ly) { return ly.iid() == iid; });
        BN_ASSERT(iter != _layer_instances.end(), "Layer not found with (gen::layer_iid)", (int)iid,
                  " - perhaps it's a layer of different level");

        return *iter;
    }

    /// @brief Looks up a field with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the field to look up.
    /// @return Reference to the field.
    [[nodiscard]] constexpr auto get_field(gen::level_field_ident identifier) const -> const field&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::level_field_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _field_instances.size(), "Out of bound identifier (gen::level_field_ident)",
                  (int)identifier);

        return _field_instances.data()[(int)identifier];
    }

public:
    /// @brief Creates a `level_bgs_ptr` using the information contained in this level.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] auto create_bgs() const -> level_bgs_ptr
    {
        return level_bgs_ptr::create(*this);
    }

    /// @brief Creates a `level_bgs_ptr` using the information contained in this level.
    /// @param x Horizontal position of the level backgrounds.
    /// @param y Vertical position of the level backgrounds.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] auto create_bgs(bn::fixed x, bn::fixed y) const -> level_bgs_ptr
    {
        return level_bgs_ptr::create(x, y, *this);
    }

    /// @brief Creates a `level_bgs_ptr` using the information contained in this level.
    /// @param position Position of the level backgrounds.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] auto create_bgs(const bn::fixed_point& position) const -> level_bgs_ptr
    {
        return level_bgs_ptr::create(position, *this);
    }

public:
    /// @brief Background color of the level
    [[nodiscard]] constexpr auto bg_color() const -> bn::color
    {
        return _bg_color;
    }

    /// @brief An array containing this level custom field values.
    [[nodiscard]] constexpr auto field_instances() const -> const bn::span<const field>&
    {
        return _field_instances;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::level_ident
    {
        return _identifier;
    }

    /// @brief Unique level instance id
    [[nodiscard]] constexpr auto iid() const -> gen::level_iid
    {
        return _iid;
    }

    /// @brief An array containing all Layer instances.
    /// @note This array is **sorted in display order**: the 1st layer is the top-most and the last is behind.
    [[nodiscard]] constexpr auto layer_instances() const -> const bn::span<const layer>&
    {
        return _layer_instances;
    }

    /// @brief Size of the level in pixels
    [[nodiscard]] constexpr auto px_size() const -> const bn::size&
    {
        return _px_size;
    }

    /// @brief Width of the level in pixels
    [[nodiscard]] constexpr auto px_width() const -> int
    {
        return px_size().width();
    }

    /// @brief Height of the level in pixels
    [[nodiscard]] constexpr auto px_height() const -> int
    {
        return px_size().height();
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

    /// @brief Index that represents the "depth" of the level in the world.
    /// Default is 0, greater means "above", lower means "below". \n
    /// This value is mostly used for display only and is intended to make stacking of levels easier to manage.
    [[nodiscard]] constexpr auto world_depth() const -> int
    {
        return _world_depth;
    }

    /// @brief World coordinate in pixels. \n
    /// Only relevant for world layouts where level spatial positioning is manual (ie. GridVania, Free).
    /// For Horizontal and Vertical layouts, the value is always -1 here.
    [[nodiscard]] constexpr auto world_coord() const -> const bn::point&
    {
        return _world_coord;
    }

    /// @brief World X coordinate in pixels. \n
    /// Only relevant for world layouts where level spatial positioning is manual (ie. GridVania, Free).
    /// For Horizontal and Vertical layouts, the value is always -1 here.
    [[nodiscard]] constexpr auto world_x() const -> int
    {
        return world_coord().x();
    }

    /// @brief World Y coordinate in pixels. \n
    /// Only relevant for world layouts where level spatial positioning is manual (ie. GridVania, Free).
    /// For Horizontal and Vertical layouts, the value is always -1 here.
    [[nodiscard]] constexpr auto world_y() const -> int
    {
        return world_coord().y();
    }

private:
    bn::color _bg_color;
    bn::span<const field> _field_instances;
    gen::level_ident _identifier;
    gen::level_iid _iid;
    bn::span<const layer> _layer_instances;
    bn::size _px_size;
    int _uid;
    int _world_depth;
    bn::point _world_coord;
};

} // namespace ldtk
