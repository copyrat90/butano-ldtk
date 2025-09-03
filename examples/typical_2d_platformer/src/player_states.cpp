// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#include "player_states.h"

#include "player.h"
#include "player_state_ptr.h"

#include "wall_grab_direction.h"

#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_math.h>

namespace t2p
{

namespace
{

constexpr bn::fixed_size STAND_UP_COLLISION_SIZE(14, 20);
constexpr bn::fixed_size CROUCH_COLLISION_SIZE(14, 14);

constexpr bn::top_left_fixed_rect RELATIVE_STAND_UP_COLLISION{
    bn::fixed_point(-STAND_UP_COLLISION_SIZE.width() / 2, -STAND_UP_COLLISION_SIZE.height()),
    STAND_UP_COLLISION_SIZE,
};
constexpr bn::top_left_fixed_rect RELATIVE_CROUCH_COLLISION{
    bn::fixed_point(-CROUCH_COLLISION_SIZE.width() / 2, -CROUCH_COLLISION_SIZE.height()),
    CROUCH_COLLISION_SIZE,
};

constexpr auto get_player_collision(const player& player_, const bn::top_left_fixed_rect& relative_collision)
    -> bn::top_left_fixed_rect
{
    return bn::top_left_fixed_rect(player_.position() + relative_collision.position(), relative_collision.dimensions());
}

constexpr bn::fixed FAST_MOVE_SPEED_X = 2.0f;
constexpr bn::fixed SLOW_MOVE_SPEED_X = 0.4f;

constexpr bn::fixed WALL_BOUNCE_SPEED_X = FAST_MOVE_SPEED_X;
constexpr int WALL_BOUNCE_TIME = 8;

constexpr int COYOTE_TIME = 14;

// If slower than this, go to `IDLE` animation instead of `RUN`
constexpr bn::fixed RUN_ANIM_SPEED_X = 0.5f;

static_assert(FAST_MOVE_SPEED_X >= RUN_ANIM_SPEED_X);
static_assert(RUN_ANIM_SPEED_X > SLOW_MOVE_SPEED_X);

constexpr bn::fixed NORMAL_JUMP_SPEED_Y = 4.0f;
constexpr bn::fixed WALL_JUMP_SPEED_Y = 3.5f;

} // namespace

void player_state_base::enter(player&)
{
}

void player_state_base::exit(player&)
{
}

void player_state_base::handle_input(player&)
{
}

auto player_state_base::update(player&, const ldtk::level&) -> player_state_ptr
{
    return nullptr;
}

auto player_state_base::collision(const player& player_) const -> bn::top_left_fixed_rect
{
    return get_player_collision(player_, RELATIVE_STAND_UP_COLLISION);
}

void player_state_base::handle_left_right_input(player& player_, bn::fixed speed)
{
    if (bn::keypad::left_held())
        player_.set_vel_x(-speed);
    else if (bn::keypad::right_held())
        player_.set_vel_x(speed);
}

auto player_state_base::handle_press_a_jump(player& player_, bn::fixed jump_speed) -> bool
{
    if (bn::keypad::a_pressed())
    {
        player_.set_vel_y(-jump_speed);
        return true;
    }

    return false;
}

auto player_state_base::hold_down_crouch_transition(player& player_) -> player_state_ptr
{
    return bn::keypad::down_held() ? player_.state_machine().create_state<player_crouch_state>() : nullptr;
}

auto player_state_base::hold_up_look_up_transition(player& player_) -> player_state_ptr
{
    return bn::keypad::up_held() ? player_.state_machine().create_state<player_look_up_state>() : nullptr;
}

auto player_state_base::run_or_idle_transition(player& player_) -> player_state_ptr
{
    return bn::abs(player_.vel_x()) < RUN_ANIM_SPEED_X ? player_.state_machine().create_state<player_idle_state>()
                                                       : player_.state_machine().create_state<player_run_state>();
}

void player_ground_state::handle_input(player& player_)
{
    handle_press_a_jump(player_, NORMAL_JUMP_SPEED_Y);
}

auto player_ground_state::update(player& player_, const ldtk::level&) -> player_state_ptr
{
    if (!player_.physics().grounded())
    {
        if (player_.vel_y() < 0)
            return player_.state_machine().create_state<player_jump_state>();
        else
            return player_.state_machine().create_state<player_fall_state>(COYOTE_TIME);
    }

    return nullptr;
}

void player_air_state::handle_input(player& player_)
{
    handle_left_right_input(player_, FAST_MOVE_SPEED_X);
}

auto player_air_state::update(player& player_, const ldtk::level&) -> player_state_ptr
{
    player_state_ptr next_state = nullptr;

    // If landed on the ground,
    if (player_.physics().grounded())
    {
        // Crouch?
        if ((next_state = hold_down_crouch_transition(player_)))
            return next_state;
        // Look up?
        else if ((next_state = hold_up_look_up_transition(player_)))
            return next_state;
        // Run or idle?
        return run_or_idle_transition(player_);
    }
    // If grabbed a wall,
    else if (player_.physics().wall_grabbed_direction() != wall_grab_direction::NONE)
    {
        return player_.state_machine().create_state<player_wall_grab_state>();
    }

    return next_state;
}

void player_idle_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::IDLE);
}

void player_idle_state::exit(player&)
{
}

void player_idle_state::handle_input(player& player_)
{
    handle_left_right_input(player_, FAST_MOVE_SPEED_X);

    // Jump pressed?
    player_ground_state::handle_input(player_);
}

auto player_idle_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    // Jump or fall?
    player_state_ptr next_state = player_ground_state::update(player_, level);
    if (next_state)
        return next_state;

    // Nah, we're on the ground.

    // Crouch?
    if ((next_state = hold_down_crouch_transition(player_)))
        return next_state;

    // Look up?
    if ((next_state = hold_up_look_up_transition(player_)))
        return next_state;

    // Run?
    if (bn::abs(player_.vel_x()) >= RUN_ANIM_SPEED_X)
        next_state = player_.state_machine().create_state<player_run_state>();

    return next_state;
}

void player_run_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::RUN);
}

void player_run_state::exit(player&)
{
}

void player_run_state::handle_input(player& player_)
{
    handle_left_right_input(player_, FAST_MOVE_SPEED_X);

    // Jump pressed?
    player_ground_state::handle_input(player_);
}

auto player_run_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    // Jump or fall?
    player_state_ptr next_state = player_ground_state::update(player_, level);
    if (next_state)
        return next_state;

    // Nah, we're on the ground.

    // Crouch?
    if ((next_state = hold_down_crouch_transition(player_)))
        return next_state;

    // Look up?
    if ((next_state = hold_up_look_up_transition(player_)))
        return next_state;

    // Stop running?
    if (bn::abs(player_.vel_x()) < RUN_ANIM_SPEED_X)
        next_state = player_.state_machine().create_state<player_idle_state>();

    return next_state;
}

player_jump_state::player_jump_state(wall_grab_direction wall_grabbed_jump) : _wall_grabbed_direction(wall_grabbed_jump)
{
    if (wall_grabbed_jump != wall_grab_direction::NONE)
        _wall_jump_force_bounce_timer = WALL_BOUNCE_TIME;
}

void player_jump_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::JUMP);
}

void player_jump_state::exit(player&)
{
}

void player_jump_state::handle_input(player& player_)
{
    // Forced bounce
    if (_wall_jump_force_bounce_timer > 0)
    {
        BN_ASSERT(_wall_grabbed_direction != wall_grab_direction::NONE, "Wall jump direction does not exist");

        if (_wall_grabbed_direction == wall_grab_direction::LEFT_WALL)
            player_.set_vel_x(WALL_BOUNCE_SPEED_X);
        else // right wall
            player_.set_vel_x(-WALL_BOUNCE_SPEED_X);

        --_wall_jump_force_bounce_timer;
    }
    // Normal user control
    else
    {
        player_air_state::handle_input(player_);
    }
}

auto player_jump_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    player_state_ptr next_state = player_air_state::update(player_, level);
    if (next_state)
        return next_state;

    if (player_.vel_y() > 0)
        next_state = player_.state_machine().create_state<player_fall_state>();

    return next_state;
}

player_fall_state::player_fall_state(int coyote_time) : _coyote_timer(coyote_time)
{
}

void player_fall_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::FALL);
}

void player_fall_state::exit(player&)
{
}

void player_fall_state::handle_input(player& player_)
{
    player_air_state::handle_input(player_);

    if (_coyote_timer > 0)
    {
        if (handle_press_a_jump(player_, NORMAL_JUMP_SPEED_Y))
            _jumped = true;

        --_coyote_timer;
    }
}

auto player_fall_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    if (_jumped)
        return player_.state_machine().create_state<player_jump_state>();

    return player_air_state::update(player_, level);
}

void player_look_up_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::LOOK_UP);
}

void player_look_up_state::exit(player&)
{
}

void player_look_up_state::handle_input(player& player_)
{
    // Jump pressed?
    player_ground_state::handle_input(player_);
}

auto player_look_up_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    // Jump or fall?
    player_state_ptr next_state = player_ground_state::update(player_, level);
    if (next_state)
        return next_state;

    // Nah, we're on the ground.

    // Crouch?
    if ((next_state = hold_down_crouch_transition(player_)))
        return next_state;

    // Stop looking up?
    if (!bn::keypad::up_held())
        next_state = run_or_idle_transition(player_);

    return next_state;
}

void player_crouch_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::CROUCH);
}

void player_crouch_state::exit(player&)
{
}

void player_crouch_state::handle_input(player& player_)
{
    // Respect the inertia
    if (bn::abs(player_.vel_x()) <= SLOW_MOVE_SPEED_X)
    {
        // Crouch movement speed is slower
        handle_left_right_input(player_, SLOW_MOVE_SPEED_X);
    }

    // Jump pressed?
    player_ground_state::handle_input(player_);
}

auto player_crouch_state::update(player& player_, const ldtk::level& level) -> player_state_ptr
{
    player_state_ptr next_state = nullptr;

    // You can't stop crouching when standing up will collide your head with the wall
    if (!player_.physics().detect_upside_collision(get_player_collision(player_, RELATIVE_STAND_UP_COLLISION), level))
    {
        // Jump or fall?
        next_state = player_ground_state::update(player_, level);
        if (next_state)
            return next_state;

        // Nah, we're on the ground.

        // Look up?
        if ((next_state = hold_up_look_up_transition(player_)))
            return next_state;

        // Stop the crouch?
        if (!bn::keypad::down_held())
            next_state = run_or_idle_transition(player_);
    }

    return next_state;
}

auto player_crouch_state::collision(const player& player_) const -> bn::top_left_fixed_rect
{
    return get_player_collision(player_, RELATIVE_CROUCH_COLLISION);
}

void player_wall_grab_state::enter(player& player_)
{
    player_.animation().start(player_animation::kind::WALL_GRAB);
}

void player_wall_grab_state::exit(player&)
{
}

void player_wall_grab_state::handle_input(player& player_)
{
    handle_left_right_input(player_, FAST_MOVE_SPEED_X);

    if (handle_press_a_jump(player_, WALL_JUMP_SPEED_Y))
    {
        const auto grabbed_direction = player_.physics().wall_grabbed_direction();
        BN_ASSERT(grabbed_direction != wall_grab_direction::NONE, "Wall grab direction doesn't exist");

        if (grabbed_direction == wall_grab_direction::LEFT_WALL)
            player_.set_vel_x(WALL_BOUNCE_SPEED_X);
        else // right wall
            player_.set_vel_x(-WALL_BOUNCE_SPEED_X);

        _wall_jump = grabbed_direction;
    }
}

auto player_wall_grab_state::update(player& player_, const ldtk::level&) -> player_state_ptr
{
    player_state_ptr next_state = nullptr;

    if (_wall_jump != wall_grab_direction::NONE)
    {
        next_state = player_.state_machine().create_state<player_jump_state>(_wall_jump);
    }
    // If landed on the ground,
    else if (player_.physics().grounded())
    {
        // Crouch?
        if ((next_state = hold_down_crouch_transition(player_)))
            return next_state;
        // Look up?
        else if ((next_state = hold_up_look_up_transition(player_)))
            return next_state;
        // Run or idle?
        return run_or_idle_transition(player_);
    }
    // If stop grabbing the wall,
    else if (player_.physics().wall_grabbed_direction() == wall_grab_direction::NONE)
    {
        if (player_.vel_y() < 0)
            next_state = player_.state_machine().create_state<player_jump_state>();
        else
            next_state = player_.state_machine().create_state<player_fall_state>();
    }

    return next_state;
}

} // namespace t2p
