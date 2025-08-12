#pragma once

#include "ldtk_definitions.h"
#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_iid_fwd.h"
#include "ldtk_level.h"

#include <bn_assert.h>
#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class project
{
public:
    constexpr project(const definitions& def, gen::iid iid, const bn::span<const level>& levels)
        : _def(def), _iid(iid), _levels(levels)
    {
    }

public:
    /// @brief Looks up a level with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1). \n
    /// @b Never use non-level identifier,
    /// that would result in an error, or getting the wrong level.
    /// @param identifier Unique identifier of the level to look up.
    /// @return Reference to the level.
    [[nodiscard]] constexpr auto get_level(gen::ident identifier) const -> const level&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::ident)", (int)identifier);
        BN_ASSERT((int)identifier < _levels.size(), "Out of bound identifier (get::ident)", identifier,
                  " - it's a non-level identifier");

        return _levels.data()[(int)identifier];
    }

    /// @brief Linear searches a level with its Instance id.
    /// @note If you know the identifier, prefer `get_level()` instead, as that's O(1). \n
    /// Not finding the level errors out;
    /// You should @b never use non-level IID, then you'll always find the level.
    /// @param iid Instance id of the level to search for.
    /// @return Reference to the found level.
    [[nodiscard]] constexpr auto find_level(gen::iid iid) const -> const level&
    {
        auto iter = std::ranges::find_if(_levels, [iid](const level& lv) { return lv.iid() == iid; });
        BN_ASSERT(iter != _levels.end(), "Level not found with (gen::iid)", (int)iid, " - it's a non-level IID");

        return *iter;
    }

public:
    /// @brief A structure containing all the definitions of this project
    [[nodiscard]] constexpr auto def() const -> const definitions&
    {
        return _def;
    }

    /// @brief Unique project identifier
    [[nodiscard]] constexpr auto iid() const -> gen::iid
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

private:
    const definitions& _def;

    gen::iid _iid;
    bn::span<const level> _levels;
};

} // namespace ldtk
