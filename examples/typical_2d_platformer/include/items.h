// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "item.h"

#include "ldtk_gen_project.h"
#include "ldtk_gen_idents.h"

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

class items final
{
public:
    void load_from_level(const ldtk::level&, const bn::camera_ptr&);

    void update();

private:
    static constexpr int MAX_ITEMS_COUNT = ldtk::gen::gen_project.defs().get_entity_def(ldtk::gen::entity_ident::item).max_count();

    bn::vector<item, MAX_ITEMS_COUNT> _items;
};

} // namespace t2p
