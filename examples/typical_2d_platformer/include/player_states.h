// SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: Zlib

#pragma once

#include "player_state_ptr_fwd.h"

#include "wall_grab_direction.h"

#include <bn_top_left_fixed_rect.h>

#include <algorithm>
#include <cstdint>

namespace ldtk
{
class level;
}

namespace t2p
{

class player;

class player_state_base
{
public:
    virtual ~player_state_base() = 0;

    virtual void enter(player&);
    virtual void exit(player&);

    virtual void handle_input(player&);
    [[nodiscard]] virtual auto update(player&, const ldtk::level&) -> player_state_ptr;

    [[nodiscard]] virtual auto collision(const player&) const -> bn::top_left_fixed_rect;

protected:
    static void handle_left_right_input(player&, bn::fixed speed);
    static auto handle_press_a_jump(player&, bn::fixed jump_speed) -> bool;

    [[nodiscard]] static auto hold_down_crouch_transition(player&) -> player_state_ptr;
    [[nodiscard]] static auto hold_up_look_up_transition(player&) -> player_state_ptr;

    [[nodiscard]] static auto run_or_idle_transition(player&) -> player_state_ptr;
};

inline player_state_base::~player_state_base() = default;

class player_ground_state : public player_state_base
{
public:
    ~player_ground_state() = 0;

    void handle_input(player&) override;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override;
};

inline player_ground_state::~player_ground_state() = default;

class player_air_state : public player_state_base
{
public:
    ~player_air_state() = 0;

    void handle_input(player&) override;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override;
};

inline player_air_state::~player_air_state() = default;

class player_idle_state final : public player_ground_state
{
public:
    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;
};

class player_run_state final : public player_ground_state
{
public:
    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;
};

class player_jump_state final : public player_air_state
{
public:
    player_jump_state() = default;
    player_jump_state(wall_grab_direction wall_grabbed_jump);

    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;

private:
    int _wall_jump_force_bounce_timer = 0;
    wall_grab_direction _wall_grabbed_direction = wall_grab_direction::NONE;
};

class player_fall_state final : public player_air_state
{
public:
    player_fall_state() = default;
    player_fall_state(int coyote_time);

    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;

private:
    int _coyote_timer = 0;
    bool _jumped = false;
};

class player_look_up_state final : public player_ground_state
{
public:
    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;
};

class player_crouch_state final : public player_ground_state
{
public:
    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;

    [[nodiscard]] auto collision(const player&) const -> bn::top_left_fixed_rect override final;
};

class player_wall_grab_state final : public player_state_base
{
public:
    void enter(player&) override final;
    void exit(player&) override final;

    void handle_input(player&) override final;
    [[nodiscard]] auto update(player&, const ldtk::level&) -> player_state_ptr override final;

private:
    wall_grab_direction _wall_jump = wall_grab_direction::NONE;
};

inline constexpr int MAX_PLAYER_STATE_ALIGN = std::max({
    alignof(player_idle_state),
    alignof(player_run_state),
    alignof(player_jump_state),
    alignof(player_fall_state),
    alignof(player_look_up_state),
    alignof(player_crouch_state),
    alignof(player_wall_grab_state),
});

inline constexpr int MAX_PLAYER_STATE_SIZE = std::max({
    sizeof(player_idle_state),
    sizeof(player_run_state),
    sizeof(player_jump_state),
    sizeof(player_fall_state),
    sizeof(player_look_up_state),
    sizeof(player_crouch_state),
    sizeof(player_wall_grab_state),
});

} // namespace t2p
