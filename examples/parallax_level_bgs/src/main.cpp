#include "ldtk_core.h"
#include "ldtk_level.h"
#include "ldtk_level_bgs_ptr.h"
#include "ldtk_project.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

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
            bool top_layer_visible = level_bgs.visible(ldtk::gen::layer_ident::tiles_layer_24);

            top_layer_visible = !top_layer_visible;
            level_bgs.set_visible(top_layer_visible, ldtk::gen::layer_ident::tiles_layer_24);
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
