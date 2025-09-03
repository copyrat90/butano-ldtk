// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "player_state_ptr.h"
#include "player_states.h"

#include <concepts>
#include <utility>

namespace ldtk
{
class level;
}

namespace t2p
{

class player;

class player_state_machine final
{
public:
    player_state_machine(player&);

    void handle_input(player&);
    void update(player&, const ldtk::level&);

    auto collision(const player&) const -> bn::top_left_fixed_rect;

public:
    template <std::derived_from<player_state_base> State, typename... Args>
    [[nodiscard]] auto create_state(Args&&... args) -> player_state_ptr
    {
        return player_state_ptr(&_states_pool.create<State>(std::forward<Args>(args)...),
                                player_state_deleter(_states_pool));
    }

private:
    [[nodiscard]] auto init_state(player&) -> player_state_ptr;

private:
    player_states_pool_t _states_pool;
    player_state_ptr _state;
};

} // namespace t2p
