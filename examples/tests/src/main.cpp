// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_core.h"

#include "ldtk_gen_project.h"

#include <bn_backdrop.h>
#include <bn_core.h>

int main()
{
    bn::core::init();
    ldtk::core::init();

    bn::backdrop::set_color(ldtk::gen::gen_project.bg_color());

    while (true)
        bn::core::update();
}
