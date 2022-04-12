.feature c_comments
.linecont +

.define NO_CHR_BANK 255

.segment "ZEROPAGE"
	BP_BANK: .res 1
    nmiChrTileBank: .res 1
    .exportzp nmiChrTileBank, BP_BANK

.segment "CODE"

.export _set_prg_bank, _get_prg_bank, _set_chr_bank_0
.export _set_nmi_chr_tile_bank, _unset_nmi_chr_tile_bank

A53_REG_SELECT	= $5000
A53_REG_VALUE	= $8000
A53_SELECT_CHR	= $00
A53_SELECT_PRG	= $01

_set_prg_bank:

    ; Store new bank into BP_BANK
    sta BP_BANK

    lda #A53_SELECT_PRG
    sta A53_REG_SELECT

    lda BP_BANK
    sta A53_REG_VALUE

    rts

; returns the current bank at $8000-bfff	
_get_prg_bank:
    lda BP_BANK
	ldx #0
    rts

; sets the first CHR bank
_set_chr_bank_0:
    pha
    lda #A53_SELECT_CHR
    sta A53_REG_SELECT
    pla

    sta A53_REG_VALUE
    rts


; for split screens with different CHR bank at top
; Both of these just set/unset a varible used in `neslib.asm` to trigger this during nmi.
_set_nmi_chr_tile_bank: 
    sta nmiChrTileBank
    rts

; for split screens with different CHR bank at top... disable it
_unset_nmi_chr_tile_bank:
    lda #NO_CHR_BANK
    sta nmiChrTileBank
    rts
