# SPDX-FileCopyrightText: Copyright 2025 Guyeon Yu <copyrat90@gmail.com>
# SPDX-License-Identifier: Zlib

from typing import Final


class UnsupportedProjectVersionException(Exception):
    SUPPORTED_LDTK_VERSION: Final[str] = "1.5.3"

    def __init__(self, version: str):
        super().__init__(f"Unsupported LDtk project version - {version}")
        self.version = version


class IdentifierStyleNotLowercase(Exception):
    def __init__(self, style: str):
        super().__init__(
            f"Identifier style should be Full lowercase, not {style} - Change it in the project settings"
        )
        self.style = style


class ExternalEnumNotSupportedException(Exception):
    def __init__(self, source: str):
        super().__init__(f"External enum is not supported (found in {source})")
        self.source = source


class TilesetImageNotProvidedException(Exception):
    def __init__(self, source: str):
        super().__init__(f"Tileset has no image (found in {source})")
        self.source = source


class UnsupportedTileDimensionException(Exception):
    def __init__(self, dimension: int, source: str):
        super().__init__(
            f"Unsupported tile dimension - {dimension} (found in {source})"
        )
        self.dimension = dimension
        self.source = source


class ParallaxScalingNotSupportedException(Exception):
    def __init__(self, source: str):
        super().__init__(f"Parallax scaling is not supported (found in {source})")
        self.source = source


class UnsupportedFieldTypeException(Exception):
    def __init__(self, field_type: str, source: str):
        super().__init__(f"Unsupported field type - {field_type} (found in {source})")
        self.field_type = field_type
        self.source = source


class DifferentOpacitiesNotSupportedException(Exception):
    def __init__(self, opacity_1: float, layer_1: str, opacity_2: float, layer_2: str):
        super().__init__(
            f'Different opacities are not supported - (Layer "{layer_1}" has {opacity_1 * 100:.0f}% / Layer "{layer_2}" has {opacity_2 * 100:.0f}%)'
        )
        self.opacity_1 = opacity_1
        self.layer_1 = layer_1
        self.opacity_2 = opacity_2
        self.layer_2 = layer_2


class LdtkInternalIconUsedAsVisibleTileException(Exception):
    def __init__(self):
        super().__init__("LDtk internal icon is used as a tile, which is forbidden")


class TooManyUsedTilesInTilesetException(Exception):
    def __init__(self, tiles_count: int, tileset: str):
        super().__init__(
            f'Too many used tiles - {tiles_count} (found in Tileset "{tileset}")'
        )
        self.tiles_count = tiles_count
        self.tileset = tileset


class NoLayerException(Exception):
    def __init__(self):
        super().__init__("No layer exists")


class UnalignedTileNotSupportedException(Exception):
    def __init__(self, level: str, layer: str, x: int, y: int):
        super().__init__(
            f'Tiles not aligned to the grid is not supported (found in Level "{level}", Layer "{layer}", pos ({x}, {y}))'
        )
        self.level = level
        self.layer = layer
        self.x = x
        self.y = y


class TooManyVisibleLayersException(Exception):
    def __init__(self, visible_layers_count: int, level: str):
        super().__init__(
            f'More than 4 visible layers are not supported - {visible_layers_count} (found in Level "{level}")'
        )
        self.visible_layers_count = visible_layers_count
        self.level = level
