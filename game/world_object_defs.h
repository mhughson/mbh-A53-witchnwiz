static const world_object_def object_def_table[OBJ_DEF_MAX] =
{
 
    // PUSH BLOCK

    { 
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        18,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_PUSH_BLOCK,
    },

    // AI HEAVY

    { 
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        19,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_AI_HEAVY,
    },

    // AI FLOAT

    { 
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        22,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_AI_FLOAT,
    },

    // WEB

    { 
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        23,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_WEB,
    },

    // DOOR

    { 
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        29,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_DOOR,
    },

    // KEY

    {
        // Meta tile index used to draw. Any animation will be done via bank swapped.
        //unsigned char metatile;
        39,

        // The TILE_... used to represent this object in maps.
        //unsigned char map_tile_id;
        TILE_KEY,
    },
};