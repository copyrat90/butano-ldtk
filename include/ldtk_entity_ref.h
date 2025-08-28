#pragma once

#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_iids_fwd.h"

namespace ldtk
{

class entity_ref
{
public:
    constexpr entity_ref(gen::entity_iid entity_iid, gen::layer_ident layer_ident, gen::level_ident level_ident)
        : _entity_iid(entity_iid), _layer_ident(layer_ident), _level_ident(level_ident)
    {
    }

public:
    /// @brief IID of the refered EntityInstance
    [[nodiscard]] constexpr auto entity_iid() -> gen::entity_iid
    {
        return _entity_iid;
    }

    /// @brief Identifier of the LayerInstance containing the refered EntityInstance
    [[nodiscard]] constexpr auto layer_ident() -> gen::layer_ident
    {
        return _layer_ident;
    }

    /// @brief Identifier of the Level containing the refered EntityInstance
    [[nodiscard]] constexpr auto level_ident() -> gen::level_ident
    {
        return _level_ident;
    }

private:
    gen::entity_iid _entity_iid;
    gen::layer_ident _layer_ident;
    gen::level_ident _level_ident;
};

} // namespace ldtk
