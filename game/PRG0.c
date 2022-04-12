#include "PRG0.h"
#include "main.h"
#include "PRG1.h"
#include "PRG4.h"
#include "PRG5.h"
#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "A53/bank_helpers.h"
#include "../include/stdlib.h"

#pragma rodata-name ("BANK0")
#pragma code-name ("BANK0")

//#include "meta_sprites.h"
//#include "meta_tiles.h"

const unsigned char shake_offsets_x[] = { 0, 1, 2};
const unsigned char shake_offsets_y[] = { 0, 1, 2};

const unsigned char bg_banks[NUM_BG_BANKS] = { 0, 1 };

#include "world_object_defs.h"

// param: in_index_begin
// param: in_index_end
// param: dir_temp
void try_push_object(void);
void save_time_if_best(void);

void update_player()
{
	static signed char prev_dir_x;
	static signed char prev_dir_y;

	// Reached destination last frame.
	if (cur_player->ticks_since_move == (MOVE_FRAMES - 1))
	{
		x = cur_player->start_x_grid;
		y = cur_player->start_y_grid;
		index2 = GRID_XY_TO_ROOM_INDEX(x, y);

		cur_player->start_x_grid += cur_player->dir_x;
		cur_player->start_y_grid += cur_player->dir_y;

		prev_dir_x = cur_player->dir_x;
		prev_dir_y = cur_player->dir_y;

		cur_player->dir_x = 0;
		cur_player->dir_y = 0;		

		x = cur_player->start_x_grid;
		y = cur_player->start_y_grid;
		index = GRID_XY_TO_ROOM_INDEX(x, y);

		target_obj = cur_player;

		if (GET_META_TILE_FLAGS(index) & FLAG_WEB)
		{
			// The web is just getting cleared, not moving, so just save
			// the previous value.
			nametable_index_save_queue[cur_nametable_save_queue] = index;
			nametable_value_save_queue[cur_nametable_save_queue] = current_room[index];
			++cur_nametable_save_queue;

			// Update the room data first...
			tile_index_param = index;
			banked_call(BANK_4, copy_original_room_to_current);

			// Then copy the 2x2 block of meta tiles over again.
			// We do 2x2 because copying over attributes for 1 tiles is troublesome.
			in_x_tile = x;
			in_y_tile = y;
			vram_buffer_load_2x2_metatile();//x, y);
			need_save = 1;
		}
		else if (enemies_remaining == 1 && (current_room[index] == TILE_DOOR_OPEN || current_room[index] == TILE_DOOR))
		{
			SFX_PLAY_WRAPPER(SOUND_KILL_ENEMY);
			
			cur_player->alive = 0;
			queued_stars_for_player_index = player_index;

			obj_temp = players[(player_index + 1) % 2];

			if (obj_temp != NULL && obj_temp->alive == 1)
			{
				queue_player_swap = 1;
			}
			else 
			{
				// only trigger the end if this is the last thing
				if (enemies_remaining == 1)
				{
					--enemies_remaining;
					// TODO: Is save really needed here?
					need_save = 1;
				}
			}
		}
		else if (current_room[index] == TILE_AI_FLOAT || current_room[index] == TILE_AI_HEAVY)
		{
			shake_remaining = 5;

			SFX_PLAY_WRAPPER(SOUND_KILL_ENEMY);

			// // too expensive to do on the same frame that the enemy is cleared.
			queued_stars_for_player_index = player_index;

			i = find_next_free_fx();
			if (i != 0xff)
			{
				fx_x[i] = (cur_player->start_x_grid << 4) << 8;
				fx_y[i] = (cur_player->start_y_grid << 4) << 8;
				fx_dir_x[i] = cur_player->type_specific_data.player_data.facing_left ? (-1 << 8) : (1 << 8); // random number from 0-2 using high bits, with additional random in the low bits.
				fx_dir_y[i] = -(0x200); // same as above but force upwards.
				fx_anim[i].anim_queued = 0xff;
				fx_spawn_stars_on_death[i] = 1;
				global_working_anim = &fx_anim[i];
				if (current_room[index] == TILE_AI_FLOAT)
				{
					queue_next_anim(ANIM_AI_FLOAT_IDLE);
				}
				else
				{
					queue_next_anim(ANIM_AI_HEAVY_IDLE);
				}					
			}

			// The enemy is getting killed, not moving, so just save the index
			// and value so that it can be restored on rewind.
			nametable_index_save_queue[cur_nametable_save_queue] = index;
			nametable_value_save_queue[cur_nametable_save_queue] = current_room[index];
			++cur_nametable_save_queue;

			// Update the room data first...
			tile_index_param = index;
			banked_call(BANK_4, copy_original_room_to_current);

			// Then copy the 2x2 block of meta tiles over again.
			// We do 2x2 because copying over attributes for 1 tiles is troublesome.
			in_x_tile = x;
			in_y_tile = y;
			vram_buffer_load_2x2_metatile();//x, y);

			--enemies_remaining;
			need_save = 1;
		}
		else if (prev_dir_x != 0 && current_room[index] == TILE_LADDER)
		{
			SFX_PLAY_WRAPPER(SOUND_GRAB_LADDER);
		}
		else if (prev_dir_y > 0 && current_room[index] != TILE_LADDER && is_obj_below()) // falling onto something. Check last since it is expensive.
		{
			SFX_PLAY_WRAPPER(SOUND_PLAYER_LAND);
		}
	}

	// Reached destination... try reading input...
	if (cur_player->ticks_since_move >= (MOVE_FRAMES - 1))
	{
		target_obj = cur_player;

		x = cur_player->start_x_grid;
		y = cur_player->start_y_grid;
		index = GRID_XY_TO_ROOM_INDEX(x, y);
		
		if (((GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0) && !is_obj_below())
		{
			cur_player->dir_y = 1;
			cur_player->ticks_since_move = 0;
			global_working_anim = &cur_player->type_specific_data.player_data.anim;
			queue_next_anim(cur_player->type_specific_data.player_data.facing_left ? ANIM_FALL_LEFT : ANIM_FALL_RIGHT);
			need_save = 1;

			grav_search_queue_x[player_index] = cur_player->start_x_grid;
			grav_search_queue_y[player_index] = cur_player->start_y_grid;

			// In the case that they are going to continue to fall, skip all the input stuff.
			goto movement_end;
		}

		dir_temp = 0;

		// NOTE: This is protecting us from being able to walk on air now that falling check favors horizontal movement.
		if (block_movement == 0 && cur_player->type_specific_data.player_data.is_cur_player == 1)
		{
			if (pad1 & PAD_RIGHT)
			{
				dir_temp = 1;
			}
			else if (pad1 & PAD_LEFT)
			{
				dir_temp = -1;
			}

			if (cur_player->type_specific_data.player_data.is_mirror)
			{
				dir_temp *= -1;
			}

			if (dir_temp != 0)
			{
				x = cur_player->start_x_grid + dir_temp;
				y = cur_player->start_y_grid;
				index2 = (y << 4) + x;
				index = index2 - dir_temp; // hack: setting index first was resulting in weird bug, and too lazy to debug.
				index3 = index2 + dir_temp;

				// This will all the player to rotate in place when holding something.
				// I don't like how it feels though, so my thinking right now is just that I will design
				// the puzzles to take into acount that the player can't rotate without moving.
				//
				
				// if (cur_player->type_specific_data.player_data.facing_left != (dir_temp == 1 ? 0 : 1))
				// {
				// 	if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0)
				// 	{
				// 		if (GET_META_TILE_FLAGS(index - 16) & FLAG_CAN_CARRY)			
				// 		{		
				// 			//cur_player->ticks_since_move = (MOVE_FRAMES - 1);
				// 			cur_player->dir_x = 0;
				// 			cur_player->type_specific_data.player_data.facing_left = dir_temp == 1 ? 0 : 1;

				// 			cur_player->ticks_since_move = 0;						
				// 			global_working_anim = &cur_player->type_specific_data.player_data.anim;
				// 			queue_next_anim(dir_temp == 1 ? ANIM_WALK_RIGHT : ANIM_WALK_LEFT);					
				// 			// TODO: Cancel save.
				// 			goto movement_end;
				// 		}
				// 	}
				// }

				// change facing direction regardless of whether the move is blocked or not.
				cur_player->type_specific_data.player_data.facing_left = dir_temp == 1 ? 0 : 1;

				// are we pushing into another player
				if (is_other_player_in_dir(dir_temp, 0))
				{
					// Shake the camera as the player pushes into the reflection.
					if (mirror_fade != 0xff)
					{
						if (mirror_fade % 15 == 0)
						{
							SFX_PLAY_WRAPPER(SOUND_RAMP_1 + MIN(3, (mirror_fade / 15)));
						}
						shake_remaining = 5;
						if (mirror_fade < 254) // 255 means no mirror.
						{
							++mirror_fade;
						}
						// we want to prevent the screen from being mid shake when coming back
						// from the white fade (it looks weird if it pops).
						// For some reason stopping the shake on the frame on or right before
						// the fade completes doesn't work, so instead I stap it at 55 frames.
						// Note: I can't just clear the shake in update because this transition
						// relies on sprites not getting cleared, but they also don't get updated
						// so you end up with the background being reset but the sprites are not.
						if (mirror_fade > 55)
						{
							shake_remaining = 0;
							cur_cam_y = cur_cam_x = 0;
						}
					}
					cur_player->ticks_since_move = (MOVE_FRAMES - 1);
					cur_player->dir_x = 0;
					// TODO: Cancel save.
					goto movement_end;
				}
				

				// Not solid
				if ((GET_META_TILE_FLAGS(index2) & FLAG_SOLID) == 0)
				{
					cur_player->dir_x = dir_temp;
					cur_player->ticks_since_move = 0;						
					global_working_anim = &cur_player->type_specific_data.player_data.anim;

					if ((GET_META_TILE_FLAGS(index - 16) & (FLAG_CAN_CARRY)) != 0)
					{
						queue_next_anim(dir_temp == 1 ? ANIM_WALK_CARRY_RIGHT : ANIM_WALK_CARRY_LEFT);
					}
					else
					{
						queue_next_anim(dir_temp == 1 ? ANIM_WALK_RIGHT : ANIM_WALK_LEFT);
					}

					// TODO: This will trigger a save is pressed up to a player.
					need_save = 1;

					if (GET_META_TILE_FLAGS(index2) & FLAG_WEB)
					{
						SFX_PLAY_WRAPPER(SOUND_DESTROY_WEB);
					}
					else
					{
						SFX_PLAY_WRAPPER(SOUND_WALK);
					}

					// Don't do a search if we were standing on a ladder.
					// TODO: Check for blocks_push.
					if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0)
					{
						grav_search_queue_x[player_index] = cur_player->start_x_grid;
						grav_search_queue_y[player_index] = cur_player->start_y_grid;

						if ((GET_META_TILE_FLAGS(index2) & FLAG_LADDER) == 0)
						{
							in_index_begin = index - 16;
							if (GET_META_TILE_FLAGS(in_index_begin) & FLAG_CAN_CARRY)
							{
								in_index_end = in_index_begin + dir_temp;
								in_lerp_type = LERP_LINEAR;
								try_push_object();
							}
						}
					}
				}
				if (GET_META_TILE_FLAGS(index2) & FLAG_LADDER)
				{
					SFX_PLAY_WRAPPER(SOUND_JUMP_AT);

					global_working_anim = &cur_player->type_specific_data.player_data.anim;
					queue_next_anim(dir_temp == 1 ? ANIM_JUMP_RIGHT : ANIM_JUMP_LEFT);
					need_save = 1;
				}
				else if (GET_META_TILE_FLAGS(index2) & FLAG_MONSTER)
				{
					SFX_PLAY_WRAPPER(SOUND_JUMP_AT);

					global_working_anim = &cur_player->type_specific_data.player_data.anim;
					queue_next_anim(dir_temp == 1 ? ANIM_ATTACK_RIGHT : ANIM_ATTACK_LEFT);
					need_save = 1;
				}
				else if (GET_META_TILE_FLAGS(index2) & FLAG_PUSH_BLOCK)
				{
					// TODO: Replace with try_push_object(). Needs to handle different lerp function, and checking for player.
					//		 Also, updating the player animation at the end.

					// make sure the block has somewhere to go.
					if ((GET_META_TILE_FLAGS(index3) & FLAGS_BLOCK_PUSH) == 0 && !is_other_player_in_dir(dir_temp << 1, 0))
					{
						// Spawn a temp object to animate the block movement.
						for (i = 0; i < MAX_WORLD_OBJECTS; ++i)
						{
							obj_temp = &objs[i];

							if (!obj_temp->alive)
							{
								// Create the temp animation.
								obj_temp->alive = 1;
								obj_temp->ticks_since_move = 0;
								obj_temp->start_x_grid = x;
								obj_temp->start_y_grid = y;
								obj_temp->dir_x = dir_temp;
								obj_temp->dir_y = 0;
								obj_temp->type_specific_data.obj_data.lerp_type_x = LERP_EASE_OUT_QUINT;

								if (current_room[index2] == TILE_PUSH_BLOCK)
								{
									obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_BLOCK;
								}
								else
								{
									obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_KEY;
								}

								// Store the current position, and the destination as the looked prior to moving.
								nametable_index_save_queue[cur_nametable_save_queue] = index2;
								nametable_value_save_queue[cur_nametable_save_queue] = current_room[index2];
								++cur_nametable_save_queue;
								nametable_index_save_queue[cur_nametable_save_queue] = index3;
								nametable_value_save_queue[cur_nametable_save_queue] = current_room[index3];
								++cur_nametable_save_queue;

								// Update the room data.
								tile_index_param = index2;
								banked_call(BANK_4, copy_original_room_to_current);

								// Then copy the 2x2 block of meta tiles over again.
								// We do 2x2 because copying over attributes for 1 tiles is troublesome.
								in_x_tile = x;
								in_y_tile = y;
								vram_buffer_load_2x2_metatile();

								cur_player->ticks_since_move = 0;
								global_working_anim = &cur_player->type_specific_data.player_data.anim;
								
								SFX_PLAY_WRAPPER(SOUND_PUSH_BLOCK);

								if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER))
								{
									queue_next_anim(dir_temp == 1 ? ANIM_LADDER_PUSH_RIGHT : ANIM_LADDER_PUSH_LEFT);
								}
								else
								{
									queue_next_anim(dir_temp == 1 ? ANIM_PUSH_RIGHT : ANIM_PUSH_LEFT);
								}
								
								need_save = 1;

								break; // found one, no need to continue.
							}
						}
					}
					else
					{
						// reset the timer, so that the player can immediately move in the other directions.
						cur_player->ticks_since_move = (MOVE_FRAMES - 1);
					}
				}
			}
			else if (pad1 & PAD_UP)
			{
				// Check if we are currently overlapping a ladder. The player
				// cannot jump UP to a ladder above them.
				x = cur_player->start_x_grid;
				y = cur_player->start_y_grid;
				index = (y << 4) + x;
				index2 = index - 16;


				// are we pushing into another player
				if (is_other_player_in_dir(0, -1))
				{
					goto movement_end;
				}	

				// On a ladder and the tile above is clear, and the is not the top row of the map.
				if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER) && ((GET_META_TILE_FLAGS(index2) & FLAG_LADDER) || ((GET_META_TILE_FLAGS(index2) & FLAGS_BLOCK_CLIMB) == 0)) && index >= 16)
				{
					SFX_PLAY_WRAPPER(SOUND_LADDER_UP);

					cur_player->dir_y = -1;
					cur_player->ticks_since_move = 0;
					need_save = 1;
					global_working_anim = &cur_player->type_specific_data.player_data.anim;
					queue_next_anim(ANIM_CLIMB_UP);
				}
			}
			else if (pad1 & PAD_DOWN)
			{
				// Can climb down ladders when overlapping ladder tile, but also
				// when above ladder tile.
				x = cur_player->start_x_grid;
				y = cur_player->start_y_grid;
				index = (y << 4) + x;
				index2 = index + 16;
				// Standing above a ladder.
				// Standing on a ladder with empty space below.
				// Standing on a ladder with more ladder below.
				//		Standing on a ladder with not solid below.
				// Not at the bottom of the map.

				// are we pushing into another player
				if (is_other_player_in_dir(dir_temp, 1))
				{
					goto movement_end;
				}

				if (index <= 0xff-16)
				{
					if ((GET_META_TILE_FLAGS(index2) & FLAG_LADDER) || ((GET_META_TILE_FLAGS(index) & FLAG_LADDER) && !(GET_META_TILE_FLAGS(index2) & FLAGS_BLOCK_CLIMB)))
					{
						SFX_PLAY_WRAPPER(SOUND_LADDER_DOWN);

						// Don't search if we are standing on a ladder.
						// TODO: Check for 
						if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0)
						{
							grav_search_queue_x[player_index] = cur_player->start_x_grid;
							grav_search_queue_y[player_index] = cur_player->start_y_grid;
						}
						cur_player->dir_y = 1;
						cur_player->ticks_since_move = 0;
						need_save = 1;
						global_working_anim = &cur_player->type_specific_data.player_data.anim;
						queue_next_anim(ANIM_CLIMB_DOWN);
					}
				}
			}
		}

		// Allow character swapping even if input is blocked, so that things feel just that little bit
		// more responsive.
		if (pad1_new & PAD_A && cur_player->type_specific_data.player_data.is_cur_player == 1)
		{

			x = cur_player->start_x_grid;
			y = cur_player->start_y_grid;
			index = (y << 4) + x;
			index2 = index + 16;
			index3 = index - 16; // above.

			if (block_movement == 0 && (GET_META_TILE_FLAGS(index2) & FLAG_CAN_CARRY) && (GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0)
			{
				nametable_index_save_queue[cur_nametable_save_queue] = index;
				nametable_value_save_queue[cur_nametable_save_queue] = current_room[index];
				++cur_nametable_save_queue;
				nametable_index_save_queue[cur_nametable_save_queue] = index2;
				nametable_value_save_queue[cur_nametable_save_queue] = current_room[index2];
				++cur_nametable_save_queue;

				//index3 = current_room[index];
				//current_room[index] = current_room[index2];
				// current_room[index] = current_room[index2];

				// in_x_tile = x;
				// in_y_tile = y;
				// vram_buffer_load_2x2_metatile();

				// Update the room data.
				tile_index_param = index2;
				banked_call(BANK_4, copy_original_room_to_current);
				in_x_tile = x;
				in_y_tile = y + 1;
				vram_buffer_load_2x2_metatile();

				//++cur_player->start_y_grid;

				cur_player->dir_y = 1;
				cur_player->ticks_since_move = 0;
				global_working_anim = &cur_player->type_specific_data.player_data.anim;
				queue_next_anim(cur_player->type_specific_data.player_data.facing_left == 0 ? ANIM_PICKUP_RIGHT : ANIM_PICKUP_LEFT);


				// Spawn a temp object to animate the block movement.
				for (i = 0; i < MAX_WORLD_OBJECTS; ++i)
				{
					obj_temp = &objs[i];

					if (!obj_temp->alive)
					{
						// Create the temp animation.
						obj_temp->alive = 1;
						obj_temp->ticks_since_move = 0;
						obj_temp->start_x_grid = x;
						obj_temp->start_y_grid = y + 1;
						obj_temp->dir_x = 0;
						obj_temp->dir_y = -1;
						obj_temp->type_specific_data.obj_data.lerp_type_x = LERP_EASE_OUT_QUINT;

						obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_KEY;

						break;
					}
				}

				need_save = 1;
			}
			else if (block_movement == 0 && (GET_META_TILE_FLAGS(index3) & FLAG_CAN_CARRY) && (GET_META_TILE_FLAGS(index) & FLAG_LADDER) == 0) // are we holding something?
			{
				// TODO: Can this be in try_push_object? This will break if the key is blocked
				grav_search_queue_x[player_index] = x;
				grav_search_queue_y[player_index] = y - 1;

				in_index_begin = index3;
				dir_temp = cur_player->type_specific_data.player_data.facing_left ? -1 : 1;
				in_index_end = in_index_begin + dir_temp;
				in_lerp_type = LERP_EASE_OUT_QUINT;
				try_push_object();
			}
			else
			{
				// TODO: Wish this was where queue_player_swap is checked, but since
				// 		 we don't know who requested it, and sometimes we force the change
				//		 when one player enters a door, its easier to do here.
				obj_temp = players[(player_index + 1) % 2];
				if (obj_temp != NULL && obj_temp->alive)
				{
					queue_player_swap = 1;
				}
			}
		}
	}

	if (mirror_fade != 0xff && mirror_fade > 0)
	{
		--mirror_fade;
	}

movement_end:

	// Still at destination... play idle anim.
	if (cur_player->ticks_since_move >= (MOVE_FRAMES - 1))
	{
		x = cur_player->start_x_grid;
		y = cur_player->start_y_grid;
		index = (y << 4) + x;

		index2 = index - 16;

		if (GET_META_TILE_FLAGS(index) & FLAG_LADDER)
		{
			global_working_anim = &cur_player->type_specific_data.player_data.anim;
			queue_next_anim(ANIM_IDLE_CLIMB);
		}
		else
		{
			if (cur_player->type_specific_data.player_data.facing_left)
			{
				global_working_anim = &cur_player->type_specific_data.player_data.anim;
				
				if ((GET_META_TILE_FLAGS(index2) & (FLAG_CAN_CARRY)) != 0)
				{
					queue_next_anim(ANIM_IDLE_CARRY_LEFT);
				}
				else
				{
					queue_next_anim(ANIM_IDLE_LEFT);
				}

			}
			else
			{
				global_working_anim = &cur_player->type_specific_data.player_data.anim;

				if ((GET_META_TILE_FLAGS(index2) & (FLAG_CAN_CARRY)) != 0)
				{
					queue_next_anim(ANIM_IDLE_CARRY_RIGHT);
				}
				else
				{
					queue_next_anim(ANIM_IDLE_RIGHT);
				}
			}
		}
	}
	else
	{
		++moving_object_count;
	}

	// TODO: rather than doing all these array lookups every frame, it should
	// 		 probably just store it all at the moment the animation changes.
	global_working_anim = &cur_player->type_specific_data.player_data.anim;
	commit_next_anim();
}

// param obj_temp
void update_dynamic_game_object()
{
	static unsigned char prev_dir_y;

	// TODO: Dust.
	// if (obj_temp->dir_x != 0 && (ticks8 % 4) == 0)
	// {
	// 	x16 = (obj_temp->start_x_grid * CELL_SIZE);
	// 	y16 = (obj_temp->start_y_grid * CELL_SIZE);
	
	// 	if (obj_temp->dir_x > 0)
	// 	{
	// 		x16 += ease_easeoutquint_right[obj_temp->ticks_since_move];
	// 	}
	// 	else if (obj_temp->dir_x < 0)
	// 	{
	// 		x16 += ease_easeoutquint_left[obj_temp->ticks_since_move];
	// 	}
	// }

	// Finished moving?
	if (obj_temp->ticks_since_move == (MOVE_FRAMES - 1))
	{
		// Don't do a gravity search if this object moved up.
		if (obj_temp->dir_y >= 0)
		{
			grav_search_queue_x[2] = obj_temp->start_x_grid;
			grav_search_queue_y[2] = obj_temp->start_y_grid;
		}

		// First move the postition for falling calculations.
		obj_temp->start_x_grid += obj_temp->dir_x;
		obj_temp->start_y_grid += obj_temp->dir_y;

		prev_dir_y = obj_temp->dir_y;

		obj_temp->dir_x = 0;
		obj_temp->dir_y = 0;

		// NOTE: ATM floating objects can never move, so a floating check is not needed.
		target_obj = obj_temp;

		if (!is_obj_below())
		{
			// standing over gap. start falling!

			obj_temp->dir_y = 1;
			obj_temp->ticks_since_move = 0;
			need_save = 1;

			tile_index_param = GRID_XY_TO_ROOM_INDEX(obj_temp->start_x_grid, obj_temp->start_y_grid);

			// This object is going to fall down so, save the previous value
			// of the destination. We don't need to save the value at it's current
			// position because, since this is a dynamic object, we know the original
			// position data would have been queued up already.
			nametable_index_save_queue[cur_nametable_save_queue] = tile_index_param + 16;
			nametable_value_save_queue[cur_nametable_save_queue] = current_room[tile_index_param + 16];
			++cur_nametable_save_queue;

			// Copy over the original background layer value.
			banked_call(BANK_4, copy_original_room_to_current);
			in_x_tile = obj_temp->start_x_grid;
			in_y_tile = obj_temp->start_y_grid;
			vram_buffer_load_2x2_metatile();
		}
		else
		{
			// Switch over to nametable.
			obj_temp->alive = 0;

			if (prev_dir_y > 0)
			{
				switch (object_def_table[obj_temp->type_specific_data.obj_data.object_def].map_tile_id)
				{
					case TILE_PUSH_BLOCK:
					{
						SFX_PLAY_WRAPPER(SOUND_BLOCK_LAND);
						break;
					}

					default:
					{
						// TODO: Sharing with player for now.
						SFX_PLAY_WRAPPER(SOUND_PLAYER_LAND);
					}

				}
			}

			index = GRID_XY_TO_ROOM_INDEX(obj_temp->start_x_grid, obj_temp->start_y_grid);

			if (object_def_table[obj_temp->type_specific_data.obj_data.object_def].map_tile_id == TILE_KEY 
				&& current_room[index] == TILE_DOOR)
			{
				--enemies_remaining;

				in_x_tile = obj_temp->start_x_grid;
				in_y_tile = obj_temp->start_y_grid;
				spawn_stars_at_tile();

				// Should the door open now?
				if (enemies_remaining == 1)
				{
					SFX_PLAY_WRAPPER(SOUND_KILL_ENEMY);
					// Update the room data, and then copy over a 2x2 chunk of
					// metatiles.
					current_room[index] = TILE_DOOR_OPEN;
					in_x_tile = obj_temp->start_x_grid;
					in_y_tile = obj_temp->start_y_grid;
					vram_buffer_load_2x2_metatile();//obj_temp->start_x_grid, obj_temp->start_y_grid);
				}
			}
			else
			{
				// Update the room data, and then copy over a 2x2 chunk of
				// metatiles.
				current_room[index] = object_def_table[obj_temp->type_specific_data.obj_data.object_def].map_tile_id;
				in_x_tile = obj_temp->start_x_grid;
				in_y_tile = obj_temp->start_y_grid;
				vram_buffer_load_2x2_metatile();//obj_temp->start_x_grid, obj_temp->start_y_grid);
			}
			
			// DONT SAVE DYNAMIC OBJECTS! ONLY WHEN STUFF IS LEAVING NAMETABLE.
			// nametable_save_queue[cur_nametable_save_queue] = index;
			// ++cur_nametable_save_queue;
		}
	}
}

// param obj_temp
void update_non_player_object()
{
	//obj_temp->ticks_since_move = MIN(obj_temp->ticks_since_move + 1, MOVE_FRAMES);
	if (obj_temp->ticks_since_move < MOVE_FRAMES)
	{
		++obj_temp->ticks_since_move;
	}

	// Note: if this is the last frame of animation, we still consider them moving.
	if (obj_temp->alive)
	{
		update_dynamic_game_object();
	}

	// At the end of the update, if this object is still alive and hasn't finished moving, it is
	// considered moving.
	if (obj_temp->alive && obj_temp->ticks_since_move < (MOVE_FRAMES - 1))
	{
		++moving_object_count;
	}
}

void update_gameplay()
{
	moving_object_count = 0;

	if (pad1_new & PAD_SELECT && cur_state == STATE_GAME)
	{
		// skip level

#if DEBUG_ENABLED
		if (pad1 & PAD_A)
		{
			++cur_room_index;
		}
#endif
		fade_to_black_fast();
		oam_clear();
		load_current_map_force_ppu_off();
		banked_call(BANK_1, load_level_pal);
		fade_from_black_fast();
	}

	for (i = 0; i < 3; ++i)
	{
		// TODO: This actually only needs to search 1 tile up. Additional blocks will be detected
		//		 when this object starts moving.
		if (grav_search_queue_x[i] != 0xff && grav_search_queue_y[i] != 0xff)
		{
			index2 = GRID_XY_TO_ROOM_INDEX(grav_search_queue_x[i], grav_search_queue_y[i]);

			if(grav_search_queue_y[i] > 0)
			{
				index2 -= 16;
				grav_search_queue_y[i] -= 1;

				// Search for falling objects over multiple frames (if finding one).
				if (current_room[index2] == TILE_PUSH_BLOCK || current_room[index2] == TILE_AI_HEAVY || current_room[index2] == TILE_KEY)
				{
					for (i2 = 0; i2 < MAX_WORLD_OBJECTS; ++i2)
					{
						// Is this object at the destination.
						obj_temp = &objs[i2];

						if (!obj_temp->alive)
						{
							// Little bit of a hack for Clone levels. In those levels
							// it is possible for p1 to move out from under a block,
							// trigger the search to fall, but then the clone move right
							// under the block before it starts falling in the same frame.
							// To prevent the block from landing on the the 2nd player, we
							// do a last minute check for players below us.
							target_obj = obj_temp;
							target_obj->start_x_grid = grav_search_queue_x[i];
							target_obj->start_y_grid = grav_search_queue_y[i];
							target_obj->dir_x = 0;
							target_obj->dir_y = 0; // is_obj_below() assumes we are not moving yet.

							// Handle case where the 2nd player moves under the object that is about to fall.
							if (!is_obj_below())
							{
								// Create the temp animation.
								obj_temp->alive = 1;
								obj_temp->ticks_since_move = 0;
								obj_temp->start_x_grid = grav_search_queue_x[i];
								obj_temp->start_y_grid = grav_search_queue_y[i];
								obj_temp->dir_x = 0;
								obj_temp->dir_y = 1;
								obj_temp->type_specific_data.obj_data.lerp_type_x = LERP_EASE_OUT_QUINT;

								if (current_room[index2] == TILE_PUSH_BLOCK)
								{
									obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_BLOCK;
								}
								else if (current_room[index2] == TILE_KEY)
								{
									obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_KEY;
								}
								else
								{
									obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_HEAVY_AI;
								}

								// Save current location and destination as they were BEFORE this
								// object started moving.
								
								nametable_index_save_queue[cur_nametable_save_queue] = index2;
								nametable_value_save_queue[cur_nametable_save_queue] = current_room[index2];
								++cur_nametable_save_queue;
								nametable_index_save_queue[cur_nametable_save_queue] = index2 + 16;
								nametable_value_save_queue[cur_nametable_save_queue] = current_room[index2 + 16];
								++cur_nametable_save_queue;

								// Update the room data first...
								tile_index_param = index2;
								banked_call(BANK_4, copy_original_room_to_current);

								// Then copy the 2x2 block of meta tiles over again.
								// We do 2x2 because copying over attributes for 1 tiles is troublesome.
								in_x_tile = grav_search_queue_x[i];
								in_y_tile = grav_search_queue_y[i];
								vram_buffer_load_2x2_metatile();//x, y);

								need_save = 1;

								// search above.
								grav_search_queue_x[2] = grav_search_queue_x[i];
								grav_search_queue_y[2] = grav_search_queue_y[i];
							}
							goto skip_grav_clear;
						}
					}
				}
			}

			// If we make it to here, there was no object to fall down, so
			// clear the search queue so it doesn't keep checking.
			grav_search_queue_x[i] = grav_search_queue_y[i] = 0xff;				
		}
	}

skip_grav_clear:

	if (cur_state == STATE_LEVEL_COMPLETE && (pad1_new & PAD_A || pad1_new & PAD_B || pad1_new & PAD_START))
	{
//		fade_to_black();
		++cur_room_index;
//		load_current_map_force_ppu_off();
//		oam_clear();
		if (cur_room_index < NUM_ROOMS)
		{
			go_to_state(STATE_GAME);
		}
		else
		{
			cur_room_index = 0;
			go_to_state(STATE_ENDING);
			return;
		}
		
//		fade_from_black();
	}
	else if (cur_state != STATE_LEVEL_COMPLETE && pad1_new & PAD_START)
	{
		SFX_PLAY_WRAPPER(SOUND_UI_OK);
		go_to_state(STATE_PAUSE);
		return;
	}
	// Don't allow rewind until after everything has stopped moving. Without this
	// moving objects will not get restored (since we only save player and nt).
	// NOTE: This is the state from LAST frame, so there may be a single frame
	//		 racecondition where things break.
	else if (pad1_new & PAD_B && !block_movement) 
	{
		// Restore the save point.
		restore_room_delta();
	}

	if (queued_stars_for_player_index != 0xff)
	{
		cur_player = players[queued_stars_for_player_index];
		spawn_stars_at_player();
		queued_stars_for_player_index = 0xff;
	}
PROFILE_POKE(PROF_W);

	//oam_clear();
	oam_set(0);

	if (shake_remaining > 0)
	{
		cur_cam_x = shake_offsets_x[rand() % 3];
		cur_cam_y = shake_offsets_y[rand() % 3];
		--shake_remaining;
	}
	else
	{
		cur_cam_x = 0;
		cur_cam_y = 0;
	}	

	// fade in the level as the player moves right.
	if (mirror_fade != 0xff)
	{
		pal_bg_bright(MIN(player1.start_x_grid,4));
		if (mirror_fade != 0)
		{
			pal_bright(4 + MIN((mirror_fade / 15), 4));
		}
	}

	scroll(cur_cam_x, cur_cam_y);

PROFILE_POKE(PROF_G);

	banked_call(BANK_1, draw_fx_objects);

PROFILE_POKE(PROF_R);

	if (cur_state != STATE_LEVEL_COMPLETE)
	{
		// Update and draw players from bottom to top of screen so that
		// physics behave as expected.
		if (players[1] != NULL)
		{
			if (player2.start_y_grid < player1.start_y_grid)
			{
				player_index = 1;
				cur_player = players[player_index];
				if (cur_player->alive)
				{
					update_player();
					banked_call(BANK_1, draw_player);
				}

				player_index = 0;
				cur_player = players[player_index];
				if (cur_player->alive)
				{
					update_player();
					banked_call(BANK_1, draw_player);
				}
			}
			else
			{
				player_index = 0;
				cur_player = players[player_index];
				if (cur_player->alive)
				{
					update_player();
					banked_call(BANK_1, draw_player);
				}

				player_index = 1;
				cur_player = players[player_index];
				if (cur_player->alive)
				{
					update_player();
					banked_call(BANK_1, draw_player);
				}
			}
			
		}
		else
		{
			player_index = 0;
			cur_player = players[player_index];
			if (cur_player->alive)
			{
				update_player();
				banked_call(BANK_1, draw_player);
			}
		}
	}

	if (queue_player_swap != 0)
	{
		if (players[1] != NULL 
			&& player2.type_specific_data.player_data.is_cur_player != player1.type_specific_data.player_data.is_cur_player)
		{
			player2.type_specific_data.player_data.is_cur_player = !player2.type_specific_data.player_data.is_cur_player;
			player1.type_specific_data.player_data.is_cur_player = !player1.type_specific_data.player_data.is_cur_player;

			if (player1.type_specific_data.player_data.is_cur_player)
			{
				cur_player = &player1;
			}
			else
			{
				cur_player = &player2;
			}
			

			for (i2 = 0; i2 < SMALL_STAR_1; ++i2)
			{
				if (fx_alive[i2] == 0)
				{
					fx_spawn_stars_on_death[i2] = 0;
					fx_dir_x[i2] = 0;
					fx_dir_y[i2] = 0;
					fx_floats[i2] = 1;

					fx_alive[i2] = 1;
					fx_x[i2] = (cur_player->start_x_grid << 4) << 8;
					fx_y[i2] = ((cur_player->start_y_grid << 4) - 20) << 8;
					fx_anim[i2].anim_queued = 0xff;
					global_working_anim = &fx_anim[i2];

					SFX_PLAY_WRAPPER(SOUND_SWITCH_CHAR);
					queue_next_anim(ANIM_CARD_SPIN);

					fx_sprite_offset[i2] = cur_player == &player2 ? 2 : 0;
					break;
				}
			}
		}
		queue_player_swap = 0;
	}

PROFILE_POKE(PROF_B);

	// Updating/drawing the dynamic objects ensures that that get drawn on TOP
	// of the player, and don't flicker during the transition from NT to Sprite.
	// However, this means that picking up objects like the key goes behind the player
	// which looks odd.

	objs_temp_head = NULL;
	objs_temp_prev = NULL;

	// Order game objects from bottom to top in world, to ensure physics behave in 
	// an expected way.
	for (obj_index = 0; obj_index < MAX_WORLD_OBJECTS; ++obj_index)
	{
		obj_temp = &objs[obj_index];

		if (obj_temp && obj_temp->alive)
		{
			if (objs_temp_head == NULL)
			{
				objs_temp_head = obj_temp;
				obj_temp->next = NULL;
			}
			else
			{			
				target_obj = objs_temp_head;
				while (target_obj != NULL)
				{
					if (obj_temp->start_y_grid > target_obj->start_y_grid)
					{
						if (target_obj == objs_temp_head)
						{
							objs_temp_head = obj_temp;
						}
						obj_temp->next = target_obj;

						// Case of inserted into middle of 2 nodes UNTESTED.
						if (objs_temp_prev != NULL)
						{
							objs_temp_prev->next = obj_temp;
						}
						break;
					}
					if (target_obj->next == NULL)
					{
						target_obj->next = obj_temp;
						obj_temp->next = NULL;
						break;
					}

					objs_temp_prev = target_obj;
					target_obj = target_obj->next;			
				}
			}			
		}		
	}

PROFILE_POKE(PROF_G);
	// Update and draw objects in sorted order. No NULL entries.
	obj_index = 0;
	obj_temp = objs_temp_head;
	if (cur_state != STATE_LEVEL_COMPLETE)
	{
		while (obj_temp != NULL)
		{
			update_non_player_object();
			banked_call(BANK_1, draw_non_player_object);
			obj_temp = obj_temp->next;
			++obj_index;
		}
	}

PROFILE_POKE(PROF_W);
	oam_hide_rest();

	// Check if all killable objects have been killed.
	if (cur_state != STATE_LEVEL_COMPLETE && enemies_remaining == 0)
	{
		oam_clear();
		
		for (i = 0; i < 16; ++i)
		{
			current_room[GRID_XY_TO_ROOM_INDEX(i, 6)] = 104;
			in_x_tile = i;
			in_y_tile = 6;
			vram_buffer_load_2x2_metatile();//x, y);
			if (i % 6 == 0)
			{
				delay(1);
				clear_vram_buffer();
			}
		}
		for (i = 0; i < 16; ++i)
		{
			current_room[GRID_XY_TO_ROOM_INDEX(i, 7)] = 21;
			in_x_tile = i;
			in_y_tile = 7;
			vram_buffer_load_2x2_metatile();//x, y);
			if (i % 6 == 0)
			{
				delay(1);
				clear_vram_buffer();
			}
		}
		for (i = 0; i < 16; ++i)
		{
			current_room[GRID_XY_TO_ROOM_INDEX(i, 8)] = 105;
			in_x_tile = i;
			in_y_tile = 8;
			vram_buffer_load_2x2_metatile();//x, y);
			if (i % 6 == 0)
			{
				delay(1);
				clear_vram_buffer();
			}
		}

		for (i = 0; i < 8; ++i)
		{
			current_room[GRID_XY_TO_ROOM_INDEX(4+i, 7)] = 96+i;
			in_x_tile = 4+i;
			in_y_tile = 7;
			vram_buffer_load_2x2_metatile();//x, y);
			if (i % 6 == 0)
			{
				delay(1);
				clear_vram_buffer();
			}
		}

		go_to_state(STATE_LEVEL_COMPLETE);

		save_time_if_best();
	}

	draw_cur_time();

#if DEBUG_ENABLED
	// // Save queue debug data.
	// one_vram_buffer('S', get_ppu_addr(0, 0, 0));
	// one_vram_buffer('0' + need_save, get_ppu_addr(0, 8, 0));
	// one_vram_buffer('I', get_ppu_addr(0, 16, 0));
	// one_vram_buffer('0' + cur_history_index, get_ppu_addr(0, 24, 0));
	// one_vram_buffer('C', get_ppu_addr(0, 32, 0));
	// one_vram_buffer('0' + cur_history_count, get_ppu_addr(0, 40, 0));
	// multi_vram_buffer_horz("AI", 2, get_ppu_addr(0, 64, 0));
	// one_vram_buffer('0' + enemies_remaining, get_ppu_addr(0, 88, 0));

	// multi_vram_buffer_horz("MV", 2, get_ppu_addr(0, 104, 0));
	// one_vram_buffer('0' + moving_object_count, get_ppu_addr(0, 120, 0));
#endif //DEBUG_ENABLED

PROFILE_POKE(PROF_B);

	block_movement = (moving_object_count > 0) 
		|| cur_state == STATE_LEVEL_COMPLETE
		|| (grav_search_queue_x[0] != 0xff) || (grav_search_queue_y[0] != 0xff)
		|| (grav_search_queue_x[1] != 0xff) || (grav_search_queue_y[1] != 0xff)
		|| (grav_search_queue_x[2] != 0xff) || (grav_search_queue_y[2] != 0xff);

	// TODO: Consider moving to start of next frame, since a lot usually happens on the frame that all objects
	// 		 stop moving (eg. name table swaps).
	if (!block_movement && need_save)
	{
		banked_call(BANK_5, build_room_delta);
		need_save = 0;
	}
PROFILE_POKE(PROF_W);

	if (mirror_fade == 62)
	{
		// Stop the heart beat.
		music_stop();

		save_time_if_best();
		if (max_room_completed == -1 || max_room_completed < cur_room_index)
		{
			max_room_completed = cur_room_index;
		}
		++cur_room_index;
		//cur_state = 0xff;
		//go_to_state(STATE_GAME);
		load_current_map_force_ppu_off();
		banked_call(BANK_1, load_level_pal);
		delay(30);
		SFX_PLAY_WRAPPER(SOUND_SHATTER);
		fade_from_white();
		return;
	}
}

void update_2_0()
{
	// swap banks roughly ever second (60 frames vs 64 frames).
	if (cur_state != STATE_ENDING && cur_state != STATE_MENU && cur_state != STATE_MENU_TRANS && (ticks8 & 31) == 0)
	{
		++char_state;
		char_state = char_state & (NUM_BG_BANKS - 1); // %NUM_BG_BANKS assumes power of 2
		set_chr_bank_0(bg_banks[char_state]); // switch the BG bank
	}

	switch (cur_state)
	{
	case STATE_BOOT:
		banked_call(BANK_1, update_boot);
		break;
	case STATE_P8:
		banked_call(BANK_1, update_p8);
		break;
	case STATE_SOUND_TEST:
		// A53 HACK: This function was moved to CODE segment, but for some reaon
		// removing the banked ball has some visual knock ons. I'm guess something in there assumes
		// bank1, but I haven't been able to find it. Anyway, it seems to work like this, and likely
		// will remove before ship of A53 anyway.
		banked_call(BANK_1, update_sound_test);
		break;
	case STATE_MENU:
		banked_call(BANK_1, update_menu);
		break;
	case STATE_MENU_TRANS:
		banked_call(BANK_1, update_menu_trans);
		break;
	case STATE_NEW_GAME_MENU:
		// In reality, this is in bank 1, but bank 1 enum points to 
		// bank 2... bank 4 points to bank 1...
		banked_call(BANK_4, update_new_game_menu);
		break;
	case STATE_PASSWORD:
		banked_call(BANK_1, update_password_entry);
		break;
	case STATE_MENU_LEVEL_SELECT:
		banked_call(BANK_1, update_level_select);
		break;
	case STATE_LEVEL_COMPLETE:
	case STATE_GAME:
		update_gameplay();
		break;
	case STATE_PAUSE:
		banked_call(BANK_1, update_pause);
		break;
	case STATE_ENDING:
		banked_call(BANK_1, update_ending_screen);
		break;
	default:
		break;
	}
}

void main_real(void)
{
	// if (sizeof(game_object) != 16)
	// {
	// 	assert();
	// }

	// if (&save_version_validation != 0x6000)
	// {
	// 	assert();
	// }

	ppu_off(); // screen off

	// SAVE_VERSION is a magic number which means that this is valid save data.
	// Any other value is assumed to be garbage and therefore this is
	// a first time booting.
	// We check multiple entries in an array to reduce the chance of a fluke where the
	// random piece of data happens to match the version number.
	// for (i = 0; i < NUM_SAVE_VERSION_VALIDATION; ++i)
	// {
	// 	if (save_version_validation[i] != SAVE_VERSION)
	// 	{
	// 		// clear all of WRAM.
	// 		memfill(save_version_validation, 0, 0x2000);

	// 		for (i = 0; i < NUM_SAVE_VERSION_VALIDATION; ++i)
	// 		{
	// 			// Store the save version as our magic number to check next time.
	// 			save_version_validation[i] = SAVE_VERSION;
	// 		}

	// 		// Clear all the best times to 0xff so that we can tell the difference between a 0 time, and an 
	// 		// unset time (note: each digit caps at 5 or 9, so this is safe).
	// 		memfill(wram_best_times, 0xff, BEST_TIME_TOTAL_SAVE_SIZE);

	// 		break;
	// 	}
	// }

	// use the second set of tiles for sprites
	// both bg and sprites are set to 0 by default
	bank_spr(1);

	set_vram_buffer(); // do at least once, sets a pointer to a buffer
	clear_vram_buffer();

	// pal_bg(palette_bg);
	// pal_spr(palette_sp);

	//load_current_map();	

	bank_spr(1);

	// Setup the pause menu which lives in the 2nd nametable for the life
	// of the program.
	banked_call(BANK_1, load_pause_menu_into_nametable);

	ppu_on_all(); // turn on screen

	cur_state = 0xff;
	go_to_state(STATE_BOOT);

	shake_remaining = 0;

	// infinite loop
	while (1)
	{
		ppu_wait_nmi(); // wait till beginning of the frame

PROFILE_POKE(PROF_W);

		++ticks8;
		++ticks16;
		++ticks16_in_state;

		pad1 = pad_poll(0) | pad_poll(1); // read the first controller
		pad1_new = get_pad_new(0) | get_pad_new(1); // newly pressed button. do pad_poll first

		clear_vram_buffer(); // do at the beginning of each frame

		update_2_0();
PROFILE_POKE(PROF_CLEAR);
	}
}

unsigned char rng512_index;
const signed int rng512[256] =
{
	392, 235, 57, 80, 282, 51, 96, 233, 335, 29, 162, 36, 413, 169, 390, 377, 142, 472, 131, 504, 33, 288, 35, 145, 85, 266, 236, 461, 164, 406, 328, 290, 405, 60, 206, 417, 295, 388, 109, 58, 348, 366, 175, 373, 426, 506, 403, 202, 291, 297, 280, 329, 46, 113, 184, 482, 98, 91, 249, 286, 313, 231, 477, 115, 42, 397, 219, 242, 444, 213, 258, 395, 361, 191, 324, 62, 63, 331, 416, 27, 304, 498, 293, 18, 195, 450, 197, 307, 247, 428, 262, 487, 326, 432, 490, 452, 55, 222, 368, 443, 457, 38, 271, 220, 510, 16, 337, 399, 76, 156, 53, 364, 317, 323, 306, 355, 208, 275, 346, 508, 260, 253, 411, 312, 339, 257, 503, 277, 68, 47, 381, 268, 470, 375, 284, 421, 11, 353, 350, 224, 225, 493, 66, 189, 466, 148, 455, 180, 357, 476, 359, 333, 273, 454, 424, 137, 488, 82, 140, 102, 217, 384, 394, 93, 107, 200, 433, 246, 24, 178, 499, 49, 238, 318, 214, 14, 479, 485, 468, 5, 370, 301, 372, 158, 422, 279, 437, 474, 501, 419, 153, 439, 230, 73, 31, 430, 120, 25, 446, 71, 173, 3, 0, 386, 251, 7, 228, 351, 492, 310, 481, 342, 383, 126, 9, 495, 435, 104, 74, 299, 2, 244, 302, 264, 379, 410, 44, 255, 269, 362, 459, 408, 186, 340, 13, 211, 400, 344, 241, 40, 129, 135, 118, 167, 20, 463, 22, 320, 448, 441, 87, 124, 151, 69, 315, 465
};


void spawn_stars_at_player()
{
	in_x_tile = cur_player->start_x_grid;
	in_y_tile = cur_player->start_y_grid;

	spawn_stars_at_tile();
}

void spawn_stars_at_tile()
{
	static signed int temp_x;
	static signed int temp_y;

	// Note: faster than << 12
	temp_x = (in_x_tile << 4) << 8;
	temp_y = (in_y_tile << 4) << 8;

	// NOTE: This is faster than memfill/set.
	fx_alive[BIG_STAR] = 1;
	fx_x[BIG_STAR] = temp_x;
	fx_y[BIG_STAR] = temp_y;
 	fx_anim[BIG_STAR].anim_queued = 0xff;
 	global_working_anim = &fx_anim[BIG_STAR];
	queue_next_anim(ANIM_FX_STAR_HIT);

	fx_alive[SMALL_STAR_1] = 1;
	fx_x[SMALL_STAR_1] = temp_x;
	fx_y[SMALL_STAR_1] = temp_y;
	fx_dir_x[SMALL_STAR_1] = rng512[++rng512_index] - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_1] = -rng512[++rng512_index]; // same as above but force upwards.
 	fx_anim[SMALL_STAR_1].anim_queued = 0xff;
 	global_working_anim = &fx_anim[SMALL_STAR_1];
	queue_next_anim(ANIM_FX_STAR_SHORT);

	fx_alive[SMALL_STAR_2] = 1;
	fx_x[SMALL_STAR_2] = temp_x;
	fx_y[SMALL_STAR_2] = temp_y;
	fx_dir_x[SMALL_STAR_2] = rng512[++rng512_index] - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_2] = -rng512[++rng512_index]; // same as above but force upwards.
 	fx_anim[SMALL_STAR_2].anim_queued = 0xff;
 	global_working_anim = &fx_anim[SMALL_STAR_2];
	queue_next_anim(ANIM_FX_STAR_MED);

	fx_alive[SMALL_STAR_3] = 1;
	fx_x[SMALL_STAR_3] = temp_x;
	fx_y[SMALL_STAR_3] = temp_y;
	fx_dir_x[SMALL_STAR_3] = rng512[++rng512_index] - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_3] = -rng512[++rng512_index]; // same as above but force upwards.
 	fx_anim[SMALL_STAR_3].anim_queued = 0xff;
 	global_working_anim = &fx_anim[SMALL_STAR_3];
	queue_next_anim(ANIM_FX_STAR_LONG);
}

void draw_cur_time()
{
	// The number of digits to display (00 00 00)
	i = 5;
	// The total number of characters including spacers (00:00:00)
	counter_signed = 7;

	// Loop through from the right most digit to the left most. Increment the "frames" digit, and 
	// handle if it overflows into double digits. Then do the same on the next digit. Once a digit
	// doesn't overflow, the function can exit. So worst case scenario is "59:59:59".
	// NOTE: The right most 2 digits are FRAMES not MILLISECONDS, and thus rolls over at 60 (60 fps), not 100.
	for (; counter_signed >= 0; --counter_signed, --i)
	{
		// Increment this digit...
		if (cur_state == STATE_GAME)
		{
			++cur_time_digits[i];
		}
		// ... and check for overflow. Depending on if it is the left or right digit
		// we check for 6 or 10 (since the max value is 59 in each section).
		if (cur_time_digits[i] == ((i % 2) == 0 ? 6 : 10))
		{
			// A roll over has occured. Reset this digit back to 0. The incrementing of the next
			// widget will happen in the next iteration of the loop.
			// Special case for the left most digit. If that rolls over, we don't want the timer
			// to reset to 0, as that could be an exploit (waiting for 60 minutes and then finishing).
			// In that case we keep the timer at 50 minutes.
			cur_time_digits[i] = (i == 0) ? 5 : 0;
			one_vram_buffer('0' + cur_time_digits[i], get_ppu_addr(0, (12<<3) + (counter_signed * 8), 24));

			// Special case for ':' character.
			if ((i % 2) == 0)
			{
				--counter_signed;
			}
		}
		else
		{
			// This digit changed, but it didn't overflow, so we can just draw and then exit the loop now.
			one_vram_buffer('0' + cur_time_digits[i], get_ppu_addr(0, (12<<3) + (counter_signed * 8), 24));
			break;
		}
		
	}
}

// Trying to use string literals right in calls after banked function calls
// results in garbage values. Not sure why, but putting them into PRG ROM bank
// fixes it.
const unsigned char TEXT_Time[] = "TIME";
const unsigned char TEXT_Best[] = "BEST";
const unsigned char TEXT_EmptyTime[] = "00:00:00";

void load_current_map()
{
	grav_search_queue_x[0] = 0xff;
	grav_search_queue_y[0] = 0xff;
	grav_search_queue_x[1] = 0xff;
	grav_search_queue_y[1] = 0xff;
	grav_search_queue_x[2] = 0xff;
	grav_search_queue_y[2] = 0xff;

	cur_history_index = 0;
	cur_history_count = 0;
	mirror_fade = 0xff;
	shake_remaining = 0;

	memfill(serialized_frames, 0, sizeof(frame_serialized) * MAX_HISTORY);

	cur_nametable_save_queue = 0;
	memfill(nametable_value_save_queue, 0, MAX_NAMETABLE_SAVE_QUEUE);
	memfill(nametable_index_save_queue, 0, MAX_NAMETABLE_SAVE_QUEUE);

	banked_call(BANK_5, copy_and_process_map_data);

	// preinitialize stars

	queued_stars_for_player_index = 0xff;

	// Assumes fx_spawn_stars_on_death is the first property, and simple_fx matches the size of all the arrays.
	memfill(fx_spawn_stars_on_death, 0, sizeof(simple_fx) * MAX_FX_OBJECTS);

//	fx_alive[BIG_STAR] = 1;
//	fx_x[BIG_STAR] = (player1.start_x_grid << 4) << 8;
//	fx_y[BIG_STAR] = (player1.start_y_grid << 4) << 8;
	fx_floats[BIG_STAR] = 1;
	fx_dir_x[BIG_STAR] = 0;
	fx_dir_y[BIG_STAR] = 0;
	fx_anim[BIG_STAR].anim_queued = 0xff;
	fx_spawn_stars_on_death[BIG_STAR] = 0;
	global_working_anim = &fx_anim[BIG_STAR];
	queue_next_anim(ANIM_FX_STAR_HIT);

//	fx_alive[SMALL_STAR_1] = 1;
//	fx_x[SMALL_STAR_1] = (player1.start_x_grid << 4) << 8;
//	fx_y[SMALL_STAR_1] = (player1.start_y_grid << 4) << 8;
	fx_dir_x[SMALL_STAR_1] = (rand() % 0x200) - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_1] = -(rand() % 0x200); // same as above but force upwards.
	fx_anim[SMALL_STAR_1].anim_queued = 0xff;
	fx_spawn_stars_on_death[SMALL_STAR_1] = 0;
	global_working_anim = &fx_anim[SMALL_STAR_1];
	queue_next_anim(ANIM_FX_STAR_LONG);	

//	fx_alive[SMALL_STAR_2] = 1;
//	fx_x[SMALL_STAR_2] = (player1.start_x_grid << 4) << 8;
//	fx_y[SMALL_STAR_2] = (player1.start_y_grid << 4) << 8;
	fx_dir_x[SMALL_STAR_2] = (rand() % 0x200) - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_2] = -(rand() % 0x200); // same as above but force upwards.
	fx_anim[SMALL_STAR_2].anim_queued = 0xff;
	fx_spawn_stars_on_death[SMALL_STAR_2] = 0;
	global_working_anim = &fx_anim[SMALL_STAR_2];
	queue_next_anim(ANIM_FX_STAR_MED);	

//	fx_alive[SMALL_STAR_3] = 1;
//	fx_x[SMALL_STAR_3] = (player1.start_x_grid << 4) << 8;
//	fx_y[SMALL_STAR_3] = (player1.start_y_grid << 4) << 8;
	fx_dir_x[SMALL_STAR_3] = (rand() % 0x200) - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
	fx_dir_y[SMALL_STAR_3] = -(rand() % 0x200); // same as above but force upwards.
	fx_anim[SMALL_STAR_3].anim_queued = 0xff;
	fx_spawn_stars_on_death[SMALL_STAR_3] = 0;
	global_working_anim = &fx_anim[SMALL_STAR_3];
	queue_next_anim(ANIM_FX_STAR_SHORT);	

	memfill(cur_time_digits, 0, 6);

	for (y = 0; y < 15; ++y)
	{
		for (x = 0; x < 16; ++x)
		{
			index16 = (y * 16) + (x);
			//index16 *= 5; // each entry is 6 bytes.

			// Handle case where the door is the only enemy.
			if (current_room[index16] == TILE_DOOR && enemies_remaining == 1)
			{
				current_room[index16] = TILE_DOOR_OPEN;
			}
			index16 = current_room[index16] * META_TILE_NUM_BYTES;
			vram_adr(NTADR_A(x*2,y*2));	
			vram_write(&metatiles[index16], 2);
			vram_adr(NTADR_A(x*2,(y*2)+1));	
			vram_write(&metatiles[index16+2], 2);
		}
	}

	index2 = 0;
	for (y = 0; y < 15; y+=2)
	{
		for (x = 0; x < 16; x+=2)
		{
			i = 0;

			// room index.
			index16 = (y * 16) + (x);
			// meta tile palette index.
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			// bit shift amount
			i |= (metatiles[index16]);

			index16 = (y * 16) + (x + 1);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 2;

			index16 = ((y + 1) * 16) + (x);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 4;

			index16 = ((y + 1) * 16) + (x + 1);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 6;	

			vram_adr(NAMETABLE_A + 0x3c0 + index2);	
			vram_write(&i, 1);
			++index2;
		}
	}

	cur_player = players[0];
	force_update_player_idle_anim();
	if (players[1] != NULL)
	{
		cur_player = players[1];
		force_update_player_idle_anim();
	}

	draw_time_background_ppu_off();

	banked_call(BANK_5, build_room_delta);
}

void force_update_player_idle_anim()
{
	
	// force update and draw the player so that they appear correctly when fading in.
	x = cur_player->start_x_grid;
	y = cur_player->start_y_grid;
	index = (y << 4) + x;
	index2 = index - 16;

	if (GET_META_TILE_FLAGS(index) & FLAG_LADDER)
	{
		global_working_anim = &cur_player->type_specific_data.player_data.anim;
		queue_next_anim(ANIM_IDLE_CLIMB);
	}
	else
	{
		if (cur_player->type_specific_data.player_data.facing_left)
		{
			global_working_anim = &cur_player->type_specific_data.player_data.anim;
			if ((GET_META_TILE_FLAGS(index2) & (FLAG_CAN_CARRY)) != 0)
			{
				queue_next_anim(ANIM_IDLE_CARRY_LEFT);
			}
			else
			{
				queue_next_anim(ANIM_IDLE_LEFT);
			}

		}
		else
		{
			global_working_anim = &cur_player->type_specific_data.player_data.anim;
			if ((GET_META_TILE_FLAGS(index2) & (FLAG_CAN_CARRY)) != 0)
			{
				queue_next_anim(ANIM_IDLE_CARRY_RIGHT);
			}
			else
			{
				queue_next_anim(ANIM_IDLE_RIGHT);
			}
		}
	}		
	
	global_working_anim = &cur_player->type_specific_data.player_data.anim;
	commit_next_anim();
}

void draw_best_time_level_select()
{
	// static char best_time[8];

	// memfill(best_time, 0, 8);

 	// // vram_adr(NTADR_A(22, 2));
	// // vram_write(TEXT_Best, 4);
	// //multi_vram_buffer_horz(TEXT_Best, 4, get_ppu_addr(0,22*8,2*8));

	// counter = 0;
	// for (i = 0; i < 6; ++i, ++counter)
	// {
	// 	if (wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i] == 0xff) // means not set.
	// 	{
	// 		best_time[counter] = '-';
	// 	}
	// 	else
	// 	{
	// 		best_time[counter] = '0' + wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i];
	// 	}
	// 	if (i == 1 || i == 3)
	// 	{
	// 		++counter;
	// 		best_time[counter] = ':';
	// 	}
	// }

 	// // vram_adr(NTADR_A(22, 3));	
	// // vram_write(best_time, 8);
	// multi_vram_buffer_horz(best_time, 8, get_ppu_addr(0,12*8, 26*8));
	
}

const char TEXT_Level[] = "LEVEL";

void draw_time_background_ppu_off()
{	
	static char best_time[8];	
	static unsigned char local_temp1;
	static unsigned char local_temp2;

	local_temp1 = (cur_room_index) / ROOMS_PER_WORLD;
	local_temp2 = (cur_room_index) % ROOMS_PER_WORLD;

	one_vram_buffer('0' + (local_temp1 + 1), get_ppu_addr(0, 3*8, 3*8));
	one_vram_buffer('-', get_ppu_addr(0, 4*8, 3*8));
	one_vram_buffer('0' + (local_temp2 + 1), get_ppu_addr(0, 5*8, 3*8));

 	vram_adr(NTADR_A(2, 2));	
	vram_write(TEXT_Level, 5);

	
	memfill(best_time, 0, 8);

 	vram_adr(NTADR_A(14, 2));	
	vram_write(TEXT_Time, 4);

	vram_adr(NTADR_A(12, 3));	
	vram_write(TEXT_EmptyTime, 8);

	// counter = 0;
	// for (i = 0; i < 6; ++i, ++counter)
	// {
	// 	if (wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i] == 0xff) // means not set.
	// 	{
	// 		best_time[counter] = '-';
	// 	}
	// 	else
	// 	{
	// 		best_time[counter] = '0' + wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i];
	// 	}
	// 	if (i == 1 || i == 3)
	// 	{
	// 		++counter;
	// 		best_time[counter] = ':';
	// 	}
	// }

 	// vram_adr(NTADR_A(22, 3));	
	// vram_write(best_time, 8);

	// Draw the current time for the case where the player reset the
	// whole nametable in the middle of gameplay. The standard draw time
	// function only draws numbers that change.
	counter = 0;
	for (i = 0; i < 6; ++i, ++counter)
	{

		best_time[counter] = '0' + cur_time_digits[i];
		if (i == 1 || i == 3)
		{
			++counter;
			best_time[counter] = ':';
		}
	}

 	vram_adr(NTADR_A(12, 3));	
	vram_write(best_time, 8);
}

void load_current_map_force_ppu_off()
{
	ppu_off();
	load_current_map();
	ppu_on_all();

	in_x_tile = 25<<3;
	in_y_tile = 3<<3;
	index = 0;
	multi_vram_buffer_horz("PASSWORD", 8, get_ppu_addr(0, in_x_tile-16, in_y_tile-8));
	banked_call(BANK_1, load_pause_menu_password_into_nametable);
}
void restore_room_delta()
{
	// always leave 
	if (cur_history_count <= 1)
	{
		return;
	}

	grav_search_queue_x[0] = 0xff;
	grav_search_queue_y[0] = 0xff;
	grav_search_queue_x[1] = 0xff;
	grav_search_queue_y[1] = 0xff;
	grav_search_queue_x[2] = 0xff;
	grav_search_queue_y[2] = 0xff;

	//fade_to_white();
	fade_to_black_fast();
	index = (cur_history_index - 1);
	index2 = (MAX_HISTORY - 1);
	cur_history_index = MIN(index,index2);

	enemies_remaining = serialized_frames[cur_history_index].enemies_remaining;

	oam_clear();

	for (i = 0; i < 2; ++i)
	{
		cur_player = players[i];

		if (cur_player != NULL)
		{
			memfill(cur_player, 0, sizeof(game_object));
			cur_player->start_x_grid = serialized_frames[cur_history_index].player_data[i].xy >> 4;
			cur_player->start_y_grid = serialized_frames[cur_history_index].player_data[i].xy & 0xf;
			cur_player->type_specific_data.player_data.facing_left = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_FACING_LEFT) != 0;
			cur_player->type_specific_data.player_data.is_cur_player = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_IS_CUR_PLAYER) != 0;
			cur_player->type_specific_data.player_data.is_clone = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_IS_CLONE) != 0;
			cur_player->type_specific_data.player_data.is_wiz = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_IS_WIZ) != 0;
			cur_player->type_specific_data.player_data.is_mirror = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_IS_MIRROR) != 0;
			cur_player->alive = (serialized_frames[cur_history_index].player_data[i].flags & FLAG_IS_ALIVE) != 0;
			cur_player->ticks_since_move = MOVE_FRAMES; // start at the end.

			force_update_player_idle_anim();

			// TODO: Pal change for player 2.
			// TODO: PRG1
			// oam_meta_spr(cur_player->start_x_grid * 16, (cur_player->start_y_grid * 16) - 1, 
			// 	metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);

		}
	}


	memfill(objs, 0, sizeof(game_object) * MAX_WORLD_OBJECTS);
	memfill(fx_spawn_stars_on_death, 0, sizeof(simple_fx) * MAX_FX_OBJECTS);
	
	//memcpy(current_room, serialized_frames[cur_history_index].nametable_data, 240);

	// The same nametable entry can change multiple times in a single frame serialization
	// so go backwards to ensure that multiple writes are done in the correct
	// order.
	for(i = MAX_NAMETABLE_SAVE_QUEUE - 1; i < MAX_NAMETABLE_SAVE_QUEUE; --i)
    {
		// 0xff represents an unused entry.
		if (serialized_frames[cur_history_index].nametable_value[i] != 0xff)
		{
			current_room[serialized_frames[cur_history_index].nametable_index[i]] = serialized_frames[cur_history_index].nametable_value[i];
		}		
    }

	ppu_off();

	for (y = 0; y < 15; ++y)
	{
		for (x = 0; x < 16; ++x)
		{
			index16 = (y * 16) + (x);
			//index16 *= 5; // each entry is 6 bytes.
			index16 = current_room[index16] * META_TILE_NUM_BYTES;
			vram_adr(NTADR_A(x*2,y*2));	
			vram_write(&metatiles[index16], 2);
			vram_adr(NTADR_A(x*2,(y*2)+1));	
			vram_write(&metatiles[index16+2], 2);
		}
	}

	index2 = 0;
	for (y = 0; y < 15; y+=2)
	{
		for (x = 0; x < 16; x+=2)
		{
			i = 0;

			// room index.
			index16 = (y * 16) + (x);
			// meta tile palette index.
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			// bit shift amount
			i |= (metatiles[index16]);

			index16 = (y * 16) + (x + 1);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 2;

			index16 = ((y + 1) * 16) + (x);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 4;

			index16 = ((y + 1) * 16) + (x + 1);
			index16 = (current_room[index16] * META_TILE_NUM_BYTES) + 4;
			i |= (metatiles[index16]) << 6;	

			vram_adr(NAMETABLE_A + 0x3c0 + index2);	
			vram_write(&i, 1);
			++index2;
		}
	}	

	draw_time_background_ppu_off();

	ppu_on_all();
	// check for 0 at top of function.
	--cur_history_count;

	in_x_tile = 25<<3;
	in_y_tile = 3<<3;
	index = 0;

	banked_call(BANK_1, load_pause_menu_password_into_nametable);
	//multi_vram_buffer_horz("PASS", 4, get_ppu_addr(0, in_x_tile, in_y_tile-8));	
	multi_vram_buffer_horz("PASSWORD", 8, get_ppu_addr(0, in_x_tile-16, in_y_tile-8));

	//fade_from_white();
	fade_from_black_fast();
}

unsigned char is_obj_below()
{
	// use all local variables, as this is likely
	// called from with nested loops.
	static unsigned char local_i;
	static unsigned char local_x;
	static unsigned char local_y;
	static unsigned char local_x2;
	static unsigned char local_y2;
	static unsigned char local_index;
	static game_object* local_obj_temp;

	// calculate final position.
	local_x = target_obj->start_x_grid + target_obj->dir_x;
	local_y = target_obj->start_y_grid + target_obj->dir_y;

	local_index = (local_y << 4) + local_x;

	// Is there a nametable entry which is solid below us?
	if ((GET_META_TILE_FLAGS(local_index + 16) & FLAGS_BLOCK_FALLING))
	{
		return 1;
	}

// CAN WE GET AWAY WITH JUST TILE FLAG CHECKS? THIS MEANS IT WILL MISSING MOVING OBJECTS.
// PROFILE_POKE(PROF_B);
// 	// Look for active objects which are below us.
// 	for (local_i = 0; local_i < MAX_WORLD_OBJECTS; ++local_i)
// 	{
// 		local_obj_temp = &objs[local_i];

// 		// can assume these are non-player objects.
// 		if (local_obj_temp != target_obj && local_obj_temp->alive && local_obj_temp->type_specific_data.obj_data.visible)
// 		{
// 			local_x2 = local_obj_temp->start_x_grid + local_obj_temp->dir_x;
// 			local_y2 = local_obj_temp->start_y_grid + local_obj_temp->dir_y;

// 			if (local_x == local_x2 && (local_y + 1) == local_y2)
// 			{
// 				return 1;
// 			}
// 		}
// 	}

	// Look for active players below us.
	for (local_i = 0; local_i < 2; ++local_i)
	{
		local_obj_temp = players[local_i];

		if (local_obj_temp != target_obj && local_obj_temp != NULL && local_obj_temp->alive)
		{
			// NOTE: No longer needed as we now update objects from bottom to
			// 		 top of screen which means we only need to compare
			//		 destinations to keep players walking on top of each other.

			// if (target_obj == &player1 || target_obj == &player2)
			// {
			// 	// allow players to move on top of each other, but this means that
			// 	// essentially players are in 2 positions at the same time. Maybe have
			// 	// knock-on issues.
			// 	local_x2 = local_obj_temp->start_x_grid;
			// 	local_y2 = local_obj_temp->start_y_grid;

			// 	if (local_x == local_x2 && (local_y + 1) == local_y2)
			// 	{
			// 		return 1;
			// 	}
			// }

			local_x2 = local_obj_temp->start_x_grid + local_obj_temp->dir_x;
			local_y2 = local_obj_temp->start_y_grid + local_obj_temp->dir_y;

			if (local_x == local_x2 && (local_y + 1) == local_y2)
			{
				return 1;
			}
		}
	}

	return 0;
}

unsigned char is_other_player_in_dir(signed char in_x, signed char in_y)
{
	obj_temp = players[(player_index + 1) % 2];

	return (obj_temp != NULL && obj_temp->alive
	    && (obj_temp->start_x_grid + obj_temp->dir_x) == (cur_player->start_x_grid + in_x) 
		&& (obj_temp->start_y_grid + obj_temp->dir_y) == (cur_player->start_y_grid + in_y));
}

// param: in_index_begin
// param: in_index_end
// param: dir_temp
// param: in_lerp_type
void try_push_object(void)
{
	// Basically a dupe of the block pushing code below.
	// Too lazy to turn into a function, and deal with all the 
	// TODO: Temp disable check for player for simplicity.
	if ((GET_META_TILE_FLAGS(in_index_end) & FLAGS_BLOCK_PUSH) == 0 /*&& !is_other_player_in_dir(dir_temp << 1, 0)*/)
	{
		// Spawn a temp object to animate the block movement.
		for (i = 0; i < MAX_WORLD_OBJECTS; ++i)
		{
			obj_temp = &objs[i];

			if (!obj_temp->alive)
			{
				// Create the temp animation.
				obj_temp->alive = 1;
				obj_temp->ticks_since_move = 0;
				obj_temp->start_x_grid = cur_player->start_x_grid;
				obj_temp->start_y_grid = cur_player->start_y_grid - 1;
				obj_temp->dir_x = dir_temp;
				obj_temp->dir_y = 0;
				obj_temp->type_specific_data.obj_data.lerp_type_x = in_lerp_type;

				if (current_room[in_index_begin] == TILE_PUSH_BLOCK)
				{
					obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_BLOCK;
				}
				else
				{
					obj_temp->type_specific_data.obj_data.object_def = OBJ_DEF_KEY;
				}

				// Store the current position, and the destination as the looked prior to moving.
				nametable_index_save_queue[cur_nametable_save_queue] = in_index_end;
				nametable_value_save_queue[cur_nametable_save_queue] = current_room[in_index_end];
				++cur_nametable_save_queue;
				nametable_index_save_queue[cur_nametable_save_queue] = in_index_begin;
				nametable_value_save_queue[cur_nametable_save_queue] = current_room[in_index_begin];
				++cur_nametable_save_queue;

				// Update the room data.
				tile_index_param = in_index_begin;
				banked_call(BANK_4, copy_original_room_to_current);

				// Then copy the 2x2 block of meta tiles over again.
				// We do 2x2 because copying over attributes for 1 tiles is troublesome.
				in_x_tile = obj_temp->start_x_grid;
				in_y_tile = obj_temp->start_y_grid;
				vram_buffer_load_2x2_metatile();
				
				need_save = 1;

				
				SFX_PLAY_WRAPPER(SOUND_PUSH_BLOCK);

				break; // found one, no need to continue.
			}
		}
	}
}

void save_time_if_best(void)
{
	// // potentionally save the best time.
	// for (i = 0; i < 6; ++i)
	// {
	// 	if (wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i] > cur_time_digits[i])
	// 	{
	// 		// best time
	// 		// NOTE: i reused, but exiting after so won't matter.
	// 		for (i = 0; i < 6; ++i)
	// 		{
	// 			wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i] = cur_time_digits[i];
	// 		}
	// 		break;
	// 	}
	// 	else if (wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE) + i] < cur_time_digits[i])
	// 	{
	// 		// worse time, exit.
	// 		break;
	// 	}
	// }
}