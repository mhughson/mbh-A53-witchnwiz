/** (C) Matt Hughson 2020 */

// Note: all variables need extern and should be defined in main.c

#ifndef ONCE_MAIN_H
#define ONCE_MAIN_H

// test

#define DEBUG_ENABLED 0

#if DEBUG_ENABLED
#define PROFILE_POKE(val) //POKE((0x2001),(val));
#else
#define PROFILE_POKE(val)
#endif

#include "map_defs.h"

#define PROF_CLEAR 0x1e // none
#define PROF_R_TINT 0x3e // red
#define PROF_G_TINT 0x5e // green
#define PROF_B_TINT 0x9e // blue
#define PROF_W_TINT 0x1e // white
#define PROF_R 0x3f // red + grey
#define PROF_G 0x5f // green + grey
#define PROF_B 0x9f // blue + grey
#define PROF_W 0x1f // white + grey

#define NUM_BG_BANKS 2 // MUST BE POWER OF 2

#define ROOM_BG 0
#define ROOM_OBJS 1

#define MAX_WORLD_OBJECTS 8

#define INVALID_OBJECT_Y 0xffff

#define TILE_EMPTY 21
#define TILE_SOLID 17
#define TILE_LADDER 41
#define TILE_AI_HEAVY 66
#define TILE_AI_FLOAT 67
#define TILE_PUSH_BLOCK 68
#define TILE_SPAWN_POINT_WITCH 64
#define TILE_SPAWN_POINT_WIZ 65
#define TILE_WEB 69
#define TILE_DOOR 70
#define TILE_KEY 71
#define TILE_DOOR_OPEN 72
#define TILE_MIRROR 73

#define CELL_SIZE 16
#define MAX_FX_OBJECTS 8
#define NUM_STAR_ANIMS 3
#define MAX_HISTORY 2

#define GRID_XY_TO_ROOM_INDEX(x,y) (((y) * CELL_SIZE) + (x))

#define SMALL_STAR_1 4
#define SMALL_STAR_2 5
#define SMALL_STAR_3 6
#define BIG_STAR 7

#define META_TILE_FLAGS_OFFSET 5
#define GET_META_TILE_FLAGS(room_table_index) metatiles[current_room[(room_table_index)] * META_TILE_NUM_BYTES + META_TILE_FLAGS_OFFSET]

//#define SAVE_VERSION 1
//#define NUM_SAVE_VERSION_VALIDATION 4
//#define BEST_TIME_SINGLE_SAVE_SIZE 6
//#define BEST_TIME_TOTAL_SAVE_SIZE (NUM_ROOMS * BEST_TIME_SINGLE_SAVE_SIZE)

// The max number of nametable entries that can be saved in a single serialization.
// Asset in the saving code if going over this.
#define MAX_NAMETABLE_SAVE_QUEUE 16

#define FAST_FADE_DELAY 1

// temp till settings get worked out.
extern unsigned char sfx_on;
extern unsigned char music_on;

// NOTE: cur_sfx_chan goes out of range of FT_SFX_STREAMS, but that seems to be fine.
#define SFX_PLAY_WRAPPER(id) if (sfx_on) { sfx_play((id), ++cur_sfx_chan); }
// play a sound effect that is treated like music to the user (jingles, etc).
#define SFX_MUSIC_PLAY_WRAPPER(id) if (music_on) { sfx_play((id), ++cur_sfx_chan); }
#define MUSIC_PLAY_WRAPPER(id) if (music_on) { music_play((id)); }


// Induvidual flag meanings.
enum 
{  
	FLAG_SOLID = 1 << 0,
	FLAG_LADDER = 1 << 1,
	FLAG_WEB = 1 << 2,
	FLAG_PUSH_BLOCK = 1 << 3,
	FLAG_CAN_CARRY = 1 << 4,

	FLAG_MONSTER = 1 << 7,
};

enum
{
	MUSIC_TITLE_THEME = 0,
	MUSIC_GAMEPLAY_THEME,
	MUSIC_VICTORY_THEME,
	MUSIC_MIRROR_THEME,

	MUSIC_MAX,
};

enum
{
	SOUND_UI_OK = 0,
	SOUND_UI_CANCEL,
	SOUND_UI_OPTION_UP,
	SOUND_UI_OPTION_DOWN,

	SOUND_WALK,
	SOUND_JUMP_AT,
	SOUND_KILL_ENEMY,
	SOUND_LADDER_UP,
	SOUND_LADDER_DOWN,
	SOUND_PUSH_BLOCK,
	SOUND_DESTROY_WEB,
	SOUND_SWITCH_CHAR,
	SOUND_LEVEL_COMPLETE,
	SOUND_BLOCK_LAND,
	SOUND_GRAB_LADDER,
	SOUND_PLAYER_LAND,
	SOUND_PRESS_START,

	SOUND_RAMP_1,
	SOUND_RAMP_2,
	SOUND_RAMP_3,
	SOUND_RAMP_4,

	SOUND_SHATTER,

	SOUND_MAX,
};

// Combinations of flags which have combined meaning if ANY are true.
#define FLAGS_BLOCK_FALLING (FLAG_SOLID | FLAG_LADDER | FLAG_WEB | FLAG_PUSH_BLOCK | FLAG_MONSTER)
#define FLAGS_BLOCK_CLIMB (FLAG_SOLID | FLAG_PUSH_BLOCK | FLAG_MONSTER)
#define FLAGS_BLOCK_PUSH (FLAG_SOLID | FLAG_LADDER | FLAG_WEB | FLAG_PUSH_BLOCK | FLAG_MONSTER)

#define META_TILE_NUM_BYTES 6

// How many frames does it take the character to move to the next postition.
// TODO: code would be cleaner if this was 15
#define MOVE_FRAMES 16

#define ROOMS_PER_WORLD 4

#pragma bss-name(push, "ZEROPAGE")
#pragma bss-name(pop)

//extern unsigned char wram_array[0x500];
//extern unsigned char save_version_validation[NUM_SAVE_VERSION_VALIDATION];
//extern unsigned char wram_best_times[BEST_TIME_TOTAL_SAVE_SIZE];

//#pragma bss-name(push, "ZEROPAGE")

//#pragma bss-name(push, "BSS")

// System
//

// 7 shouldn't be needed, that's the fixed bank, just call it normally
enum { BANK_0 = 0, BANK_1 = 2, BANK_2 = 2, BANK_3 = 3, BANK_4 = 1, BANK_5 = 1, BANK_6 = 2 };

// Palettes
//

// Input
//

extern unsigned char pad1;
extern unsigned char pad1_new;

// Animation
//

typedef struct anim_info
{
	// index into sprite_anims array.
	unsigned char anim_current;
	unsigned char anim_queued;

	// how many ticks have passed since the last frame change.
	unsigned char anim_ticks;

	// the currently displaying frame of the current anim.
	unsigned char anim_frame;
} anim_info;

// Tracks bank animation.
extern unsigned char char_state;

extern anim_info* global_working_anim;// index into sprite_anims.

enum
{
	ANIM_IDLE_RIGHT,
	ANIM_WALK_RIGHT,
	ANIM_IDLE_LEFT,
	ANIM_WALK_LEFT,
	ANIM_CLIMB_UP,
	ANIM_CLIMB_DOWN,
	ANIM_IDLE_CLIMB,
	ANIM_FALL_RIGHT,
	ANIM_FALL_LEFT,
	ANIM_JUMP_RIGHT,
	ANIM_JUMP_LEFT,
	ANIM_ATTACK_RIGHT,
	ANIM_ATTACK_LEFT,
	ANIM_PUSH_RIGHT,
	ANIM_PUSH_LEFT,
	ANIM_LADDER_PUSH_RIGHT,
	ANIM_LADDER_PUSH_LEFT,
	ANIM_FX_STAR_LONG,
	ANIM_FX_STAR_MED,
	ANIM_FX_STAR_SHORT,
	ANIM_FX_STAR_HIT,
	ANIM_AI_HEAVY_IDLE,
	ANIM_AI_FLOAT_IDLE,
	ANIM_DUST,
	ANIM_DOOR,
	ANIM_CARD_SPIN,
	ANIM_PICKUP_RIGHT,
	ANIM_PICKUP_LEFT,

	ANIM_WALK_CARRY_RIGHT,
	ANIM_IDLE_CARRY_RIGHT,
	ANIM_WALK_CARRY_LEFT,
	ANIM_IDLE_CARRY_LEFT,
};

// Camera
//

extern unsigned char shake_remaining;
extern unsigned int cur_cam_x;
extern unsigned int cur_cam_y; 
extern unsigned char mirror_fade;

// World
//

// Which room is currently active.
extern unsigned char cur_room_index;

// Parameter to copy_original_room_to_current.
extern unsigned char tile_index_param;

// A copy of the current room data as it appears to the user. The changes as the player
// moves dynamic objects around and they settle.
// Reference the original room data for read-only version.
extern unsigned char current_room[240];

// Used to define constant parts of world objects.
typedef struct world_object_def
{
	// Meta tile index used to draw. Any animation will be done via bank swapped.
	unsigned char metatile;

	// The TILE_... used to represent this object in maps.
	unsigned char map_tile_id;
} world_object_def;

#define LERP_EASE_OUT_QUINT 0 // default
#define LERP_LINEAR 1

// data specific to non-player game objects.
typedef struct non_player_object_data
{
	// Lookup into object_def_table.
	// TODO: REMOVE (maybe, mostly unused).
	unsigned char object_def;

	// The lerp animations to use.
	unsigned char lerp_type_x;
	unsigned char lerp_type_y;

} non_player_object_data;

// data speciic to player game objects.
typedef struct player_object_data
{
	// On the levels where there are multiple active players, is this one the currently
	// active player?
	unsigned char is_cur_player;

	// Is this a witch clone player?
	unsigned char is_clone;

	// Is this a wizard player?
	unsigned char is_wiz;

	// TODO: Doesn't this put it over 16 bytes?
	unsigned char is_mirror;

	// Was the last horizontal move *attempted* in the left direction?
	unsigned char facing_left;

	// Stores all of the active animation info.
	anim_info anim;

} player_object_data;

// All objects are stored as game_object structs, but to avoid bloating the struct with unused
// data, each specialized object has its own sub struct. That struct gets stored as a union
// since that data should be mutually exclusive.
typedef union
{
	non_player_object_data obj_data;
	player_object_data player_data;

} game_object_data_union;

// The basic type 
typedef struct game_object
{
	game_object_data_union type_specific_data;

	// Is this object active at all. Can be completely ignored if this is false.
	unsigned char alive;

	// How long since the current move started.
	unsigned char ticks_since_move;

	// Position at the start of the current move.
	unsigned char start_x_grid;
	unsigned char start_y_grid;

	// Position the object is trying to move to.
	signed char dir_x;
	signed char dir_y;

	// Helper used when putting these objecs into a sorted list.
	struct game_object* next;

	//unsigned char padding[1]; // bring it up to power of 2 -> 16 bytes
} game_object;

typedef struct simple_fx
{
	unsigned char spawn_stars_on_death;

	unsigned char alive;

	// Posisition with fixed point math.
	signed int x;
	signed int y;

	// Direction the object is moving. Left 8 bits are the whole number, and right 8 bits are the floating point.
	signed int dir_x;
	signed int dir_y;

	// Stores all of the active animation info.
	anim_info anim;

	// if true, the object will fall.
	unsigned char floats;
	unsigned char sprite_offset;

} simple_fx;

extern unsigned char fx_spawn_stars_on_death[MAX_FX_OBJECTS];
extern unsigned char fx_alive[MAX_FX_OBJECTS];
extern signed int fx_x[MAX_FX_OBJECTS]; // Posisition with fixed point math.
extern signed int fx_y[MAX_FX_OBJECTS];
extern signed int fx_dir_x[MAX_FX_OBJECTS]; // Direction the object is moving. Left 8 bits are the whole number, and right 8 bits are the floating point.
extern signed int fx_dir_y[MAX_FX_OBJECTS];
extern anim_info fx_anim[MAX_FX_OBJECTS]; // Stores all of the active animation info.
extern unsigned char fx_floats[MAX_FX_OBJECTS];
extern unsigned char fx_sprite_offset[MAX_FX_OBJECTS]; // if non-zero, will use special sprite drawing with offsets built in.

typedef enum player_serialized_flags
{
	FLAG_FACING_LEFT = 1 << 0,
	FLAG_IS_CUR_PLAYER = 1 << 1,
	FLAG_IS_CLONE = 1 << 2,
	FLAG_IS_WIZ = 1 << 3,
	FLAG_IS_ALIVE = 1 << 4,
	FLAG_IS_MIRROR = 1 << 5,
};

typedef struct player_serialized
{
	unsigned char xy;
	// Multiple flags combined into 1 to save space. See: player_serialized_flags
	unsigned char flags;
} player_serialized;

typedef struct frame_serialized
{
	// player x, y
	player_serialized player_data[2]; // left 4 bits x, right 4 bits y

	// without objects representing enemies, we need to remember the previous value
	// to avoid parsing the entire nametable looking for enemy tiles.
	unsigned char enemies_remaining;

	// An array of all the nametable entries that changed this frame, and the 
	// value they should be restored to.
	unsigned char nametable_index[MAX_NAMETABLE_SAVE_QUEUE];
	unsigned char nametable_value[MAX_NAMETABLE_SAVE_QUEUE];
} frame_serialized;

enum { OBJ_DEF_BLOCK, OBJ_DEF_HEAVY_AI, OBJ_DEF_FLOAT_AI, OBJ_DEF_WEB, OBJ_DEF_DOOR, OBJ_DEF_KEY, OBJ_DEF_MAX };

extern game_object* players[2];
extern game_object player1;
extern game_object player2;
extern game_object* cur_player;
extern game_object objs[MAX_WORLD_OBJECTS];
extern game_object* objs_temp_head;
extern game_object* objs_temp_prev;
extern game_object* obj_temp;
extern game_object* target_obj; // param
extern frame_serialized serialized_frames[MAX_HISTORY];
extern unsigned char cur_history_index;
extern unsigned char cur_history_count;
extern unsigned char moving_object_count;
extern unsigned char block_movement;
extern unsigned char need_save;
extern unsigned char queued_stars_for_player_index;
extern unsigned char queue_player_swap;

// Gameplay
//

extern unsigned char cur_time_digits[6];


// Audio
//

// Counter used to ensure each sound effect ends up on a different
// channel. (no regard to priority)
extern unsigned char cur_sfx_chan;


// Game State
//

enum STATES { STATE_BOOT, STATE_P8, STATE_SOUND_TEST, STATE_MENU, STATE_MENU_TRANS, STATE_NEW_GAME_MENU, STATE_PASSWORD, STATE_MENU_LEVEL_SELECT, STATE_GAME, STATE_LEVEL_COMPLETE, STATE_PAUSE, STATE_ENDING };
extern unsigned char cur_state;

// Global Temps
//

#define KONAMI_CODE_LEN 10
extern const unsigned char konami_code[KONAMI_CODE_LEN];
extern unsigned char cur_konami_index;
extern unsigned char cur_option;
extern signed char max_room_completed;
extern unsigned char cur_pass_digits[4];
extern unsigned int cur_pass;
extern unsigned char x;
extern unsigned char y;
extern unsigned int x16;
extern unsigned int y16;
extern unsigned char i;
extern unsigned char i2;
extern unsigned char player_index;
extern unsigned char obj_index;
extern int address;
extern unsigned char index;
extern unsigned char index2;
extern unsigned char index3;
extern unsigned char ticks8;
extern unsigned int ticks16;
extern unsigned int ticks16_in_state;
extern signed char dir_temp;
extern signed char counter_signed;
extern unsigned char counter;
extern unsigned int index16;
extern unsigned char enemies_remaining;
extern unsigned char star_fx_index;
extern unsigned char loaded_obj_id;
extern unsigned char loaded_obj_index;
extern unsigned char grav_search_queue_x[3];
extern unsigned char grav_search_queue_y[3];
extern unsigned char cur_nametable_save_queue;
extern unsigned char nametable_value_save_queue[MAX_NAMETABLE_SAVE_QUEUE];
extern unsigned char nametable_index_save_queue[MAX_NAMETABLE_SAVE_QUEUE];
extern char in_x_tile; 
extern char in_y_tile;
extern unsigned char in_index_begin;
extern unsigned char in_index_end;
extern unsigned char in_lerp_type;

// had to put this in fixed bank due to being used for both logic and rendering.
extern const unsigned char metatiles[];

unsigned char find_next_free_fx();
void queue_next_anim(unsigned char next_anim_index);
void commit_next_anim();
void go_to_state(unsigned char next_state);

void fade_to_black();
void fade_from_black();
void fade_to_black_fast();
void fade_from_black_fast();
void fade_to_white();
void fade_from_white();
void vram_buffer_load_2x2_metatile();

#endif // ONCE_MAIN_H