#pragma once

#include "ldtk_gen_idents_fwd.h"

#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_size.h>

#include <cstdint>
#include <utility>

namespace bn
{

class window;

enum class green_swap_mode : std::uint8_t;

} // namespace bn

namespace ldtk
{

class level;
class level_bgs_builder;

class level_bgs_ptr
{
public:
    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] static auto create(const level& level) -> level_bgs_ptr;

    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param position Position of the level backgrounds.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] static auto create(const bn::fixed_point& position, const level& level) -> level_bgs_ptr;

    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param x Horizontal position of the level backgrounds.
    /// @param y Vertical position of the level backgrounds.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] static auto create(bn::fixed x, bn::fixed y, const level& level) -> level_bgs_ptr;

    /// @brief Creates a `level_bgs_ptr` from a `level_bgs_builder` reference.
    /// @param builder `level_bgs_builder` reference.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] static auto create(const level_bgs_builder& builder) -> level_bgs_ptr;

    /// @brief Creates a `level_bgs_ptr` from a moved `level_bgs_builder`.
    /// @param builder `level_bgs_builder` to move.
    /// @return The requested `level_bgs_ptr`.
    [[nodiscard]] static auto create(level_bgs_builder&& builder) -> level_bgs_ptr;

    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr` if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] static auto create_optional(const level& level) -> bn::optional<level_bgs_ptr>;

    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param position Position of the level backgrounds.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr` if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] static auto create_optional(const bn::fixed_point& position, const level& level)
        -> bn::optional<level_bgs_ptr>;

    /// @brief Creates a `level_bgs_ptr` from the given `level`.
    /// @param x Horizontal position of the level backgrounds.
    /// @param y Vertical position of the level backgrounds.
    /// @param level `level` containing the required information to generate the level backgrounds.
    /// @return The requested `level_bgs_ptr` if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] static auto create_optional(bn::fixed x, bn::fixed y, const level& level)
        -> bn::optional<level_bgs_ptr>;

    /// @brief Creates a `level_bgs_ptr` from a `level_bgs_builder` reference.
    /// @param builder `level_bgs_builder` reference.
    /// @return The requested `level_bgs_ptr` if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] static auto create_optional(const level_bgs_builder& builder) -> bn::optional<level_bgs_ptr>;

    /// @brief Creates a `level_bgs_ptr` from a moved `level_bgs_builder`.
    /// @param builder `level_bgs_builder` to move.
    /// @return The requested `level_bgs_ptr` if it could be allocated; `bn::nullopt` otherwise.
    [[nodiscard]] static auto create_optional(level_bgs_builder&& builder) -> bn::optional<level_bgs_ptr>;

public:
    /// @brief Copy constructor.
    /// @param other `level_bgs_ptr` to copy.
    level_bgs_ptr(const level_bgs_ptr& other);

    /// @brief Copy assignment operator.
    /// @param other `level_bgs_ptr` to copy.
    /// @return Reference to `this`.
    auto operator=(const level_bgs_ptr& other) -> level_bgs_ptr&;

    /// @brief Move constructor.
    /// @param other `level_bgs_ptr` to move.
    level_bgs_ptr(level_bgs_ptr&& other) noexcept : _handle(other._handle)
    {
        other._handle = nullptr;
    }

    /// @brief Move assignment operator.
    /// @param other `level_bgs_ptr` to move.
    /// @return Reference to `this`.
    auto operator=(level_bgs_ptr&& other) noexcept -> level_bgs_ptr&
    {
        std::swap(_handle, other._handle);
        return *this;
    }

    /// @brief Releases the referenced level backgrounds if no more `level_bgs_ptr` objects reference to them.
    ~level_bgs_ptr();

public:
    /// @brief Checks if the given layer has a background generated or not.
    /// @note Before calling gettes & setters for individual layers, you @b must check if this returns `true`.
    /// @param layer_identifier identifier of the layer to check if it has a generated background.
    [[nodiscard]] auto has_background(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Replace the level used by these level backgrounds.
    /// @param level It creates the resources to use by this level.
    void set_level(const level& level);

    /// @brief Replace the level used by these level backgrounds.
    /// @param builder It creates the resources to use by this level builder.
    void set_level(const level_bgs_builder& builder);

    /// @brief Replace the level used by these level backgrounds.
    /// @param builder It creates the resources to use by this level builder.
    void set_level(level_bgs_builder&& builder);

public:
    /// @brief Returns the size in pixels of the level backgrounds.
    [[nodiscard]] auto dimensions() const -> bn::size;

    /// @brief Returns the horizontal position of the level backgrounds (relative to its camera, if they have one).
    [[nodiscard]] auto x() const -> bn::fixed;

    /// @brief Sets the horizontal position of the level backgrounds (relative to its camera, if they have one).
    void set_x(bn::fixed x);

    /// @brief Returns the vertical position of the level backgrounds (relative to its camera, if they have one).
    [[nodiscard]] auto y() const -> bn::fixed;

    /// @brief Sets the vertical position of the level backgrounds (relative to its camera, if they have one).
    void set_y(bn::fixed y);

    /// @brief Returns the position of the level backgrounds (relative to its camera, if they have one).
    [[nodiscard]] auto position() const -> const bn::fixed_point&;

    /// @brief Sets the position of the level backgrounds (relative to its camera, if they have one).
    /// @param x Horizontal position of the level backgrounds (relative to its camera, if they have one).
    /// @param y Vertical position of the level backgrounds (relative to its camera, if they have one).
    void set_position(bn::fixed x, bn::fixed y);

    /// @brief Sets the position of the level backgrounds (relative to its camera, if they have one).
    void set_position(const bn::fixed_point& position);

    /// @brief Returns the horizontal top-left position of the level backgrounds
    /// (relative to its camera, if they have one).
    [[nodiscard]] auto top_left_x() const -> bn::fixed;

    /// @brief Sets the horizontal top-left position of the level backgrounds
    /// (relative to its camera, if they have one).
    void set_top_left_x(bn::fixed top_left_x);

    /// @brief Returns the vertical top-left position of the level backgrounds
    /// (relative to its camera, if they have one).
    [[nodiscard]] auto top_left_y() const -> bn::fixed;

    /// @brief Sets the vertical top-left position of the level backgrounds (relative to its camera, if they have one).
    void set_top_left_y(bn::fixed top_left_y);

    /// @brief Returns the top-left position of the level backgrounds (relative to its camera, if they have one).
    [[nodiscard]] auto top_left_position() const -> bn::fixed_point;

    /// @brief Sets the top-left position of the level backgrounds (relative to its camera, if they have one).
    /// @param top_left_x Horizontal top-left position of the level backgrounds
    /// (relative to its camera, if they have one).
    /// @param top_left_y Vertical top-left position of the level backgrounds
    /// (relative to its camera, if they have one).
    void set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y);

    /// @brief Sets the top-left position of the level backgrounds (relative to its camera, if they have one).
    void set_top_left_position(const bn::fixed_point& top_left_position);

    /// @brief Returns the priority of a level background of the given layer relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the background priority from.
    [[nodiscard]] auto priority(gen::layer_ident layer_identifier) const -> int;

    /// @brief Sets the priority of the level backgrounds relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    ///
    /// @param priority Priority in the range [0..3].
    void set_priority(int priority);

    /// @brief Sets the priority of a level background of the given layer relative to sprites and other backgrounds.
    ///
    /// Backgrounds with higher priority are drawn first
    /// (and therefore can be covered by later sprites and backgrounds).
    ///
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param priority Priority in the range [0..3].
    /// @param layer_identifier identifier of the layer to set the background priority to.
    void set_priority(int priority, gen::layer_ident layer_identifier);

    /// @brief Returns the priority of a level background of the given layer
    /// relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the background priority from.
    [[nodiscard]] auto z_order(gen::layer_ident layer_identifier) const -> int;

    /// @brief Sets the priority of the level backgrounds relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    ///
    /// @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
    void set_z_order(int z_order);

    /// @brief Sets the priority of a level background of the given layer
    /// relative to other backgrounds, excluding sprites.
    ///
    /// Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
    ///
    /// Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
    ///
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
    /// @param layer_identifier identifier of the layer to set the background priority to.
    void set_z_order(int z_order, gen::layer_ident layer_identifier);

    /// @brief Modify these level backgrounds to be drawn above all of the other backgrounds with the same priority.
    void put_above();

    /// @brief Modify a level background of the given layer
    /// to be drawn above all of the other backgrounds with the same priority.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to put above.
    void put_above(gen::layer_ident layer_identifier);

    /// @brief Modify these level backgrounds to be drawn below all of the other backgrounds with the same priority.
    void put_below();

    /// @brief Modify a level background of the given layer
    /// to be drawn below all of the other backgrounds with the same priority.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to put below.
    void put_below(gen::layer_ident layer_identifier);

    /// @brief Indicates if the mosaic effect must be applied to a level background with the given layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto mosaic_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if the mosaic effect must be applied to these level backgrounds or not.
    void set_mosaic_enabled(bool mosaic_enabled);

    /// @brief Sets if the mosaic effect must be applied to a level background of the given layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_mosaic_enabled(bool mosaic_enabled, gen::layer_ident layer_identifier);

    /// @brief Indicates if blending must be applied to a level background with the given layer or not.
    ///
    /// Blending is applied to these level backgrounds by making it part of the blending top layer.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_enabled(gen::layer_ident layer_identifier) const -> bool
    {
        return blending_top_enabled(layer_identifier);
    }

    /// @brief Sets if blending must be applied to these level backgrounds or not.
    ///
    /// Blending is applied to these level backgrounds by making it part of the blending top layer.
    void set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
    }

    /// @brief Sets if blending must be applied to these level backgrounds or not.
    ///
    /// Blending is applied to these level backgrounds by making it part of the blending top layer.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_blending_enabled(bool blending_enabled, gen::layer_ident layer_identifier)
    {
        set_blending_top_enabled(blending_enabled, layer_identifier);
    }

    /// @brief Indicates if a level background with the given layer is part of the blending top layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_top_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if these level backgrounds is part of the blending top layer or not.
    void set_blending_top_enabled(bool blending_top_enabled);

    /// @brief Sets if a level background of the given layer is part of the blending top layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_blending_top_enabled(bool blending_top_enabled, gen::layer_ident layer_identifier);

    /// @brief Indicates if a level background with the given layer is part of the blending bottom layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto blending_bottom_enabled(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if these level backgrounds is part of the blending bottom layer or not.
    void set_blending_bottom_enabled(bool blending_bottom_enabled);

    /// @brief Sets if a level background of the given layer is part of the blending bottom layer or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_blending_bottom_enabled(bool blending_bottom_enabled, gen::layer_ident layer_identifier);

    /// @brief Indicates how a level background with the given layer must be displayed when green swap is enabled.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the mode from.
    [[nodiscard]] auto green_swap_mode(gen::layer_ident layer_identifier) const -> bn::green_swap_mode;

    /// @brief Sets how these level backgrounds must be displayed when green swap is enabled.
    void set_green_swap_mode(bn::green_swap_mode green_swap_mode);

    /// @brief Sets how a level background of the given layer must be displayed when green swap is enabled.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the mode to.
    void set_green_swap_mode(bn::green_swap_mode green_swap_mode, gen::layer_ident layer_identifier);

    /// @brief Indicates if a level background with the given layer must be committed to the GBA or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto visible(gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if these level backgrounds must be committed to the GBA or not.
    void set_visible(bool visible);

    /// @brief Sets if a level background of the given layer must be committed to the GBA or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_visible(bool visible, gen::layer_ident layer_identifier);

    /// @brief Indicates if a level background with the given layer is visible in the given window or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to get the flag from.
    [[nodiscard]] auto visible_in_window(const bn::window& window, gen::layer_ident layer_identifier) const -> bool;

    /// @brief Sets if these level backgrounds must be visible in the given window or not.
    void set_visible_in_window(bool visible, bn::window& window);

    /// @brief Sets if a level background of the given layer must be visible in the given window or not.
    /// @note Before calling this, you @b must make sure `has_background()` returns `true`.
    /// @param layer_identifier identifier of the layer to set the flag to.
    void set_visible_in_window(bool visible, bn::window& window, gen::layer_ident layer_identifier);

    /// @brief Returns the `bn::camera_ptr` attached to these level backgrounds (if any).
    [[nodiscard]] auto camera() const -> const bn::optional<bn::camera_ptr>&;

    /// @brief Sets the `bn::camera_ptr` attached to these level backgrounds.
    /// @param camera `bn::camera_ptr` to copy to these level backgrounds.
    void set_camera(const bn::camera_ptr& camera);

    /// @brief Sets the `bn::camera_ptr` attached to these level backgrounds.
    /// @param camera `bn::camera_ptr` to move to these level backgrounds.
    void set_camera(bn::camera_ptr&& camera);

    /// @brief Sets or removes the `bn::camera_ptr` attached to these level backgrounds.
    /// @param camera Optional `bn::camera_ptr` to copy to these level backgrounds.
    void set_camera(const bn::optional<bn::camera_ptr>& camera);

    /// @brief Sets or removes the `bn::camera_ptr` attached to these level backgrounds.
    /// @param camera Optional `bn::camera_ptr` to move to these level backgrounds.
    void set_camera(bn::optional<bn::camera_ptr>&& camera);

    /// @brief Removes the `bn::camera_ptr` attached to these level backgrounds (if any).
    void remove_camera();

public:
    /// @brief Returns the internal handle.
    [[nodiscard]] const auto handle() const -> void*
    {
        return _handle;
    }

public:
    /// @brief Exchanges the contents of this `level_bgs_ptr` with those of the other one.
    /// @param other `level_bgs_ptr` to exchange the contents with.
    void swap(level_bgs_ptr& other) noexcept
    {
        std::swap(_handle, other._handle);
    }

    /// @brief Exchanges the contents of a `level_bg_ptr` with those of another one.
    /// @param a First `level_bg_ptr` to exchange the contents with.
    /// @param b Second `level_bg_ptr` to exchange the contents with.
    friend void swap(level_bgs_ptr& a, level_bgs_ptr& b) noexcept
    {
        std::swap(a._handle, b._handle);
    }

    /// @brief Default equal operator.
    [[nodiscard]] friend auto operator==(const level_bgs_ptr& a, const level_bgs_ptr& b) -> bool = default;

private:
    using handle_t = void*;

    handle_t _handle;

    explicit level_bgs_ptr(handle_t handle) : _handle(handle)
    {
    }
};

} // namespace ldtk
