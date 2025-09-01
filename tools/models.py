import LdtkJson
from typing import Final, Optional, List, Dict, Set, NamedTuple
from PIL import ImageColor
from math import floor


class Point(NamedTuple):
    x: int
    y: int

    def __str__(self) -> str:
        return f"bn::point({self.x}, {self.y})"


class FixedPoint(NamedTuple):
    x: float
    y: float

    def __str__(self) -> str:
        return f"bn::fixed_point({self.x}, {self.y})"


class Size(NamedTuple):
    width: int
    height: int

    def __str__(self) -> str:
        return f"bn::size({self.width}, {self.height})"


class Color:
    def __init__(self, color_code: str):
        color = ImageColor.getrgb(color_code)
        self.r = min(31, floor(color[0] / 8))
        self.g = min(31, floor(color[1] / 8))
        self.b = min(31, floor(color[2] / 8))

    def __str__(self) -> str:
        return f"bn::color({self.r}, {self.g}, {self.b})"


FIELD_TYPE: Final[Dict[str, str]] = {
    "Int": "INT",
    "Float": "FIXED",
    "Bool": "BOOL",
    "String": "STRING",
    "Multilines": "STRING",
    "Color": "COLOR",
    "LocalEnum": "TYPED_ENUM",
    "EntityRef": "ENTITY_REF",
    "Point": "POINT",
}


class FieldDef(NamedTuple):
    field_type: str
    enum_type: Optional[str]
    can_be_null: bool
    identifier: str
    uid: int


class ParsedFieldType(NamedTuple):
    field_type: str
    enum_type: Optional[str]


def parse_field_type(raw: str, can_contain_null: bool) -> ParsedFieldType:
    arr_split = raw.replace(">", "<").split("<")
    if arr_split[0] == "Array":
        enum_split = arr_split[1].split(".")
        if enum_split[0] == "LocalEnum":
            enum_type = enum_split[1]
            field_type = (
                ("OPTIONAL_" if can_contain_null else "")
                + FIELD_TYPE[enum_split[0]]
                + "_SPAN"
            )
        else:
            enum_type = None
            field_type = (
                ("OPTIONAL_" if can_contain_null else "")
                + FIELD_TYPE[arr_split[1]]
                + "_SPAN"
            )
    else:
        enum_split = raw.split(".")
        if enum_split[0] == "LocalEnum":
            enum_type = enum_split[1]
            field_type = FIELD_TYPE[enum_split[0]]
        else:
            enum_type = None
            field_type = FIELD_TYPE[raw]

    return ParsedFieldType(field_type, enum_type)


class EnumInfos:
    def __init__(self, ldtk_project: LdtkJson.LdtkJSON):
        enum_defs = ldtk_project.defs.enums
        self.__enum_names: Dict[int, str] = {
            enum_def.uid: enum_def.identifier for enum_def in enum_defs
        }
        self.__enum_values: Dict[str, List[str]] = {
            enum_def.identifier: [value_def.id for value_def in enum_def.values]
            for enum_def in enum_defs
        }

    def get_enum_name_with_uid(self, enum_uid: int) -> str:
        return self.__enum_names[enum_uid]

    def get_enum_values_with_enum_name(self, enum_name: str) -> List[str]:
        return self.__enum_values[enum_name]

    def get_enum_values_with_uid(self, enum_uid: int) -> List[str]:
        return self.get_enum_values_with_enum_name(
            self.get_enum_name_with_uid(enum_uid)
        )


class TilesetInfos:
    def __init__(self, ldtk_project: LdtkJson.LdtkJSON):
        tileset_idx_to_def: Dict[int, LdtkJson.TilesetDefinition] = {
            idx: tileset_def
            for idx, tileset_def in enumerate(ldtk_project.defs.tilesets)
        }

        tileset_uid_to_idx: Dict[int, int] = {
            tileset_def.uid: idx
            for idx, tileset_def in enumerate(ldtk_project.defs.tilesets)
        }

        # Get tileset by index -> Set of used tiles (src points)
        tilesets_used_tiles: List[Set[Point]] = [
            set() for _ in range(len(ldtk_project.defs.tilesets))
        ]

        for level in ldtk_project.levels:
            if level.layer_instances is None:
                raise AssertionError(
                    f'Level "{level.identifier}" does not have layer_instances'
                )
            for layer in level.layer_instances:
                # To skip overlapped tiles in the same grid pos
                overlapped_pos: Set[Point] = set()
                assert not (layer.auto_layer_tiles and layer.grid_tiles)
                tiles: List[LdtkJson.TileInstance] = layer.auto_layer_tiles
                if not tiles:
                    tiles = layer.grid_tiles
                if tiles:
                    assert layer.tileset_def_uid is not None
                    used_tiles: Set[Point] = tilesets_used_tiles[
                        tileset_uid_to_idx[layer.tileset_def_uid]
                    ]
                    # Reversed to use skip overlapping tile behind the same px pos
                    for tile in reversed(tiles):
                        pos: Point = Point(
                            tile.px[0] // layer.grid_size, tile.px[1] // layer.grid_size
                        )
                        # Ignore OOB tile
                        if (
                            pos.x < 0
                            or pos.x >= layer.c_wid
                            or pos.y < 0
                            or pos.y >= layer.c_hei
                        ):
                            continue
                        if pos not in overlapped_pos:
                            overlapped_pos.add(pos)
                            used_tiles.add(Point(tile.src[0], tile.src[1]))

        self.__tileset_uid_to_idx: Dict[int, int] = tileset_uid_to_idx
        self.__tileset_idx_to_def: Dict[int, LdtkJson.TilesetDefinition] = (
            tileset_idx_to_def
        )
        self.__used_tile_idxes: List[Dict[Point, int]] = [
            {src: tile_idx for tile_idx, src in enumerate(srcs)}
            for srcs in tilesets_used_tiles
        ]
        self.__used_tile_srcs: List[List[Point]] = [
            list(srcs) for srcs in tilesets_used_tiles
        ]

    def get_tileset_idx(self, tileset_uid: int) -> int:
        return self.__tileset_uid_to_idx[tileset_uid]

    def get_tileset_def(self, tileset_uid: int) -> LdtkJson.TilesetDefinition:
        return self.__tileset_idx_to_def[self.get_tileset_idx(tileset_uid)]

    def get_tileset_used_tiles_count(self, tileset_uid: int) -> int:
        return len(self.__used_tile_idxes[self.get_tileset_idx(tileset_uid)])

    def get_tileset_used_tile_idx(self, tileset_uid: int, tile_src: Point) -> int:
        return self.__used_tile_idxes[self.get_tileset_idx(tileset_uid)][tile_src]

    def get_tileset_is_used_tile_id(self, tileset_uid: int, tile_id: int):
        tile_src = self.__get_tile_src(tileset_uid, tile_id)
        return tile_src in self.__used_tile_idxes[self.get_tileset_idx(tileset_uid)]

    def get_tileset_used_tile_id_to_idx(self, tileset_uid: int, tile_id: int) -> int:
        tile_src = self.__get_tile_src(tileset_uid, tile_id)
        return self.get_tileset_used_tile_idx(tileset_uid, tile_src)

    def get_tileset_used_tile_src(self, tileset_uid: int, tile_idx: int) -> Point:
        return self.__used_tile_srcs[self.get_tileset_idx(tileset_uid)][tile_idx]

    def __get_tile_src(self, tileset_uid: int, tile_id: int):
        tileset_def = self.get_tileset_def(tileset_uid)
        grid_x = tile_id % tileset_def.c_wid
        grid_y = tile_id // tileset_def.c_wid
        square_diff = tileset_def.tile_grid_size + tileset_def.padding
        return Point(
            tileset_def.padding + grid_x * square_diff,
            tileset_def.padding + grid_y * square_diff,
        )
