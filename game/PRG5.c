#include "PRG5.h"
#include "main.h"
#include "PRG4.h"
#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "A53/bank_helpers.h"
//#include "MMC1/bank_helpers.c"
#include "../include/stdlib.h"

#pragma rodata-name ("BANK1")
#pragma code-name ("BANK1")


// Read-Only map data lives in a seperate bank, so the inital copy of the bg layer
// and the processing of dynamic objects needs to be done in that bank to avoid 
// filling up RAM with copies of data.
void copy_and_process_map_data()
{
	static unsigned char is_clone;

	memfill(objs, 0, sizeof(game_object) * MAX_WORLD_OBJECTS);

	// Copy over the read only version of the room into working memory.
	banked_call(BANK_4, copy_bg_to_current_room);
	//memcpy(current_room, &rooms[cur_room_index][ROOM_BG], 240);

	players[0] = NULL;
	players[1] = NULL;

	enemies_remaining = 0;

	// counter for world object initialization.
	i = 0;

	// since gravity affects goes downwards, we initialize the objects backwards,
	// so that the bottom objects get updated first, allowing things to fall in an
	// expected way.
	// note: x is unsigned, so this loop detects rollover to 255.
	
	// x is used as a running index by get_obj_id.
	x = 0;
	do
	{
		// auto increments.
		banked_call(BANK_4, get_obj_id);
		switch (loaded_obj_id)
		{
			case TILE_SPAWN_POINT_WITCH:
			{
				is_clone = 0;
				// support clones.
				if (players[0] == NULL)
				{
					cur_player = &player1;
					players[0] = &player1;
				}
				else
				{
					cur_player = &player2;
					players[1] = &player2;
					is_clone = 1;
				}
				memfill(cur_player, 0, sizeof(game_object));

				cur_player->ticks_since_move = 0;
				cur_player->start_x_grid = loaded_obj_index % CELL_SIZE;
				cur_player->start_y_grid = loaded_obj_index / CELL_SIZE;
				cur_player->dir_x = 0;
				cur_player->dir_y = 0;
				cur_player->alive = 1;

				// Regardless of whether this is the real witch or the clone, they will be considered
				// active.
				cur_player->type_specific_data.player_data.is_cur_player = 1;
				cur_player->type_specific_data.player_data.is_wiz = 0;
				cur_player->type_specific_data.player_data.is_clone = is_clone;
				cur_player->type_specific_data.player_data.is_mirror = 0;

				// nothign queued.
				cur_player->type_specific_data.player_data.anim.anim_queued = 0xff;			

				cur_player->type_specific_data.player_data.facing_left = cur_player->start_x_grid < 8 ? 0 : 1;	

				break;
			}
			case TILE_SPAWN_POINT_WIZ:
			{
				// wiz is always player 2.
				cur_player = &player2;
				players[1] = &player2;
				memfill(cur_player, 0, sizeof(game_object));

				cur_player->ticks_since_move = 0;
				cur_player->start_x_grid = loaded_obj_index % CELL_SIZE;
				cur_player->start_y_grid = loaded_obj_index / CELL_SIZE;
				cur_player->dir_x = 0;
				cur_player->dir_y = 0;
				cur_player->alive = 1;

				// nothign queued.
				cur_player->type_specific_data.player_data.anim.anim_queued = 0xff;		
				cur_player->type_specific_data.player_data.is_clone = 0;
				cur_player->type_specific_data.player_data.is_wiz = 1;	
				cur_player->type_specific_data.player_data.is_mirror = 0;

				cur_player->type_specific_data.player_data.facing_left = cur_player->start_x_grid < 8 ? 0 : 1;	

				break;
			}
			case TILE_MIRROR:
			{
				// When mirror_fade is not -1, we know that this is the mirror level.
				mirror_fade = 0;

				cur_player = &player2;
				players[1] = &player2;

				memfill(cur_player, 0, sizeof(game_object));

				cur_player->ticks_since_move = 0;
				cur_player->start_x_grid = loaded_obj_index % CELL_SIZE;
				cur_player->start_y_grid = loaded_obj_index / CELL_SIZE;
				cur_player->dir_x = 0;
				cur_player->dir_y = 0;
				cur_player->alive = 1;

				// Regardless of whether this is the real witch or the clone, they will be considered
				// active.
				cur_player->type_specific_data.player_data.is_cur_player = 1;
				cur_player->type_specific_data.player_data.is_wiz = 0;
				cur_player->type_specific_data.player_data.is_clone = 0;
				cur_player->type_specific_data.player_data.is_mirror = 1;

				cur_player->type_specific_data.player_data.facing_left = 1;

				// nothign queued.
				cur_player->type_specific_data.player_data.anim.anim_queued = 0xff;	

				++enemies_remaining;

				break;
			}
			case TILE_KEY:
			{
				current_room[loaded_obj_index] = loaded_obj_id;
				++i;
				++enemies_remaining;
				break;
			}
			case TILE_PUSH_BLOCK:
			{
				current_room[loaded_obj_index] = loaded_obj_id;
				++i;
				break;
			}

			case TILE_AI_HEAVY:
			{
				current_room[loaded_obj_index] = TILE_AI_HEAVY;
				++enemies_remaining;
				++i;
				break;
			}

			case TILE_AI_FLOAT:
			{
				current_room[loaded_obj_index] = TILE_AI_FLOAT;
				++enemies_remaining;
				++i;
				break;
			}

			case TILE_WEB:
			{
				current_room[loaded_obj_index] = TILE_WEB;
				++i;
				break;
			}

            case TILE_DOOR:
            {
				current_room[loaded_obj_index] = TILE_DOOR;
				++enemies_remaining;
				++i;
				break;
            }

			default:
				break;
		}
	} while(loaded_obj_id != 0xff);
}


void build_room_delta()
{
PROFILE_POKE(PROF_B);

	// Do the name table work before incrementing the history index, because this part of
	// this data is actually about the state of the world PRIOR to this change.
	//

    // fill with "unused" value to start.
    memfill(serialized_frames[cur_history_index].nametable_value, 0xff, MAX_NAMETABLE_SAVE_QUEUE);
//    memfill(serialized_frames[cur_history_index].nametable_index, 0xff, MAX_NAMETABLE_SAVE_QUEUE);

    if (cur_nametable_save_queue >= MAX_NAMETABLE_SAVE_QUEUE)
    {
        assert();
    }

    for(i = 0; i < cur_nametable_save_queue; ++i)
    {
        // -1 because we always ignore the latest history when rewinding, so this data is actually about the
        // state of the world PRIOR to this change.
        serialized_frames[cur_history_index].nametable_value[i] = nametable_value_save_queue[i];
        serialized_frames[cur_history_index].nametable_index[i] = nametable_index_save_queue[i];
    }

	cur_history_index = (cur_history_index + 1) % MAX_HISTORY;
	cur_history_count = MIN((cur_history_count + 1), MAX_HISTORY);

	serialized_frames[cur_history_index].enemies_remaining = enemies_remaining;

	// new system:

	for (i = 0; i < 2; ++i)
	{
		if (players[i] != NULL)
		{
			serialized_frames[cur_history_index].player_data[i].xy = (players[i]->start_x_grid << 4) | players[i]->start_y_grid;
			serialized_frames[cur_history_index].player_data[i].flags = 0;
			serialized_frames[cur_history_index].player_data[i].flags = players[i]->type_specific_data.player_data.facing_left;
			serialized_frames[cur_history_index].player_data[i].flags |= (players[i]->type_specific_data.player_data.is_cur_player << 1);
			serialized_frames[cur_history_index].player_data[i].flags |= (players[i]->type_specific_data.player_data.is_clone << 2);
			serialized_frames[cur_history_index].player_data[i].flags |= (players[i]->type_specific_data.player_data.is_wiz << 3);
			serialized_frames[cur_history_index].player_data[i].flags |= (players[i]->alive << 4);
			serialized_frames[cur_history_index].player_data[i].flags |= (players[i]->type_specific_data.player_data.is_mirror << 5);
		}
	}

	//memcpy(serialized_frames[cur_history_index].nametable_data, current_room, 240);
    // counter = 0;
	// for(i=0; i < 240; ++i)
	// {
	// 	if (current_room[i] != rooms[cur_room_index][ROOM_BG][i])
	// 	{
	// 		serialized_frames[cur_history_index].nametable_data[counter] = i;
    //         ++counter;
	// 		serialized_frames[cur_history_index].nametable_data[counter] = current_room[i];
    //         ++counter;
	// 	}
	// }

//     // fill with "unused" value to start.
//     memfill(serialized_frames[cur_history_index-1].nametable_value, 0xff, MAX_NAMETABLE_SAVE_QUEUE);
// //    memfill(serialized_frames[cur_history_index].nametable_index, 0xff, MAX_NAMETABLE_SAVE_QUEUE);

//     if (cur_nametable_save_queue >= MAX_NAMETABLE_SAVE_QUEUE)
//     {
//         assert();
//     }

//     for(i = 0; i < cur_nametable_save_queue; ++i)
//     {
//         // -1 because we always ignore the latest history when rewinding, so this data is actually about the
//         // state of the world PRIOR to this change.
//         serialized_frames[cur_history_index-1].nametable_value[i] = nametable_value_save_queue[i];
//         serialized_frames[cur_history_index-1].nametable_index[i] = nametable_index_save_queue[i];
//     }

	cur_nametable_save_queue = 0;
	memfill(nametable_value_save_queue, 0, MAX_NAMETABLE_SAVE_QUEUE);
    memfill(nametable_index_save_queue, 0, MAX_NAMETABLE_SAVE_QUEUE);
PROFILE_POKE(PROF_CLEAR);	
}