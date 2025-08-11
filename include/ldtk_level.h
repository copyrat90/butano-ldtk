#pragma once

#include "ldtk_field.h"
#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_iid_fwd.h"
#include "ldtk_layer.h"

#include <bn_color.h>
#include <bn_span.h>

#include <algorithm>

namespace ldtk
{

class level
{
public:
    constexpr level(bn::color bg_color, const bn::span<const field>& field_instances, gen::ident identifier,
                    gen::iid iid, const bn::span<const layer>& layer_instances, int px_height, int px_width, int uid,
                    int world_depth, int world_x, int world_y)
        : _bg_color(bg_color), _field_instances(field_instances), _identifier(identifier), _iid(iid),
          _layer_instances(layer_instances), _px_height(px_height), _px_width(px_width), _uid(uid),
          _world_depth(world_depth), _world_x(world_x), _world_y(world_y)
    {
    }

public:
    /// @brief Linear searches a layer.
    /// @param iid Instance id of the layer to search for.
    /// @return Pointer to the found layer, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto find_layer(gen::iid iid) const -> const layer*
    {
        auto iter = std::ranges::find_if(_layer_instances, [iid](const layer& ly) { return ly.iid() == iid; });

        if (iter == _layer_instances.end())
            return nullptr;

        return &*iter;
    }

    /// @brief Linear searches a layer.
    /// @param identifier Unique identifier of the layer to search for.
    /// @return Pointer to the found layer, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto find_layer(gen::ident identifier) const -> const layer*
    {
        auto iter = std::ranges::find_if(_layer_instances,
                                         [identifier](const layer& ly) { return ly.identifier() == identifier; });

        if (iter == _layer_instances.end())
            return nullptr;

        return &*iter;
    }

    /// @brief Linear searches a field.
    /// @param identifier Unique identifier of the field to search for.
    /// @return Pointer to the found field, or `nullptr` if it doesn't exist.
    [[nodiscard]] constexpr auto find_field(gen::ident identifier) const -> const field*
    {
        auto iter = std::ranges::find_if(_field_instances,
                                         [identifier](const field& fd) { return fd.identifier() == identifier; });

        if (iter == _field_instances.end())
            return nullptr;

        return &*iter;
    }

public:
    /// @brief Background color of the level
    [[nodiscard]] constexpr auto bg_color() const -> bn::color
    {
        return _bg_color;
    }

    /// @brief An array containing this level custom field values.
    [[nodiscard]] constexpr auto field_instances() const -> const bn::span<const field>&
    {
        return _field_instances;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief Unique project identifier
    [[nodiscard]] constexpr auto iid() const -> gen::iid
    {
        return _iid;
    }

    /// @brief An array containing all Layer instances.
    /// @note This array is **sorted in display order**: the 1st layer is the top-most and the last is behind.
    [[nodiscard]] constexpr auto layer_instances() const -> const bn::span<const layer>&
    {
        return _layer_instances;
    }

    /// @brief Height of the level in pixels
    [[nodiscard]] constexpr auto px_height() const -> int
    {
        return _px_height;
    }

    /// @brief Width of the level in pixels
    [[nodiscard]] constexpr auto px_width() const -> int
    {
        return _px_width;
    }

    /// @brief Unique Int identifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

    /// @brief Index that represents the "depth" of the level in the world.
    /// Default is 0, greater means "above", lower means "below". \n
    /// This value is mostly used for display only and is intended to make stacking of levels easier to manage.
    [[nodiscard]] constexpr auto world_depth() const -> int
    {
        return _world_depth;
    }

    /// @brief World X coordinate in pixels. \n
    /// Only relevant for world layouts where level spatial positioning is manual (ie. GridVania, Free).
    /// For Horizontal and Vertical layouts, the value is always -1 here.
    [[nodiscard]] constexpr auto world_x() const -> int
    {
        return _world_x;
    }

    /// @brief World Y coordinate in pixels. \n
    /// Only relevant for world layouts where level spatial positioning is manual (ie. GridVania, Free).
    /// For Horizontal and Vertical layouts, the value is always -1 here.
    [[nodiscard]] constexpr auto world_y() const -> int
    {
        return _world_y;
    }

private:
    bn::color _bg_color;
    bn::span<const field> _field_instances;
    gen::ident _identifier;
    gen::iid _iid;
    bn::span<const layer> _layer_instances;
    int _px_height;
    int _px_width;
    int _uid;
    int _world_depth;
    int _world_x;
    int _world_y;
};

} // namespace ldtk
