; Startup code for cc65 and Shiru's NES library
; based on code by Groepaz/Hitmen <groepaz@gmx.net>, Ullrich von Bassewitz <uz@cc65.org>




; Edited to work with MMC1 code
.define SOUND_BANK 2


FT_BASE_ADR		= $0100		;page in RAM, should be $xx00
FT_DPCM_OFF		= $f000		;$c000..$ffc0, 64-byte steps
FT_SFX_STREAMS	= 4			;number of sound effects played at once, 1..4

FT_THREAD       = 1		;undefine if you call sound effects in the same thread as sound update
FT_PAL_SUPPORT	= 1		;undefine to exclude PAL support
FT_NTSC_SUPPORT	= 1		;undefine to exclude NTSC support
FT_DPCM_ENABLE  = 1		;undefine to exclude all DMC code
FT_SFX_ENABLE   = 1		;undefine to exclude all sound effects code





;REMOVED initlib
;this called the CONDES function

    .export _exit,__STARTUP__:absolute=1
	.import push0,popa,popax,_main,zerobss,copydata

; Linker generated symbols
	.import __STACK_START__   ,__STACKSIZE__ ;changed
	.import __ROM0_START__  ,__ROM0_SIZE__
	.import __STARTUP_LOAD__,__STARTUP_RUN__,__STARTUP_SIZE__
	.import	__CODE_LOAD__   ,__CODE_RUN__   ,__CODE_SIZE__
	.import	__RODATA_LOAD__ ,__RODATA_RUN__ ,__RODATA_SIZE__
	.import NES_MAPPER, NES_PRG_BANKS, NES_CHR_BANKS, NES_MIRRORING
    
	.import upload_chars

    .include "zeropage.inc"


PPU_CTRL	=$2000
PPU_MASK	=$2001
PPU_STATUS	=$2002
PPU_OAM_ADDR=$2003
PPU_OAM_DATA=$2004
PPU_SCROLL	=$2005
PPU_ADDR	=$2006
PPU_DATA	=$2007
PPU_OAM_DMA	=$4014
PPU_FRAMECNT=$4017
DMC_FREQ	=$4010
CTRL_PORT1	=$4016
CTRL_PORT2	=$4017

OAM_BUF		=$0200
;PAL_BUF		=$01c0
VRAM_BUF	=$0700



.segment "ZEROPAGE"

NTSC_MODE: 			.res 1
FRAME_CNT1: 		.res 1
FRAME_CNT2: 		.res 1
VRAM_UPDATE: 		.res 1
NAME_UPD_ADR: 		.res 2
NAME_UPD_ENABLE: 	.res 1
PAL_UPDATE: 		.res 1
PAL_BG_PTR: 		.res 2
PAL_SPR_PTR: 		.res 2
SCROLL_X: 			.res 1
SCROLL_Y: 			.res 1
SCROLL_X1: 			.res 1
SCROLL_Y1: 			.res 1
PAD_STATE: 			.res 2		;one byte per controller
PAD_STATEP: 		.res 2
PAD_STATET: 		.res 2
PPU_CTRL_VAR: 		.res 1
PPU_CTRL_VAR1: 		.res 1
PPU_MASK_VAR: 		.res 1
RAND_SEED: 			.res 2
FT_TEMP: 			.res 3
BANK_WRITE_IP:		.res 1

TEMP: 				.res 11
SPRID:				.res 1

PAD_BUF		=TEMP+1

PTR			=TEMP	;word
LEN			=TEMP+2	;word
NEXTSPR		=TEMP+4
SCRX		=TEMP+5
SCRY		=TEMP+6
SRC			=TEMP+7	;word
DST			=TEMP+9	;word

RLE_LOW		=TEMP
RLE_HIGH	=TEMP+1
RLE_TAG		=TEMP+2
RLE_BYTE	=TEMP+3

OFFSET:				.res 1
PAL_OVERRIDE:		.res 1

;nesdoug code requires
VRAM_INDEX:			.res 1
META_PTR:			.res 2
DATA_PTR:			.res 2


; bss segment added my mhughson. not really sure if this is correct :|
.segment "BSS"

PAL_BUF:			.res 32 ; originally hardcode to $01c0, inside stack memory, be was getting hit with stack overflow.

.segment "HEADER"

    .byte $4e,$45,$53,$1a
	.byte <NES_PRG_BANKS
	.byte <NES_CHR_BANKS
	.byte <NES_MIRRORING|(<NES_MAPPER&$f<<4); ;|2 ;battery save
	.byte <NES_MAPPER&$f0|8
	.byte $00,$00,$00 ;$07 ;8kb work RAM
	.byte $09 ;32k of CHR RAM
	.byte $00,$00,$00,$00


; linker complains if I don't have at least one mention of each bank
.segment "ONCE"
.segment "BANK0"
.segment "BANK1"
.segment "BANK2"
; .segment "BANK3"
; .segment "BANK4"
; .segment "BANK5"
; .segment "BANK6"

.segment "STARTUP"

start:
_exit:

    sei
	cld
	ldx #$40
	stx CTRL_PORT2
    ldx #$ff
    txs
    inx
    stx PPU_MASK
    stx DMC_FREQ
    stx PPU_CTRL		;no NMI
	
clearRAM:
    txa
@1:
    sta $000,x
    sta $100,x
    sta $200,x
    sta $300,x
    sta $400,x
    sta $500,x
    sta $600,x
    sta $700,x
    inx
    bne @1

	;x is still zero
	
; Mapper reset

	lda #$81		; outer PRG bank
	sta A53_REG_SELECT
	lda #7	; 
	sta A53_REG_VALUE
	
	lda #$80		; mode
	sta A53_REG_SELECT
	lda #%101111	; 
	sta A53_REG_VALUE
	

	lda #$00 ;CHR bank #0 for first tileset
	jsr _set_chr_bank_0
	
	;lda #$01 ;CHR bank #1 for second tileset
	;jsr _set_chr_bank_1
	
	lda #$01 ;PRG bank #1 at $8000
	jsr _set_prg_bank
	

initPPU:
    bit PPU_STATUS
@1:
    bit PPU_STATUS
    bpl @1
@2:
    bit PPU_STATUS
    bpl @2

clearPalette:
	lda #$3f
	sta PPU_ADDR
	stx PPU_ADDR
	lda #$0f
	ldx #$20
@1:
	sta PPU_DATA
	dex
	bne @1

clearVRAM:
	txa
	ldy #$20
	sty PPU_ADDR
	sta PPU_ADDR
	ldy #$10
@1:
	sta PPU_DATA
	inx
	bne @1
	dey
	bne @1
	
	jsr upload_chars

	lda #4
	jsr _pal_bright
	jsr _pal_clear
	jsr _oam_clear

    ;jsr	zerobss
	jsr	copydata

    lda #<(__STACK_START__+__STACKSIZE__) ;changed
    sta	sp
    lda	#>(__STACK_START__+__STACKSIZE__)
    sta	sp+1            ; Set argument stack ptr

;	jsr	initlib
; removed. this called the CONDES function

	lda #%10000000
	sta <PPU_CTRL_VAR
	sta PPU_CTRL		;enable NMI
	lda #%00000110
	sta <PPU_MASK_VAR

waitSync3:
	lda <FRAME_CNT1
@1:
	cmp <FRAME_CNT1
	beq @1

detectNTSC:
	ldx #52				;blargg's code
	ldy #24
@1:
	dex
	bne @1
	dey
	bne @1

	lda PPU_STATUS
	and #$80
	sta <NTSC_MODE

	jsr _ppu_off

	lda #0
	ldx #0
	jsr _set_vram_update

	lda #$fd
	sta <RAND_SEED
	sta <RAND_SEED+1

	lda #0
	sta PPU_SCROLL
	sta PPU_SCROLL
	
	
	
	lda #SOUND_BANK ;PRG bank where all the music stuff is there
					;SOUND_BANK is defined above
	jsr _set_prg_bank
	
	ldx #<music_data
	ldy #>music_data
	lda <NTSC_MODE
	jsr FamiToneInit

	ldx #<sounds_data
	ldy #>sounds_data
	jsr FamiToneSfxInit
	
	lda #$00 ;PRG bank #0 at $8000, back to basic
	jsr _set_prg_bank
	
	;for split screens with different CHR bank at top... disable it
	jsr _unset_nmi_chr_tile_bank

	jmp _main			;no parameters
	
	

	;.include "MMC1/mmc1_macros.asm"
	.include "A53/bank_helpers.asm"
	.include "LIB/neslib.s"
	.include "LIB/nesdoug.s"
	



	
; I put all the music stuff on bank 6
; all the music functions swap in bank 6
	
;.segment "CODE" ;does nothing-segments set in famitone5.s again
	.include "MUSIC/famitone5.s"

.segment "BANK2" ; im suprised this works, having music/sound data in different bank from famitone, but it does.
music_data:
 	.include "MUSIC/songs.s"

sounds_data:
 	.include "MUSIC/sounds.s"


	
; NOTE: This won't work with music/sound other than the MMC1 demo. I think this is become the 
;		memory layout needs to be really specific: https://nesdoug.com/2018/09/05/17-dmc-sound/
;
;		"This is the tricky part. DMC samples must go between $c000 and $ffc0. preferably as far 
;		to the end as possible. Subtract the byte size of the samples from ffc0 and round down 
;		to the next xx40. In the cfg file, define the SAMPLE segment to start there."
;
;.segment "SAMPLES"
;	.incbin "MUSIC/BassDrum.dmc"



.segment "VECTORS"

    .word nmi	;$fffa vblank nmi
    .word start	;$fffc reset
   	.word irq	;$fffe irq / brk


;.segment "CHARS"

;	.incbin "chrrom_bank0.chr"
;	.incbin "chrrom_bank1.chr"
;	.incbin "chrrom_bank2.chr"
; the CHARS segment is much bigger, and I could have 
; incbin-ed many more chr files
	
