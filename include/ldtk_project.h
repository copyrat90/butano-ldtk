#pragma once

#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_iid_fwd.h"
#include "ldtk_level.h"

#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class project
{
public:
    constexpr project(gen::iid iid, const bn::span<const level>& levels) : _iid(iid), _levels(levels)
    {
    }

public:
    /// @brief Linear searches a level.
    /// @param iid Instance id of the level to search for.
    /// @return Pointer to the found level, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto find_level(gen::iid iid) const -> const level*
    {
        auto iter = std::ranges::find_if(_levels, [iid](const level& lv) { return lv.iid() == iid; });

        if (iter == _levels.end())
            return nullptr;

        return &*iter;
    }

    /// @brief Linear searches a level.
    /// @param identifier Unique identifier of the level to search for.
    /// @return Pointer to the found level, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto find_level(gen::ident identifier) const -> const level*
    {
        auto iter =
            std::ranges::find_if(_levels, [identifier](const level& lv) { return lv.identifier() == identifier; });

        if (iter == _levels.end())
            return nullptr;

        return &*iter;
    }

public:
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
    gen::iid _iid;
    bn::span<const level> _levels;
};

} // namespace ldtk
