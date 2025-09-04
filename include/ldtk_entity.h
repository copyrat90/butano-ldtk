#pragma once

#include "ldtk_entity_definition.h"

#include "ldtk_field.h"
#include "ldtk_gen_idents_fwd.h"
#include "ldtk_gen_iids_fwd.h"
#include "ldtk_gen_tags_fwd.h"

#include <bn_assert.h>
#include <bn_fixed_point.h>
#include <bn_point.h>
#include <bn_size.h>
#include <bn_span.h>

namespace ldtk
{

class entity
{
public:
    /// @cond DO_NOT_DOCUMENT
    constexpr entity(const entity_definition& def, const bn::point& grid, const bn::span<const field>& field_instances,
                     const bn::size& size, gen::entity_iid iid, const bn::point& px)
        : _def(def), _grid(grid), _field_instances(field_instances), _size(size), _iid(iid), _px(px)
    {
    }
    /// @endcond

public:
    /// @brief Looks up a field with its identifier.
    /// @note Look-up is done via indexing, thus it's O(1). \n
    /// @b Never use unrelated entity's field identifier,
    /// that would result in an error, or getting the wrong field.
    /// @param identifier Unique identifier of the field to look up.
    /// @return Reference to the field.
    [[nodiscard]] constexpr auto get_field(gen::entity_field_ident identifier) const -> const field&
    {
        BN_ASSERT((int)identifier >= 0, "Invalid identifier (gen::entity_field_ident)", (int)identifier);
        BN_ASSERT((int)identifier < _field_instances.size(), "Out of bound identifier (gen::entity_field_ident)",
                  (int)identifier, " - perhaps it's a field of different entity kind");

        return _field_instances.data()[(int)identifier];
    }

public:
    /// @brief Reference to the Entity definition
    [[nodiscard]] constexpr auto def() const -> const entity_definition&
    {
        return _def;
    }

    /// @brief Grid-based coordinates (`[x,y]` format)
    [[nodiscard]] constexpr auto grid() const -> const bn::point&
    {
        return _grid;
    }

    /// @brief Entity definition identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::entity_ident
    {
        return def().identifier();
    }

    /// @brief Pivot coordinates (`[x,y]` format, values are from 0 to 1) of the Entity
    [[nodiscard]] constexpr auto pivot() const -> const bn::fixed_point&
    {
        return def().pivot();
    }

    /// @brief Array of tags defined in this Entity definition
    [[nodiscard]] constexpr auto tags() const -> const bn::span<const gen::entity_tag>&
    {
        return def().tags();
    }

    /// @brief An array of all custom fields and their values.
    [[nodiscard]] constexpr auto field_instances() const -> const bn::span<const field>&
    {
        return _field_instances;
    }

    /// @brief Entity size in pixels.
    /// For non-resizable entities, it will be the same as Entity definition.
    [[nodiscard]] constexpr auto size() const -> const bn::size&
    {
        return _size;
    }

    /// @brief Entity width in pixels.
    /// For non-resizable entities, it will be the same as Entity definition.
    [[nodiscard]] constexpr auto width() const -> int
    {
        return size().width();
    }

    /// @brief Entity height in pixels.
    /// For non-resizable entities, it will be the same as Entity definition.
    [[nodiscard]] constexpr auto height() const -> int
    {
        return size().height();
    }

    /// @brief Unique instance identifier
    [[nodiscard]] constexpr auto iid() const -> gen::entity_iid
    {
        return _iid;
    }

    /// @brief Pixel coordinates (`[x,y]` format) in current level coordinate space.
    /// Don't forget optional layer offsets, if they exist!
    [[nodiscard]] constexpr auto px() const -> const bn::point&
    {
        return _px;
    }

private:
    const entity_definition& _def;

    bn::point _grid;

    bn::span<const field> _field_instances;
    bn::size _size;
    gen::entity_iid _iid;
    bn::point _px;
};

} // namespace ldtk
