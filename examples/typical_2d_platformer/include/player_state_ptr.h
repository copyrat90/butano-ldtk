// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "player_state_ptr_fwd.h"

#include "player_states.h"

#include <bn_generic_pool.h>

namespace t2p
{

using player_states_pool_t = bn::generic_pool<MAX_PLAYER_STATE_SIZE, 3>;

class player_state_deleter
{
public:
    player_state_deleter() = default;
    player_state_deleter(player_states_pool_t&);

    void operator()(player_state_base* state) const;

private:
    player_states_pool_t* _pool = nullptr;
};

#if BN_VERSION_MAJOR > 20 || (BN_VERSION_MAJOR == 20 && BN_VERSION_MINOR >= 3)
// Custom deleters for `bn::unique_ptr` are supported after Butano 20.3.0
using player_state_ptr = bn::unique_ptr<player_state_base, player_state_deleter>;
#else
using player_state_ptr = std::unique_ptr<player_state_base, player_state_deleter>;
#endif

} // namespace t2p
