#pragma once

#include "ldtk_gen_idents_fwd.h"
#include "ldtk_level_bgs_ptr.h"

#include <bn_camera_ptr.h>
#include <bn_config_bgs.h>
#include <bn_fixed_point.h>
#include <bn_green_swap_mode.h>
#include <bn_optional.h>
#include <bn_size.h>
#include <bn_vector.h>

#include <cstdint>
#include <utility>

namespace ldtk
{

class level;
class layer;

class level_bgs_builder
{
public:
    /// @brief Constructor.
    /// @param level `level` containing the required information to generate the level backgrounds.
    explicit level_bgs_builder(const level& level);

public:
    /// @brief Returns the `level` containing the required information to generate the
    /// level backgrounds.
    [[nodiscard]] auto level() const -> const level&
    {
        return _level;
    }

    /// @brief Checks if the given layer will generate a background or not.
    /// @note Before calling gettes & setters for individual layers, you @b must check if this returns `true`.
    /// @param layer_identifier identifier of the layer to check if it will generate a background.
    [[nodiscard]] auto has_background(gen::layer_ident layer_identifier) const -> bool;

public:
    /// @brief Returns the horizontal position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto x() const -> bn::fixed
    {
        return _position.x();
    }

    /// @brief Sets the horizontal position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param x Horizontal position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_x(bn::fixed x) -> level_bgs_builder&
    {
        _position.set_x(x);
        return *this;
    }

    /// @brief Returns the vertical position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto y() const -> bn::fixed
    {
        return _position.y();
    }

    /// @brief Sets the vertical position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param y vertical position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_y(bn::fixed y) -> level_bgs_builder&
    {
        _position.set_y(y);
        return *this;
    }

    /// @brief Returns the position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto position() const -> const bn::fixed_point&
    {
        return _position;
    }

    /// @brief Sets the position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param x Horizontal position of the level backgrounds to generate.
    /// @param y Vertical position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_position(bn::fixed x, bn::fixed y) -> level_bgs_builder&
    {
        _position = bn::fixed_point(x, y);
        return *this;
    }

    /// @brief Sets the position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param position Position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_position(const bn::fixed_point& position) -> level_bgs_builder&
    {
        _position = position;
        return *this;
    }

    /// @brief Returns the horizontal top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto top_left_x() const -> bn::fixed;

    /// @brief Sets the horizontal top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param top_left_x Horizontal top-left position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_top_left_x(bn::fixed top_left_x) -> level_bgs_builder&;

    /// @brief Returns the vertical top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto top_left_y() const -> bn::fixed;

    /// @brief Sets the vertical top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param top_left_y Vertical top-left position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_top_left_y(bn::fixed top_left_y) -> level_bgs_builder&;

    /// @brief Returns the top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    [[nodiscard]] auto top_left_position() const -> bn::fixed_point;

    /// @brief Sets the top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param top_left_x Horizontal top-left position of the level backgrounds to generate.
    /// @param top_left_y Vertical top-left position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y) -> level_bgs_builder&;

    /// @brief Sets the top-left position of the level backgrounds to generate
    /// (relative to their camera, if they are going to have one).
    /// @param top_left_position Top-left position of the level backgrounds to generate.
    /// @return Reference to `this`.
    auto set_top_left_position(const bn::fixed_point& top_left_position) -> level_bgs_builder&;

    /// @brief Returns the priority of a background of the given layer
    /// to generate relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the background priority from.
    [[nodiscard]] auto priority(gen::layer_ident layer_identifier) const -> int;

    /// @brief Sets the priority of the level backgrounds to generate relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    ///
    /// @param priority Priority in the range [0..3].
    /// @return Reference to `this`.
    auto set_priority(int priority) -> level_bgs_builder&;

    /// @brief Sets the priority of a level background of the given layer
    /// to generate relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    ///
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param priority Priority in the range [0..3].
    /// @param layer_identifier identifier of the layer to set the background priority to.
    /// @return Reference to `this`.
    auto set_priority(int priority, gen::layer_ident layer_identifier) -> level_bgs_builder&;

    /// @brief Returns the priority of a background of the given layer
    /// to generate relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the background priority from.
    [[nodiscard]] auto z_order(gen::layer_ident layer_identifier) const -> int;

    /// @brief Sets the priority of the level backgrounds
    /// to generate relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    ///
    /// @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
    /// @return Reference to `this`.
    auto set_z_order(int z_order) -> level_bgs_builder&;

    /// @brief Sets the priority of a level background of the given layer
    /// to generate relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    ///
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
    /// @param layer_identifier identifier of the layer to set the background priority to.
    /// @return Reference to `this`.
    auto set_z_order(int z_order, gen::layer_ident layer_identifier) -> level_bgs_builder&;

    /// @brief Indicates if the mosaic effect must be applied to a level background with the given layer
    /// to generate or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto mosaic_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if the mosaic effect must be applied to the level backgrounds to generate or not.
    /// @param mosaic_enabled `true` if the mosaic effect must be applied; `false` otherwise.
    /// @return Reference to `this`.
    auto set_mosaic_enabled(bool mosaic_enabled) -> level_bgs_builder&;

    /// @brief Sets if the mosaic effect must be applied to a level background of the given layer
    /// to generate or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param mosaic_enabled `true` if the mosaic effect must be applied; `false` otherwise.
    /// @param layer_identifier identifier of the layer to set the flag to.
    /// @return Reference to `this`.
    auto set_mosaic_enabled(bool mosaic_enabled, gen::layer_ident layer_identifier) -> level_bgs_builder&;

    /// @brief Indicates if blending must be applied to a level background with the given layer
    /// to generate or not.
    ///
    /// Blending is applied to level backgrounds by making them part of the blending top layer.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_enabled(gen::layer_ident layer_identifier) const -> bool
    {
        return blending_top_enabled(layer_identifier);
    }

    /// @brief Sets if blending must be applied to the level backgrounds to generate or not.
    ///
    /// Blending is applied to level backgrounds by making them part of the blending top layer.
    /// @param blending_enabled `true` if blending must be applied; `false` otherwise.
    /// @return Reference to `this`.
    auto set_blending_enabled(bool blending_enabled) -> level_bgs_builder&
    {
        return set_blending_top_enabled(blending_enabled);
    }

    /// @brief Sets if blending must be applied to a level background of the given layer
    /// to generate or not.
    ///
    /// Blending is applied to level backgrounds by making them part of the blending top layer.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param blending_enabled `true` if blending must be applied; `false` otherwise.
    /// @param layer_identifier identifier of the layer to set the flag to.
    /// @return Reference to `this`.
    auto set_blending_enabled(bool blending_enabled, gen::layer_ident layer_identifier) -> level_bgs_builder&
    {
        return set_blending_top_enabled(blending_enabled, layer_identifier);
    }

    /// @brief Indicates if a level background with the given layer
    /// to generate must be part of the blending top layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_top_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if the level backgrounds to generate must be part of the blending top layer or not.
    /// @param blending_top_enabled `true` if generated backgrounds must be part of the blending top layer;
    /// `false` otherwise.
    /// @return Reference to `this`.
    auto set_blending_top_enabled(bool blending_top_enabled) -> level_bgs_builder&;

    /// @brief Sets if a level background of the given layer
    /// to generate must be part of the blending top layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param blending_top_enabled `true` if generated background must be part of the blending top layer;
    /// `false` otherwise.
    /// @param layer_identifier identifier of the layer to set the flag to.
    /// @return Reference to `this`.
    auto set_blending_top_enabled(bool blending_top_enabled, gen::layer_ident layer_identifier) -> level_bgs_builder&;

    /// @brief Indicates if a level background with the given layer
    /// to generate must be part of the blending bottom layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_bottom_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if the level backgrounds to generate must be part of the blending bottom layer or not.
    /// @param blending_bottom_enabled `true` if generated backgrounds must be part of the blending bottom layer;
    /// `false` otherwise.
    /// @return Reference to `this`.
    auto set_blending_bottom_enabled(bool blending_bottom_enabled) -> level_bgs_builder&;

    /// @brief Sets if a level background of the given layer
    /// to generate must be part of the blending bottom layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param blending_bottom_enabled `true` if generated background must be part of the blending bottom layer;
    /// `false` otherwise.
    /// @param layer_identifier identifier of the layer to set the flag to.
    /// @return Reference to `this`.
    auto set_blending_bottom_enabled(bool blending_bottom_enabled, gen::layer_ident layer_identifier)
        -> level_bgs_builder&;

    /// @brief Indicates how a level background with the given layer
    /// to generate must be displayed when green swap is enabled.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the mode from.
    [[nodiscard]] auto green_swap_mode(gen::layer_ident layer_identifier) const -> bn::green_swap_mode;

    /// @brief Sets how the level backgrounds to generate must be displayed when green swap is enabled.
    /// @param green_swap_mode Green swap mode.
    /// @return Reference to `this`.
    auto set_green_swap_mode(bn::green_swap_mode green_swap_mode) -> level_bgs_builder&;

    /// @brief Sets how a level background of the given layer to generate must be displayed when green swap is enabled.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param green_swap_mode Green swap mode.
    /// @param layer_identifier identifier of the layer to set the mode to.
    /// @return Reference to `this`.
    auto set_green_swap_mode(bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier)
        -> level_bgs_builder&;

    /// @brief Indicates if a level background with the given layer
    /// to generate must be committed to the GBA or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto visible(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if the level backgrounds to generate must be committed to the GBA or not.
    /// @param visible `true` if the level backgrounds must be committed to the GBA; `false` otherwise.
    /// @return Reference to `this`.
    auto set_visible(bool visible) -> level_bgs_builder&;

    /// @brief Sets if a level background of the given layer
    /// to generate must be committed to the GBA or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param visible `true` if the level backgrounds must be committed to the GBA; `false` otherwise.
    /// @param layer_identifier identifier of the layer to set the flag to.
    /// @return Reference to `this`.
    auto set_visible(bool visible, gen::layer_ident layer_identifier) -> level_bgs_builder&;

    /// @brief Returns the camera_ptr to attach to the level backgrounds to generate (if any).
    [[nodiscard]] auto camera() const -> const bn::optional<bn::camera_ptr>&
    {
        return _camera;
    }

    /// @brief Sets the camera_ptr to attach to the level backgrounds to generate.
    /// @param camera camera_ptr to copy to the builder.
    /// @return Reference to `this`.
    auto set_camera(const bn::camera_ptr& camera) -> level_bgs_builder&
    {
        _camera = camera;
        return *this;
    }

    /// @brief Sets the camera_ptr to attach to the level backgrounds to generate.
    /// @param camera camera_ptr to move to the builder.
    /// @return Reference to `this`.
    auto set_camera(bn::camera_ptr&& camera) -> level_bgs_builder&
    {
        _camera = std::move(camera);
        return *this;
    }

    /// @brief Sets or removes the camera_ptr to attach to the level backgrounds to generate.
    /// @param camera Optional camera_ptr to copy to the builder.
    /// @return Reference to `this`.
    auto set_camera(const bn::optional<bn::camera_ptr>& camera) -> level_bgs_builder&
    {
        _camera = camera;
        return *this;
    }

    /// @brief Sets or removes the camera_ptr to attach to the level backgrounds to generate.
    /// @param camera Optional camera_ptr to move to the builder.
    /// @return Reference to `this`.
    auto set_camera(bn::optional<bn::camera_ptr>&& camera) -> level_bgs_builder&
    {
        _camera = std::move(camera);
        return *this;
    }

    /// @brief Removes the camera_ptr to attach to the level backgrounds to generate.
    /// @return Reference to `this`.
    auto remove_camera() -> level_bgs_builder&
    {
        _camera.reset();
        return *this;
    }

    /// @brief Releases and returns the camera_ptr to attach to the level backgrounds to generate (if any).
    [[nodiscard]] auto release_camera() -> bn::optional<bn::camera_ptr>;

    /// @brief Generates and returns a `level_bgs_ptr` without releasing the acquired resources.
    [[nodiscard]] auto build() const -> level_bgs_ptr;

    /// @brief Generates and returns a `level_bgs_ptr` releasing the acquired resources.
    ///
    /// `level_bgs_ptr` generation after calling this method may stop working.
    [[nodiscard]] auto release_build() -> level_bgs_ptr;

    /// @brief Generates and returns a `level_bgs_ptr`
    /// without releasing the acquired resources if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] auto build_optional() const -> bn::optional<level_bgs_ptr>;

    /// @brief Generates and returns a `level_bgs_ptr` releasing the acquired resources if it could be allocated;
    /// `bn::nullopt` otherwise.
    ///
    /// `level_bgs_ptr` generation after calling this method may stop working.
    [[nodiscard]] auto release_build_optional() -> bn::optional<level_bgs_ptr>;

private:
    /// @cond DO_NOT_DOCUMENT
    struct bg_unique_attributes
    {
        const layer& layer_instance;
        bool visible = true;
        bool blending_top_enabled = false;

        std::int16_t z_order = 0;
        std::uint8_t priority = 3;
        bn::green_swap_mode green_swap_mode = bn::green_swap_mode::DEFAULT;
        bool mosaic_enabled = false;
        bool blending_bottom_enabled = true;
    };
    /// @endcond

private:
    [[nodiscard]] auto dimensions() const -> const bn::size&;

    [[nodiscard]] auto bg_attr(gen::layer_ident layer_identifier) -> bg_unique_attributes&;

    [[nodiscard]] auto bg_attr(gen::layer_ident layer_identifier) const -> const bg_unique_attributes&;

    [[nodiscard]] auto bg_attr_nullable(gen::layer_ident layer_identifier) -> bg_unique_attributes*;

    [[nodiscard]] auto bg_attr_nullable(gen::layer_ident layer_identifier) const -> const bg_unique_attributes*;

private:
    const ldtk::level& _level;
    bn::fixed_point _position;
    bn::optional<bn::camera_ptr> _camera;
    bn::vector<bg_unique_attributes, BN_CFG_BGS_MAX_ITEMS> _bgs_attrs;
};

} // namespace ldtk
