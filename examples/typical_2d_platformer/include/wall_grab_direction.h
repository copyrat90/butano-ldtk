// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include <cstdint>

namespace t2p
{

enum class wall_grab_direction : std::uint8_t
{
    NONE,
    LEFT_WALL,
    RIGHT_WALL,
};

} // namespace t2p
