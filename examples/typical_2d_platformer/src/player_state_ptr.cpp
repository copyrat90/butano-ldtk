// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player_state_ptr.h"

namespace t2p
{

player_state_deleter::player_state_deleter(player_states_pool_t& pool) : _pool(&pool)
{
}

void player_state_deleter::operator()(player_state_base* state) const
{
    _pool->destroy(*state);
}

} // namespace t2p
