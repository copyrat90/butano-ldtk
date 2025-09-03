// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_core.h"

#include "game.h"

#include <bn_core.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string_view.h>

#include "common_info.h"
#include "common_stats.h"
#include "common_variable_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"

int main()
{
    bn::core::init();
    ldtk::core::init();

    constexpr bn::string_view info_text_lines[] = {
        "Left/Right: move", "A: jump (Wall+A: wall jump)", "Down: crouch", "Up: look up", "", "START: go to next level",
    };

    bn::sprite_text_generator big_text_generator(common::variable_8x16_sprite_font);
    common::info info("Typical 2d platformer", info_text_lines, big_text_generator);

    bn::sprite_text_generator small_text_generator(common::variable_8x8_sprite_font);
    common::stats stats(small_text_generator);

    t2p::game game;

    while (true)
    {
        if (bn::keypad::start_pressed())
            game.next_level();

        game.update();

        info.update();
        stats.update();
        bn::core::update();
    }
}
