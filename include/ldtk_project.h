// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "ldtk_definitions.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_iids_fwd.h"
#include "ldtk_level.h"

#include <bn_assert.h>
#include <bn_color.h>
#include <bn_fixed.h>
#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class project
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr project(const definitions& defs, gen::project_iid iid, const bn::span<const level>& levels,
                      bn::color bg_color, bn::fixed opacity)
        : _defs(defs), _iid(iid), _levels(levels), _bg_color(bg_color), _opacity(opacity)
    {
    }
    /// @endcond

    /// @brief Deleted copy constructor.
    constexpr project(const project&) = delete;

    /// @brief Deleted copy assignment operator.
    constexpr project& operator=(const project&) = delete;

    /// @brief Defaulted move constructor.
    constexpr project(project&&) = default;

    /// @brief Defaulted move assignment operator.
    constexpr project& operator=(project&&) = default;

public:
    /// @brief Looks up a level with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1).
    /// @param identifier Unique identifier of the level to look up.
    /// @return Reference to the level.
    [[nodiscard]] constexpr auto get_level(gen::level_ident identifier) const -> const level&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::level_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _levels.size(), "Out of bound identifier (gen::level_ident)", (int)identifier);

        return _levels.data()[(int)identifier];
    }

    /// @brief Linear searches a level with its Instance id.
    /// @note If you know the identifier, prefer `get_level()` instead, as that's O(1).
    /// @param iid Instance id of the level to search for.
    /// @return Reference to the found level.
    [[deprecated("Use `get_level()` instead. Linear searching a level with `iid` shouldn't be "
                 "necessary.")]] [[nodiscard]] constexpr auto
    find_level(gen::level_iid iid) const -> const level&
    {
        auto iter = std::ranges::find_if(_levels, [iid](const level& lv) { return lv.iid() == iid; });
        BN_ASSERT(iter != _levels.end(), "Level not found with (gen::level_iid)", (int)iid);

        return *iter;
    }

public:
    /// @brief A structure containing all the definitions of this project
    [[nodiscard]] constexpr auto defs() const -> const definitions&
    {
        return _defs;
    }

    /// @brief Unique project identifier
    [[nodiscard]] constexpr auto iid() const -> gen::project_iid
    {
        return _iid;
    }

    /// @brief All levels.
    /// The order of this array is only relevant in `linear_horizontal` and `linear_vertical` world layouts
    /// (see `worldLayout` value). \n
    /// Otherwise, you should refer to the `world_x`,`world_y` coordinates of each Level.
    [[nodiscard]] constexpr auto levels() const -> const bn::span<const level>&
    {
        return _levels;
    }

    /// @brief Project background color
    [[nodiscard]] constexpr auto bg_color() const -> bn::color
    {
        return _bg_color;
    }

    /// @brief Global opacity used for semi-transparent layers
    [[nodiscard]] constexpr auto opacity() const -> bn::fixed
    {
        return _opacity;
    }

private:
    const definitions& _defs;

    gen::project_iid _iid;
    bn::span<const level> _levels;
    bn::color _bg_color;
    bn::fixed _opacity;
};

} // namespace ldtk
