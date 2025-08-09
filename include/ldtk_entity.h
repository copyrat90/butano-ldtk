#pragma once

#include "ldtk_field.h"
#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_iid_fwd.h"
#include "ldtk_gen_tag_fwd.h"

#include <bn_fixed_point.h>
#include <bn_point.h>
#include <bn_span.h>

namespace ldtk
{

class entity
{
public:
    constexpr entity(const bn::point& grid, gen::ident identifier, const bn::fixed_point& pivot,
                     const bn::span<const gen::tag>& tags, const bn::span<const field>& field_instances, int height,
                     gen::iid iid, const bn::point& px, int width)
        : _grid(grid), _identifier(identifier), _pivot(pivot), _tags(tags), _field_instances(field_instances),
          _height(height), _iid(iid), _px(px), _width(width)
    {
    }

public:
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
    /// @brief Grid-based coordinates (`[x,y]` format)
    [[nodiscard]] constexpr auto grid() const -> const bn::point&
    {
        return _grid;
    }

    /// @brief Entity definition identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief Pivot coordinates (`[x,y]` format, values are from 0 to 1) of the Entity
    [[nodiscard]] constexpr auto pivot() const -> const bn::fixed_point&
    {
        return _pivot;
    }

    /// @brief Array of tags defined in this Entity definition
    [[nodiscard]] constexpr auto tags() const -> const bn::span<const gen::tag>&
    {
        return _tags;
    }

    /// @brief An array of all custom fields and their values.
    [[nodiscard]] constexpr auto field_instances() const -> const bn::span<const field>&
    {
        return _field_instances;
    }

    /// @brief Entity height in pixels.
    /// For non-resizable entities, it will be the same as Entity definition.
    [[nodiscard]] constexpr auto height() const -> int
    {
        return _height;
    }

    /// @brief Unique instance identifier
    [[nodiscard]] constexpr auto iid() const -> gen::iid
    {
        return _iid;
    }

    /// @brief Pixel coordinates (`[x,y]` format) in current level coordinate space.
    /// Don't forget optional layer offsets, if they exist!
    [[nodiscard]] constexpr auto px() const -> const bn::point&
    {
        return _px;
    }

    /// @brief Entity width in pixels.
    /// For non-resizable entities, it will be the same as Entity definition.
    [[nodiscard]] constexpr auto width() const -> int
    {
        return _width;
    }

private:
    bn::point _grid;
    gen::ident _identifier;
    bn::fixed_point _pivot;
    bn::span<const gen::tag> _tags;

    bn::span<const field> _field_instances;
    int _height;
    gen::iid _iid;
    bn::point _px;
    int _width;
};

} // namespace ldtk
