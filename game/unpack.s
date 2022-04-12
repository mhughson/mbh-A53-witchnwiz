
.import 	donut_decompress_block, _set_chr_bank_0
.importzp	donut_stream_ptr, donut_block_buffer, donut_block_count

.export 	upload_chars
	
.segment "ZEROPAGE"
xor_buffer:		.res 64
ppu_ptr:		.res 2
chr0_ptr:		.res 2
chr1_ptr:		.res 2

.segment "CODE"

PPU_ADDR	= $2006
PPU_DATA	= $2007

upload_chars:
	lda #0
	sta ppu_ptr
	sta ppu_ptr+1
	lda #<chars0
	sta chr0_ptr
	lda #>chars0
	sta chr0_ptr+1
	lda #<chars1
	sta chr1_ptr
	lda #>chars1
	sta chr1_ptr+1
	
	banks01_loop:
		lda chr0_ptr
		sta donut_stream_ptr
		lda chr0_ptr+1
		sta donut_stream_ptr+1
		
		lda #0
		jsr _set_chr_bank_0
		jsr set_addr_and_decompress
		ldx #$40
		
		upload_loop:
			lda donut_block_buffer-$40, X
			sta xor_buffer-$40, X
			sta PPU_DATA
			inx
		bpl upload_loop
		
		lda donut_stream_ptr
		sta chr0_ptr
		lda donut_stream_ptr+1
		sta chr0_ptr+1
		
		lda chr1_ptr
		sta donut_stream_ptr
		lda chr1_ptr+1
		sta donut_stream_ptr+1
		
		lda #1
		jsr _set_chr_bank_0
		jsr set_addr_and_decompress
		ldx #$40
		
		upload_loop1:
			lda donut_block_buffer-$40, X
			eor xor_buffer-$40, X
			sta PPU_DATA
			inx
		bpl upload_loop1
		
		lda donut_stream_ptr
		sta chr1_ptr
		lda donut_stream_ptr+1
		sta chr1_ptr+1
		
		lda #$40
		clc
		adc ppu_ptr
		sta ppu_ptr
		lda #0
		adc ppu_ptr+1
		sta ppu_ptr+1
		sbc #$20-1
	bne banks01_loop
	
	sta PPU_ADDR
	sta PPU_ADDR
	lda #2
	jsr _set_chr_bank_0
	lda #<chars2
	sta donut_stream_ptr
	lda #>chars2
	sta donut_stream_ptr+1
	lda #128
	sta donut_block_count
	block_loop:
		ldx #0
		jsr donut_decompress_block

		upload_loop2:
			lda donut_block_buffer-63-1, x
			sta PPU_DATA
			inx
		bpl upload_loop2
		ldx donut_block_count
	bne block_loop
	rts
	
set_addr_and_decompress:
	lda ppu_ptr+1
	sta PPU_ADDR
	lda ppu_ptr
	sta PPU_ADDR
	ldx #0
	jmp donut_decompress_block
	
.segment "BANK1"

chars0:
	.incbin "gfx/chrrom_bank0.d"
chars1:
	.incbin "gfx/0xor1.d"

; Move to another bank just to make space. Ran out in PRG1.
.segment "CODE"

chars2:
	.incbin "gfx/chrrom_bank2.d"