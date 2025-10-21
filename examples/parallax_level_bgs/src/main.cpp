// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "ldtk_core.h"
#include "ldtk_level.h"
#include "ldtk_level_bgs_ptr.h"
#include "ldtk_project.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include <bn_assert.h>
#include <bn_backdrop.h>
#include <bn_blending.h>
#include <bn_camera_ptr.h>
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
        "PAD: move camera",
        "A: toggle all layer bgs visibility",
        "B: toggle top layer bg visibility",
        "L: toggle bottom layer oob tile",
    };

    bn::sprite_text_generator big_text_generator(common::variable_8x16_sprite_font);
    common::info info("Parallax level bgs", info_text_lines, big_text_generator);

    bn::sprite_text_generator small_text_generator(common::variable_8x8_sprite_font);
    common::stats stats(small_text_generator);

    bn::camera_ptr camera = bn::camera_ptr::create();

    const ldtk::project& project = ldtk::gen::gen_project;
    const ldtk::level& level = project.get_level(ldtk::gen::level_ident::level_0);

    // Top-most layer "tiles_layer_24" has an opacity value of `0.75`
    bn::blending::set_transparency_alpha(project.opacity());
    bn::backdrop::set_color(level.bg_color());

    // Blending will be only enabled for "tiles_layer_24" layer BG
    ldtk::level_bgs_ptr level_bgs = level.create_bgs();
    level_bgs.set_camera(camera);

    bool all_bgs_visible = true;

    while (true)
    {
        if (bn::keypad::a_pressed())
        {
            all_bgs_visible = !all_bgs_visible;
            level_bgs.set_visible(all_bgs_visible);
        }

        if (bn::keypad::b_pressed())
        {
            // You MUST check if `level_bgs` has the background of the layer before calling gettes/setters;
            // If `tiles_layer_24` doesn't have a single visible tile, it doesn't generate a BG, so it will error out.
            if (level_bgs.has_background(ldtk::gen::layer_ident::tiles_layer_24))
            {
                bool top_layer_visible = level_bgs.visible(ldtk::gen::layer_ident::tiles_layer_24);

                top_layer_visible = !top_layer_visible;
                level_bgs.set_visible(top_layer_visible, ldtk::gen::layer_ident::tiles_layer_24);
            }
        }

        if (bn::keypad::l_pressed())
        {
            // You MUST check if `level_bgs` has the background of the layer before calling gettes/setters;
            // If `tiles_layer_8` doesn't have a single visible tile, it doesn't generate a BG, so it will error out.
            if (level_bgs.has_background(ldtk::gen::layer_ident::tiles_layer_8))
            {
                ldtk::tile_grid_base::tile_info oob_tile_info =
                    level_bgs.out_of_bound_tile_info(ldtk::gen::layer_ident::tiles_layer_8);

                // Tile index `0` is always a valid tile index, which denotes a fully transparent tile.
                if (oob_tile_info.index == 0)
                {
                    // Get the tile grid from the `tiles_layer_8` layer.
                    // If `tiles_layer_8` was an auto-tile layer, you need to use `auto_layer_tiles()` instead.
                    const ldtk::tile_grid_base* tile_grid =
                        level.get_layer(ldtk::gen::layer_ident::tiles_layer_8).grid_tiles();
                    BN_ASSERT(tile_grid != nullptr, "Layer is not a tile layer");

                    // Set the tile to the same one as the top-left(x=0, y=0) tile in the grid.
                    oob_tile_info = tile_grid->cell_tile_info(0, 0);
                }
                else // OOB tile was previously non-transparent
                {
                    // Set it to transparent again
                    oob_tile_info.index = 0;
                }

                // Apply the OOB tile change
                level_bgs.set_out_of_bound_tile_info(oob_tile_info, ldtk::gen::layer_ident::tiles_layer_8);
            }
        }

        static constexpr bn::fixed CAMERA_MOVE_SPEED = 4.0f;

        if (bn::keypad::left_held())
            camera.set_x(camera.x() - CAMERA_MOVE_SPEED);
        if (bn::keypad::right_held())
            camera.set_x(camera.x() + CAMERA_MOVE_SPEED);
        if (bn::keypad::up_held())
            camera.set_y(camera.y() - CAMERA_MOVE_SPEED);
        if (bn::keypad::down_held())
            camera.set_y(camera.y() + CAMERA_MOVE_SPEED);

        info.update();
        stats.update();
        bn::core::update();
    }
}
