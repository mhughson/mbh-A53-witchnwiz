#include "PRG4.h"
#include "main.h"
#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "A53/bank_helpers.h"
//#include "MMC1/bank_helpers.c"
#include "../include/stdlib.h"

#pragma rodata-name ("BANK1")
#pragma code-name ("BANK1")
#include "maps.h"

#define NUM_CUSTOM_PROPS 2
void copy_bg_to_current_room()
{
	// NUM_CUSTOM_PROPS because the level data starts after the custom props
	memcpy(current_room, &rooms[cur_room_index][NUM_CUSTOM_PROPS], 240);
}

// tile_index_param
void copy_original_room_to_current()
{
	current_room[tile_index_param] = rooms[cur_room_index][tile_index_param+NUM_CUSTOM_PROPS];
}

void get_obj_id()
{
	loaded_obj_index = rooms[cur_room_index][240 + x + NUM_CUSTOM_PROPS];
	++x;
	loaded_obj_id = rooms[cur_room_index][240 + x + NUM_CUSTOM_PROPS];
	++x;
}

void get_cur_room_palettes()
{
	index = rooms[cur_room_index][0];
	index2 = rooms[cur_room_index][1];
}