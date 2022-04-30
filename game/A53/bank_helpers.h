// Contains functions to help with working with multiple PRG/CHR banks

// Maximum level of recursion to allow with banked_call and similar functions.

#ifndef A0A9308F_36AC_4592_AF8A_4A5584C7B746
#define A0A9308F_36AC_4592_AF8A_4A5584C7B746

#define MAX_BANK_DEPTH 10

// NOTE: Only the right most 2 bits are for mirroring. The other bits
//       need to match was your game uses and was set during reset in
//       crt.s (see "Mapper reset")
#define MIRROR_MODE_1_SCREEN_LOWER 0x2c
#define MIRROR_MODE_1_SCREEN_UPPER 0x2d
#define MIRROR_MODE_VERT 0x2e
#define MIRROR_MODE_HORZ 0x2f

extern unsigned char bankLevel;
extern unsigned char bankBuffer[MAX_BANK_DEPTH];



// Switch to another bank and call this function.
// Note: Using banked_call to call a second function from within  
// another banked_call is safe. This will break if you nest more  
// than 10 calls deep.
void banked_call(unsigned char bankId, void (*method)(void));


// Internal function used by banked_call(), don't call this directly.
// Switch to the given bank, and keep track of the current bank, so 
// that we may jump back to it as needed.
void bank_push(unsigned char bankId);


// Internal function used by banked_call(), don't call this directly.
// Go back to the last bank pushed on using bank_push.
void bank_pop(void);

// Switch to the given bank (to $8000-bfff). Your prior bank is not saved.
// Can be used for reading data with a function in the fixed bank.
// bank_id: The bank to switch to.
void __fastcall__ set_prg_bank(unsigned char bank_id);


// Get the current PRG bank at $8000-bfff.
// returns: The current bank.
unsigned char __fastcall__ get_prg_bank(void);


// Set the current 1st 4k chr bank to the bank with this id.
void __fastcall__ set_chr_bank_0(unsigned char bank_id);


// Set what chr bank to set at the top of the screen, for a split screen.
void __fastcall__ set_nmi_chr_tile_bank(unsigned char bank);


// Don't change the chr bank at the top of the screen.
void __fastcall__ unset_nmi_chr_tile_bank(void);

// Change the current mirror mode.
// See: MIRROR_MODE_* at top of this file for "mode" params.
void __fastcall__ set_mirror_mode(unsigned char mode);

// some things deleted


#endif /* A0A9308F_36AC_4592_AF8A_4A5584C7B746 */
