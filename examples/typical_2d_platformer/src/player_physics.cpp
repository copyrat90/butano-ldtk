// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player_physics.h"

#include "player.h"

#include "ldtk_gen_idents.h"
#include "ldtk_gen_project.h"

#include <bn_assert.h>
#include <bn_fixed_point.h>
#include <bn_math.h>
#include <bn_optional.h>
#include <bn_point.h>
#include <bn_top_left_fixed_rect.h>

#include <algorithm>
#include <cstdint>

namespace t2p
{

namespace
{

// Collision check is actually done for slightly smaller collision rect
// to avoid false positives for edge-edge meet
constexpr bn::fixed COLL_EPSILON = 0.01f;

constexpr bn::fixed FRICTION_X = 0.1f;

constexpr bn::fixed GRAVITY_Y = 0.15f;

constexpr bn::fixed WALL_GRABBED_TERMINAL_SPEED = 1.0f;

constexpr auto get_player_position(const bn::top_left_fixed_rect& player_collision) -> bn::fixed_point
{
    return player_collision.position() + bn::fixed_point(player_collision.width() / 2, player_collision.height());
}

} // namespace

void player_physics::update(player& player_, const ldtk::level& level)
{
    update_y_fall_speed(player_);
    update_position(player_, level);
    apply_x_friction(player_);
}

auto player_physics::grounded() const -> bool
{
    return _grounded;
}

auto player_physics::wall_grabbed_direction() const -> wall_grab_direction
{
    return _wall_grabbed_direction;
}

auto player_physics::detect_point_collision_with_level_walls(const bn::fixed_point& point, const ldtk::level& level)
    -> bool
{
    // Get the "collisions" layer definition in compile-time
    static constexpr const ldtk::layer_definition& COLLISION_LAYER_DEFINITION =
        ldtk::gen::gen_project.defs().get_layer_def(ldtk::gen::layer_ident::collisions);

    // Get the "collisions" layer instance of the current level
    const ldtk::layer& collision_layer = level.get_layer(ldtk::gen::layer_ident::collisions);

    // Get the int grid from the "collisions" layer
    //   - "collisions" layer is an IntGrid layer, so it's safe to dereference the pointer without checking
    static_assert(COLLISION_LAYER_DEFINITION.type() == ldtk::layer_type::INT_GRID);
    const ldtk::int_grid_base& int_grid = *collision_layer.int_grid();

    // Get the concrete type of the int grid to avoid run-time virtual function call overhead
    //
    // You can skip this part and just call `int_grid.cell_int()` below,
    // but that would be a run-time virtual function call, which might cause a performance issue
    static_assert(
        [] {
            // Check all "collision" IntGrid layers in all levels store int values with `std::uint8_t` (1 byte)
            //   - If all int values are [0, 256), it's stored in `std::uint8_t`.
            //   - Actually, if there's an "empty" collision layer (all int values are strictly `0`),
            //     it will be `ldtk::int_grid_empty_t` (0 byte!), and this check will fail.
            //     But we don't have an empty int grid in this project, so this works.
            for (const ldtk::level& level_inst : ldtk::gen::gen_project.levels())
            {
                const ldtk::layer& coll_layer = level_inst.get_layer(ldtk::gen::layer_ident::collisions);
                if (coll_layer.int_grid()->cell_storage_size() != sizeof(std::uint8_t))
                    return false;
            }
            return true;
        }(),
        "Size of each cell is not 1 byte as expected");
    const auto& concrete_int_grid = static_cast<const ldtk::int_grid_t<std::uint8_t>&>(int_grid);

    // Convert world coordinate `point` to grid coordinate point
    static constexpr int TILE_SIZE = COLLISION_LAYER_DEFINITION.grid_size();
    const bn::point grid_point((point.x() / TILE_SIZE).floor_integer(), (point.y() / TILE_SIZE).floor_integer());

    // If this `grid_point` goes out of the grid boundary, we will consider it as an wall.
    //   - This check is REQUIRED, since calling `cell_int()` with OOB point will error out.
    //   - `ldtk::int_grid_base::c_width()` and `c_height()` are not virtual functions,
    //     so you don't need concrete type for this.
    //     (it's a normal member function in the base class)
    if (grid_point.x() < 0 || grid_point.x() >= int_grid.c_width() || grid_point.y() < 0 ||
        grid_point.y() >= int_grid.c_height())
    {
        // Out-of-bound grid point is considered as a wall
        return true;
    }

    // Get the int grid value with the `grid_point`
    //   - Using `concrete_int_grid` here to avoid run-time virtual function call
    const int int_grid_value = concrete_int_grid.cell_int_no_virtual(grid_point.x(), grid_point.y());

    // Get the metadata of this int grid value
    const ldtk::int_grid_value_info* int_grid_value_info =
        COLLISION_LAYER_DEFINITION.get_int_grid_value_info(int_grid_value);

    // If the metadata doesn't exist, it's an empty tile, thus it's not a wall
    if (int_grid_value_info == nullptr)
        return false;

    // Check if the metadata says that the int grid value is part of "walls" group
    //   - "dirt" and "stone" are considered as walls
    //   - You could also check the int grid value directly instead:
    //       (int_grid_value == (int)ldtk::gen::layer_int_grid_value_ident::LAYER_collisions_INT_GRID_VALUE_dirt) ||
    //       (int_grid_value == (int)ldtk::gen::layer_int_grid_value_ident::LAYER_collisions_INT_GRID_VALUE_stone);
    if (int_grid_value_info->group_uid() ==
        (int)ldtk::gen::layer_int_grid_value_group_ident::LAYER_collisions_INT_GRID_VALUE_GROUP_walls)
        return true;

    return false;
}

auto player_physics::detect_upside_collision(const bn::top_left_fixed_rect& collision, const ldtk::level& level) -> bool
{
    // Check 3 points on the up side of the collision
    return detect_point_collision_with_level_walls(collision.top_left() + bn::fixed_point(COLL_EPSILON, COLL_EPSILON),
                                                   level) ||
           detect_point_collision_with_level_walls(collision.top_right() + bn::fixed_point(-COLL_EPSILON, COLL_EPSILON),
                                                   level) ||
           detect_point_collision_with_level_walls(
               bn::fixed_point(collision.center_x(), collision.top() + COLL_EPSILON), level);
}

void player_physics::update_y_fall_speed(player& player_)
{
    // Apply gravity
    player_.set_vel_y(player_.vel_y() + GRAVITY_Y);

    // Wall grabbing should limit the downward velocity
    if (wall_grabbed_direction() != wall_grab_direction::NONE)
    {
        if (player_.vel_y() > WALL_GRABBED_TERMINAL_SPEED)
            player_.set_vel_y(WALL_GRABBED_TERMINAL_SPEED);
    }
}

void player_physics::update_position(player& player_, const ldtk::level& level)
{
    move_and_collide_with_level_walls(player_, level);
}

void player_physics::move_and_collide_with_level_walls(player& player_, const ldtk::level& level)
{
    // Resolve collisions for each axis seperately

    static constexpr int TILE_SIZE =
        ldtk::gen::gen_project.defs().get_layer_def(ldtk::gen::layer_ident::collisions).grid_size();

    bn::optional<bn::top_left_fixed_rect> player_collision;

    // X axis
    if (player_.vel_x() != 0)
    {
        bool ever_collided_x = false;

        const int tile_dig_limit = (bn::abs(player_.vel_x()) / TILE_SIZE).ceil_integer();

        player_collision = player_.state_machine().collision(player_);

        // 1-1. Move player in X axis
        player_collision->set_x(player_collision->x() + player_.vel_x());

        for (int push_x_count = 0; push_x_count < tile_dig_limit; ++push_x_count)
        {
            bool collided;

            // 1-2. Detect wall collision
            if (player_.vel_x() > 0) // moving right
            {
                // Check 3 points on the right side of the player collision
                collided =
                    detect_point_collision_with_level_walls(
                        player_collision->top_right() + bn::fixed_point(-COLL_EPSILON, COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        player_collision->bottom_right() + bn::fixed_point(-COLL_EPSILON, -COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        bn::fixed_point(player_collision->right() - COLL_EPSILON, player_collision->center_y()), level);
            }
            else // moving left
            {
                // Check 3 points on the left side of the player collision
                collided =
                    detect_point_collision_with_level_walls(
                        player_collision->top_left() + bn::fixed_point(COLL_EPSILON, COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        player_collision->bottom_left() + bn::fixed_point(COLL_EPSILON, -COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        bn::fixed_point(player_collision->left() + COLL_EPSILON, player_collision->center_y()), level);
            }

            // 1-3. Resolve wall collision
            if (collided)
            {
                ever_collided_x = true;

                if (player_.vel_x() > 0) // moving right
                {
                    // Snap out to the level tile on the left
                    const int coll_snap_right = (player_collision->right() / TILE_SIZE).floor_integer() * TILE_SIZE;
                    const bn::fixed coll_snap_x = coll_snap_right - player_collision->width();
                    player_collision->set_x(coll_snap_x);
                }
                else // moving left
                {
                    // Snap out to the level tile on the right
                    const int coll_snap_x = ((player_collision->left() / TILE_SIZE).floor_integer() + 1) * TILE_SIZE;
                    player_collision->set_x(coll_snap_x);
                }
            }
            else
            {
                break;
            }
        }

        // 1-4. Update `_wall_grabbed_direction` flag
        _wall_grabbed_direction = !ever_collided_x      ? wall_grab_direction::NONE
                                  : player_.vel_x() > 0 ? wall_grab_direction::RIGHT_WALL
                                                        : wall_grab_direction::LEFT_WALL;

        // 1-5. Reset X velocity if collided
        if (ever_collided_x)
            player_.set_vel_x(0);
    }
    else
    {
        _wall_grabbed_direction = wall_grab_direction::NONE;
    }

    // Y axis
    if (player_.vel_y() != 0)
    {
        bool ever_collided_y = false;
        bool ever_grounded = false;

        const int tile_dig_limit = (bn::abs(player_.vel_y()) / TILE_SIZE).ceil_integer();

        if (!player_collision.has_value())
            player_collision = player_.state_machine().collision(player_);

        // 2-1. Move player in Y axis
        player_collision->set_y(player_collision->y() + player_.vel_y());

        for (int push_y_count = 0; push_y_count < tile_dig_limit; ++push_y_count)
        {
            bool collided;

            // 2-2. Detect wall collision
            if (player_.vel_y() > 0) // moving down
            {
                // Check 3 points on the down side of the player collision
                collided =
                    detect_point_collision_with_level_walls(
                        player_collision->bottom_left() + bn::fixed_point(COLL_EPSILON, -COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        player_collision->bottom_right() + bn::fixed_point(-COLL_EPSILON, -COLL_EPSILON), level) ||
                    detect_point_collision_with_level_walls(
                        bn::fixed_point(player_collision->center_x(), player_collision->bottom() - COLL_EPSILON),
                        level);

                if (collided)
                {
                    ever_grounded = true;
                }
            }
            else // moving up
            {
                collided = detect_upside_collision(*player_collision, level);
            }

            // 2-3. Resolve wall collision
            if (collided)
            {
                ever_collided_y = true;

                if (player_.vel_y() > 0) // moving down
                {
                    // Snap out to the level tile on the up
                    const int coll_snap_down = (player_collision->bottom() / TILE_SIZE).floor_integer() * TILE_SIZE;
                    const bn::fixed coll_snap_y = coll_snap_down - player_collision->height();
                    player_collision->set_y(coll_snap_y);
                }
                else // moving up
                {
                    // Snap out to the level tile on the down
                    const int coll_snap_y = ((player_collision->top() / TILE_SIZE).floor_integer() + 1) * TILE_SIZE;
                    player_collision->set_y(coll_snap_y);
                }
            }
            else
            {
                break;
            }
        }

        // 2-4. Update `_grounded` flag
        _grounded = ever_grounded;

        // 2-5. Reset Y velocity if collided
        if (ever_collided_y)
            player_.set_vel_y(0);
    }

    // 3. Apply player position change
    if (player_collision.has_value())
        player_.set_position(get_player_position(player_collision.value()));
}

void player_physics::apply_x_friction(player& player_)
{
    if (player_.vel_x() > 0)
        player_.set_vel_x(std::max(bn::fixed(0), player_.vel_x() - FRICTION_X));
    else if (player_.vel_x() < 0)
        player_.set_vel_x(std::min(bn::fixed(0), player_.vel_x() + FRICTION_X));
}

} // namespace t2p
