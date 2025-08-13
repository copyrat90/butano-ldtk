from pathlib import Path
import LdtkJson
from gen_sources import *
from typing import Final, Any
from PIL import Image
import math


def create_folder(folder_path: Path):
    folder_path.mkdir(parents=True, exist_ok=True)


def remove_built_files(build_folder_path: Path):
    for path in build_folder_path.joinpath("graphics").glob("ldtk_gen_*.bmp"):
        path.unlink(missing_ok=True)
    for path in build_folder_path.joinpath("graphics").glob("ldtk_gen_*.json"):
        path.unlink(missing_ok=True)
    for path in build_folder_path.joinpath("include").glob("ldtk_gen_*.h"):
        path.unlink(missing_ok=True)
    for path in build_folder_path.joinpath("src").glob("ldtk_gen_*.cpp"):
        path.unlink(missing_ok=True)


def layer_def_has_visible_tiles(layer: LdtkJson.LayerDefinition) -> bool:
    return (
        (layer.type == "IntGrid" and layer.tileset_def_uid is not None)
        or layer.type == "Tiles"
        or layer.type == "AutoLayer"
    )


def load_ldtk_project(ldtk_project_file_path: Path) -> LdtkJson.LdtkJSON:
    import json

    with open(ldtk_project_file_path) as ldtk_project_file:
        ldtk_project_raw_dict: dict[Any, Any] = json.load(ldtk_project_file)
        ldtk_project_version: str = ldtk_project_raw_dict["jsonVersion"]
        if (
            ldtk_project_version
            != UnsupportedProjectVersionException.SUPPORTED_LDTK_VERSION
        ):
            raise UnsupportedProjectVersionException(ldtk_project_version)

        return LdtkJson.ldtk_json_from_dict(ldtk_project_raw_dict)


def load_ldtk_project_if_process_required(
    ldtk_project_file_path: Path, build_folder_path: Path
) -> Optional[LdtkJson.LdtkJSON]:
    gen_project_header_path = build_folder_path.joinpath("include/ldtk_gen_project.h")
    tools_path = Path(__file__).parent

    if not gen_project_header_path.exists():
        return load_ldtk_project(ldtk_project_file_path)

    source_project_modified_time = ldtk_project_file_path.stat().st_mtime
    gen_project_modified_time = gen_project_header_path.stat().st_mtime

    if source_project_modified_time >= gen_project_modified_time:
        return load_ldtk_project(ldtk_project_file_path)
    for script_path in tools_path.glob("*.py"):
        script_modified_time = script_path.stat().st_mtime
        if script_modified_time >= gen_project_modified_time:
            return load_ldtk_project(ldtk_project_file_path)

    ldtk_project_folder_path = ldtk_project_file_path.parent
    ldtk_project = load_ldtk_project(ldtk_project_file_path)

    for tileset in ldtk_project.defs.tilesets:
        if tileset.rel_path is None:
            continue

        tileset_file_path = ldtk_project_folder_path.joinpath(tileset.rel_path)
        tileset_modified_time = tileset_file_path.stat().st_mtime
        if tileset_modified_time >= gen_project_modified_time:
            return ldtk_project

    return None


def ensure_identifier_style_lowercase(ldtk_project: LdtkJson.LdtkJSON):
    if ldtk_project.identifier_style != LdtkJson.IdentifierStyle.LOWERCASE:
        raise IdentifierStyleNotLowercase(str(ldtk_project.identifier_style))


def ensure_no_external_levels(ldtk_project: LdtkJson.LdtkJSON):
    if ldtk_project.external_levels:
        raise ExternalLevelsNotSupportedException()


def ensure_no_tileset_without_image(ldtk_project: LdtkJson.LdtkJSON):
    for tileset_def in ldtk_project.defs.tilesets:
        if tileset_def.rel_path is None and not isinstance(
            tileset_def.embed_atlas, LdtkJson.EmbedAtlas
        ):
            raise TilesetImageNotProvidedException(f"Tileset {tileset_def.identifier}")


def ensure_tile_dimensions_valid(ldtk_project: LdtkJson.LdtkJSON):
    for tileset_def in ldtk_project.defs.tilesets:
        if tileset_def.tile_grid_size % 8 != 0:
            raise UnsupportedTileDimensionException(
                tileset_def.tile_grid_size, f'Tileset "{tileset_def.identifier}"'
            )

    for layer_def in ldtk_project.defs.layers:
        if layer_def_has_visible_tiles(layer_def):
            if layer_def.grid_size % 8 != 0:
                raise UnsupportedTileDimensionException(
                    layer_def.grid_size, f'Layer "{layer_def.identifier}"'
                )


def ensure_no_parallax_scaling(ldtk_project: LdtkJson.LdtkJSON):
    for layer_def in ldtk_project.defs.layers:
        if layer_def_has_visible_tiles(layer_def):
            if layer_def.parallax_scaling and (
                layer_def.parallax_factor_x != 0 or layer_def.parallax_factor_y != 0
            ):
                raise ParallaxScalingNotSupportedException(
                    f'Layer "{layer_def.identifier}"'
                )


def ensure_no_unsupported_fields(ldtk_project: LdtkJson.LdtkJSON):
    def check_field_type(field_def: LdtkJson.FieldDefinition, is_level: bool):
        def raise_error():
            raise UnsupportedFieldTypeException(
                field_def.type,
                f'{("Level" if is_level else "Entity")} field "{field_def.identifier}"',
            )

        arr_split = field_def.type.replace(">", "<").split("<")
        if arr_split[0] == "Array":
            elem_type = arr_split[1]
        else:
            elem_type = field_def.type

        if elem_type.split(".")[0] == "ExternEnum":
            raise_error()
        if elem_type == "FilePath":
            raise_error()
        if elem_type == "Tile":
            raise_error()

    for level_field_def in ldtk_project.defs.level_fields:
        check_field_type(level_field_def, is_level=True)

    for entity_def in ldtk_project.defs.entities:
        for entity_field_def in entity_def.field_defs:
            check_field_type(entity_field_def, is_level=False)


def ensure_no_different_opacities(ldtk_project: LdtkJson.LdtkJSON):
    first_opacity_layer_identifier: str = ""
    first_opacity: float = 1

    for layer_def in ldtk_project.defs.layers:
        if layer_def.display_opacity != 1:
            if not first_opacity_layer_identifier:
                first_opacity_layer_identifier = layer_def.identifier
                first_opacity = layer_def.display_opacity
            elif first_opacity != layer_def.display_opacity:
                raise DifferentOpacitiesNotSupportedException(
                    first_opacity,
                    first_opacity_layer_identifier,
                    layer_def.display_opacity,
                    layer_def.identifier,
                )


def ensure_minimum_level_size(ldtk_project: LdtkJson.LdtkJSON):
    for level in ldtk_project.levels:
        if level.px_wid < 240 or level.px_hei < 160:
            raise LevelSizeTooSmallException(
                level.px_wid, level.px_hei, level.identifier
            )


def ensure_no_unaligned_tiles(ldtk_project: LdtkJson.LdtkJSON):
    def raise_if_unaligned(
        tile: LdtkJson.TileInstance,
        layer: LdtkJson.LayerInstance,
        level: LdtkJson.Level,
    ):
        if tile.px[0] % layer.grid_size != 0 or tile.px[1] % layer.grid_size != 0:
            raise UnalignedTileNotSupportedException(
                level.identifier, layer.identifier, tile.px[0], tile.px[1]
            )

    for level in ldtk_project.levels:
        if level.layer_instances is None:
            raise NoLayerException()

        for layer in level.layer_instances:
            for tile in layer.auto_layer_tiles:
                raise_if_unaligned(tile, layer, level)
            for tile in layer.grid_tiles:
                raise_if_unaligned(tile, layer, level)


def ensure_no_more_than_4_visible_layers(ldtk_project: LdtkJson.LdtkJSON):
    for level in ldtk_project.levels:
        if level.layer_instances is None:
            raise NoLayerException()

        visible_layers_count = 0
        for layer in level.layer_instances:
            if layer.visible and (layer.auto_layer_tiles or layer.grid_tiles):
                visible_layers_count += 1

        if visible_layers_count > 4:
            raise TooManyVisibleLayersException(visible_layers_count, level.identifier)


def ensure_no_unsupported_features(ldtk_project: LdtkJson.LdtkJSON):
    ensure_identifier_style_lowercase(ldtk_project)
    ensure_no_external_levels(ldtk_project)
    ensure_no_tileset_without_image(ldtk_project)
    ensure_tile_dimensions_valid(ldtk_project)
    ensure_no_parallax_scaling(ldtk_project)
    ensure_no_unsupported_fields(ldtk_project)
    ensure_no_different_opacities(ldtk_project)
    ensure_minimum_level_size(ldtk_project)
    ensure_no_unaligned_tiles(ldtk_project)
    ensure_no_more_than_4_visible_layers(ldtk_project)


def generate_tilesets_bg_items(
    tileset_infos: TilesetInfos,
    ldtk_project: LdtkJson.LdtkJSON,
    ldtk_project_folder_path: Path,
    build_folder_path: Path,
):
    TRANSPARENT_COLOR: Final[str] = "#00FF0000"

    TILESET_BG_WIDTH: Final[int] = 256
    # BG width is large at first to ensure the "dominant" transparent color
    # ends up the first entry of the quantized palette.
    TEMP_LARGE_WIDTH: Final[int] = TILESET_BG_WIDTH * 2 + 1
    # BG height is always multiple of this.
    TILESET_BG_HEIGHT_UNIT: Final[int] = 256

    for tileset_def in ldtk_project.defs.tilesets:
        tiles_count = tileset_infos.get_tileset_used_tiles_count(tileset_def.uid)

        if tileset_def.rel_path is None:
            assert isinstance(tileset_def.embed_atlas, LdtkJson.EmbedAtlas)
            if tiles_count != 0:
                raise LdtkInternalIconUsedAsVisibleTileException()

        if tiles_count >= (1 << 14):
            raise TooManyUsedTilesInTilesetException(
                tiles_count, tileset_def.identifier
            )

        tileset_src_path: Optional[Path] = (
            ldtk_project_folder_path.joinpath(tileset_def.rel_path)
            if tileset_def.rel_path is not None
            else None
        )
        tileset_out_path: Path = build_folder_path.joinpath(
            f"graphics/ldtk_gen_priv_tileset_{tileset_def.identifier}"
        )

        tile_size = tileset_def.tile_grid_size
        tiles_count_per_height_unit = 1024 / ((tile_size >> 3) ** 2)
        tileset_bg_height = TILESET_BG_HEIGHT_UNIT * math.ceil(
            (1 + tiles_count) / tiles_count_per_height_unit
        )

        with Image.new(
            "RGBA", (TEMP_LARGE_WIDTH, tileset_bg_height), color=TRANSPARENT_COLOR
        ) as tileset_bg:
            # Start from after the first transparent tile
            paste_x, paste_y = ((tile_size >> 3) ** 2) * 8, 0
            while paste_x >= TILESET_BG_WIDTH:
                paste_x -= TILESET_BG_WIDTH
                paste_y += 8

            # Internal icons are not copied via this check
            if tileset_src_path is not None:
                with Image.open(tileset_src_path) as tileset_src:
                    for i in range(
                        tileset_infos.get_tileset_used_tiles_count(tileset_def.uid)
                    ):
                        src = tileset_infos.get_tileset_used_tile_src(
                            tileset_def.uid, i
                        )
                        for y in range(tile_size >> 3):
                            for x in range(tile_size >> 3):
                                sub_x = src.x + x * 8
                                sub_y = src.y + y * 8
                                tile = tileset_src.crop(
                                    (sub_x, sub_y, sub_x + 8, sub_y + 8)
                                )
                                tileset_bg.paste(tile, (paste_x, paste_y))

                                paste_x += 8
                                if paste_x >= TILESET_BG_WIDTH:
                                    assert paste_x == TILESET_BG_WIDTH
                                    paste_x = 0
                                    paste_y += 8

            # Finalize the tileset BG & save it
            tileset_bg = tileset_bg.quantize(256)
            tileset_bg = tileset_bg.crop((0, 0, TILESET_BG_WIDTH, tileset_bg_height))
            tileset_bg.save(tileset_out_path.with_suffix(".bmp"))

        with tileset_out_path.with_suffix(".json").open("w") as tileset_json:
            tileset_json.write('{"type":"regular_bg","bpp_mode":"bpp_4_auto"}')


def generate_tileset_definitions(
    enum_infos: EnumInfos,
    tileset_infos: TilesetInfos,
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    custom_datas_header = TilesetDefinitionsCustomDatasHeader()
    enum_tags_header = TilesetDefinitionsEnumTagsHeader()
    enum_tag_tile_indexes_header = TilesetDefinitionsEnumTagTileIndexesHeader()
    tags_header = TilesetDefinitionsTagsHeader()
    defs_header = TilesetDefinitionsHeader()

    for tileset_def in ldtk_project.defs.tilesets:
        custom_datas_header.add_tileset(tileset_def, tileset_infos)
        enum_tags_header.add_tileset(tileset_def)
        enum_tag_tile_indexes_header.add_tileset(tileset_def, tileset_infos)
        tags_header.add_tileset(tileset_def)
        defs_header.add_tileset(tileset_def, tileset_infos, enum_infos)

    custom_datas_header.write(build_folder_path)
    enum_tags_header.write(build_folder_path)
    enum_tag_tile_indexes_header.write(build_folder_path)
    tags_header.write(build_folder_path)
    defs_header.write(build_folder_path)


def generate_level_field_definitions(
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    level_fields_header = LevelFieldDefinitionsHeader(ldtk_project.defs.level_fields)
    level_fields_header.write(build_folder_path)


def generate_layer_definitions(
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    int_grid_values_header = LayerDefinitionsIntGridValuesHeader()
    int_grid_value_groups_header = LayerDefinitionsIntGridValueGroupsHeader()
    defs_header = LayerDefinitionsHeader()

    for layer_def in ldtk_project.defs.layers:
        int_grid_values_header.add_layer(layer_def)
        int_grid_value_groups_header.add_layer(layer_def)
        defs_header.add_layer(layer_def)

    int_grid_values_header.write(build_folder_path)
    int_grid_value_groups_header.write(build_folder_path)
    defs_header.write(build_folder_path)


def generate_entity_definitions(
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    entity_fields_header = EntityFieldDefinitionsHeader()
    entity_tags_header = EntityDefinitionsTagsHeader()
    defs_header = EntityDefinitionsHeader()

    for entity_def in ldtk_project.defs.entities:
        entity_fields_header.add_entity(entity_def)
        entity_tags_header.add_entity(entity_def)
        defs_header.add_entity(entity_def)

    entity_fields_header.write(build_folder_path)
    entity_tags_header.write(build_folder_path)
    defs_header.write(build_folder_path)


def generate_enum_headers(
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    enums_header = EnumsHeader()
    ident_header = IdentHeader()
    iid_header = IidHeader()
    tag_header = TagHeader()

    iid_header.add_iid(ldtk_project.iid, "PROJECT")

    for enum_def in ldtk_project.defs.enums:
        if enum_def.external_rel_path is not None:
            raise ExternalEnumNotSupportedException(f'Enum "{enum_def.identifier}"')
        enums_header.add_enum(enum_def)

    ident_header.add_tileset_idents(ldtk_project.defs.tilesets)
    for tileset_def in ldtk_project.defs.tilesets:
        for tag in tileset_def.tags:
            tag_header.add_tag(tag, "TILESET")

    ident_header.add_level_idents(ldtk_project.levels)
    ident_header.add_level_field_idents(ldtk_project.defs.level_fields)
    for level in ldtk_project.levels:
        iid_header.add_iid(level.iid, "LEVEL")
        assert level.layer_instances is not None
        for layer in level.layer_instances:
            iid_header.add_iid(layer.iid, "LAYER")
            for entity in layer.entity_instances:
                iid_header.add_iid(entity.iid, "ENTITY")

    ident_header.add_layer_idents(ldtk_project.defs.layers)
    for layer in ldtk_project.defs.layers:
        ident_header.add_layer_int_grid_value_idents(
            layer.identifier, layer.int_grid_values
        )
        ident_header.add_layer_int_grid_value_group_idents(
            layer.identifier, layer.int_grid_values_groups
        )

    ident_header.add_entity_idents(ldtk_project.defs.entities)
    for entity_def in ldtk_project.defs.entities:
        ident_header.add_entity_field_idents(
            entity_def.identifier, entity_def.field_defs
        )
        for tag in entity_def.tags:
            tag_header.add_tag(tag, "ENTITY")

    enums_header.write(build_folder_path)
    ident_header.write(build_folder_path)
    iid_header.write(build_folder_path)
    tag_header.write(build_folder_path)


def generate_definitions_headers(
    enum_infos: EnumInfos,
    tileset_infos: TilesetInfos,
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    generate_tileset_definitions(
        enum_infos, tileset_infos, ldtk_project, build_folder_path
    )
    generate_level_field_definitions(ldtk_project, build_folder_path)
    generate_layer_definitions(ldtk_project, build_folder_path)
    generate_entity_definitions(ldtk_project, build_folder_path)

    defs_header = DefinitionsHeader()
    defs_header.write(build_folder_path)


def generate_levels_headers(
    tileset_infos: TilesetInfos,
    ldtk_project: LdtkJson.LdtkJSON,
    build_folder_path: Path,
):
    level_fields_header = LevelFieldInstancesHeader()
    level_field_arrays_header = LevelFieldArraysHeader()

    auto_layer_tiles_header = LayerAutoLayerTilesHeader()
    auto_layer_tiles_cells_header = LayerAutoLayerTilesCellsHeader()
    grid_tiles_header = LayerGridTilesHeader()
    grid_tiles_cells_header = LayerGridTilesCellsHeader()
    int_grids_header = LayerIntGridsHeader()
    int_grid_cells_header = LayerIntGridCellsHeader()
    layers_header = LevelLayerInstancesHeader()

    entity_fields_header = LayerEntityFieldInstancesHeader()
    entity_field_arrays_header = LayerEntityFieldArraysHeader()
    entities_header = LayerEntityInstancesHeader()

    levels_header = LevelsHeader(ldtk_project.levels)

    entity_def_lut: Dict[int, LdtkJson.EntityDefinition] = {
        entity_def.uid: entity_def for entity_def in ldtk_project.defs.entities
    }
    """Entity def uid -> def"""

    entity_idx_lut: Dict[int, int] = {
        entity_def.uid: idx for idx, entity_def in enumerate(ldtk_project.defs.entities)
    }
    """Entity def uid -> def idx"""

    for level in ldtk_project.levels:
        level_fields_header.add_fields(level.identifier, level.field_instances)
        for field_idx, field in enumerate(level.field_instances):
            level_field_arrays_header.add_field_array(
                level.identifier, field, ldtk_project.defs.level_fields[field_idx]
            )

        assert level.layer_instances is not None
        layers_header.add_layers(level.identifier, level.layer_instances, tileset_infos)
        for layer in level.layer_instances:
            # Visible tiles
            if layer.tileset_def_uid is not None:
                if len(layer.auto_layer_tiles) != 0:
                    auto_layer_tiles_header.add_grid(level.identifier, layer)
                    auto_layer_tiles_cells_header.add_tiles(
                        layer.auto_layer_tiles, level.identifier, layer, tileset_infos
                    )

                if len(layer.grid_tiles) != 0:
                    grid_tiles_header.add_grid(level.identifier, layer)
                    grid_tiles_cells_header.add_tiles(
                        layer.grid_tiles, level.identifier, layer, tileset_infos
                    )

            # IntGrid
            if layer.int_grid_csv:
                # if has non-zero cell
                if any(layer.int_grid_csv):
                    int_grids_header.add_grid(level.identifier, layer, is_empty=False)
                    int_grid_cells_header.add_cells(level.identifier, layer)
                # if all cells are zero
                else:
                    int_grids_header.add_grid(level.identifier, layer, is_empty=True)

            # Entities
            entities_header.add_entities(level.identifier, layer, entity_idx_lut)
            for entity in layer.entity_instances:
                entity_fields_header.add_fields(
                    entity.iid.replace("-", "_"), entity.field_instances
                )
                entity_fields_header.add_entity_iid_identifier_mapping(
                    entity.iid.replace("-", "_"),
                    entity_def_lut[entity.def_uid].identifier,
                )
                for field_idx, field in enumerate(entity.field_instances):
                    entity_field_arrays_header.add_field_array(
                        entity.iid.replace("-", "_"),
                        field,
                        entity_def_lut[entity.def_uid].field_defs[field_idx],
                    )

    level_fields_header.write(build_folder_path)
    level_field_arrays_header.write(build_folder_path)

    auto_layer_tiles_header.write(build_folder_path)
    auto_layer_tiles_cells_header.write(build_folder_path)
    grid_tiles_header.write(build_folder_path)
    grid_tiles_cells_header.write(build_folder_path)
    int_grids_header.write(build_folder_path)
    int_grid_cells_header.write(build_folder_path)
    layers_header.write(build_folder_path)

    entity_fields_header.write(build_folder_path)
    entity_field_arrays_header.write(build_folder_path)
    entities_header.write(build_folder_path)

    levels_header.write(build_folder_path)


def process_ldtk(ldtk_project_file_path: Path, build_folder_path: Path) -> bool:
    """Returns `False` if the process is skipped, because there's no modification"""
    try:
        create_folder(build_folder_path.joinpath("include"))
        create_folder(build_folder_path.joinpath("graphics"))
        create_folder(build_folder_path.joinpath("src"))

        ldtk_project = load_ldtk_project_if_process_required(
            ldtk_project_file_path, build_folder_path
        )
        if ldtk_project is None:
            return False

        print("Start converting LDtk project...")

        remove_built_files(build_folder_path)
        ensure_no_unsupported_features(ldtk_project)

        ldtk_project_folder_path: Path = ldtk_project_file_path.parent

        enum_infos = EnumInfos(ldtk_project)
        tileset_infos = TilesetInfos(ldtk_project)
        generate_tilesets_bg_items(
            tileset_infos, ldtk_project, ldtk_project_folder_path, build_folder_path
        )
        generate_definitions_headers(
            enum_infos, tileset_infos, ldtk_project, build_folder_path
        )
        generate_levels_headers(tileset_infos, ldtk_project, build_folder_path)

        # This one should be last, because functions above might sort identifiers
        generate_enum_headers(ldtk_project, build_folder_path)

        # Finally, generate the main project header
        project_header = ProjectHeader(ldtk_project)
        project_header.write(build_folder_path)

        return True
    except:
        remove_built_files(build_folder_path)
        raise


if __name__ == "__main__":
    import argparse
    import sys
    import traceback

    parser = argparse.ArgumentParser(description="LDtk converter for Butano.")
    parser.add_argument("--input", required=True, help="input LDtk project file")
    parser.add_argument("--build", required=True, help="build folder path")

    try:
        args = parser.parse_args()
        ldtk_project_file_path = Path(args.input)
        build_folder_path = Path(args.build)

        if process_ldtk(ldtk_project_file_path, build_folder_path):
            print(
                f'Successfully converted LDtk project "{ldtk_project_file_path}" to "{build_folder_path}"'
            )
        else:
            print(
                f'Skipped converting LDtk project "{ldtk_project_file_path}" (not modified since last conversion)'
            )
    except Exception as ex:
        sys.stderr.write(f"Error: {ex}\n")
        traceback.print_exc()
        exit(-1)
