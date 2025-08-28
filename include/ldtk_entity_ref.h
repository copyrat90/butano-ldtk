#pragma once

#include "ldtk_gen_iids_fwd.h"

namespace ldtk
{

class entity_ref
{
public:
    constexpr entity_ref(gen::entity_iid entity_iid, gen::layer_iid layer_iid, gen::level_iid level_iid)
        : _entity_iid(entity_iid), _layer_iid(layer_iid), _level_iid(level_iid)
    {
    }

public:
    /// @brief IID of the refered EntityInstance
    [[nodiscard]] constexpr auto entity_iid() -> gen::entity_iid
    {
        return _entity_iid;
    }

    /// @brief IID of the LayerInstance containing the refered EntityInstance
    [[nodiscard]] constexpr auto layer_iid() -> gen::layer_iid
    {
        return _layer_iid;
    }

    /// @brief IID of the Level containing the refered EntityInstance
    [[nodiscard]] constexpr auto level_iid() -> gen::level_iid
    {
        return _level_iid;
    }

private:
    gen::entity_iid _entity_iid;
    gen::layer_iid _layer_iid;
    gen::level_iid _level_iid;
};

} // namespace ldtk
