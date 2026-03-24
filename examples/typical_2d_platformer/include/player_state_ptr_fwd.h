// SPDX-FileCopyrightText: Copyright 2025-2026 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <bn_version.h>

#if BN_VERSION_MAJOR > 20 || (BN_VERSION_MAJOR == 20 && BN_VERSION_MINOR >= 3)
#include <bn_unique_ptr.h>
#else
#include <memory>
#endif

namespace t2p
{

class player_state_base;
class player_state_deleter;

#if BN_VERSION_MAJOR > 20 || (BN_VERSION_MAJOR == 20 && BN_VERSION_MINOR >= 3)
// Custom deleters for `bn::unique_ptr` are supported after Butano 20.3.0
using player_state_ptr = bn::unique_ptr<player_state_base, player_state_deleter>;
#else
using player_state_ptr = std::unique_ptr<player_state_base, player_state_deleter>;
#endif

} // namespace t2p
