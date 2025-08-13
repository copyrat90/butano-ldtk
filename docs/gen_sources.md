# Generated headers

## enum

* ldtk_gen_ident.h
* ldtk_gen_iid.h
* ldtk_gen_tag.h

* ldtk_gen_enums.h

## definitions

* ldtk_gen_priv_definitions.h
    > `definitions   gen_priv_definitions;`
    * ldtk_gen_priv_entity_definitions.h
        > `entity_definition[]   gen_priv_entity_definitions;`
        * ldtk_gen_priv_entity_field_definitions.h
            > `field_definition[]   gen_priv_entity_<identifier>_field_definitions;`
        * ldtk_gen_priv_entity_definitions_tags.h
            > `tag[]   gen_priv_entity_<identifier>_tags;`
    * ldtk_gen_priv_layer_definitions.h
        > `layer_definition[]   gen_priv_layer_definitions;`
        * ldtk_gen_priv_layer_definitions_int_grid_values.h
            > `int_grid_value_info[]   gen_priv_layer_<identifier>_definition_int_grid_values;`
        * ldtk_gen_priv_layer_definitions_int_grid_value_groups.h
            > `int_grid_value_group_info[]   gen_priv_layer_<identifier>_definition_int_grid_value_groups;`
    * ldtk_gen_priv_level_field_definitions.h
        > `field_definition[]   gen_priv_level_field_definitions;`
    * ldtk_gen_priv_tileset_definitions.h
        > `tileset_definition[]   gen_priv_tileset_definitions;`
        * ldtk_gen_priv_tileset_definitions_custom_datas.h
            > `tileset_custom_data[]   gen_priv_tileset_<identifier>_custom_datas;`
        * ldtk_gen_priv_tileset_definitions_enum_tags.h
            > `tileset_enum_tag[]   gen_priv_tileset_<identifier>_enum_tags;`
            * ldtk_gen_priv_tileset_definitions_enum_tag_tile_indexes.h
                > `tile_index[]   gen_priv_tileset_<identifier>_enum_value_<enum_value>_tag_tile_indexes;`
        * ldtk_gen_priv_tileset_definitions_tags.h
            > `tag[]   gen_priv_tileset_<identifier>_tags;`

## levels

* ldtk_gen_priv_levels.h
    > `level[]   gen_priv_levels;`
    * ldtk_gen_priv_level_field_instances.h
        > `field[]   gen_priv_level_<identifier>_field_instances;`
        * ldtk_gen_priv_level_field_arrays.h
            > `<element_type>[]   gen_priv_level_<identifier>_field_<identifier>_array;`
    * ldtk_gen_priv_level_layer_instances.h
        > `layer[]   gen_priv_level_<identifier>_layer_instances;`
        * ldtk_gen_priv_layer_auto_layer_tiles.h
            > `tile_grid_t<bool>   gen_priv_level_<identifier>_layer_<identifier>_auto_layer_tiles;`
            * ldtk_gen_priv_layer_auto_layer_tiles_cells.h
                > `std::uint8/16_t[]   gen_priv_level_<identifier>_layer_<identifier>_auto_layer_tiles_cells;`
        * ldtk_gen_priv_layer_entity_instances.h
            > `entity[]   gen_priv_level_<identifier>_layer_<identifier>_entities;`
            * ldtk_gen_priv_layer_entity_field_instances.h
                > `field[]   gen_priv_entity_<iid>_field_instances;`
                * ldtk_gen_priv_entity_field_arrays.h
                    > `<element_type>[]   gen_priv_entity_<iid>_field_<identifier>_array;`
        * ldtk_gen_priv_layer_grid_tiles.h
            > `tile_grid_t<bool>   gen_priv_level_<identifier>_layer_<identifier>_grid_tiles;`
            * ldtk_gen_priv_layer_grid_tiles_cells.h
                > `std::uint8/16_t[]   gen_priv_level_<identifier>_layer_<identifier>_grid_tiles_cells;`
        * ldtk_gen_priv_layer_int_grids.h
            > `int_grid_t<std::uint8/16/32_t>   gen_priv_level_<identifier>_layer_<identifier>_int_grid;`
            * ldtk_gen_priv_layer_int_grid_cells.h
                > `std::uint8/16/32_t[]   gen_priv_level_<identifier>_layer_<identifier>_int_grid_cells;`

## project

* ldtk_gen_project.h
    > `project   gen_project;`
