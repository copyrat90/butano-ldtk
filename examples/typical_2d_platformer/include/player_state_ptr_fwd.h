// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <memory>

namespace t2p
{

class player_state_base;
class player_state_deleter;

using player_state_ptr = std::unique_ptr<player_state_base, player_state_deleter>;

} // namespace t2p
