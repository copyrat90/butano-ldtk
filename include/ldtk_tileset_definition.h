#pragma once

#include "ldtk_tileset_custom_data.h"
#include "ldtk_tileset_enum_tag.h"

#include "ldtk_gen_ident_fwd.h"
#include "ldtk_gen_tag_fwd.h"

#include <bn_optional.h>
#include <bn_regular_bg_item.h>
#include <bn_span.h>
#include <bn_type_id.h>

namespace ldtk
{

class tileset_definition
{
public:
    constexpr tileset_definition(const bn::regular_bg_item& bg_item, int tiles_count,
                                 bn::span<const tileset_custom_data> custom_data,
                                 bn::span<const tileset_enum_tag> enum_tags, gen::ident identifier,
                                 bn::span<const gen::tag> tags, bn::optional<bn::type_id_t> tags_source_enum_id,
                                 int tile_grid_size, int uid)
        : _bg_item(bg_item), _tiles_count(tiles_count), _custom_data(custom_data), _enum_tags(enum_tags),
          _identifier(identifier), _tags(tags), _tags_source_enum_id(tags_source_enum_id),
          _tile_grid_size(tile_grid_size), _uid(uid)
    {
    }

public:
    /// @brief Background item (actual tileset data)
    [[nodiscard]] constexpr auto bg_item() const -> const bn::regular_bg_item&
    {
        return _bg_item;
    }

    /// @brief Number of tiles this tileset has
    [[nodiscard]] constexpr auto tiles_count() const -> int
    {
        return _tiles_count;
    }

    /// @brief An array of custom tile metadata
    [[nodiscard]] constexpr auto custom_data() const -> const bn::span<const tileset_custom_data>&
    {
        return _custom_data;
    }

    /// @brief Tileset tags using Enum values specified by `tags_source_enum_id`. \n
    /// This array contains 1 element per Enum value, which contains an array of all Tile IDs that are tagged with it.
    [[nodiscard]] constexpr auto enum_tags() const -> const bn::span<const tileset_enum_tag>&
    {
        return _enum_tags;
    }

    /// @brief User defined unique identifier
    [[nodiscard]] constexpr auto identifier() const -> gen::ident
    {
        return _identifier;
    }

    /// @brief An array of user-defined tags to organize the Tilesets
    [[nodiscard]] constexpr auto tags() const -> const bn::span<const gen::tag>&
    {
        return _tags;
    }

    /// @brief Optional Enum definition ID used for this tileset meta-data
    [[nodiscard]] constexpr auto tags_source_enum_id() const -> const bn::optional<bn::type_id_t>&
    {
        return _tags_source_enum_id;
    }

    [[nodiscard]] constexpr auto tile_grid_size() const -> int
    {
        return _tile_grid_size;
    }

    /// @brief Unique Intidentifier
    [[nodiscard]] constexpr auto uid() const -> int
    {
        return _uid;
    }

private:
    const bn::regular_bg_item& _bg_item;

    int _tiles_count;
    bn::span<const tileset_custom_data> _custom_data;
    bn::span<const tileset_enum_tag> _enum_tags;
    gen::ident _identifier;
    bn::span<const gen::tag> _tags;
    bn::optional<bn::type_id_t> _tags_source_enum_id;
    int _tile_grid_size;
    int _uid;
};

} // namespace ldtk
