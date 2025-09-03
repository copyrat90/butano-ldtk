// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player_state_machine.h"

namespace t2p
{

player_state_machine::player_state_machine(player& player_) : _state(init_state(player_))
{
}

void player_state_machine::handle_input(player& player_)
{
    _state->handle_input(player_);
}

void player_state_machine::update(player& player_, const ldtk::level& level)
{
    player_state_ptr next_state = _state->update(player_, level);

    if (next_state)
    {
        _state->exit(player_);
        _state = std::move(next_state);
        _state->enter(player_);
    }
}

auto player_state_machine::collision(const player& player_) const -> bn::top_left_fixed_rect
{
    return _state->collision(player_);
}

auto player_state_machine::init_state(player& player_) -> player_state_ptr
{
    player_state_ptr state = create_state<player_idle_state>();
    state->enter(player_);
    return state;
}

} // namespace t2p
