#pragma once

#include "ldtk_gen_iid_fwd.h"

namespace ldtk
{

class entity_ref
{
public:
    constexpr entity_ref(gen::iid entity_iid, gen::iid layer_iid, gen::iid level_iid)
        : _entity_iid(entity_iid), _layer_iid(layer_iid), _level_iid(level_iid)
    {
    }

public:
    /// @brief IID of the refered EntityInstance
    [[nodiscard]] constexpr auto entity_iid() -> gen::iid
    {
        return _entity_iid;
    }

    /// @brief IID of the LayerInstance containing the refered EntityInstance
    [[nodiscard]] constexpr auto layer_iid() -> gen::iid
    {
        return _layer_iid;
    }

    /// @brief IID of the Level containing the refered EntityInstance
    [[nodiscard]] constexpr auto level_iid() -> gen::iid
    {
        return _level_iid;
    }

private:
    gen::iid _entity_iid;
    gen::iid _layer_iid;
    gen::iid _level_iid;
};

} // namespace ldtk
