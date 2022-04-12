#ifndef D12B32D3_39DD_4865_9236_3AEDEF9ACEDE
#define D12B32D3_39DD_4865_9236_3AEDEF9ACEDE

// There should be no globals here; just function prototypes.
// Non constant (RAM) variables go in common header.
// Constant (ROM) variables go in this c file, so that they are no accessed by accident by other files.

// To make things simpler, this function can be called
// multiple times in the same frame without actually changing
// anything. If at the end of the frame, the last anim queued is
// different than the current anim playing, THEN it will do the
// work to reset everything.

void main_real(void);
void spawn_stars_at_player();
// in_x_tile
// in_y_tile
void spawn_stars_at_tile();
void draw_cur_time();
void draw_best_time_level_select();
void load_current_map();
void load_current_map_force_ppu_off();
void draw_time_background_ppu_off();
void restore_room_delta();
// param: game_object* target_obj
unsigned char is_obj_below();
// param: player_index
// param: cur_player
unsigned char is_other_player_in_dir(signed char in_x, signed char in_y);
unsigned char find_next_free_fx();
void force_update_player_idle_anim();

#endif /* D12B32D3_39DD_4865_9236_3AEDEF9ACEDE */
