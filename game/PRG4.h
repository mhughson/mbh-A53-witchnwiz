#ifndef A680BA7E_492E_48D4_AF35_35C06832E86E
#define A680BA7E_492E_48D4_AF35_35C06832E86E

void copy_bg_to_current_room();

// cur_room_index
// tile_index_param
// OUT: current_room[tile_index_param]
void copy_original_room_to_current();

// cur_room_index
// x
// OUT: loaded_obj_id
void get_obj_id();

//void build_room_delta();

// cur_room_index
// OUT: index  : BG
// OUT: index2 : SPR
void get_cur_room_palettes();


#endif /* A680BA7E_492E_48D4_AF35_35C06832E86E */
