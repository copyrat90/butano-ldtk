// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "mob.h"

#include <bn_vector.h>

namespace ldtk
{
class level;
}

namespace bn
{
class camera_ptr;
}

namespace t2p
{

class mobs final
{
public:
    void load_from_level(const ldtk::level&, const bn::camera_ptr&);

    void update();

private:
    static constexpr int MAX_MOBS_COUNT = 16;

    bn::vector<mob, MAX_MOBS_COUNT> _mobs;
};

} // namespace t2p
