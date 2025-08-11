#pragma once

#include "ldtk_gen_ident_fwd.h"

#include <bn_optional.h>

namespace ldtk
{

class int_grid_value_group_info
{
public:
    constexpr int_grid_value_group_info(const bn::optional<gen::ident>& identifier, int uid)
        : _identifier(identifier), _uid(uid)
    {
    }

public:
    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> const bn::optional<gen::ident>&
    {
        return _identifier;
    }

    /// @brief Group unique ID
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

private:
    bn::optional<gen::ident> _identifier;
    int _uid;
};

} // namespace ldtk
