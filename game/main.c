/** (C) Matt Hughson 2020 */

#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "A53/bank_helpers.h"
//#include "MMC1/bank_helpers.c"
#include "../include/stdlib.h"
#include "main.h"
#include "PRG0.h"
#include "PRG1.h"

/*
	IDEAS:
	* SWITCH (not add) to a new mechanic every 5-10 levels to keep things fresh.
	* Lava rising more platfomer-like levels.
	* Exit - helps hint at the last enemy to kill. Pick up key.
	* Multiple keys?
	* Optional pickups.
	* One way blocks.
	* Secret exits.
	* Key you can pick up and throw up one level, like Mario 2.
		* Hop into to pick up.
		* Blocks movement through <2 open spots.
		* Can throw under 1 open spot and kick throught? Might make blocking pointless
		* 
	* Reflection on floor on levels with new mechanics. Limited metasprites needed maybe (just walk?).
	* Fade out the background as the player moves towards an exit for a cool trippy effect. Maybe the final boss pops in after black.
	* lightning flashes.
	* Optional hints like "go left first" or "the cobwebs are key"
	* What if the NESDev build, since it can't have best time and stuff, was more of a linear, how far can you get in say, 10 minutes.
		* On top of that there will be secret paths, which will shorten the trip the subsequent times through.
		* This might actually be more fun than time attack.
		* It would be really cool if these are things that the player COULD do the first time through if they know what to do,
		rather than being give a pass code they would have know what of knowing etc.
	* Labrynth levels, where the board loops left/right up/down. Have some crazy stairs the the background.

	TOOL NEEDS:
	* Covert dynamics layer to x,y,tileid (x,y could be packed into 1 byte).
	* [done] Generate all files on compile.
	* [done] Generated files directly into h files.
	* [done] Metatile flags (solid, ladder, etc).
	* [done] Give each bank its own c file, and put all constants for that bank in the c file, so that trying to access them from
	  another bank file gives compiler error.
	  RAM can go in header since it is accessible everywhere.

	ANIMATION ISSUES:
	* [done] Kick from ladder rotates whole body. prefer side kick.

	SAVE FORMAT:
	* [ rand8() ] XOR [ last level completed ] XOR [ check sum ]
	* random number should not change after password first set.
	* Seems to fail with such little data.
	* Instead maybe just have 32 hardcoded passwords in an array?


	TODO:
	* Detect when a duplicate save frame occurs, and move current save index back to that point, to avoid unbounded growth.
	* [done] Get rid of pause between movements as much as possible. seems like there might be extra frames.
	* [done] Make a really big level for worst case testing.
	* [done] Pause Menu
	* [done] Sound Test
	* [done] Static meta tiles could possibly also move.
	* [done] Frame save should only include nametable delta, allowing for much higher number of save frames.
	* [done] Compltely remove the concept of world objects, other than the player.
		- instead everything will be current_room array, and moving object will use temp sprites similar to simple vfx (maybe even actually using them)
		- should make almost everying quicker, saving/restore included.
	* [done] Clear up dead code after re-write.
	* [done] Level data should move to banks.
	* [done] Potention bug with game objects using player 0's search index (for falling).


	::2021 NES DEV COMPO::

	MUST FIX:
	* FIXED - BUG: Can walk off the left side of the screen on the last level. (https://discord.com/channels/731554439055278221/806935376069132328/807035962600718396)
	* FIXED - BUG: Undo while block is falling causes it to vanish. (https://discord.com/channels/731554439055278221/806935376069132328/806992178306875412)

	SHOULD FIX:
	* FIXED - Put a border on Level complete.
	* FIXED - BUG: Sound effects cut out a lot.
	* FIXED - BUG: 5-4 can move clone through box. (https://discord.com/channels/731554439055278221/806935376069132328/806983307119689730)
	* FIXED - Press Start should work with any button.
	* FIXED [kept them but moved 6-1 to the end] - Replace hard levels (6-1, 6-3 probably) with simple Key puzzles prior to finding the Wizard.
	* FIXED - Move 7-3 to end.
	* FIXED - Replace attic with key, and put it before wiz.
	* FIXED - Add outline to "THE END"
	* FIXED [thanks Vectrex] - Famicom controller support. (https://discord.com/channels/731554439055278221/806935376069132328/808345834067591181)

	NICE TO HAVE:
	* FIXED - Show password on Level Complete or even at the top of the screen.
	* FIXED - Show level number somewhere during gameplay.
	* FIXED - Press B to return to the Title Screen from New Game Screen.
	* FIXED - Sound effect on mirror build up.
	* FIXED - Music should not play on mirror level (ideally get heart thump)
	* FIXED - Sound effect on enter door.
	* FIXED - Sky for last level.
	* FIXED - BUG: Player sprite overlaps "Level Complete"
	* FIXED - Wobble "Level Complete"
	* Jump animation when moving to door.
	* BUG: Can kill enemy if falling in the same move as kill. (https://discord.com/channels/731554439055278221/806935376069132328/806983992989581353)
	* Screen shake does not reset if you reset the level (you get a few frames of shake after fade from black).
	* Clouds for 2nd level. (maybe add a key?)

	CUT:
	* CUT [will include controls in release] - Undo is not explained.
	* CUT [will include controls in release] - Restart is not explained.
	* CUT [people seem to be figuring it out] - Cobwebs, and Ghosts should be better explained.

	LEVEL FEEDBACK:
	* 3-1 +
	* 3-2 +
	* 3-4 - (tedious)
	* 4-1 - (very tedious)
	* 6-1 -
	* 6-3 +- Very Hard


	KEY INFO:
	1) Objects need only check for falling after a) sliding left/right and b) something below them moved.
	2) It is also preceeded by a player action.
	3) When ever an object finishes moving, it should check if it should fall.

	* player action (move, push, kill)
	* wait for action to complete.
	* start search up from newly cleared position.
		- bottom up is important for eventual "check if obj below" call as things reach destination.
	* if find physics object, spawn "mover" and break.
	* continue search next frame.
	* halt once hitting top of level.
	* wait for everything to complete.

	BUGS:
	* Rewind during fall results in missing objects (they return if you keep rewinding).
	* Can move onto falling block if player movest left the right, with block starting on top.
	* Clone player sometimes flickers. Suspect it could be either the pal change, or the frame is just going over time.
	* [done] Clone in mirror flips after fade in.
	* [done] Mirror clone can escape mirror on 2nd level.
	* Sprite flicker on level select.
	* [fixed] Can move players at slightly different times, resulting in the ability to cross gaps they shouldn't be able to.
		A.E|
		..B|
	  A starts moving right, and then immediately B moves left. By the time A checks for falling, B is considered below (x + dir).
	  Solution may be to not take dir into acount, but there is probably another bug when stacked players fall.
	  Solution is probably to prevent player movement until ALL players have stopped moving (maybe even all objects).
	* [fixed] If you drop a block by destroying a web multiple tiles down, it will still trigger block to drop. it should only check 1 spot above.
		Can result in level being impossible to complete (abby bug).
	* [fixed] Skipping to the first BIG Team level causes purple sprite palette to get messed up (falling heavy). Confusing given change in memory location (BSS).
	* [fixed] Can push block through clone.
	* [fixed] Clearying web with HEAVY above, doesn't trigger gravity.

	ART REQUESTS:
	* Door sprite - maybe a magic book which teleports the player?
	* Level complete sequence


	ROM BREAKDOWN:
	PRG0: Core gameplay logic.
	PRG1: Drawing gameplay logic. Animation data. Meta sprites.
	PRG2: -- (test function)
	PRG3: --
	PRG4: Level data
	PRG5: Loading logic, rewind serialization.
	PRG6: Sound and Music.
	PRG7: FIXED BANK. Famitone5. DMC samples. Meta tiles. Some shared animation functions.

	CHR0-A: DungeonA
	CHR0-B: --
	CHR1-A: DungeonB
	CHR1-B: Sprites
	
*/

/*

	UPDATING META TILES:
	* Open "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\NES_ST\test_screen.nss"
	* Update meta tiles at top of tile.
	* Export -> Nametable as BMP: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\NES_ST\nametable.bmp"
	* Nametable -> Save Nametable and Attributes: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\NES_ST\test_meta_tiles.nam"
	* Open in Tiled: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\nametable.tsx"
	* Edit flagN as needed. Save.
	* Export: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\nametable.json"
	* In VS: Run "Build Maps"...
	* This will output: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\meta_tiles.h"
	* Compile, Run.

	UPDATE MAPS:
	* NO LONGER NEEDED: Just edit map, save TMX, and run the "Build Maps" build task! (it will auto-export json for you)
	*
	* Open in tiled: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\small_map.tmx"
	* Place tiles from: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\nametable.tsx"
	* Export As -> json: small_map.json
	* This will generate a json file: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\small_map.json"
	* Run "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MAPS\generate_maps_header.py"
	* This will output: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\maps.h"
	* Compile, Run.

	UPDATE MUSIC:
	* In Famitracker, File->Export Text: "C:\Users\Matt\Documents\Dev\NES\mbh-secondnes\game\MUSIC\songs.txt"
	* Build->Build Audio

*/

// It's not clear to me why this wasn't need before, but after removing XRAM,
// this is now required.
#pragma rodata-name ("CODE")
#pragma code-name ("CODE")

//#include "PRG0.c"
//#pragma bss-name(push, "XRAM")
//unsigned char wram_array[0x2000];
// MUST BE FIRST ENTRY IN XRAM. USED FOR MEMSET.
//unsigned char save_version_validation[NUM_SAVE_VERSION_VALIDATION];
//unsigned char wram_best_times[BEST_TIME_TOTAL_SAVE_SIZE];
frame_serialized serialized_frames[MAX_HISTORY];
//#pragma bss-name(pop)

// global definitions.
const unsigned char konami_code[KONAMI_CODE_LEN] = { PAD_UP, PAD_UP, PAD_DOWN, PAD_DOWN, PAD_LEFT, PAD_RIGHT, PAD_LEFT, PAD_RIGHT, PAD_B, PAD_A };
unsigned char cur_konami_index;
unsigned char cur_option;
signed char max_room_completed;
unsigned char cur_pass_digits[4] = { "0000" };
unsigned int cur_pass;
unsigned char pad1;
unsigned char pad1_new;
unsigned char char_state;
anim_info* global_working_anim;
unsigned char shake_remaining;
unsigned int cur_cam_x;
unsigned int cur_cam_y; 
unsigned char mirror_fade;
unsigned char cur_room_index;
unsigned char tile_index_param;
unsigned char current_room[240];
game_object* players[2];
game_object player1;
game_object player2;
game_object* cur_player;
game_object objs[MAX_WORLD_OBJECTS];
game_object* objs_temp_head;
game_object* objs_temp_prev;
game_object* obj_temp;
game_object* target_obj; // param
unsigned char fx_spawn_stars_on_death[MAX_FX_OBJECTS];
unsigned char fx_alive[MAX_FX_OBJECTS];
signed int fx_x[MAX_FX_OBJECTS]; // Posisition with fixed point math.
signed int fx_y[MAX_FX_OBJECTS];
signed int fx_dir_x[MAX_FX_OBJECTS]; // Direction the object is moving. Left 8 bits are the whole number, and right 8 bits are the floating point.
signed int fx_dir_y[MAX_FX_OBJECTS];
anim_info fx_anim[MAX_FX_OBJECTS]; // Stores all of the active animation info.
unsigned char fx_floats[MAX_FX_OBJECTS];
unsigned char fx_sprite_offset[MAX_FX_OBJECTS]; // if non-zero, will use special sprite drawing with offsets built in.
unsigned char cur_time_digits[6];
unsigned char cur_sfx_chan;
unsigned char cur_state;
unsigned char x;
unsigned char y;
unsigned int x16;
unsigned int y16;
unsigned char i;
unsigned char i2;
unsigned char player_index;
unsigned char obj_index;
int address;
unsigned char index;
unsigned char index2;
unsigned char index3;
unsigned char ticks8;
unsigned int ticks16;
unsigned int ticks16_in_state;
signed char dir_temp;
signed char counter_signed;
unsigned char counter;
unsigned int index16;
unsigned char cur_history_index;
unsigned char cur_history_count;
unsigned char moving_object_count;
unsigned char block_movement;
unsigned char need_save;
unsigned char queued_stars_for_player_index;
unsigned char queue_player_swap;
unsigned char enemies_remaining;
unsigned char star_fx_index;
unsigned char loaded_obj_id;
unsigned char loaded_obj_index;
unsigned char grav_search_queue_x[3];
unsigned char grav_search_queue_y[3];
unsigned char cur_nametable_save_queue;
unsigned char nametable_value_save_queue[MAX_NAMETABLE_SAVE_QUEUE];
unsigned char nametable_index_save_queue[MAX_NAMETABLE_SAVE_QUEUE];
// TODO: Not unsigned for some reason? Why?
char in_x_tile; 
char in_y_tile;
unsigned char in_index_begin;
unsigned char in_index_end;
unsigned char in_lerp_type;
unsigned char sfx_on = 1;
unsigned char music_on = 1;
unsigned char cur_sfx_chan;

#pragma rodata-name ("CODE")
#pragma code-name ("CODE")

#include "meta_tiles.h"

const unsigned char title_text[] = "WITCH N' WIZ!";
const unsigned char level_select_title_text[] = "LEVEL SELECT";

// It seems like main() MUST be in fixed back!
void main (void) 
{
	// just right away kick off into another bank.
	banked_call(BANK_0, main_real);
}

unsigned char find_next_free_fx()
{
	static unsigned char local_i;

	for (local_i = 0; local_i < SMALL_STAR_1; ++local_i)
	{
		if (fx_alive[local_i] == 0)
		{
			fx_spawn_stars_on_death[local_i] = 0;
			fx_alive[local_i] = 1;
			//fx_x[local_i] = 0; // Posisition with fixed point math.
			//fx_y[local_i] = 0;;
			fx_dir_x[local_i] = 0; // Direction the object is moving. Left 8 bits are the whole number, and right 8 bits are the floating point.
			fx_dir_y[local_i] = 0;
			//fx_anim[local_i]; // Stores all of the active animation info.
			fx_floats[local_i] = 0;
			fx_sprite_offset[local_i] = 0; // if non-zero, will use special sprite drawing with offsets built in.
		
			return local_i;
		}						
	}

	return 0xff;
}

void queue_next_anim(unsigned char next_anim_index)
{
	global_working_anim->anim_queued = next_anim_index;
}

void commit_next_anim()
{
	if (global_working_anim->anim_queued != 0xff && global_working_anim->anim_queued != global_working_anim->anim_current)
	{
		global_working_anim->anim_current = global_working_anim->anim_queued;
		global_working_anim->anim_frame = 0;
		global_working_anim->anim_ticks = 0;
	}

	global_working_anim->anim_queued = 0xff;
}

void go_to_state(unsigned char next_state)
{
	if (next_state == cur_state)
	{
		return;
	}

	// switch exit
	switch (cur_state)
	{
	case STATE_MENU:
	{
		cur_konami_index = 0;
		music_stop();
		break;
	}
	case STATE_PAUSE:
	{
		pal_bright(4);
		break;
	}

	default:
		break;
	}

	// switch enter
	switch (next_state)
	{
	case STATE_BOOT:
		pal_bright(0);
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		banked_call(BANK_1, load_boot_screen_into_nametable);
		fade_from_black_fast();	
		break;

	case STATE_P8:
		fade_to_black_fast();
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		//bank_bg(0);
		//bank_spr(1);
		banked_call(BANK_0, load_pico8_credits_into_nametable);
		fade_from_black_fast();	
		break;

	case STATE_SOUND_TEST:
		SFX_PLAY_WRAPPER(SOUND_PRESS_START);
		fade_to_black_fast();
		oam_clear();
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		banked_call(BANK_1, load_sound_test_into_nametable);
		music_stop();
		fade_from_black();
		break;

	case STATE_MENU:
		fade_to_black_fast();
		set_chr_bank_0(2); // switch the BG bank
		//set_chr_bank_1(5); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		banked_call(BANK_0, load_title_screen_into_nametable);
		// Could be coming from pause, and pause can't reset the scroll
		// because it will happen before the fade to black.
		scroll(0, 0);
		MUSIC_PLAY_WRAPPER(MUSIC_TITLE_THEME);
		fade_from_black();
		break;

	case STATE_MENU_TRANS:
	{
		//SFX_MUSIC_PLAY_WRAPPER(SOUND_LEVEL_COMPLETE);
		SFX_PLAY_WRAPPER(SOUND_PRESS_START);
		break;
	}

	case STATE_NEW_GAME_MENU:
	{
		fade_to_black_fast();
		oam_clear();
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		// Could be coming from pause, and pause can't reset the scroll
		// because it will happen before the fade to black.
		cur_option = 1;
		scroll(0, 0);
		banked_call(BANK_1, load_new_game_menu_into_nametable);
		fade_from_black_fast();		
		break;
	}

	case STATE_PASSWORD:
		fade_to_black_fast();
		oam_clear();
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		banked_call(BANK_1, load_password_entry_into_nametable);
		fade_from_black_fast();
		break;

	case STATE_MENU_LEVEL_SELECT:
		fade_to_black_fast();
		oam_clear();
		set_chr_bank_0(0); // switch the BG bank
		//set_chr_bank_1(1); // switch the spr bank
		bank_bg(0);
		bank_spr(1);
		// Could be coming from pause, and pause can't reset the scroll
		// because it will happen before the fade to black.
		scroll(0, 0);
		in_x_tile = 5;
		banked_call(BANK_1, load_level_select_into_nametable);
		fade_from_black_fast();
		break;

	case STATE_GAME:

		if (cur_state != STATE_PAUSE)
		{
			fade_to_black();
			banked_call(BANK_0, load_current_map_force_ppu_off);
			banked_call(BANK_1, load_level_pal);

			// TODO: I think this is causing sprites to pop. It should really
			// 		 move all sprites into place and THEN fade up.
			oam_clear();

			if (mirror_fade != 0xff)
			{
				
				MUSIC_PLAY_WRAPPER(MUSIC_MIRROR_THEME);
				// We know that the BG will want to start partially faded.
				pal_bright(1);
				delay(2);
				pal_spr_bright(2);
				delay(2);
				pal_spr_bright(3);
				delay(2);
				pal_spr_bright(4);
				//pal_bg_bright(MIN(player1.start_x_grid,4));
			}
			else
			{
				// Don't play music on mirror level.
				MUSIC_PLAY_WRAPPER(MUSIC_GAMEPLAY_THEME);
				fade_from_black();	
			}			
		}
		break;

	case STATE_LEVEL_COMPLETE:
	{
		// stop the gameplay music and trigger to jingle.
		MUSIC_PLAY_WRAPPER(MUSIC_VICTORY_THEME);

		// Did we go beyond the previous max?
		if (max_room_completed == -1 || max_room_completed < cur_room_index)
		{
			max_room_completed = cur_room_index;
		}
		
		break;
	}

	case STATE_PAUSE:

		// Setup the pause menu which lives in the 2nd nametable for the life
		// of the program.
		in_x_tile = 14<<3;
		in_y_tile = 8<<3;
		index = 2;

		banked_call(BANK_1, load_pause_menu_password_into_nametable);
		oam_clear();
		// NOTE: Gameplay resets scroll in update, so this doesn't need to be
		//		 reset currently.
		scroll(0, 240);
		cur_option = 0;
		break;

	case STATE_ENDING:
		fade_to_black_fast();
		set_chr_bank_0(2); // switch the BG bank
		//set_chr_bank_1(7); // switch the spr bank
		bank_bg(1);
		bank_spr(0);
		oam_clear();
		banked_call(BANK_0, load_ending_into_nametable);
		fade_from_black_fast();
		break;
	default:
		break;
	}

	// wait to do the assign until the end, so that enter and exit can query both values.
	cur_state = next_state;

	ticks16_in_state = 0;
}


void fade_to_black()
{
	pal_bright(3);
	delay(2);
	pal_bright(2);
	delay(2);
	pal_bright(1);
	delay(2);
	pal_bright(0);
	delay(2);
}

void fade_from_black()
{
	pal_bright(1);
	delay(2);
	pal_bright(2);
	delay(2);
	pal_bright(3);
	delay(2);
	pal_bright(4);
	delay(2);
}

void fade_to_black_fast()
{
	pal_bright(3);
	delay(FAST_FADE_DELAY);
	pal_bright(2);
	delay(FAST_FADE_DELAY);
	pal_bright(1);
	delay(FAST_FADE_DELAY);
	pal_bright(0);
	delay(FAST_FADE_DELAY);
}

void fade_from_black_fast()
{
	pal_bright(1);
	delay(FAST_FADE_DELAY);
	pal_bright(2);
	delay(FAST_FADE_DELAY);
	pal_bright(3);
	delay(FAST_FADE_DELAY);
	pal_bright(4);
	delay(FAST_FADE_DELAY);
}

void fade_to_white()
{
	pal_bright(5);
	delay(FAST_FADE_DELAY);
	pal_bright(6);
	delay(FAST_FADE_DELAY);
	pal_bright(7);
	delay(FAST_FADE_DELAY);
	pal_bright(8);
	delay(FAST_FADE_DELAY);
}

#define SLOW_FADE_DELAY 10
void fade_from_white()
{
	pal_bright(7);
	delay(SLOW_FADE_DELAY);
	pal_bright(6);
	delay(SLOW_FADE_DELAY);
	pal_bright(5);
	delay(SLOW_FADE_DELAY);
	pal_bright(4);
	delay(SLOW_FADE_DELAY);
}

void vram_buffer_load_2x2_metatile()
{
	// Function gets called from a lot of places, so not safe to use globals.
	static unsigned char local_x;
	static unsigned char local_y;
	static unsigned char local_i;
	static unsigned int local_index16;
	static unsigned int local_att_index16;

	// SPRITES

	local_index16 = GRID_XY_TO_ROOM_INDEX(in_x_tile, in_y_tile);
	local_att_index16 = current_room[local_index16] * META_TILE_NUM_BYTES;
	multi_vram_buffer_horz(&metatiles[local_att_index16], 2, get_ppu_addr(0, in_x_tile * CELL_SIZE, in_y_tile * CELL_SIZE));
	multi_vram_buffer_horz(&metatiles[local_att_index16+2], 2, get_ppu_addr(0, in_x_tile * CELL_SIZE, (in_y_tile * CELL_SIZE) + 8));

	// ATTRIBUTES

	// Attributes are in 2x2 meta tile chunks, so we need to round down to the nearest,
	// multiple of 2 (eg. if you pass in index 5, we want to start on 4).
	local_x = (in_x_tile / 2) * 2;
	local_y = (in_y_tile / 2) * 2;

	local_i = 0;

	// room index.
	local_index16 = GRID_XY_TO_ROOM_INDEX(local_x, local_y);
	// meta tile palette index.
	local_att_index16 = (current_room[local_index16] * META_TILE_NUM_BYTES) + 4;
	// bit shift amount
	local_i |= (metatiles[local_att_index16]);

	local_index16 = local_index16 + 1; //(local_y * 16) + (local_x + 1);
	local_att_index16 = (current_room[local_index16] * META_TILE_NUM_BYTES) + 4;
	local_i |= (metatiles[local_att_index16]) << 2;

	local_index16 = local_index16 + 15; //((local_y + 1) * 16) + (local_x);
	local_att_index16 = (current_room[local_index16] * META_TILE_NUM_BYTES) + 4;
	local_i |= (metatiles[local_att_index16]) << 4;

	local_index16 = local_index16 + 1; //((local_y + 1) * 16) + (local_x + 1);
	local_att_index16 = (current_room[local_index16] * META_TILE_NUM_BYTES) + 4;
	local_i |= (metatiles[local_att_index16]) << 6;	

	one_vram_buffer(local_i, get_at_addr(0, (local_x) * CELL_SIZE, (local_y) * CELL_SIZE));
}