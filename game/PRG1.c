#include "PRG1.h"
#include "main.h"
#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "A53/bank_helpers.h"
#include "../include/stdlib.h"
#include "PRG0.h"
#include "PRG4.h"
#include "PRG5.h"

#pragma rodata-name ("BANK2")
#pragma code-name ("BANK2")

#include "meta_sprites.h"
#include "world_object_defs.h"
#include "NES_ST/pause_menu.h"
#include "NES_ST/level_select_menu.h"
#include "NES_ST/boot_screen.h"
#include "NES_ST/sound_test.h"
#include "NES_ST/password_entry.h"
#include "NES_ST/new_game_menu.h"

#pragma rodata-name ("BANK0")
#pragma code-name ("BANK0")
#include "NES_ST/ending.h"
#include "NES_ST/title_screen.h"
#include "NES_ST/pico_credits_screen.h"

#pragma rodata-name ("BANK2")
#pragma code-name ("BANK2")
#include "meta_sprites_title.h"

const unsigned char star_anims[NUM_STAR_ANIMS] = { ANIM_FX_STAR_LONG, ANIM_FX_STAR_MED, ANIM_FX_STAR_SHORT };

// TODO: Move to a tables header?
const signed char ease_backinquart_right[MOVE_FRAMES] =
{
	0,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	1,
	3,
	5,
	8,
	10,
	13,
	14,
};

const signed char ease_easeoutquint_right[MOVE_FRAMES] =
{
	0,
	4,
	8,
	10,
	12,
	14,
	14,
	15,
	16,
	16,
	16,
	16,
	16,
	16,
	16,
	16,
};


const signed char ease_linear_right[MOVE_FRAMES] =
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
};

const signed char ease_backinquart_left[MOVE_FRAMES] =
{
	0,
	0,
	0,
	1,
	1,
	1,
	1,
	1,
	0,
	-1,
	-3,
	-5,
	-8,
	-10,
	-13,
	-14,
};

const signed char ease_easeoutquint_left[MOVE_FRAMES] =
{
	0,
	-4,
	-8,
	-10,
	-12,
	-14,
	-14,
	-15,
	-16,
	-16,
	-16,
	-16,
	-16,
	-16,
	-16,
	-16,
};


const signed char ease_linear_left[MOVE_FRAMES] =
{
	0,
	-1,
	-2,
	-3,
	-4,
	-5,
	-6,
	-7,
	-8,
	-9,
	-10,
	-11,
	-12,
	-13,
	-14,
	-15,
};

const signed char ease_pickup_top[MOVE_FRAMES] = 
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	2,
	3,
	4,
	6,
	8,
	11,
	15,
};

// A53 HACK: Moved to RAM as this is used across multiple swapped banks.
char ease_pulse[4] = { -1, 0, 1, 0 };

enum { PAL_BG_STANDARD = 0, PAL_BG_MIRROR, PAL_BG_TITLE, PAL_BG_BOOT, PAL_BG_ENDING };

#pragma rodata-name ("CODE")
#pragma code-name ("CODE")
const unsigned char BG_palettes[][16] =
{
	// Standard
	{ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x09,0x13,0x24,0x0f,0x06,0x27,0x30 },

	// Mirror Level
	{ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x03,0x13,0x23,0x0f,0x06,0x27,0x30 },

	// Title
	{ 0x0f,0x11,0x26,0x37,0x0f,0x06,0x26,0x36,0x0f,0x03,0x00,0x37,0x0f,0x06,0x26,0x37 },

	// Boot Screen
	{ 0x0f,0x22,0x30,0x30,0x0f,0x30,0x38,0x38,0x0f,0x22,0x0f,0x21,0x0f,0x33,0x0f,0x25 },

	// Ending
	{ 0x0f,0x11,0x20,0x31,0x0f,0x06,0x37,0x31,0x0f,0x06,0x17,0x1a,0x0f,0x00,0x10,0x31 },
};

enum { PAL_SPR_STANDARD = 0, PAL_SPR_MIRROR };

const unsigned char SPR_palettes[][16] =
{
	// Standard
	{ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x09,0x13,0x24,0x0f,0x06,0x27,0x30 },

	// Mirror Level
	{ 0x0f,0x13,0x03,0x0f,0x0f,0x11,0x21,0x30,0x0f,0x03,0x13,0x23,0x0f,0x06,0x27,0x30 },
};
#pragma rodata-name ("BANK2")
#pragma code-name ("BANK2")

// const unsigned char palette_title_screen[16]={ 0x0f,0x11,0x26,0x37,0x0f,0x06,0x26,0x36,0x0f,0x03,0x00,0x37,0x0f,0x06,0x26,0x37 };
// //const unsigned char palette_title_screen[16]={ 0x0f,0x11,0x26,0x37,0x0f,0x06,0x26,0x36,0x0f,0x03,0x00,0x37,0x0f,0x09,0x19,0x29 };
// const unsigned char palette_boot[16]={ 0x0f,0x22,0x31,0x30,0x0f,0x30,0x0f,0x38,0x0f,0x22,0x0f,0x21,0x0f,0x33,0x0f,0x25 };


// const unsigned char palette_bg[16]={ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x09,0x13,0x24,0x0f,0x06,0x27,0x30 };
// const unsigned char palette_sp[16]={ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x09,0x13,0x24,0x0f,0x06,0x27,0x30 };
// const unsigned char palette_bg_mirror[16]={ 0x0f,0x00,0x10,0x30,0x0f,0x11,0x21,0x30,0x0f,0x03,0x13,0x23,0x0f,0x06,0x27,0x30 };
// const unsigned char palette_sp_mirror[16]={ 0x0f,0x13,0x03,0x0f,0x0f,0x11,0x21,0x30,0x0f,0x03,0x13,0x23,0x0f,0x06,0x27,0x30 };


typedef struct anim_def
{
	// how many frames to hold on each frame of animation.
	unsigned char ticks_per_frame;

	// how many frames are used in frames array.
	// TODO: would it be better to just use a special value (eg. 0xff)
	//		 to signify the end of the anim?
	unsigned char anim_len;

	// index into meta_sprites array
	unsigned char frames[17];

	// pointer to the table which will be used to ease the animation movement.
	const signed char *ease_table;

	const signed char *ease_table_y;
} anim_def;

const anim_def walk_right = { 8, 4, { 0, 1, 0, 2}, ease_linear_right };
const anim_def idle_right = { 16, 1, { 0 }, ease_linear_right };
const anim_def walk_left = { 8, 4, { 3, 4, 3, 5}, ease_linear_left };
const anim_def idle_left = { 16, 1, { 3 }, ease_linear_left };
const anim_def climb_up = { 8, 2, { 6, 7 } };
const anim_def idle_climb = { 16, 1, { 6 } };
const anim_def fall_right = {16, 1, { 8 } };
const anim_def fall_left = {16, 1, { 9 } };
const anim_def jump_right = {8, 2, { 10, 11 }, ease_backinquart_right };
const anim_def jump_left = {8, 2, { 12, 13 }, ease_backinquart_left };
const anim_def attack_right = {8, 2, { 14, 15 }, ease_backinquart_right };
const anim_def attack_left = {8, 2, { 16, 17 }, ease_backinquart_left };
const anim_def push_right = {16, 1, { 15 } };
const anim_def push_left = {16, 1, { 17 } };
const anim_def ladder_push_right = {16, 1, { 20 } };
const anim_def ladder_push_left = {16, 1, { 21 } };
const anim_def fx_star_long = { 16, 2, { 24, 25 } };
const anim_def fx_star_med = { 8, 2, { 24, 25 }} ;
const anim_def fx_star_short = { 4, 2, { 24, 25 } };
const anim_def fx_star_hit = { 2, 3, { 26, 24, 25 } };
#define AI_DEATH_LEN 8
const anim_def ai_heavy_idle = { AI_DEATH_LEN, 1, { 19 } };
const anim_def ai_float_idle = { AI_DEATH_LEN, 1, { 22 } };
const anim_def dust = { 8, 2, { 27, 28 } };
const anim_def door = { 16, 1, { 29 } };
const anim_def card_spin = { 3, 17, { 34, 33, 32, 31, 30, 31, 32, 33, 34, 35, 36, 37, 38, 37, 36, 35, 34  }};
const anim_def pickup_right = { 8, 2, { 40, 11 }, NULL, ease_pickup_top };
const anim_def pickup_left = { 8, 2, { 41, 13 }, NULL, ease_pickup_top };
const anim_def walk_carry_right = { 8, 2, { 43, 44 }, ease_linear_right };
const anim_def idle_carry_right = { 16, 1, { 43 }, ease_linear_right };
const anim_def walk_carry_left = { 8, 2, { 45, 46 }, ease_linear_left };
const anim_def idle_carry_left = { 16, 1, { 45 }, ease_linear_left };

const struct anim_def* sprite_anims[] =
{
	&idle_right,
	&walk_right,

	&idle_left,
	&walk_left,

	&climb_up,
	&climb_up, //temp
	&idle_climb,

	&fall_right,
	&fall_left,

	&jump_right,
	&jump_left,

	&attack_right,
	&attack_left,

	&push_right,
	&push_left,

	&ladder_push_right,
	&ladder_push_left,

	&fx_star_long,
	&fx_star_med,
	&fx_star_short,

	&fx_star_hit,

	&ai_heavy_idle,
	&ai_float_idle,

	&dust,

    &door,

	&card_spin,

	&pickup_right,
	&pickup_left,

	&walk_carry_right,
	&idle_carry_right,
	&walk_carry_left,
	&idle_carry_left,
};

const unsigned char press_start_text[] = "-PRESS\x0START-";

#define NUM_PASSWORDS (NUM_ROOMS)
// hard coded passwords. Represents which levels
// have been COMPLETED. So password entry 0 means
// level 0 is completed.
const unsigned int passwords[NUM_PASSWORDS] =
{
	1027,
	1352,
	1135,
	1450,
	3885,
	1184,
	2431,
	1647,
	3241,
	2927,
	3153, //10
	2711,
	3671,
	295,
	3715,
	4072,
	2303,
	1541,
	3732,
	3232,
	2280, //20
	1154,
	3170,
	3281,
	3659,
	561,
	3301,
	4010,
	2850,
	50,
	3577, // 30
	418, // 31


	// 418, // Filler
	// 418, // Filler
	// 418, // Filler

};

unsigned char update_anim();

#define MIRROR_COVER_X ((22<<3)-8)
// operates on cur_player.
void draw_player()
{
	if (mirror_fade != 0xff && cur_player->start_x_grid >= 4 && !cur_player->type_specific_data.player_data.is_mirror)
	{
		oam_meta_spr(
			(MIRROR_COVER_X - 8) - cur_cam_x, 
			((16<<3)-1) - cur_cam_y, 
			metasprite_47_data);

		oam_meta_spr(
			MIRROR_COVER_X - cur_cam_x, 
			((16<<3)-1) - cur_cam_y, 
			metasprite_42_data);
	}

	update_anim();

	// Linear movement.
	// NOTE: (n * 16) is the same as (n << 4) in asm.

	x16 = (cur_player->start_x_grid * CELL_SIZE);
	y16 = (cur_player->start_y_grid * CELL_SIZE);
 
	if (cur_player->dir_x != 0)
	{
		x16 += sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->ease_table[cur_player->ticks_since_move];
	}
	if (cur_player->dir_y < 0)
	{
		if (sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->ease_table_y != NULL)
		{
			y16 -= sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->ease_table_y[cur_player->ticks_since_move];
		}
		else
		{
			y16 -= cur_player->ticks_since_move;
		}
	}
	else if (cur_player->dir_y > 0)
	{
		if (sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->ease_table_y != NULL)
		{
			y16 += sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->ease_table_y[cur_player->ticks_since_move];
		}
		else
		{
			y16 += cur_player->ticks_since_move;
		}
	}

	// TODO: Animate upwards during jump:
	else if (cur_player->type_specific_data.player_data.anim.anim_current == ANIM_JUMP_RIGHT || cur_player->type_specific_data.player_data.anim.anim_current == ANIM_JUMP_LEFT)
	{
		if (cur_player->ticks_since_move > 13)
		{
			y16 -= 3;
		}
        else if (cur_player->ticks_since_move > 9)
		{
			y16 -= 4;
		}
		else if (cur_player->ticks_since_move > 8)
		{
			y16 -= 2;
		}
		else if (cur_player->ticks_since_move > 7)
		{
			y16 -= 1;   
		}
	}	

	x = (cur_player->start_x_grid);
	y = (cur_player->start_y_grid);

	index = (y << 4) + x;

    // TODO: If this could be done in update rather than draw, we could possibly move metatiles back to PRG0.
	if ((GET_META_TILE_FLAGS(index) & FLAG_LADDER))
	{
		y16 -= 2;
	}
	
	// TODO: Palatte swap for clone.
	// if (player_index == 1)
	// {
	// 	// assumes 2x2 meta sprites.
	// 	unsigned char temp_meta[17];
	// 	memcpy(temp_meta, metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]], 17);
	// 	for(i = 0; i < 4; ++i)
	// 	{
	// 		i2 = temp_meta[(i * 4) + 3] & (OAM_FLIP_V | OAM_FLIP_H); // remove palette portion
	// 		temp_meta[(i * 4) + 3] = 2 | i2; // pal
	// 	}
	// 	oam_meta_spr(x16 - cur_cam_x, y16 - cur_cam_y - 1, temp_meta);
	// }
	// else
	//{

	if (cur_player->type_specific_data.player_data.is_wiz == 0)
	{
		// WITCH

		if (cur_player->type_specific_data.player_data.is_cur_player)
		{
			// ACTIVE PLAYER

			if (cur_player->type_specific_data.player_data.is_clone || cur_player->type_specific_data.player_data.is_mirror)
			{
				// CLONE

				if (!cur_player->type_specific_data.player_data.is_mirror || x16 < MIRROR_COVER_X)
				{
					oam_meta_spr_offset(
						x16 - cur_cam_x, 
						y16 - cur_cam_y - 1,
						0,
						2,
						metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);
				}
			}
			else
			{
				// REAL

				oam_meta_spr(
					x16 - cur_cam_x, 
					y16 - cur_cam_y - 1, 
					metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);
			}
		}
		else
		{
			// INACTIVE PLAYER

			oam_meta_spr_offset(
				x16 - cur_cam_x, 
				y16 - cur_cam_y - 1,
				0,
				0,
				metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);
		}
		
	}
	else
	{
		// WIZARD

		if (cur_player->type_specific_data.player_data.is_cur_player)
		{
			// ACTIVE PLAYER

			oam_meta_spr_offset(
				x16 - cur_cam_x, 
				y16 - cur_cam_y - 1,
				2,
				3,
				metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);
		}
		else
		{
			// INACTIVE PLAYER

			oam_meta_spr_offset(
				x16 - cur_cam_x, 
				y16 - cur_cam_y - 1,
				2,
				0,
				metasprite_list[sprite_anims[cur_player->type_specific_data.player_data.anim.anim_current]->frames[cur_player->type_specific_data.player_data.anim.anim_frame]]);
		}
	}

	// Update animation.
	++cur_player->type_specific_data.player_data.anim.anim_ticks;

	if ((ticks8 % 4) == 0)
	{
		index2 = GRID_XY_TO_ROOM_INDEX(x + cur_player->dir_x, y + cur_player->dir_y);
		// x = (cur_player->start_x_grid + cur_player->dir_x);
		// y = (cur_player->start_y_grid + cur_player->dir_y);

		// index2 = (y << 4) + x;

        // TODO: If this could be done in update rather than draw, we could possibly move metatiles back to PRG0.
		if (GET_META_TILE_FLAGS(index2) & FLAG_WEB)
		{
			i2 = find_next_free_fx();
			if (i2 != 0xff)
			{
				fx_floats[i2] = 1;
				fx_x[i2] = (x16 + ((cur_player->dir_x < 0) ? 0 : CELL_SIZE)) << 8;
				fx_y[i2] = (y16 + (rand() % CELL_SIZE)) << 8;
				fx_anim[i2].anim_queued = 0xff;
				global_working_anim = &fx_anim[i2];
				queue_next_anim(ANIM_DUST);
			}
		}
	}		

	// Move time forward.
	// allow it to go one passed the end of MOVE FRAMES so that we don't sit on the last frame
	// checking for gravity etc over and over.
	// NOTE: If you want to analyze worst cast perf, change the 2nd param to MOVE_FRAMES - 1
	cur_player->ticks_since_move = MIN(cur_player->ticks_since_move + 1, MOVE_FRAMES);
}


// param obj_temp
void draw_non_player_object()
{
	// NOTE: (n * 16) is the same as (n << 4) in asm.
	switch (obj_temp->type_specific_data.obj_data.lerp_type_x)
	{
	case LERP_LINEAR:
		x16 = (obj_temp->start_x_grid * CELL_SIZE) + obj_temp->ticks_since_move * obj_temp->dir_x;
		break;
	case LERP_EASE_OUT_QUINT:
	default:
		x16 = (obj_temp->start_x_grid * CELL_SIZE) + ease_easeoutquint_right[obj_temp->ticks_since_move] * obj_temp->dir_x;
		break;
	}
	

	// Currently the only way for a non-player object to move up, is to be picked up.
	if (obj_temp->dir_y < 0)
	{
		y16 = (obj_temp->start_y_grid * CELL_SIZE) + ease_pickup_top[obj_temp->ticks_since_move] * obj_temp->dir_y;
	}
	else
	{
		y16 = (obj_temp->start_y_grid * CELL_SIZE) + (obj_temp->ticks_since_move * obj_temp->dir_y);
	}
	oam_meta_spr(x16 - cur_cam_x, y16 - cur_cam_y - 1, metasprite_list[object_def_table[obj_temp->type_specific_data.obj_data.object_def].metatile]);

	if (obj_temp->dir_x != 0 && (ticks8 % 2) == 0 && obj_temp->type_specific_data.obj_data.lerp_type_x == LERP_EASE_OUT_QUINT)
	{
		for (counter = 0; counter < 2; ++counter)
		{
			i2 = find_next_free_fx();
			if (i2 != 0xff)
			{
				fx_alive[i2] = 1;
				// fx are 1 frame delayed, so re-calculate the x16 for next frame.
				x16 = (obj_temp->start_x_grid * CELL_SIZE) + ease_easeoutquint_right[obj_temp->ticks_since_move + 1] * obj_temp->dir_x;
				fx_x[i2] = (x16 + ((counter == 0) ? 0 : CELL_SIZE)) << 8;
				fx_y[i2] = (y16 + CELL_SIZE) << 8; // high byte
				fx_anim[i2].anim_queued = 0xff;
				global_working_anim = &fx_anim[i2];
				queue_next_anim(ANIM_DUST);
			}
		}
	}
}

void draw_fx_object()
{
	if (fx_floats[star_fx_index] == 0)
	{
		fx_dir_y[star_fx_index] += 13; //(0.05 * 0xff); // gravity
	}
	fx_y[star_fx_index] += fx_dir_y[star_fx_index];
	fx_x[star_fx_index] += fx_dir_x[star_fx_index];
	global_working_anim = &fx_anim[star_fx_index];
	banked_call(BANK_0, commit_next_anim);
	if (update_anim())
	{
		fx_alive[star_fx_index] = 0;
		if (fx_spawn_stars_on_death[star_fx_index])
		{
			if (fx_alive[star_fx_index] == 0)
			{
				// maintain the x, y
				//memfill(fx_obj_temp, 0, sizeof(simple_fx));
				fx_alive[star_fx_index] = 1;
				//fx_x = (p.start_x_grid << 4) << 8;
				//fx_y = (p.start_y_grid << 4) << 8;
				fx_dir_x[star_fx_index] = -fx_dir_x[star_fx_index]; //(rand() % 0x200) - 0x100; // random number from 0-2 using high bits, with additional random in the low bits.
				fx_dir_y[star_fx_index] = -512; //(rand() % 0x200); // same as above but force upwards.
				fx_anim[star_fx_index].anim_queued = 0xff;
				fx_spawn_stars_on_death[star_fx_index] = 0;
				global_working_anim = &fx_anim[star_fx_index];
				queue_next_anim(star_anims[rand() % NUM_STAR_ANIMS]);
			}
		}
	}
	else
	{
			if (fx_sprite_offset[star_fx_index] != 0)
			{
				oam_meta_spr_offset(
					high_byte(fx_x[star_fx_index]) - cur_cam_x, 
					high_byte(fx_y[star_fx_index]) - cur_cam_y - 1,
					fx_sprite_offset[star_fx_index],
					3, // hack - only use case atm.
					metasprite_list[sprite_anims[fx_anim[star_fx_index].anim_current]->frames[fx_anim[star_fx_index].anim_frame]]);
			}
			else
			{
				oam_meta_spr(
					high_byte(fx_x[star_fx_index]) - cur_cam_x, 
					high_byte(fx_y[star_fx_index]) - cur_cam_y - 1, 
					metasprite_list[sprite_anims[fx_anim[star_fx_index].anim_current]->frames[fx_anim[star_fx_index].anim_frame]]);
			}
		++fx_anim[star_fx_index].anim_ticks;
	}
}

void draw_fx_objects()
{
	for (i = 0; i < MAX_FX_OBJECTS; ++i)
	{
		if (fx_alive[i])
		{
			star_fx_index = i;
			draw_fx_object();
		}
	}
}


unsigned char update_anim()
{
	static const struct anim_def* cur_anim;
	cur_anim = sprite_anims[global_working_anim->anim_current];

	if (global_working_anim->anim_ticks >= cur_anim->ticks_per_frame)
	{
		global_working_anim->anim_ticks = 0;
		++global_working_anim->anim_frame;
		// todo: don't always loop.
		if (global_working_anim->anim_frame >= cur_anim->anim_len)
		{
			global_working_anim->anim_frame = 0;
			return 1;
		}
	}
	return 0;
}

#define BOOT_DELAY (60*3)

void update_boot()
{
	if (pad1_new & PAD_ANY_BUTTON || ticks16_in_state > BOOT_DELAY)
	{
		go_to_state(STATE_P8);
	}
}

void update_p8()
{
	if (pad1_new & PAD_ANY_BUTTON || ticks16_in_state > BOOT_DELAY)
	{
		go_to_state(STATE_MENU);
	}
}

#define BLINK_SPEED 4
#define BLINK_RATE 256
void update_menu()
{
	// We want the player to be able to press A to start, but that will 
	// cut the Konami code short (since it contains an A). To avoid that
	// we make sure the user isn't in the process of entering the code
	// correctly, before accepting A. This means that if they press UP for 
	// some other reason, and the A, it won't advance to the next screen.
	if (pad1_new & PAD_START || (cur_konami_index == 0 && pad1_new & PAD_A))
	{
		if (cur_konami_index >= KONAMI_CODE_LEN)
		{
			go_to_state(STATE_SOUND_TEST);
		}
		else
		{
			go_to_state(STATE_MENU_TRANS);
		}
	}
	// else
	// {
	// 	if (ticks8 % 64 == 0)
	// 	{
	// 		multi_vram_buffer_horz(press_start_text, 16, get_ppu_addr(0, 9*8, 17*8));
	// 	}
	// 	else if (ticks8 % 64 == 32)
	// 	{
	// 		multi_vram_buffer_horz("                ", 16, get_ppu_addr(0, 9*8, 17*8));
	// 	}
	// }
	else
	{
		if (pad1_new != 0)
		{
			if (pad1_new & konami_code[cur_konami_index])
			{
				++cur_konami_index;
			}
			else
			{
				cur_konami_index = 0;
			}
		}

		// sprites to cover nametable clashing
		oam_clear();
		oam_meta_spr(13*8,17*8-1,metasprite_title_list[0]);

		if ((ticks8 % BLINK_RATE) > BLINK_RATE - BLINK_SPEED)
		{
			oam_meta_spr(10*8,16*8-1,metasprite_title_list[1]);
		}
		else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 2))
		{
			oam_meta_spr(10*8,16*8-1,metasprite_title_list[2]);
		}
		else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 3))
		{
			oam_meta_spr(10*8,16*8-1,metasprite_title_list[1]);
		}
		// else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 4))
		// {
		// }
		// else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 5))
		// {
		// 	oam_meta_spr(10*8,16*8-1,metasprite_title_list[1]);
		// }
		// else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 6))
		// {
		// 	oam_meta_spr(10*8,16*8-1,metasprite_title_list[2]);
		// }
		// else if ((ticks8 % BLINK_RATE) > BLINK_RATE - (BLINK_SPEED * 7))
		// {
		// 	oam_meta_spr(10*8,16*8-1,metasprite_title_list[1]);
		// }
	}
}

const unsigned char empty_start[] = "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0";
void update_menu_trans()
{
	if (ticks16_in_state > (60*1))
	{
		go_to_state(STATE_NEW_GAME_MENU);
	}
	else
	{
		if (ticks16_in_state < 4)
		{
			pal_bright(7);
		}
		else if (ticks16_in_state < 8)
		{
			pal_bright(6);
		}
		else if (ticks16_in_state < 12)
		{
			pal_bright(5);
		}
		else if (ticks16_in_state < 16)
		{
			pal_bright(4);
		}


		if (ticks16_in_state % 8 == 5)
		{
			multi_vram_buffer_horz(press_start_text, 14, get_ppu_addr(0, 15*8, 20*8));
		}
		else if (ticks16_in_state % 8 == 1)
		{
			multi_vram_buffer_horz(empty_start, 14, get_ppu_addr(0, 15*8, 20*8));
		}
	    
		oam_clear();
		oam_meta_spr(13*8,17*8-1,metasprite_title_list[0]);
		oam_meta_spr(10*8,16*8-1,metasprite_title_list[3]);
	}
}

// A53 HACK TO SAVE SOME SPACE IN PRG2
#pragma rodata-name ("BANK1")
#pragma code-name ("BANK1")

#define NUM_NEW_GAME_OPTIONS 2

void update_new_game_menu()
{
	oam_clear();

	if (pad1_new & PAD_ANY_CONFIRM_BUTTON)
	{
		SFX_PLAY_WRAPPER(SOUND_UI_OK);
		switch (cur_option)
		{
		case 0:
			max_room_completed = -1;
			cur_room_index = 0;
			go_to_state(STATE_MENU_LEVEL_SELECT);
			break;
		
		case 1:
			go_to_state(STATE_PASSWORD);
			break;

		default:
			break;
		}
	}
	else if (pad1_new & PAD_B)
	{
		go_to_state(STATE_MENU);
	}
	else
	{
		if (pad1_new & PAD_UP && cur_option > 0)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_UP);
			--cur_option;
		}
		else if (pad1_new & PAD_DOWN && cur_option < (NUM_NEW_GAME_OPTIONS - 1))
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_DOWN);
			++cur_option;
		}

		oam_spr((10<<3)+ease_pulse[(ticks8/16)%4], (13 + (cur_option * 3))<<3, 0x02, 3|OAM_FLIP_H);
	}
	
}

// A53 HACK TO SAVE SOME SPACE IN PRG2
#pragma rodata-name ("CODE")
#pragma code-name ("CODE")


void update_password_entry()
{
	static unsigned char cur_digit = 0;

	oam_clear();
	if (pad1_new & PAD_B)
	{
		go_to_state(STATE_NEW_GAME_MENU);
	}
	else if (pad1_new & PAD_ANY_CONFIRM_BUTTON)
	{
		cur_pass = (cur_pass_digits[0] - '0') * 1000;
		cur_pass += (cur_pass_digits[1] - '0') * 100;
		cur_pass += (cur_pass_digits[2] - '0') * 10;
		cur_pass += (cur_pass_digits[3] - '0');

		/*
			1027, //0
			3153, //10		
			418, // 31
		*/

		// loop for a matching password
		for (i = 0; i < NUM_PASSWORDS; ++i)
		{
			if (passwords[i] == cur_pass)
			{
				SFX_PLAY_WRAPPER(SOUND_PRESS_START);
				cur_room_index = MIN(i + 1, (NUM_ROOMS-1));
				max_room_completed = i;
				go_to_state(STATE_MENU_LEVEL_SELECT);
				return;
			}
		}

		SFX_PLAY_WRAPPER(SOUND_UI_CANCEL);
		pal_col(0, 0x016);
		delay(60);
		pal_col(0, 0x0f);
	}
	else
	{
		if (pad1_new & PAD_LEFT && cur_digit > 0)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_DOWN);
			--cur_digit;
		}
		else if (pad1_new & PAD_RIGHT && cur_digit < 3)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_UP);
			++cur_digit;
		}
		else if (pad1_new & PAD_UP)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_UP);
			++cur_pass_digits[cur_digit];
			if (cur_pass_digits[cur_digit] > '9')
			{
				cur_pass_digits[cur_digit] = '0';
			}
		}
		else if (pad1_new & PAD_DOWN)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_DOWN);
			--cur_pass_digits[cur_digit];
			if (cur_pass_digits[cur_digit] < '0')
			{
				cur_pass_digits[cur_digit] = '9';
			}
		}

		// one_vram_buffer( '0' + (cur_pass / 1000), get_ppu_addr(0, 14<<3, 15<<3));
		// one_vram_buffer( '0' + ((cur_pass % 1000) / 100), get_ppu_addr(0, 15<<3, 15<<3));
		// one_vram_buffer( '0' + ((cur_pass % 100) / 10), get_ppu_addr(0, 16<<3, 15<<3));
		// one_vram_buffer( '0' + ((cur_pass % 10)), get_ppu_addr(0, 17<<3, 15<<3));
		multi_vram_buffer_horz(cur_pass_digits, 4, get_ppu_addr(0, 14<<3, 15<<3));

		oam_spr(((14+cur_digit)<<3)-1, (14<<3)-3-ease_pulse[(ticks8/16)%4], 0x05, 1);
		oam_spr(((14+cur_digit)<<3)-1, (16<<3) + ease_pulse[(ticks8/16)%4], 0x05, 1|OAM_FLIP_V);
	}
}

void draw_level_select_room_number(unsigned char in_room_display_number)
{
	static unsigned char old_index;
	static unsigned char local_temp1;
	static unsigned char local_temp2;

	local_temp1 = (in_room_display_number - 1) / ROOMS_PER_WORLD;
	local_temp2 = (in_room_display_number - 1) % ROOMS_PER_WORLD;

	one_vram_buffer('0' + (local_temp1 + 1), get_ppu_addr(0, 15*8, 23*8));
	one_vram_buffer('-', get_ppu_addr(0, 16*8, 23*8));
	one_vram_buffer('0' + (local_temp2 + 1), get_ppu_addr(0, 17*8, 23*8));

	// if (in_room_display_number > 9)
	// {
	// 	one_vram_buffer('0' + (in_room_display_number / 10), get_ppu_addr(0, 15*8, 23*8));
	// 	one_vram_buffer('0' + (in_room_display_number % 10), get_ppu_addr(0, 16*8, 23*8));
	// }
	// else
	// {
	// 	one_vram_buffer('0', get_ppu_addr(0, 15*8, 23*8));
	// 	one_vram_buffer('0' + in_room_display_number, get_ppu_addr(0, 16*8, 23*8));
	// }

	// yuck! draw_best_time_level_select is based on cur_room_index, but that hasn't been
	// updated yet. This code temporarily changes it to what it will become. It's complicated by
	// the fact that the index has changed either up or down based on input, but also offset + 1
	// for display pusposes.
	old_index = cur_room_index;
	cur_room_index = in_room_display_number - 1;
	banked_call(BANK_0, draw_best_time_level_select);
	cur_room_index = old_index;
} 


void update_level_select()
{
	static unsigned char local_cur_room;
	static unsigned char next_locked;

	oam_clear();

	local_cur_room = cur_room_index + 1;

	if (pad1_new & (PAD_ANY_CONFIRM_BUTTON))
	{
		SFX_PLAY_WRAPPER(SOUND_UI_OK);
		go_to_state(STATE_GAME);
	}
	else if (pad1_new & PAD_B)
	{
		go_to_state(STATE_NEW_GAME_MENU);
	}
	else
	{
		// TODO: This will change significantly after moving to A53.
		// Intentionally using cur_room_index as we allow going to 1 uncompleted room,
		// but not beyond that.
		next_locked = cur_room_index > max_room_completed; //wram_best_times[(cur_room_index * BEST_TIME_SINGLE_SAVE_SIZE)] == 0xff;// && ((cur_room_index + 1) % ROOMS_PER_WORLD == 0);

		if(pad1_new & PAD_LEFT && local_cur_room > 1)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_DOWN);
			--local_cur_room;
			draw_level_select_room_number(local_cur_room);
			in_x_tile = 5;
		}
		else if(pad1_new & PAD_RIGHT && local_cur_room < (NUM_ROOMS))
		{

			if (!next_locked)
			{
				SFX_PLAY_WRAPPER(SOUND_UI_OPTION_UP);
				++local_cur_room;

				draw_level_select_room_number(local_cur_room);
				in_x_tile = 5;
			}
		}

		cur_room_index = local_cur_room - 1;

		if (in_x_tile < 11)
		{
			if (in_x_tile == 5)
			{
				banked_call(BANK_5, copy_and_process_map_data);

				load_level_pal();
			}

			for (in_y_tile = 5; in_y_tile < 9; ++in_y_tile)
			{
				vram_buffer_load_2x2_metatile();//x, y);
			}
			++in_x_tile;
		}

		if (cur_room_index > 0)
		{
			oam_spr((4<<3)-ease_pulse[(ticks8/16)%4], 14<<3, 0x02, 1);
		}
		if (cur_room_index < (NUM_ROOMS - 1))
		{
			if (next_locked)
			{
				oam_spr(26<<3, 14<<3, 0x04, 1);
				oam_spr(27<<3, 14<<3, 0x02, 1|OAM_FLIP_H);
			}
			else
			{	
				oam_spr((27<<3)+ease_pulse[(ticks8/16)%4], 14<<3, 0x02, 1|OAM_FLIP_H);
			}
			
		}
	}
}

#define NUM_PAUSE_OPTIONS 3
void update_pause()
{	
	oam_clear();

	if (pad1_new & PAD_B)
	{
		SFX_PLAY_WRAPPER(SOUND_UI_OK);
		go_to_state(STATE_GAME);
	}
	else if (pad1_new & PAD_ANY_BUTTON)
	{
		SFX_PLAY_WRAPPER(SOUND_UI_OK);
		switch (cur_option)
		{
		case 0:
			go_to_state(STATE_GAME);
			break;
		
		case 1:
			go_to_state(STATE_MENU_LEVEL_SELECT);
			break;
		
		case 2:
			go_to_state(STATE_MENU);
			break;

		default:
			break;
		}
	}
	else
	{
		if (pad1_new & PAD_UP && cur_option > 0)
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_UP);
			--cur_option;
		}
		else if (pad1_new & PAD_DOWN && cur_option < (NUM_PAUSE_OPTIONS - 1))
		{
			SFX_PLAY_WRAPPER(SOUND_UI_OPTION_DOWN);
			++cur_option;
		}

		oam_spr((8<<3)+ease_pulse[(ticks8/16)%4], (19 + (cur_option * 2))<<3, 0x02, 3|OAM_FLIP_H);
	}
}

const char empty_password[] = "0000";

void update_sound_test()
{
	static unsigned char local_cur_sound;
	static unsigned char local_cur_music;
	static unsigned char local_playing_music = -1;
	static unsigned char local_temp;
	static unsigned char local_i;
	static unsigned char local_setting_id;
	
	oam_clear();

	if (pad1_new & (PAD_START | PAD_B))
	{
		go_to_state(STATE_MENU);
	}
	else
	{
		if (pad1_new & PAD_UP && local_setting_id > 0)
		{
			--local_setting_id;
		}
		else if (pad1_new & PAD_DOWN && local_setting_id < 1)
		{
			++local_setting_id;
		}
		else if (pad1_new & PAD_LEFT)
		{
			switch (local_setting_id)
			{
				case 0: // sound fx
				{
					if (local_cur_sound > 0)
					{
						--local_cur_sound;
					}

					break;
				}
				case 1: // music
				{
					if (local_cur_music > 0)
					{
						--local_cur_music;
					}
					break;
				}
			}
		}
		else if (pad1_new & PAD_RIGHT)
		{
			switch (local_setting_id)
			{
				case 0: // sound fx
				{
					local_cur_sound = MIN(SOUND_MAX - 1, local_cur_sound + 1);

					break;
				}
				case 1: // music
				{
					local_cur_music = MIN(MUSIC_MAX - 1, local_cur_music + 1);
					break;
				}
			}
		}
		else if (pad1_new & PAD_A)
		{
			switch (local_setting_id)
			{
				case 0: // sound fx
				{
					sfx_play(local_cur_sound, 0);
					break;
				}

				case 1: // music
				{
					if (local_playing_music != local_cur_music)
					{
						music_play(local_cur_music);
						local_playing_music = local_cur_music;
					}
					else
					{
						music_stop();
						local_playing_music = -1;
					}
				}
			}
		}

		local_temp = local_cur_sound;
		local_i = 0;

		if (local_cur_sound < 100)
		{
			multi_vram_buffer_horz(empty_password, 3, get_ppu_addr(0,(19<<3),(12<<3)));
		}

		while(local_temp != 0)
		{
			unsigned char digit = local_temp % 10;
			one_vram_buffer('0' + digit, get_ppu_addr(0, (21<<3) - (local_i << 3), (12<<3) ));

			local_temp = local_temp / 10;
			++local_i;
		}

		local_temp = local_cur_music;
		local_i = 0;

		if (local_cur_sound < 100)
		{
			multi_vram_buffer_horz(empty_password, 3, get_ppu_addr(0,(19<<3),(14<<3)));
		}

		while(local_temp != 0)
		{
			unsigned char digit = local_temp % 10;
			one_vram_buffer('0' + digit, get_ppu_addr(0, (21<<3) - (local_i << 3), (14<<3) ));

			local_temp = local_temp / 10;
			++local_i;
		}		

		oam_spr(17<<3, (local_setting_id * 16) + ((12<<3)-1), 0x02, 1);
		oam_spr(23<<3, (local_setting_id * 16) + ((12<<3)-1), 0x02, 1|OAM_FLIP_H);
	}
}

// END HACK
// #pragma rodata-name ("BANK2")
// #pragma code-name ("BANK2")

void update_ending_screen()
{
	if (pad1_new & PAD_ANY_CONFIRM_BUTTON)
	{
		go_to_state(STATE_MENU);
	}
}

void load_boot_screen_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_BOOT]);
	pal_spr(BG_palettes[PAL_BG_BOOT]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(boot_screen);

	ppu_on_all();
}


void load_pause_menu_into_nametable()
{
	vram_adr(NTADR_C(0,0));
	vram_unrle(pause_menu);
}

void load_pause_menu_password_into_nametable()
{
	if (max_room_completed >= 0)
	{
		cur_pass_digits[0] = '0' + (passwords[max_room_completed] / 1000);
		cur_pass_digits[1] = '0' + ((passwords[max_room_completed] % 1000) / 100);
		cur_pass_digits[2] = '0' + ((passwords[max_room_completed] % 100) / 10);
		cur_pass_digits[3] = '0' + ((passwords[max_room_completed] % 10));	
	}
	else
	{
		memcpy(cur_pass_digits, empty_password, 4);
	}
	
	//vram_adr(NTADR_C(14,8));
	//vram_write(cur_pass_digits, 4);
	//14<<3, 8<<3
	multi_vram_buffer_horz(cur_pass_digits, 4, get_ppu_addr(index, in_x_tile, in_y_tile));
}

void load_level_select_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_STANDARD]);
	pal_spr(SPR_palettes[PAL_SPR_STANDARD]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(level_select_menu);

	ppu_on_all(); // turn on screen
	
	draw_level_select_room_number(cur_room_index + 1);
}

void load_sound_test_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_TITLE]);
	pal_spr(BG_palettes[PAL_BG_TITLE]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(sound_test);

	ppu_on_all(); // turn on screen
}

void load_new_game_menu_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_STANDARD]);
	pal_spr(BG_palettes[PAL_SPR_STANDARD]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(new_game_menu);

	ppu_on_all(); // turn on screen
}

void load_password_entry_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_STANDARD]);
	pal_spr(SPR_palettes[PAL_SPR_STANDARD]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(password_entry);

	ppu_on_all(); // turn on screen

	max_room_completed = -1;

	// cur_pass_digits[0] = '0' + (passwords[0] / 1000);
	// cur_pass_digits[1] = '0' + ((passwords[0] % 1000) / 100);
	// cur_pass_digits[2] = '0' + ((passwords[0] % 100) / 10);
	// cur_pass_digits[3] = '0' + ((passwords[0] % 10));	
}

void load_level_pal()
{
	banked_call(BANK_4, get_cur_room_palettes);

	pal_bg(BG_palettes[index]);
	pal_spr(SPR_palettes[index2]);
}

#pragma rodata-name ("BANK0")
#pragma code-name ("BANK0")


void load_pico8_credits_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_BOOT]);
	pal_spr(BG_palettes[PAL_BG_BOOT]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(pico_credits_screen);

	ppu_on_all();
}

void load_title_screen_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_TITLE]);
	pal_spr(BG_palettes[PAL_BG_TITLE]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(title_screen);

	ppu_on_all(); // turn on screen
}

void load_ending_into_nametable()
{
	ppu_off();

	pal_bg(BG_palettes[PAL_BG_ENDING]);
	pal_spr(BG_palettes[PAL_BG_ENDING]);

	vram_adr(NTADR_A(0,0));
	vram_unrle(ending);

	ppu_on_all(); // turn on screen	
}