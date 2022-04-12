;FamiTone5.1 unofficial
;fork of Famitone2 v1.15 by Shiru 04'17
;for ca65
;Revision 6-22-2019, Doug Fraker, to be used with text2vol5
;added volume column and support for all NES notes
;added support for 1xx,2xx,3xx,4xx,Qxx,Rxx effects
;added support for duty envelopes and sound fx > 256 bytes
;Pal support has been removed, don't use it


.export FamiToneInit, FamiToneMusicPlay, FamiToneSfxPlay, FamiToneSfxInit, FamiToneUpdate




;.segment "ZEROPAGE"

;FT_TEMP:	.res 3


.segment "BSS"

volume_Sq1:	.res 1
volume_Sq2:	.res 1	
volume_Nz:	.res 1	
vol_change:	.res 1	
multiple1:	.res 1	
multiple2:	.res 1	

vibrato_depth1:	.res 1 ;zero = off
vibrato_depth2:	.res 1
vibrato_depth3:	.res 1
vibrato_count:	.res 1 ;goes up every frame, shared by all

slide_mode1: .res 1 ;0 = off, 1 = up, 2 = down, 3 = portamento, 4 q/r
slide_mode2: .res 1
slide_mode3: .res 1
slide_speed1:	.res 1 ;how much each frame, zero = off
slide_speed2:	.res 1
slide_speed3:	.res 1
slide_count_low1:	.res 1 ;how much to add / subtract from low byte - cumulative
slide_count_low2:	.res 1
slide_count_low3:	.res 1
slide_count_high1:	.res 1 ; how much to add / subtract from high byte
slide_count_high2:	.res 1
slide_count_high3:	.res 1

temp_low:		.res 1 ;low byte of frequency
temp_high:		.res 1
channel:		.res 1 ;25 new variables

temp_duty:		.res 1
qr_flag:		.res 1
qr_offset:		.res 1
qr_rate:		.res 1
zero_flag1:		.res 1 ;for remembering if 100,200,300
zero_flag2:		.res 1
zero_flag3:		.res 1

MAX_NOTE = 88

.segment "CODE"


;settings, uncomment or put them into your main program; the latter makes possible updates easier

;FT_BASE_ADR		= $0300	;page in the RAM used for FT2 variables, should be $xx00

;FT_DPCM_OFF		= $fc00	;$c000..$ffc0, 64-byte steps
;FT_SFX_STREAMS	= 1		;number of sound effects played at once, 1..4

;FT_DPCM_ENABLE	= 1		;undefine to exclude all DMC code
;FT_SFX_ENABLE	= 1		;undefine to exclude all sound effects code
;FT_THREAD		= 1		;undefine if you are calling sound effects from the same thread as the sound update call

;FT_PAL_SUPPORT	= 0			;undefine to exclude PAL support
;FT_NTSC_SUPPORT	= 1			;undefine to exclude NTSC support



;internal defines

;   removed FT_PITCH_FIX

FT_DPCM_PTR		= (FT_DPCM_OFF&$3fff)>>6


;zero page variables

FT_TEMP_PTR			= FT_TEMP		;word
FT_TEMP_PTR_L		= FT_TEMP_PTR+0
FT_TEMP_PTR_H		= FT_TEMP_PTR+1
FT_TEMP_VAR1		= FT_TEMP+2
FT_TEMP_SIZE        = 3

;envelope structure offsets, 5 bytes per envelope, grouped by variable type

FT_ENVELOPES_ALL	= 4+4+3+3	;4 for the pulse and 3 for the triangle and noise channel
								;adding duty envelope to pulse and noise
FT_ENV_STRUCT_SIZE	= 5

FT_ENV_VALUE		= FT_BASE_ADR+0*FT_ENVELOPES_ALL
FT_ENV_REPEAT		= FT_BASE_ADR+1*FT_ENVELOPES_ALL
FT_ENV_ADR_L		= FT_BASE_ADR+2*FT_ENVELOPES_ALL
FT_ENV_ADR_H		= FT_BASE_ADR+3*FT_ENVELOPES_ALL
FT_ENV_PTR			= FT_BASE_ADR+4*FT_ENVELOPES_ALL


;channel structure offsets, 7 bytes per channel

FT_CHANNELS_ALL		= 5
FT_CHN_STRUCT_SIZE	= 9

FT_CHN_PTR_L		= FT_BASE_ADR+0*FT_CHANNELS_ALL
FT_CHN_PTR_H		= FT_BASE_ADR+1*FT_CHANNELS_ALL
FT_CHN_NOTE			= FT_BASE_ADR+2*FT_CHANNELS_ALL
FT_CHN_INSTRUMENT	= FT_BASE_ADR+3*FT_CHANNELS_ALL
FT_CHN_REPEAT		= FT_BASE_ADR+4*FT_CHANNELS_ALL
FT_CHN_RETURN_L		= FT_BASE_ADR+5*FT_CHANNELS_ALL
FT_CHN_RETURN_H		= FT_BASE_ADR+6*FT_CHANNELS_ALL
FT_CHN_REF_LEN		= FT_BASE_ADR+7*FT_CHANNELS_ALL
FT_CHN_DUTY			= FT_BASE_ADR+8*FT_CHANNELS_ALL


;variables and aliases

FT_ENVELOPES	= FT_BASE_ADR
FT_CH1_ENVS		= FT_ENVELOPES+0
FT_CH2_ENVS		= FT_ENVELOPES+4
FT_CH3_ENVS		= FT_ENVELOPES+8
FT_CH4_ENVS		= FT_ENVELOPES+11

FT_CHANNELS		= FT_ENVELOPES+FT_ENVELOPES_ALL*FT_ENV_STRUCT_SIZE
FT_CH1_VARS		= FT_CHANNELS+0
FT_CH2_VARS		= FT_CHANNELS+1
FT_CH3_VARS		= FT_CHANNELS+2
FT_CH4_VARS		= FT_CHANNELS+3
FT_CH5_VARS		= FT_CHANNELS+4


FT_CH1_NOTE			= FT_CH1_VARS+.lobyte(FT_CHN_NOTE)
FT_CH2_NOTE			= FT_CH2_VARS+.lobyte(FT_CHN_NOTE)
FT_CH3_NOTE			= FT_CH3_VARS+.lobyte(FT_CHN_NOTE)
FT_CH4_NOTE			= FT_CH4_VARS+.lobyte(FT_CHN_NOTE)
FT_CH5_NOTE			= FT_CH5_VARS+.lobyte(FT_CHN_NOTE)

FT_CH1_INSTRUMENT	= FT_CH1_VARS+.lobyte(FT_CHN_INSTRUMENT)
FT_CH2_INSTRUMENT	= FT_CH2_VARS+.lobyte(FT_CHN_INSTRUMENT)
FT_CH3_INSTRUMENT	= FT_CH3_VARS+.lobyte(FT_CHN_INSTRUMENT)
FT_CH4_INSTRUMENT	= FT_CH4_VARS+.lobyte(FT_CHN_INSTRUMENT)
FT_CH5_INSTRUMENT	= FT_CH5_VARS+.lobyte(FT_CHN_INSTRUMENT)



FT_CH1_VOLUME		= FT_CH1_ENVS+.lobyte(FT_ENV_VALUE)+0
FT_CH2_VOLUME		= FT_CH2_ENVS+.lobyte(FT_ENV_VALUE)+0
FT_CH3_VOLUME		= FT_CH3_ENVS+.lobyte(FT_ENV_VALUE)+0
FT_CH4_VOLUME		= FT_CH4_ENVS+.lobyte(FT_ENV_VALUE)+0

FT_CH1_NOTE_OFF		= FT_CH1_ENVS+.lobyte(FT_ENV_VALUE)+1
FT_CH2_NOTE_OFF		= FT_CH2_ENVS+.lobyte(FT_ENV_VALUE)+1
FT_CH3_NOTE_OFF		= FT_CH3_ENVS+.lobyte(FT_ENV_VALUE)+1
FT_CH4_NOTE_OFF		= FT_CH4_ENVS+.lobyte(FT_ENV_VALUE)+1

FT_CH1_PITCH_OFF	= FT_CH1_ENVS+.lobyte(FT_ENV_VALUE)+2
FT_CH2_PITCH_OFF	= FT_CH2_ENVS+.lobyte(FT_ENV_VALUE)+2
FT_CH3_PITCH_OFF	= FT_CH3_ENVS+.lobyte(FT_ENV_VALUE)+2

FT_CH1_DUTY			= FT_CH1_ENVS+.lobyte(FT_ENV_VALUE)+3
FT_CH2_DUTY			= FT_CH2_ENVS+.lobyte(FT_ENV_VALUE)+3
FT_CH3_DUTY			= FT_CH3_VARS+.lobyte(FT_CHN_DUTY) ;see FT_PULSE1_PREV below
FT_CH4_DUTY			= FT_CH4_ENVS+.lobyte(FT_ENV_VALUE)+2 ;!!
FT_CH5_DUTY			= FT_CH5_VARS+.lobyte(FT_CHN_DUTY) ;see FT_PULSE2_PREV below


;FT_EN1_DUTY			= FT_CH1_ENVS+.lobyte(FT_ENV_VALUE)+2
;FT_EN2_DUTY			= FT_CH2_ENVS+.lobyte(FT_ENV_VALUE)+2
;FT_EN4_DUTY			= FT_CH4_ENVS+.lobyte(FT_ENV_VALUE)+2


FT_VARS			= FT_CHANNELS+FT_CHANNELS_ALL*FT_CHN_STRUCT_SIZE

FT_PAL_ADJUST	= FT_VARS+0
FT_SONG_LIST_L	= FT_VARS+1
FT_SONG_LIST_H	= FT_VARS+2
FT_INSTRUMENT_L = FT_VARS+3
FT_INSTRUMENT_H = FT_VARS+4
FT_TEMPO_STEP_L	= FT_VARS+5
FT_TEMPO_STEP_H	= FT_VARS+6
FT_TEMPO_ACC_L	= FT_VARS+7
FT_TEMPO_ACC_H	= FT_VARS+8
FT_SONG_SPEED	= FT_CH5_INSTRUMENT
FT_PULSE1_PREV	= FT_CH3_DUTY
FT_PULSE2_PREV	= FT_CH5_DUTY
FT_DPCM_LIST_L	= FT_VARS+9
FT_DPCM_LIST_H	= FT_VARS+10
FT_DPCM_EFFECT  = FT_VARS+11
FT_OUT_BUF		= FT_VARS+12	;11 bytes


;sound effect stream variables, 2 bytes and 15 bytes per stream
;when sound effects are disabled, this memory is not used

FT_SFX_ADR_L		= FT_VARS+23
FT_SFX_ADR_H		= FT_VARS+24
FT_SFX_BASE_ADR		= FT_VARS+25

FT_SFX_STRUCT_SIZE	= 15
FT_SFX_REPEAT		= FT_SFX_BASE_ADR+0
FT_SFX_PTR_L		= FT_SFX_BASE_ADR+1
FT_SFX_PTR_H		= FT_SFX_BASE_ADR+2
FT_SFX_OFF			= FT_SFX_BASE_ADR+3
FT_SFX_BUF			= FT_SFX_BASE_ADR+4	;11 bytes

FT_BASE_SIZE 		= FT_SFX_BUF+11-FT_BASE_ADR

;aliases for sound effect channels to use in user calls

FT_SFX_CH0			= FT_SFX_STRUCT_SIZE*0
FT_SFX_CH1			= FT_SFX_STRUCT_SIZE*1
FT_SFX_CH2			= FT_SFX_STRUCT_SIZE*2
FT_SFX_CH3			= FT_SFX_STRUCT_SIZE*3


;aliases for the APU registers

APU_PL1_VOL		= $4000
APU_PL1_SWEEP	= $4001
APU_PL1_LO		= $4002
APU_PL1_HI		= $4003
APU_PL2_VOL		= $4004
APU_PL2_SWEEP	= $4005
APU_PL2_LO		= $4006
APU_PL2_HI		= $4007
APU_TRI_LINEAR	= $4008
APU_TRI_LO		= $400a
APU_TRI_HI		= $400b
APU_NOISE_VOL	= $400c
APU_NOISE_LO	= $400e
APU_NOISE_HI	= $400f
APU_DMC_FREQ	= $4010
APU_DMC_RAW		= $4011
APU_DMC_START	= $4012
APU_DMC_LEN		= $4013
APU_SND_CHN		= $4015


;aliases for the APU registers in the output buffer

	.if(!FT_SFX_ENABLE)				;if sound effects are disabled, write to the APU directly
FT_MR_PULSE1_V		= APU_PL1_VOL
FT_MR_PULSE1_L		= APU_PL1_LO
FT_MR_PULSE1_H		= APU_PL1_HI
FT_MR_PULSE2_V		= APU_PL2_VOL
FT_MR_PULSE2_L		= APU_PL2_LO
FT_MR_PULSE2_H		= APU_PL2_HI
FT_MR_TRI_V			= APU_TRI_LINEAR
FT_MR_TRI_L			= APU_TRI_LO
FT_MR_TRI_H			= APU_TRI_HI
FT_MR_NOISE_V		= APU_NOISE_VOL
FT_MR_NOISE_F		= APU_NOISE_LO
	.else								;otherwise write to the output buffer
FT_MR_PULSE1_V		= FT_OUT_BUF
FT_MR_PULSE1_L		= FT_OUT_BUF+1
FT_MR_PULSE1_H		= FT_OUT_BUF+2
FT_MR_PULSE2_V		= FT_OUT_BUF+3
FT_MR_PULSE2_L		= FT_OUT_BUF+4
FT_MR_PULSE2_H		= FT_OUT_BUF+5
FT_MR_TRI_V			= FT_OUT_BUF+6
FT_MR_TRI_L			= FT_OUT_BUF+7
FT_MR_TRI_H			= FT_OUT_BUF+8
FT_MR_NOISE_V		= FT_OUT_BUF+9
FT_MR_NOISE_F		= FT_OUT_BUF+10
	.endif



;------------------------------------------------------------------------------
; reset APU, initialize FamiTone
; in: A   0 for PAL, not 0 for NTSC
;     X,Y pointer to music data
;------------------------------------------------------------------------------
   
FamiToneInit:

	stx FT_SONG_LIST_L		;store music data pointer for further use
	sty FT_SONG_LIST_H
	stx <FT_TEMP_PTR_L
	sty <FT_TEMP_PTR_H

;   removed pal support

	jsr FamiToneMusicStop	;initialize channels and envelopes

	ldy #1
	lda (FT_TEMP_PTR),y		;get instrument list address
	sta FT_INSTRUMENT_L
	iny
	lda (FT_TEMP_PTR),y
	sta FT_INSTRUMENT_H
	iny
	lda (FT_TEMP_PTR),y		;get sample list address
	sta FT_DPCM_LIST_L
	iny
	lda (FT_TEMP_PTR),y
	sta FT_DPCM_LIST_H

	lda #$ff				;previous pulse period MSB, to not write it when not changed
	sta FT_PULSE1_PREV
	sta FT_PULSE2_PREV

	lda #$0f				;enable channels, stop DMC
	sta APU_SND_CHN
	lda #$80				;disable triangle length counter
	sta APU_TRI_LINEAR
	lda #$00				;load noise length
	sta APU_NOISE_HI
	lda #0				;change to 63 for medium, change to 127 for quiet
						;also, change to 63 if using DPCM effects
	sta APU_DMC_RAW		; , for louder Triangle Channel
						

	lda #$30				;volumes to 0
	sta APU_PL1_VOL
	sta APU_PL2_VOL
	sta APU_NOISE_VOL
	lda #$08				;no sweep
	sta APU_PL1_SWEEP
	sta APU_PL2_SWEEP

	;jmp FamiToneMusicStop


;------------------------------------------------------------------------------
; stop music that is currently playing, if any
; in: none
;------------------------------------------------------------------------------

FamiToneMusicStop:

	lda #0
	sta FT_SONG_SPEED		;stop music, reset pause flag
	sta FT_DPCM_EFFECT		;no DPCM effect playing

	ldx #.lobyte(FT_CHANNELS)	;initialize channel structures

@set_channels:

	lda #0
	sta FT_CHN_REPEAT,x
	sta FT_CHN_INSTRUMENT,x
	sta FT_CHN_NOTE,x
	sta FT_CHN_REF_LEN,x
	lda #$30
	sta FT_CHN_DUTY,x

	inx						;next channel
	cpx #.lobyte(FT_CHANNELS)+FT_CHANNELS_ALL
	bne @set_channels

	ldx #.lobyte(FT_ENVELOPES)	;initialize all envelopes to the dummy envelope

@set_envelopes:

	lda #.lobyte (_FT2DummyEnvelope)
	sta FT_ENV_ADR_L,x
	lda #.hibyte(_FT2DummyEnvelope)
	sta FT_ENV_ADR_H,x
	lda #0
	sta FT_ENV_REPEAT,x
	sta FT_ENV_VALUE,x
	inx
	cpx #.lobyte(FT_ENVELOPES)+FT_ENVELOPES_ALL

	bne @set_envelopes

	jmp FamiToneSampleStop


;------------------------------------------------------------------------------
; play music
; in: A number of subsong
;------------------------------------------------------------------------------

FamiToneMusicPlay:

	ldx #$0f		; full volume to start
	stx volume_Sq1
	stx volume_Sq2
	stx volume_Nz
	
	ldx #0
	stx vibrato_depth1	; turn off by default
	stx vibrato_depth2
	stx vibrato_depth3
	stx slide_speed1
	stx slide_speed2
	stx slide_speed3
	stx slide_mode1
	stx slide_mode2
	stx slide_mode3
	;note, slide_count_low/high are reset on each new note

	ldx FT_SONG_LIST_L
	stx <FT_TEMP_PTR_L
	ldx FT_SONG_LIST_H
	stx <FT_TEMP_PTR_H

	ldy #0
	cmp (FT_TEMP_PTR),y		;check if there is such sub song
	bcs @skip

	asl a					;multiply song number by 14
	sta <FT_TEMP_PTR_L		;use pointer LSB as temp variable
	asl a
	tax
	asl a
	adc <FT_TEMP_PTR_L
	stx <FT_TEMP_PTR_L
	adc <FT_TEMP_PTR_L
	adc #5					;add offset
	tay

	lda FT_SONG_LIST_L		;restore pointer LSB
	sta <FT_TEMP_PTR_L

	jsr FamiToneMusicStop	;stop music, initialize channels and envelopes

	ldx #.lobyte(FT_CHANNELS)	;initialize channel structures

@set_channels:

	lda (FT_TEMP_PTR),y		;read channel pointers
	sta FT_CHN_PTR_L,x
	iny
	lda (FT_TEMP_PTR),y
	sta FT_CHN_PTR_H,x
	iny

	lda #0
	sta FT_CHN_REPEAT,x
	sta FT_CHN_INSTRUMENT,x
	sta FT_CHN_NOTE,x
	sta FT_CHN_REF_LEN,x
	lda #$30
	sta FT_CHN_DUTY,x

	inx						;next channel
	cpx #.lobyte(FT_CHANNELS)+FT_CHANNELS_ALL
	bne @set_channels


;  	lda FT_PAL_ADJUST		;read tempo for PAL or NTSC
;	beq @pal ;   removed pal support
	iny
	iny
;@pal:

	lda (FT_TEMP_PTR),y		;read the tempo step
	sta FT_TEMPO_STEP_L
	iny
	lda (FT_TEMP_PTR),y
	sta FT_TEMPO_STEP_H


	lda #0					;reset tempo accumulator
	sta FT_TEMPO_ACC_L
	lda #6					;default speed
	sta FT_TEMPO_ACC_H
	sta FT_SONG_SPEED		;apply default speed, this also enables music

@skip:
	rts


;------------------------------------------------------------------------------
; pause and unpause current music
; in: A 0 or not 0 to play or pause
;------------------------------------------------------------------------------

FamiToneMusicPause:

	tax					;set SZ flags for A
	beq @unpause
	
@pause:

	jsr FamiToneSampleStop
	
	lda #0				;mute sound
	sta FT_CH1_VOLUME
	sta FT_CH2_VOLUME
	sta FT_CH3_VOLUME
	sta FT_CH4_VOLUME
	lda FT_SONG_SPEED	;set pause flag
	ora #$80
	bne @done
@unpause:
	lda FT_SONG_SPEED	;reset pause flag
	and #$7f
@done:
	sta FT_SONG_SPEED

	rts


;------------------------------------------------------------------------------
; update FamiTone state, should be called every NMI
; in: none
;------------------------------------------------------------------------------

FamiToneUpdate:

	.if(FT_THREAD)
	lda FT_TEMP_PTR_L
	pha
	lda FT_TEMP_PTR_H
	pha
	.endif

	lda FT_SONG_SPEED		;speed 0 means that no music is playing currently
	bmi @pause				;bit 7 set is the pause flag
	bne @update
@pause:
	jmp update_sound

@update:

	clc						;update frame counter that considers speed, tempo, and PAL/NTSC
	lda FT_TEMPO_ACC_L
	adc FT_TEMPO_STEP_L
	sta FT_TEMPO_ACC_L
	lda FT_TEMPO_ACC_H
	adc FT_TEMPO_STEP_H
	cmp FT_SONG_SPEED
	bcs @update_row			;overflow, row update is needed
	sta FT_TEMPO_ACC_H		;no row update, skip to the envelopes update
	jmp update_envelopes

@update_row:

	sec
	sbc FT_SONG_SPEED
	sta FT_TEMPO_ACC_H


	ldx #.lobyte(FT_CH1_VARS)	;process channel 1
		lda #$ff
		sta vol_change
		ldy #0
		sty channel
	jsr _FT2ChannelUpdate
		lda vol_change
		bmi :+
		sta volume_Sq1
		:
	bcc @no_new_note1
;new note
	lda slide_mode1
	cmp #3 ;portamento
	bcs :+
	lda zero_flag1
	bne :+
	ldx FT_CH1_NOTE
	lda _FT2NoteTableLSB,x
	sta slide_count_low1
	lda _FT2NoteTableMSB,x
	sta slide_count_high1
:	
	
	ldx #.lobyte(FT_CH1_ENVS)
	lda FT_CH1_INSTRUMENT
	jsr _FT2SetInstrument
;	sta FT_CH1_DUTY
@no_new_note1:

	ldx #.lobyte(FT_CH2_VARS)	;process channel 2
		lda #$ff
		sta vol_change
		ldy #1
		sty channel
	jsr _FT2ChannelUpdate
		lda vol_change	
		bmi :+
		sta volume_Sq2
		:
	bcc @no_new_note2
;new note
	lda slide_mode2
	cmp #3 ;portamento
	bcs :+
	lda zero_flag2
	bne :+
	ldx FT_CH2_NOTE
	lda _FT2NoteTableLSB,x
	sta slide_count_low1+1
	lda _FT2NoteTableMSB,x
	sta slide_count_high1+1
:	
	
	ldx #.lobyte(FT_CH2_ENVS)
	lda FT_CH2_INSTRUMENT
	jsr _FT2SetInstrument
;	sta FT_CH2_DUTY
@no_new_note2:

	ldx #.lobyte(FT_CH3_VARS)	;process channel 3
		ldy #2
		sty channel
	jsr _FT2ChannelUpdate
	bcc @no_new_note3
;new note
	lda slide_mode3
	cmp #3 ;portamento
	bcs :+
	lda zero_flag3
	bne :+
	ldx FT_CH3_NOTE
	lda _FT2NoteTableLSB,x
	sta slide_count_low1+2
	lda _FT2NoteTableMSB,x
	sta slide_count_high1+2	
:		
	
	ldx #.lobyte(FT_CH3_ENVS)
	lda FT_CH3_INSTRUMENT
	jsr _FT2SetInstrument
@no_new_note3:

	ldx #.lobyte(FT_CH4_VARS)	;process channel 4
		lda #$ff
		sta vol_change 		
		ldy #3
		sty channel
	jsr _FT2ChannelUpdate
		lda vol_change
		bmi :+
		sta volume_Nz
		:
	bcc @no_new_note4
	ldx #.lobyte(FT_CH4_ENVS)
	lda FT_CH4_INSTRUMENT
	jsr _FT2SetInstrument
;	sta FT_CH4_DUTY
@no_new_note4:

	.if(FT_DPCM_ENABLE)

	ldx #.lobyte(FT_CH5_VARS)	;process channel 5
		ldy #4
		sty channel	;  
	jsr _FT2ChannelUpdate
	bcc @no_new_note5
	lda FT_CH5_NOTE
	bne @play_sample
	jsr FamiToneSampleStop
	bne @no_new_note5		;A is non-zero after FamiToneSampleStop
@play_sample:
	jsr FamiToneSamplePlayM
@no_new_note5:

	.endif


update_envelopes:

	ldx #.lobyte(FT_ENVELOPES)	;process 14 envelopes

@env_process:

	lda FT_ENV_REPEAT,x		;check envelope repeat counter
	beq @env_read			;if it is zero, process envelope
	dec FT_ENV_REPEAT,x		;otherwise decrement the counter
	bne @env_next

@env_read:

	lda FT_ENV_ADR_L,x		;load envelope data address into temp
	sta <FT_TEMP_PTR_L
	lda FT_ENV_ADR_H,x
	sta <FT_TEMP_PTR_H
	ldy FT_ENV_PTR,x		;load envelope pointer

@env_read_value:

	lda (FT_TEMP_PTR),y		;read a byte of the envelope data
	bpl @env_special		;values below 128 used as a special code, loop or repeat
	clc						;values above 128 are output value+192 (output values are signed -63..64)
	adc #256-192
	sta FT_ENV_VALUE,x		;store the output value
	iny						;advance the pointer
	bne @env_next_store_ptr ;bra

@env_special:

	bne @env_set_repeat		;zero is the loop point, non-zero values used for the repeat counter
	iny						;advance the pointer
	lda (FT_TEMP_PTR),y		;read loop position
	tay						;use loop position
	jmp @env_read_value		;read next byte of the envelope

@env_set_repeat:

	iny
	sta FT_ENV_REPEAT,x		;store the repeat counter value

@env_next_store_ptr:

	tya						;store the envelope pointer
	sta FT_ENV_PTR,x

@env_next:

	inx						 ;next envelope

	cpx #.lobyte(FT_ENVELOPES)+FT_ENVELOPES_ALL
	bne @env_process


update_sound:
	inc vibrato_count
	lda vibrato_count
	cmp #11 ; vibrato speed 6
	bcc :+
	lda #0
	sta vibrato_count
:

	;convert envelope and channel output data into APU register values in the output buffer

	lda FT_CH1_DUTY
	and #3 ;sanitize
	tax
	lda duty_table, x
	sta temp_duty
	
	lda FT_CH1_NOTE
	beq ch1cut
	clc
	adc FT_CH1_NOTE_OFF
		;removed pal pitch fix **
	tax
	lda FT_CH1_PITCH_OFF
	tay
	adc _FT2NoteTableLSB,x
	sta temp_low	;   FT_MR_PULSE1_L
	tya						;sign extension for the pitch offset
	ora #$7f
	bmi @ch1sign
	lda #0
@ch1sign:
	adc _FT2NoteTableMSB,x

	.if(!FT_SFX_ENABLE)
	cmp FT_PULSE1_PREV
	beq @ch1prev
	sta FT_PULSE1_PREV
	.endif

	sta temp_high	; FT_MR_PULSE1_H
@ch1prev:
		ldy #0 ;for sq 1	;  
		jsr Apply_Effects	;  
		sta FT_MR_PULSE1_L	;   a = temp_low
		stx FT_MR_PULSE1_H	;   x = temp_high
	lda FT_CH1_VOLUME
		;  
		beq ch1cut ;if zero, skip multiply
		ldx volume_Sq1
		bne :+
		lda #0 ;if volume column = zero, skip multiply
		beq ch1cut
		:
		jsr Multiply ;  
	
ch1cut:
	ora temp_duty ;FT_CH1_DUTY
	sta FT_MR_PULSE1_V


	
	
	
	lda FT_CH2_DUTY
	and #3 ;sanitize
	tax
	lda duty_table, x
	sta temp_duty
	
	lda FT_CH2_NOTE
	beq ch2cut
	clc
	adc FT_CH2_NOTE_OFF
		;removed pal pitch fix **
	tax
	lda FT_CH2_PITCH_OFF
	tay
	adc _FT2NoteTableLSB,x
	sta temp_low 	;   FT_MR_PULSE2_L
	tya
	ora #$7f
	bmi @ch2sign
	lda #0
@ch2sign:
	adc _FT2NoteTableMSB,x

	.if(!FT_SFX_ENABLE)
	cmp FT_PULSE2_PREV
	beq @ch2prev
	sta FT_PULSE2_PREV
	.endif

	sta temp_high 	;   FT_MR_PULSE2_H
@ch2prev:
		ldy #1 ;for sq 2	;  
		jsr Apply_Effects	;  
		sta FT_MR_PULSE2_L	;   a = temp_low
		stx FT_MR_PULSE2_H	;   x = temp_high
	lda FT_CH2_VOLUME
		;  
		beq ch2cut ;if zero, skip multiply
		ldx volume_Sq2
		bne :+
		lda #0 ;if volume column = zero, skip multiply
		beq ch2cut
		:
		jsr Multiply
	
ch2cut:
	ora temp_duty ;FT_CH2_DUTY
	sta FT_MR_PULSE2_V


	
	
	lda FT_CH3_NOTE
	beq ch3cut
	clc
	adc FT_CH3_NOTE_OFF
		;removed pal pitch fix **
	tax
	lda FT_CH3_PITCH_OFF
	tay
	adc _FT2NoteTableLSB,x
	sta temp_low ;   FT_MR_TRI_L
	tya
	ora #$7f
	bmi @ch3sign
	lda #0
@ch3sign:
	adc _FT2NoteTableMSB,x
	sta temp_high ;   FT_MR_TRI_H
	
		ldy #2 ;for tri	;  
		jsr Apply_Effects	;  
		sta FT_MR_TRI_L		;   a = temp_low
		stx FT_MR_TRI_H		;   x = temp_high
	lda FT_CH3_VOLUME
		;   there should be no volume column for Triangle channel
	
ch3cut:
	ora #$80
	sta FT_MR_TRI_V


	
	lda FT_CH4_DUTY
	and #3 ;sanitize
	tax
	lda duty_table_nz, x
	sta temp_duty
	
	lda FT_CH4_NOTE
	beq ch4cut
	clc
	adc FT_CH4_NOTE_OFF
	and #$0f
	eor #$0f
	ora temp_duty
	sta FT_MR_NOISE_F
	lda FT_CH4_VOLUME 
		beq ch4cut ;if zero, skip multiply
		ldx volume_Nz
		bne :+
		lda #0 ;if volume column = zero, skip multiply
		beq ch4cut
		:
		jsr Multiply
	
ch4cut:
	ora #$f0
	sta FT_MR_NOISE_V


	.if(FT_SFX_ENABLE)

	;process all sound effect streams

	.if FT_SFX_STREAMS>0
	ldx #FT_SFX_CH0
	jsr _FT2SfxUpdate
	.endif
	.if FT_SFX_STREAMS>1
	ldx #FT_SFX_CH1
	jsr _FT2SfxUpdate
	.endif
	.if FT_SFX_STREAMS>2
	ldx #FT_SFX_CH2
	jsr _FT2SfxUpdate
	.endif
	.if FT_SFX_STREAMS>3
	ldx #FT_SFX_CH3
	jsr _FT2SfxUpdate
	.endif


	;send data from the output buffer to the APU

	lda FT_OUT_BUF		;pulse 1 volume
	sta APU_PL1_VOL
	lda FT_OUT_BUF+1	;pulse 1 period LSB
	sta APU_PL1_LO
	lda FT_OUT_BUF+2	;pulse 1 period MSB, only applied when changed
	cmp FT_PULSE1_PREV
	beq @no_pulse1_upd
	sta FT_PULSE1_PREV
	sta APU_PL1_HI
@no_pulse1_upd:

	lda FT_OUT_BUF+3	;pulse 2 volume
	sta APU_PL2_VOL
	lda FT_OUT_BUF+4	;pulse 2 period LSB
	sta APU_PL2_LO
	lda FT_OUT_BUF+5	;pulse 2 period MSB, only applied when changed
	cmp FT_PULSE2_PREV
	beq @no_pulse2_upd
	sta FT_PULSE2_PREV
	sta APU_PL2_HI
@no_pulse2_upd:

	lda FT_OUT_BUF+6	;triangle volume (plays or not)
	sta APU_TRI_LINEAR
	lda FT_OUT_BUF+7	;triangle period LSB
	sta APU_TRI_LO
	lda FT_OUT_BUF+8	;triangle period MSB
	sta APU_TRI_HI

	lda FT_OUT_BUF+9	;noise volume
	sta APU_NOISE_VOL
	lda FT_OUT_BUF+10	;noise period
	sta APU_NOISE_LO

	.endif

	.if(FT_THREAD)
	pla
	sta FT_TEMP_PTR_H
	pla
	sta FT_TEMP_PTR_L
	.endif

	rts
	
	
	


duty_table:
.byte $30,$70,$b0,$f0
duty_table_nz: ;noise
.byte $00,$80,$00,$80
	
Apply_Effects:
;y = channel
;temp_low, temp_high = note frequency in
;return, a = low, x = high...out frequency

	lda FT_CH1_NOTE, y ; if note = 0, silence, no effects
	bne :+
	sta slide_count_low1, y ;zero these for later
	sta slide_count_high1, y
	tax ; now a and x are zero => output note frequency = silence
	rts
:
	lda zero_flag1, y ;keep the current slide value until a new note
	beq :+
	lda slide_count_high1, y
	sta temp_high
	lda slide_count_low1, y
	sta temp_low
	jmp Vib_Effects
:



	lda slide_mode1, y
	bne :+
	jmp Vib_Effects
;	lda #0 ;already zero
;	sta slide_speed1, y
;	beq Apply_Slide_Up  ; this is a waste of CPU, but needed
						; in case you freeze a slide with 100,200,300
						; without a new note
:
	cmp #1
	beq Apply_Slide_Up
	cmp #2
	beq Apply_Slide_Down
	;3 = port, same code for 4 Qxx/Rxx
	jmp Apply_Portamento
	
Apply_Slide_Down:
;add to the base note

	ldx slide_count_high1, y
	lda slide_count_low1, y
	clc
	adc slide_speed1, y	;downward in frequency is adding to the low frequency
	bcc Slide_Down2
	inx ;high byte
Slide_Down2:
	sta slide_count_low1, y
	txa
	cmp #8
	bcc Slide_Down3
	lda #$ff
	sta slide_count_low1, y
	lda #7
Slide_Down3:	
	sta slide_count_high1, y ;stx address, y doesn't exist
	sta temp_high
	lda slide_count_low1, y
	sta temp_low
	jmp Vib_Effects
	
	
	
	
	
Apply_Slide_Up:
	ldx slide_count_high1, y
	lda slide_count_low1, y
	sec
	sbc slide_speed1, y	;downward in frequency is adding to the low frequency
	bcs Slide_Up2
	dex ;high byte
	bmi Slide_Too_Far
Slide_Up2:
	sta slide_count_low1, y
	sta temp_low
	txa	
	sta slide_count_high1, y ;stx address, y doesn't exist
	sta temp_high
	jmp Vib_Effects

Slide_Too_Far:	
	lda #0
	sta FT_CH1_NOTE, y ;too far, end note
	sta slide_count_low1, y ;zero these for later
	sta slide_count_high1, y
	tax ; now a and x are zero => output note frequency = silence
	rts ; and exit
	


Apply_Portamento:
;if slide is at 0,0, something is wrong (maybe first note of song)
;and make sure it's something real
	lda slide_count_low1, y
	ora slide_count_high1, y
	bne :+
	ldx FT_CH1_NOTE, y ;note
	lda _FT2NoteTableLSB,x
	sta slide_count_low1, y
	lda _FT2NoteTableMSB,x
	sta slide_count_high1, y
:

	jsr Use_Note
	jsr Compare_Sub
	beq @go_up
	dex
	beq @go_down
	jmp Vib_Effects ;exactly equal, skip
	
@go_up:
	ldx slide_count_high1, y
	lda slide_count_low1, y
	sec
	sbc slide_speed1, y	;downward in frequency is adding to the low frequency
	bcs @go_up2
	dex ;high byte
	bmi @too_far
@go_up2:
	sta slide_count_low1, y
	txa	
	sta slide_count_high1, y ;stx address, y doesn't exist
	jsr Compare_Sub
	cpx #1
	beq @too_far
	
@still_ok:	
	lda slide_count_low1, y
	sta temp_low
	lda slide_count_high1, y
	sta temp_high
	jmp Vib_Effects
	
@too_far:
	jsr Use_Note
	lda temp_low
	sta slide_count_low1, y
	lda temp_high
	sta slide_count_high1, y
	
	lda slide_mode1, y
	cmp #4 ;Qxx/Rxx
	bne @too_far2
	lda #0	;end the Qxx/Rxx, the destination has been reached
	sta slide_mode1, y
@too_far2:	
	jmp Vib_Effects

@go_down:
	ldx slide_count_high1, y
	lda slide_count_low1, y
	clc
	adc slide_speed1, y	;downward in frequency is adding to the low frequency
	bcc @go_down2
	inx ;high byte
@go_down2:
	sta slide_count_low1, y
	txa	
	sta slide_count_high1, y ;stx address, y doesn't exist
	jsr Compare_Sub
	beq @too_far	
	bne @still_ok


;returns 0 = less than, 1 = more than, 2 = equal
Compare_Sub:
	lda slide_count_high1, y
	tax ;save for later
	cmp temp_high
	bcc @go_down
	bne @go_up
;equal, check low byte
	lda slide_count_low1, y
	cmp temp_low
	bcc @go_down
	bne @go_up
	ldx #2 ;equal
	rts
	
@go_up: ;in freq
	ldx #0
	rts

@go_down: ;in freq
	ldx #1
	rts
	
	
Use_Note:
	lda FT_CH1_NOTE, y
	tax
	lda _FT2NoteTableLSB,x	;destination frequency
	sta temp_low
	lda _FT2NoteTableMSB,x
	sta temp_high
	rts
	
	
Vib_Effects:	
	ldx vibrato_depth1, y
	beq Vib_Skip ; if zero, off
	lda Vib_Offset, x
	clc
	adc vibrato_count ; this increments every frame
	tax
	lda Vib_Table, x
	bmi Vib_Neg
Vib_Pos: ; a = offset amount
	clc
	adc temp_low
	bcc Vib_Done
	lda #$ff		;if overflow, just use max low byte
	bne Vib_Done

Vib_Neg:
	clc
	adc temp_low
	bcs Vib_Done
	lda #$00		;if underflow, just use min low byte

Vib_Done:
	sta temp_low
Vib_Skip:
	lda temp_low	; pass the final frequency back to the music routine
	ldx temp_high
	rts
	
	
Vib_Offset: ;zero skipped, here for filler
;speed 6
.byte 0,0,11,22,33,44,55,66,77,88,99,110


Vib_Table:	; vibrato

;speed 6
.byte 0,1,1,1,1,  0,0,256-1,256-1,256-1,  256-1 ;1
.byte 0,1,2,2,1,  0,0,256-1,256-2,256-2,  256-1 ;2
.byte 0,2,3,3,2,  1,256-1,256-3,256-4,256-4,  256-2 ;3
.byte 0,3,5,6,5,  2,256-2,256-5,256-6,256-5,  256-3 ;4
.byte 0,3,6,6,5,  2,256-2,256-5,256-6,256-6,  256-3 ;5
.byte 0,5,8,9,7,  3,256-3,256-7,256-9,256-8,  256-5 ;6
.byte 0,6,10,11,8,  3,256-3,256-8,256-11,256-10,  256-6 ;7
.byte 0,7,12,13,10,  4,256-4,256-10,256-13,256-12,  256-7 ;8
.byte 0,9,15,16,12,  4,256-4,256-12,256-16,256-15,  256-9 ;9
.byte 0,10,17,19,14,  5,256-5,256-14,256-19,256-17,  256-10 ;A












;internal routine, sets up envelopes of a channel according to current instrument
;in X envelope group offset, A instrument number
;lots changed here z50
_FT2SetInstrument: 
	asl a					;instrument number is pre multiplied by 4
	tay
	lda FT_INSTRUMENT_H
	adc #0					;use carry to extend range for 64 instruments
	sta <FT_TEMP_PTR_H
	lda FT_INSTRUMENT_L
	sta <FT_TEMP_PTR_L
							;vol envelope
	lda (FT_TEMP_PTR),y		;instrument pointer LSB
	sta FT_ENV_ADR_L,x
	iny
	lda (FT_TEMP_PTR),y		;instrument pointer MSB
	sta FT_ENV_ADR_H,x
	iny
	inx
							;arp envelope
	lda (FT_TEMP_PTR),y		;instrument pointer LSB
	sta FT_ENV_ADR_L,x
	iny
	lda (FT_TEMP_PTR),y		;instrument pointer MSB
	sta FT_ENV_ADR_H,x
	iny
	inx

	lda #0
	sta FT_ENV_REPEAT-2,x	;reset env1 repeat counter
	sta FT_ENV_PTR-2,x		;reset env1 pointer
	sta FT_ENV_REPEAT-1,x	;reset env2 repeat counter
	sta FT_ENV_PTR-1,x		;reset env2 pointer
	sta FT_ENV_REPEAT,x		;reset env3 repeat counter
	sta FT_ENV_PTR,x		;reset env3 pointer
	cpx #.lobyte(FT_CH3_ENVS)	;tri and noise channel only have 3 envelopes
	bcs @skip_4th
	
	sta FT_ENV_REPEAT+1,x	;reset env4 repeat counter
	sta FT_ENV_PTR+1,x		;reset env4 pointer
	
@skip_4th:
	cpx #.lobyte(FT_CH4_ENVS)	;noise channel skip pitch
	bcs @ch4
	
							;pitch envelope
	lda (FT_TEMP_PTR),y		;instrument pointer LSB
	sta FT_ENV_ADR_L,x
	iny
	lda (FT_TEMP_PTR),y		;instrument pointer MSB
	sta FT_ENV_ADR_H,x
	iny
	inx
	
	cpx #.lobyte(FT_CH3_ENVS) ;tri channel skip duty
	bcc @duty ;pulse channels branch
	rts ;tri exit here

@ch4:
	iny ;skip the pitch pointer
	iny
	
@duty:	
							;duty envelope
	lda (FT_TEMP_PTR),y		;instrument pointer LSB
	sta FT_ENV_ADR_L,x
	iny
	lda (FT_TEMP_PTR),y		;instrument pointer MSB
	sta FT_ENV_ADR_H,x
	
@exit:	
	rts




;internal routine, parses channel note data

_FT2ChannelUpdate:
	lda #0
	sta qr_flag
	lda FT_CHN_REPEAT,x		;check repeat counter
	beq @no_repeat
	dec FT_CHN_REPEAT,x		;decrease repeat counter
	clc						;no new note
	rts

@no_repeat:
	lda FT_CHN_PTR_L,x		;load channel pointer into temp
	sta <FT_TEMP_PTR_L
	lda FT_CHN_PTR_H,x
	sta <FT_TEMP_PTR_H
@no_repeat_r:
	ldy #0

read_byte:
	lda (FT_TEMP_PTR),y		;read byte of the channel

	inc <FT_TEMP_PTR_L		;advance pointer
	bne @no_inc_ptr1
	inc <FT_TEMP_PTR_H
@no_inc_ptr1:

	ora #0
	bpl @check_vol
	jmp @special_code
	
@check_vol:
	cmp #$70	;70-7f = 	;   start
	bcc @no_vol_change
	
	and #$0f
	sta vol_change			;   end
	jmp read_byte	;read the next byte
	
@no_vol_change:
	cmp #$5f	;  		begin changes
	bcs @pitch_effects
	jmp @no_pitch_effects
@pitch_effects:
	beq @end_slide ;5f = 1xx,2xx,3xx param 00
	cmp #$6e 
	bne @1
	jmp @qxx_set ;6e = Qxx
@1:
	cmp #$6f
	bne @2
	jmp @rxx_set ;6f = Rxx
@2:	
	cmp #$6b
	beq @slide_up_set ; 6b = 1xx
	cmp #$6c
	beq @slide_down_set ; 6c = 2xx
	cmp #$6d
	beq @portamento_set ;6d = 3xx

	
	
;vibrato = 60-6a
	and #$0f
	ldy channel
	sta vibrato_depth1, y
	ldy #0 ; y needs to be zero for the pointer to work
	jmp read_byte
	
@end_slide: ;1xx,2xx,3xx with a parameter of zero
	lda #0
	sta qr_flag
	ldy channel
	sta slide_speed1, y
	sta slide_mode1, y
	lda #1
	sta zero_flag1, y ;keep slide value unless new note
	ldy #0	; y needs to be zero for the pointer to work
	jmp read_byte	

@slide_up_set:
	jsr Read_another_byte
	ldy channel
	sta slide_speed1, y
	lda #1
	sta slide_mode1, y
	ldy #0	; y needs to be zero for the pointer to work
	jmp read_byte

@slide_down_set:
	jsr Read_another_byte
	ldy channel
	sta slide_speed1, y
	lda #2
	sta slide_mode1, y
	ldy #0	; y needs to be zero for the pointer to work
	jmp read_byte
	
@portamento_set:
	jsr Read_another_byte
	ldy channel
	sta slide_speed1, y
	lda #3
	sta slide_mode1, y
	ldy #0	; y needs to be zero for the pointer to work
	jmp read_byte
	


; Qxy, speed = 2x+1, note offset = y (q = add, r = subtract)
@qxx_set:
	jsr @QR_Common
;	ldy #0 ;is already
@qxx_read_again:	
	lda (FT_TEMP_PTR),y ;read next byte over
@end_slide_bounce:	
	beq @end_slide ;note cut, end all
	cmp #MAX_NOTE
	bcc @qxx_note
	and #$81 ;row repeat
	cmp #$81
	beq @qxx_not_note
	iny
	jmp @qxx_read_again ;keep reading till we see a note or
						;a row repeat
	
@qxx_note:	
;is note, use as current, add to note for destination
	pha ;save note
	clc				;NOTE, no range check
	adc qr_offset	;add note to offset
	
@QR_Common2:	
	sta FT_CHN_NOTE,x ;new destination
	stx <FT_TEMP_VAR1 ;save x
	pla ;get note
	tax
	ldy channel
	lda _FT2NoteTableLSB,x
	sta slide_count_low1, y
	lda _FT2NoteTableMSB,x
	sta slide_count_high1, y
	lda #4 ;QR slide
	sta slide_mode1, y
	lda qr_rate
	sta slide_speed1, y
	lda #0
	sta zero_flag1, y ;bug fix
;restore x and y	
	ldy #0
	ldx <FT_TEMP_VAR1
	jmp read_byte ;if the next isn't a note, it's probably a repeat
	
@qxx_not_note: ;add to note = destination
	lda FT_CHN_NOTE,x
	clc
	adc qr_offset
	sta FT_CHN_NOTE,x	;NOTE, no range check
	ldy #0
	jmp read_byte
	
@rxx_set:
	jsr @QR_Common

@rxx_read_again:	
	lda (FT_TEMP_PTR),y ;read next byte over	
	beq @end_slide_bounce ;note cut, end all
	cmp #MAX_NOTE
	bcc @rxx_note
	and #$81 ;row repeat
	cmp #$81
	beq @rxx_not_note
	iny
	jmp @rxx_read_again ;keep reading till we see a note or
						;a row repeat
	
@rxx_note:	
;is note, use as current, add to note for destination
	pha ;save note
	sec				;NOTE, no range check
	sbc qr_offset	;add note to offset
	jmp @QR_Common2

@rxx_not_note: ;add to note = destination
	lda FT_CHN_NOTE,x
	sec
	sbc qr_offset
	sta FT_CHN_NOTE,x	;NOTE, no range check
	ldy #0
	jmp read_byte
	

; Qxy, speed = 2x+1	
@QR_Common:	;y = channel
	inc qr_flag ;so the note doesn't overwrite, later
	jsr Read_another_byte ;parameter
	pha 
	and #$0f
	sta qr_offset
	pla
	and #$f0
	lsr a
	lsr a
	lsr a ;x2
	clc
	adc #1 ;+1
	sta qr_rate
	rts
	

	

	
@no_pitch_effects: 	;new note
	ldy channel
	cpy #3	;this mess only refers to channel 0,1,2
	bcs @new_note_3
	ldy qr_flag ;if Qxx Rxx, don't write the note
	bne @npe2	
	sta FT_CHN_NOTE,x		;new note
	ldy channel
	lda slide_mode1, y		;new note + no qr_flag = cancel Qxx Rxx
	cmp #4
	bne @npe2
	lda #0
	sta slide_mode1, y
@npe2:	
	lda #0
	sta zero_flag1, y
	sec						;new note flag is set
	bcs channel_update_done ;bra
@new_note_3:
	sta FT_CHN_NOTE,x		;new note
	sec						;new note flag is set
	bcs channel_update_done ;bra
	

@special_code:
	and #$7f
	lsr a
	bcs @set_empty_rows
	asl a
	asl a
	sta FT_CHN_INSTRUMENT,x	;store instrument number*4
	jmp read_byte ;bcc Read_byte ;  

@set_empty_rows:
	cmp #$3d
	bcc @set_repeat
	beq @set_speed
	cmp #$3e
	beq @set_loop

@set_reference:
	clc						;remember return address+3
	lda <FT_TEMP_PTR_L
	adc #3
	sta FT_CHN_RETURN_L,x
	lda <FT_TEMP_PTR_H
	adc #0
	sta FT_CHN_RETURN_H,x
	lda (FT_TEMP_PTR),y		;read length of the reference (how many rows)
	sta FT_CHN_REF_LEN,x
	iny
	lda (FT_TEMP_PTR),y		;read 16-bit absolute address of the reference
	sta <FT_TEMP_VAR1		;remember in temp
	iny
	lda (FT_TEMP_PTR),y
	sta <FT_TEMP_PTR_H
	lda <FT_TEMP_VAR1
	sta <FT_TEMP_PTR_L
	ldy #0
	jmp read_byte

@set_speed:
	lda (FT_TEMP_PTR),y
	sta FT_SONG_SPEED
	inc <FT_TEMP_PTR_L		;advance pointer after reading the speed value
	
	beq @set_speed2
	jmp read_byte	; 
@set_speed2:
	inc <FT_TEMP_PTR_H
	beq @set_loop
	jmp read_byte ;bra

@set_loop:
	lda (FT_TEMP_PTR),y
	sta <FT_TEMP_VAR1
	iny
	lda (FT_TEMP_PTR),y
	sta <FT_TEMP_PTR_H
	lda <FT_TEMP_VAR1
	sta <FT_TEMP_PTR_L
	dey
	jmp read_byte

@set_repeat:
	sta FT_CHN_REPEAT,x		;set up repeat counter, carry is clear, no new note

channel_update_done:
	lda FT_CHN_REF_LEN,x	;check reference row counter
	beq @no_ref				;if it is zero, there is no reference
	dec FT_CHN_REF_LEN,x	;decrease row counter
	bne @no_ref

	lda FT_CHN_RETURN_L,x	;end of a reference, return to previous pointer
	sta FT_CHN_PTR_L,x
	lda FT_CHN_RETURN_H,x
	sta FT_CHN_PTR_H,x
	rts

@no_ref:
	lda <FT_TEMP_PTR_L
	sta FT_CHN_PTR_L,x
	lda <FT_TEMP_PTR_H
	sta FT_CHN_PTR_H,x
	rts
	
	
Read_another_byte:	;   added, y should == 0
	lda (FT_TEMP_PTR),y		;read byte of the channel

	inc <FT_TEMP_PTR_L		;advance pointer
	bne Read_another_byte2
	inc <FT_TEMP_PTR_H
Read_another_byte2:
	rts


	
;------------------------------------------------------------------------------
; stop DPCM sample if it plays
;------------------------------------------------------------------------------

FamiToneSampleStop:

	lda #%00001111
	sta APU_SND_CHN

	rts
	
	
	
	.if(FT_DPCM_ENABLE)	
	
;------------------------------------------------------------------------------
; play DPCM sample, used by music player, could be used externally
; in: A is number of a sample, 1..63
;------------------------------------------------------------------------------

FamiToneSamplePlayM:		;for music (low priority)

	ldx FT_DPCM_EFFECT
	beq _FT2SamplePlay
	tax
	lda APU_SND_CHN
	and #16
	beq @not_busy
	rts

@not_busy:
	sta FT_DPCM_EFFECT
	txa
	jmp _FT2SamplePlay

;------------------------------------------------------------------------------
; play DPCM sample with higher priority, for sound effects
; in: A is number of a sample, 1..63
;------------------------------------------------------------------------------

FamiToneSamplePlay:

	ldx #1
	stx FT_DPCM_EFFECT

_FT2SamplePlay:

	sta <FT_TEMP		;sample number*3, offset in the sample table
	asl a
	clc
	adc <FT_TEMP
	
	adc FT_DPCM_LIST_L
	sta <FT_TEMP_PTR_L
	lda #0
	adc FT_DPCM_LIST_H
	sta <FT_TEMP_PTR_H

	lda #%00001111			;stop DPCM
	sta APU_SND_CHN

	ldy #0
	lda (FT_TEMP_PTR),y		;sample offset
	sta APU_DMC_START
	iny
	lda (FT_TEMP_PTR),y		;sample length
	sta APU_DMC_LEN
	iny
	lda (FT_TEMP_PTR),y		;pitch and loop
	sta APU_DMC_FREQ

	lda #32					;reset DAC counter
	sta APU_DMC_RAW
	lda #%00011111			;start DMC
	sta APU_SND_CHN

	rts

	.endif

	.if(FT_SFX_ENABLE)

;------------------------------------------------------------------------------
; init sound effects player, set pointer to data
; in: X,Y is address of sound effects data
;------------------------------------------------------------------------------

FamiToneSfxInit:

;removed pal pitch fix  

	stx <FT_TEMP_PTR_L
	sty <FT_TEMP_PTR_H
	
	ldy #0
	
	lda (FT_TEMP_PTR),y		;read and store pointer to the effects list
	sta FT_SFX_ADR_L
	iny
	lda (FT_TEMP_PTR),y
	sta FT_SFX_ADR_H

	ldx #FT_SFX_CH0			;init all the streams

@set_channels:
	jsr _FT2SfxClearChannel
	txa
	clc
	adc #FT_SFX_STRUCT_SIZE
	tax
	cpx #FT_SFX_STRUCT_SIZE*FT_SFX_STREAMS
	bne @set_channels

	rts


;internal routine, clears output buffer of a sound effect
;in: A is 0
;    X is offset of sound effect stream

_FT2SfxClearChannel:

	lda #0
	sta FT_SFX_PTR_H,x		;this stops the effect
	sta FT_SFX_REPEAT,x
	sta FT_SFX_OFF,x
	sta FT_SFX_BUF+6,x		;mute triangle
	lda #$30
	sta FT_SFX_BUF+0,x		;mute pulse1
	sta FT_SFX_BUF+3,x		;mute pulse2
	sta FT_SFX_BUF+9,x		;mute noise

	rts


;------------------------------------------------------------------------------
; play sound effect
; in: A is a number of the sound effect
;     X is offset of sound effect channel, should be FT_SFX_CH0..FT_SFX_CH3
;------------------------------------------------------------------------------
	
FamiToneSfxPlay:

	asl a					;get offset in the effects list
	tay

	jsr _FT2SfxClearChannel	;stops the effect if it plays

	lda FT_SFX_ADR_L
	sta <FT_TEMP_PTR_L
	lda FT_SFX_ADR_H
	sta <FT_TEMP_PTR_H

	lda (FT_TEMP_PTR),y		;read effect pointer from the table
	sta FT_SFX_PTR_L,x		;store it
	iny
	lda (FT_TEMP_PTR),y
	sta FT_SFX_PTR_H,x		;this enables the effect

	rts


;internal routine, update one sound effect stream
;in: X is offset of sound effect stream

_FT2SfxUpdate:

	lda FT_SFX_REPEAT,x		;check if repeat counter is not zero
	beq @no_repeat
	dec FT_SFX_REPEAT,x		;decrement and return
	bne @update_buf			;just mix with output buffer

@no_repeat:
	lda FT_SFX_PTR_H,x		;check if MSB of the pointer is not zero
	bne @sfx_active
	rts						;return otherwise, no active effect

@sfx_active:
	sta <FT_TEMP_PTR_H		;load effect pointer into temp
	lda FT_SFX_PTR_L,x
	sta <FT_TEMP_PTR_L
	ldy FT_SFX_OFF,x
	clc

@read_byte2:
	lda (FT_TEMP_PTR),y		;read byte of effect
	bmi @get_data			;if bit 7 is set, it is a register write
	beq @eof
	jsr @sfx_inc ;iny
	sta FT_SFX_REPEAT,x		;if bit 7 is reset, it is number of repeats
	tya
	sta FT_SFX_OFF,x
	jmp @update_buf

@get_data:
	jsr @sfx_inc ;iny
	stx <FT_TEMP_VAR1		;it is a register write
	adc <FT_TEMP_VAR1		;get offset in the effect output buffer
	tax
	lda (FT_TEMP_PTR),y		;read value
	sta FT_SFX_BUF-128,x	;store into output buffer
	ldx <FT_TEMP_VAR1
	jsr @sfx_inc ;iny
	jmp @read_byte2			;and read next byte

	
	
; per rainwarrior's suggestion
; http://forums.nesdev.com/viewtopic.php?f=6&t=17789
	
@sfx_inc:
	iny
	bne @inc_done
	; offset >= 256, need to update the pointer to keep going
	inc <FT_TEMP_PTR_H
	inc FT_SFX_PTR_H,x
@inc_done:
	rts

@eof:
	sta FT_SFX_PTR_H,x		;mark channel as inactive

@update_buf:

	lda FT_OUT_BUF			;compare effect output buffer with main output buffer
	and #$0f				;if volume of pulse 1 of effect is higher than that of the
	sta <FT_TEMP_VAR1		;main buffer, overwrite the main buffer value with the new one
	lda FT_SFX_BUF+0,x
	and #$0f
		;cmp <FT_TEMP_VAR1
		;bcc @no_pulse1
		beq @no_pulse1
	lda FT_SFX_BUF+0,x
	sta FT_OUT_BUF+0
	lda FT_SFX_BUF+1,x
	sta FT_OUT_BUF+1
	lda FT_SFX_BUF+2,x
	sta FT_OUT_BUF+2
@no_pulse1:

	lda FT_OUT_BUF+3		;same for pulse 2
	and #$0f
	sta <FT_TEMP_VAR1
	lda FT_SFX_BUF+3,x
	and #$0f
		;cmp <FT_TEMP_VAR1
		;bcc @no_pulse2
		beq @no_pulse2 
	lda FT_SFX_BUF+3,x
	sta FT_OUT_BUF+3
	lda FT_SFX_BUF+4,x
	sta FT_OUT_BUF+4
	lda FT_SFX_BUF+5,x
	sta FT_OUT_BUF+5
@no_pulse2:

	lda FT_SFX_BUF+6,x		;overwrite triangle of main output buffer if it is active
	beq @no_triangle
	sta FT_OUT_BUF+6
	lda FT_SFX_BUF+7,x
	sta FT_OUT_BUF+7
	lda FT_SFX_BUF+8,x
	sta FT_OUT_BUF+8
@no_triangle:

	lda FT_OUT_BUF+9		;same as for pulse 1 and 2, but for noise
	and #$0f
	sta <FT_TEMP_VAR1
	lda FT_SFX_BUF+9,x
	and #$0f
		;cmp <FT_TEMP_VAR1
		;bcc @no_noise
		beq @no_noise
	lda FT_SFX_BUF+9,x
	sta FT_OUT_BUF+9
	lda FT_SFX_BUF+10,x
	sta FT_OUT_BUF+10
@no_noise:

	rts

	.endif


;dummy envelope used to initialize all channels with silence

_FT2DummyEnvelope:
	.byte $c0,$00,$00

;PAL support has been removed

_FT2NoteTableLSB:

	.byte $00
	.byte $f1,$7e,$13,$ad,$4d,$f3,$9d,$4c,$00,$b8,$74,$34
	.byte $f8,$bf,$89,$56,$26,$f9,$ce,$a6,$80,$5c,$3a,$1a
	.byte $fb,$df,$c4,$ab,$93,$7c,$67,$52,$3f,$2d,$1c,$0c
	.byte $fd,$ef,$e1,$d5,$c9,$bd,$b3,$a9,$9f,$96,$8e,$86
	.byte $7e,$77,$70,$6a,$64,$5e,$59,$54,$4f,$4b,$46,$42
	.byte $3f,$3b,$38,$34,$31,$2f,$2c,$29,$27,$25,$23,$21
	.byte $1f,$1d,$1b,$1a,$18,$17,$15,$14,$13,$12,$11,$10 
	.byte $0f,$0e,$0d

_FT2NoteTableMSB:

	.byte $00
	.byte $07,$07,$07,$06,$06,$05,$05,$05,$05,$04,$04,$04 ;12
	.byte $03,$03,$03,$03,$03,$02,$02,$02,$02,$02,$02,$02 ;24
	.byte $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01 ;36
	.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;48
	.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;60
	.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;72
	.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;84
	.byte $00,$00,$00 ;87

	
	
Multiply: 
			;a = note volume
			;x = volume column
			;from 6502.org
			
	
	sta multiple1
	lda multiple1 ;set flag
	beq M_3 ;skip if already zero
	inx
	stx multiple2
	
	ldx #8
M_1:
	asl a		;it is NOT necessary to initialize A
	asl multiple1
	bcc M_2
	clc
	adc multiple2

M_2:
	dex
	bne M_1
	;a = product
; now shift right so value = 0-f
	lsr a
	lsr a
	lsr a
	lsr a
	beq M_4 ;if zero, round up to 1
M_3:
	rts
M_4:
	lda #1
	rts

	