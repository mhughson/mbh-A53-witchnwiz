;this file for FamiTone5.1 library generated by text2vol5 tool
;edited by Doug Fraker, 2019, to add volume column, all notes, 
;duty envelopes, and effects 1xx,2xx,3xx,4xx,Qxx,Rxx


songs_music_data:
	.byte 4
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,389,324 ; title
	.word @song1ch0,@song1ch1,@song1ch2,@song1ch3,@song1ch4,307,256 ; gameplay
	.word @song2ch0,@song2ch1,@song2ch2,@song2ch3,@song2ch4,307,256 ; victory
	.word @song3ch0,@song3ch1,@song3ch2,@song3ch3,@song3ch4,307,256 ; mirror

@instruments:
	.word @env1,@env0,@env0,@env0
	.word @env2,@env0,@env0,@env0
	.word @env3,@env0,@env0,@env15
	.word @env4,@env0,@env0,@env0
	.word @env5,@env0,@env0,@env0
	.word @env3,@env12,@env0,@env17
	.word @env3,@env0,@env0,@env18
	.word @env4,@env0,@env0,@env15
	.word @env5,@env0,@env0,@env16
	.word @env7,@env0,@env0,@env0
	.word @env3,@env0,@env0,@env16
	.word @env1,@env0,@env0,@env16
	.word @env8,@env13,@env0,@env17
	.word @env3,@env0,@env0,@env15
	.word @env3,@env14,@env0,@env17
	.word @env9,@env0,@env0,@env18
	.word @env9,@env0,@env0,@env0
	.word @env6,@env0,@env0,@env16
	.word @env5,@env0,@env0,@env16
	.word @env10,@env0,@env0,@env0
	.word @env11,@env13,@env0,@env0

@samples:
@env0:
	.byte $c0,$00,$00
@env1:
	.byte $cf,$cd,$cc,$cb,$ca,$ca,$c9,$c9,$c8,$c7,$c7,$c6,$c6,$c5,$02,$c4
	.byte $c4,$c3,$02,$c2,$02,$c1,$04,$c0,$c0,$c0,$00,$19
@env2:
	.byte $cf,$cf,$cd,$03,$cc,$04,$cb,$03,$ca,$04,$c9,$03,$c8,$05,$c7,$05
	.byte $c6,$05,$c5,$06,$c4,$07,$c3,$08,$c2,$09,$c1,$0b,$c0,$02,$c0,$00
	.byte $1e
@env3:
	.byte $cf,$00,$00
@env4:
	.byte $cc,$ca,$c8,$c6,$ca,$c7,$c5,$c4,$c3,$c2,$c1,$c0,$c0,$00,$0c
@env5:
	.byte $cf,$cb,$c6,$c2,$c0,$00,$04
@env6:
	.byte $cf,$ce,$cd,$cc,$cb,$ca,$c9,$c9,$c8,$c8,$c7,$c7,$c6,$02,$c5,$02
	.byte $c4,$03,$c3,$04,$c2,$05,$c1,$08,$c1,$00,$18
@env7:
	.byte $cf,$cb,$c8,$c6,$c4,$c4,$c3,$c3,$c2,$c2,$c1,$c1,$c0,$c0,$c0,$00
	.byte $0e
@env8:
	.byte $ce,$cf,$04,$ce,$cd,$cc,$ca,$c8,$c7,$c6,$c5,$c4,$c3,$c2,$c2,$c1
	.byte $c1,$c0,$09,$c0,$00,$13
@env9:
	.byte $c6,$cb,$ce,$cf,$cf,$00,$04
@env10:
	.byte $cf,$c5,$c0,$00,$02
@env11:
	.byte $cf,$c6,$c0,$00,$02
@env12:
	.byte $d8,$d8,$d3,$d3,$d0,$d0,$ce,$ce,$cc,$cc,$c7,$c7,$c4,$c4,$c2,$c2
	.byte $c0,$c0,$00,$00
@env13:
	.byte $cc,$c0,$00,$01
@env14:
	.byte $d8,$d8,$d3,$d3,$cf,$cf,$ce,$ce,$cc,$cc,$c7,$c7,$c3,$c3,$c2,$c2
	.byte $c0,$c0,$00,$00
@env15:
	.byte $c2,$00,$00
@env16:
	.byte $c1,$00,$00
@env17:
	.byte $c2,$c1,$c3,$00,$02
@env18:
	.byte $c3,$00,$00


; title
@song0ch0:
	.byte $fb,$02
@ref0:
	.byte $8f
@ref1:
	.byte $8f
@song0ch0loop:
@ref2:
	.byte $7a,$82,$04,$ad,$6c,$0a,$8f
@ref3:
	.byte $5f,$00,$8d,$04,$8d,$00,$8d,$04,$85,$00,$85
@ref4:
	.byte $7a,$04,$85,$00,$85,$04,$ad
@ref5:
	.byte $6c,$0a,$8f,$5f,$00,$9d,$04,$83,$00,$04,$83,$00
@ref6:
	.byte $7a,$04,$ad,$6c,$0a,$8f
@ref7:
	.byte $5f,$00,$8d,$04,$8d,$00,$8d,$04,$83,$00,$04,$83,$00
	.byte $ff,$06
	.word @ref4
@ref9:
	.byte $6c,$0a,$8f,$5f,$00,$8d,$0b,$9d
	.byte $ff,$03
	.word @ref6
@ref11:
	.byte $5f,$00,$8d,$04,$8d,$00,$8d,$04,$83,$00,$7c,$04,$83,$00
@ref12:
	.byte $7a,$04,$85,$00,$85,$7a,$04,$9b,$00,$04,$8b,$00
@ref13:
	.byte $03,$b3,$00,$03,$83,$00
@ref14:
	.byte $7a,$01,$9d,$7a,$0d,$9d
@ref15:
	.byte $7a,$09,$9d,$7a,$10,$9d
@ref16:
	.byte $7a,$0e,$85,$00,$85,$7a,$0e,$9d,$7a,$09,$8d
@ref17:
	.byte $7a,$06,$9b,$00,$02,$9d
@ref18:
	.byte $78,$8a,$1f,$8f,$72,$8d,$7a,$8c,$18,$19,$1a,$99
@ref19:
	.byte $1f,$95,$6c,$09,$85,$00,$5f,$25,$26,$99,$00
@ref20:
	.byte $7a,$25,$9d,$66,$8d,$00,$60,$21,$8b,$00
@ref21:
	.byte $60,$1f,$20,$21,$b7,$00
@ref22:
	.byte $77,$8a,$1f,$8f,$72,$8d,$7a,$8c,$18,$19,$1a,$8d,$1c,$81,$1a,$81
	.byte $19,$81
@ref23:
	.byte $1f,$95,$6c,$04,$85,$00,$5f,$22,$23,$99,$00
@ref24:
	.byte $7a,$21,$85,$1c,$83,$00,$1c,$ad
@ref25:
	.byte $66,$9d,$60,$00,$74,$8e,$31,$85,$75,$32,$85,$76,$34,$85,$77,$39
	.byte $83,$00
	.byte $ff,$0d
	.word @ref22
@ref27:
	.byte $1f,$95,$6c,$04,$85,$00,$5f,$25,$26,$99,$00
@ref28:
	.byte $25,$85,$26,$83,$00,$28,$8d,$66,$8d,$60,$00,$26,$85,$25,$83,$00
@ref29:
	.byte $26,$9d,$66,$8d,$60,$00,$21,$85,$23,$83,$00
@ref30:
	.byte $24,$9d,$66,$8d,$60,$00,$26,$85,$24,$83,$00
@ref31:
	.byte $23,$9d,$6d,$17,$1f,$9d
@ref32:
	.byte $23,$bd
@ref33:
	.byte $66,$9f,$60,$00,$83,$00,$5f,$82,$01,$83,$00,$02,$83,$00,$03,$83
	.byte $00
	.byte $fd
	.word @song0ch0loop

; title
@song0ch1:
@ref34:
	.byte $8f
@ref35:
	.byte $8f
@song0ch1loop:
@ref36:
	.byte $7a,$86,$28,$85,$23,$85,$20,$85,$1e,$85,$28,$85,$23,$85,$20,$85
	.byte $1e,$85
@ref37:
	.byte $28,$85,$23,$85,$20,$85,$1e,$85,$28,$85,$23,$85,$20,$85,$1e,$85
@ref38:
	.byte $79,$2a,$85,$25,$85,$22,$85,$20,$85,$2a,$85,$25,$85,$22,$85,$20
	.byte $85
@ref39:
	.byte $12,$85,$16,$85,$19,$85,$16,$85,$19,$85,$1e,$85,$22,$85,$25,$85
@ref40:
	.byte $79,$28,$85,$25,$85,$23,$85,$21,$85,$2d,$85,$28,$85,$25,$85,$21
	.byte $85
@ref41:
	.byte $28,$85,$24,$85,$23,$85,$21,$85,$28,$85,$24,$85,$23,$85,$21,$85
@ref42:
	.byte $79,$28,$85,$23,$85,$20,$85,$1e,$85,$28,$85,$23,$85,$20,$85,$1e
	.byte $85
@ref43:
	.byte $10,$85,$14,$85,$17,$85,$1c,$85,$17,$85,$1c,$85,$20,$85,$23,$85
	.byte $ff,$10
	.word @ref42
	.byte $ff,$10
	.word @ref37
@ref46:
	.byte $79,$2a,$85,$28,$85,$22,$85,$1e,$85,$2a,$85,$28,$85,$22,$85,$1e
	.byte $85
@ref47:
	.byte $2a,$85,$25,$85,$23,$85,$1e,$85,$2f,$85,$2a,$85,$27,$85,$23,$85
@ref48:
	.byte $79,$28,$85,$25,$85,$23,$85,$21,$85,$28,$85,$25,$85,$23,$85,$21
	.byte $85
@ref49:
	.byte $31,$85,$2f,$85,$2d,$85,$28,$85,$34,$85,$31,$85,$2f,$85,$2d,$85
@ref50:
	.byte $79,$36,$85,$34,$85,$32,$85,$2d,$85,$36,$85,$34,$85,$32,$85,$2d
	.byte $85
@ref51:
	.byte $32,$85,$2d,$85,$2a,$85,$2d,$85,$2a,$85,$26,$85,$2a,$85,$2d,$85
@ref52:
	.byte $79,$2f,$85,$2d,$85,$2b,$85,$26,$85,$8e,$2f,$85,$2d,$85,$2b,$85
	.byte $26,$85
@ref53:
	.byte $86,$2f,$85,$2d,$85,$2b,$85,$26,$85,$8e,$2f,$85,$2d,$85,$2b,$85
	.byte $26,$85
@ref54:
	.byte $79,$86,$31,$85,$2f,$85,$2d,$85,$28,$85,$8e,$31,$85,$2f,$85,$2d
	.byte $85,$28,$85
@ref55:
	.byte $86,$31,$85,$2f,$85,$2d,$85,$28,$85,$8e,$31,$85,$2f,$85,$2d,$85
	.byte $28,$85
@ref56:
	.byte $79,$86,$2f,$85,$2d,$85,$2b,$85,$26,$85,$8e,$2f,$85,$2d,$85,$2b
	.byte $85,$26,$85
	.byte $ff,$10
	.word @ref53
@ref58:
	.byte $79,$86,$28,$85,$25,$85,$23,$85,$1c,$85,$8e,$31,$85,$2f,$85,$2d
	.byte $85,$28,$85
@ref59:
	.byte $86,$25,$85,$28,$85,$2d,$85,$28,$85,$8e,$2d,$85,$2f,$85,$31,$85
	.byte $34,$85
	.byte $ff,$10
	.word @ref56
	.byte $ff,$10
	.word @ref53
	.byte $ff,$10
	.word @ref54
@ref63:
	.byte $86,$32,$85,$2d,$85,$2a,$85,$28,$85,$26,$85,$21,$85,$26,$85,$8e
	.byte $26,$85
@ref64:
	.byte $79,$86,$2b,$85,$28,$85,$24,$85,$1f,$85,$8e,$28,$85,$24,$85,$1f
	.byte $85,$1c,$85
@ref65:
	.byte $79,$86,$2b,$85,$26,$85,$23,$85,$1f,$85,$8e,$26,$85,$23,$85,$1f
	.byte $85,$1a,$85
@ref66:
	.byte $79,$86,$28,$85,$25,$85,$23,$85,$1e,$85,$79,$28,$85,$25,$85,$23
	.byte $85,$1e,$85
@ref67:
	.byte $79,$28,$85,$25,$85,$23,$85,$1e,$85,$2a,$85,$27,$85,$23,$85,$21
	.byte $85
	.byte $fd
	.word @song0ch1loop

; title
@song0ch2:
@ref68:
	.byte $8f
@ref69:
	.byte $8f
@song0ch2loop:
@ref70:
	.byte $9f,$84,$2d,$2e,$2f,$97,$00
@ref71:
	.byte $34,$95,$6c,$04,$85,$00,$5f,$3a,$3b,$99,$00
@ref72:
	.byte $3a,$bd
@ref73:
	.byte $63,$9f,$60,$00,$8d,$36,$85,$38,$83,$00
@ref74:
	.byte $39,$9d,$63,$9d,$00
@ref75:
	.byte $60,$3b,$9d,$63,$97,$60,$39,$83,$00
@ref76:
	.byte $38,$85,$36,$83,$00,$34,$9d,$63,$8f
@ref77:
	.byte $8f,$60,$00,$ad
@ref78:
	.byte $9f,$2d,$2e,$2f,$8d,$31,$81,$2f,$81,$2e,$81
	.byte $ff,$08
	.word @ref71
@ref80:
	.byte $3d,$9d,$63,$8d,$60,$00,$3b,$85,$3a,$83,$00
@ref81:
	.byte $3b,$9d,$00,$8d,$36,$85,$38,$83,$00
@ref82:
	.byte $39,$9d,$63,$8d,$00,$60,$38,$85,$36,$83,$00
@ref83:
	.byte $34,$95,$6c,$05,$87,$5f,$31,$8d,$36,$8d
@ref84:
	.byte $36,$bd
@ref85:
	.byte $63,$9f,$60,$00,$9d
@ref86:
	.byte $82,$13,$85,$00,$85,$13,$85,$00,$a5
@ref87:
	.byte $8f,$13,$8d,$00,$8d,$13,$83,$00,$13,$83,$00
@ref88:
	.byte $13,$85,$00,$85,$13,$85,$00,$a5
@ref89:
	.byte $97,$9a,$00,$85,$82,$0e,$95,$9a,$00,$85
	.byte $ff,$08
	.word @ref86
	.byte $ff,$0b
	.word @ref87
	.byte $ff,$08
	.word @ref88
@ref93:
	.byte $8f,$0e,$8d,$00,$8d,$0e,$8d
	.byte $ff,$08
	.word @ref88
	.byte $ff,$0b
	.word @ref87
@ref96:
	.byte $15,$9d,$6d,$20,$21,$9d
@ref97:
	.byte $5f,$1e,$9d,$0e,$9b,$00
@ref98:
	.byte $10,$bd
@ref99:
	.byte $5f,$13,$9d,$5f,$1a,$9b,$00
@ref100:
	.byte $1e,$ab,$00,$8e,$19,$83,$00,$17,$83,$00
@ref101:
	.byte $82,$12,$9b,$00,$17,$9b,$00
	.byte $fd
	.word @song0ch2loop

; title
@song0ch3:
@ref102:
	.byte $74,$80,$0c,$85,$76,$0a,$85
@ref103:
	.byte $78,$07,$85,$7a,$05,$85
@song0ch3loop:
@ref104:
	.byte $76,$82,$0a,$85,$75,$87,$74,$85,$72,$85,$71,$83,$00,$8d,$73,$88
	.byte $0e,$81,$74,$0e,$81,$75,$0e,$81,$76,$0e,$81
@ref105:
	.byte $77,$80,$06,$75,$81,$73,$81,$71,$81,$00,$85,$73,$82,$06,$85,$72
	.byte $87,$71,$81,$00,$8b,$75,$88,$0e,$81,$76,$0e,$81,$77,$0e,$81,$78
	.byte $0e,$81
@ref106:
	.byte $77,$0e,$85,$74,$0e,$85,$75,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74
	.byte $0e,$85,$75,$0e,$85,$74,$0e,$85
@ref107:
	.byte $78,$82,$0f,$85,$75,$87,$72,$83,$71,$83,$72,$88,$0e,$85,$74,$80
	.byte $0c,$85,$74,$0a,$85,$74,$08,$85,$74,$07,$85
	.byte $ff,$10
	.word @ref104
	.byte $ff,$14
	.word @ref105
	.byte $ff,$10
	.word @ref106
	.byte $ff,$0f
	.word @ref107
	.byte $ff,$10
	.word @ref104
	.byte $ff,$14
	.word @ref105
	.byte $ff,$10
	.word @ref106
	.byte $ff,$0f
	.word @ref107
	.byte $ff,$10
	.word @ref104
	.byte $ff,$14
	.word @ref105
	.byte $ff,$10
	.word @ref106
	.byte $ff,$0f
	.word @ref107
@ref120:
	.byte $7a,$90,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74,$0e,$85,$77,$0e,$85
	.byte $74,$0e,$85,$75,$0e,$81,$76,$0e,$81,$77,$0e,$81,$78,$0e,$81
@ref121:
	.byte $7a,$88,$06,$78,$81,$76,$81,$73,$81,$00,$85,$7a,$0f,$85,$78,$87
	.byte $77,$90,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74,$0e,$85
@ref122:
	.byte $7a,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74
	.byte $0e,$85,$75,$0e,$81,$76,$0e,$81,$77,$0e,$81,$78,$0e,$81
	.byte $ff,$11
	.word @ref121
	.byte $ff,$14
	.word @ref122
	.byte $ff,$11
	.word @ref121
@ref126:
	.byte $77,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74,$0e,$85,$77,$0e,$85,$74
	.byte $0e,$85,$75,$0e,$81,$76,$0e,$81,$77,$0e,$81,$78,$0e,$81
@ref127:
	.byte $7a,$88,$06,$78,$81,$76,$81,$73,$81,$00,$85,$75,$90,$0e,$81,$76
	.byte $0e,$81,$77,$0e,$81,$78,$0e,$81,$7a,$88,$06,$78,$81,$76,$81,$73
	.byte $81,$74,$90,$0e,$85,$7a,$0f,$85,$78,$87
	.byte $ff,$14
	.word @ref122
	.byte $ff,$11
	.word @ref121
	.byte $ff,$14
	.word @ref122
	.byte $ff,$11
	.word @ref121
	.byte $ff,$14
	.word @ref122
	.byte $ff,$11
	.word @ref121
	.byte $ff,$14
	.word @ref126
@ref135:
	.byte $7a,$06,$78,$81,$76,$81,$73,$81,$00,$85,$7a,$0f,$85,$78,$87,$72
	.byte $80,$0c,$85,$73,$0a,$85,$74,$07,$85,$77,$05,$85
	.byte $fd
	.word @song0ch3loop

; title
@song0ch4:
@ref136:
	.byte $8f
@ref137:
	.byte $8f
@song0ch4loop:
@ref138:
	.byte $bf
@ref139:
	.byte $bf
@ref140:
	.byte $bf
@ref141:
	.byte $bf
@ref142:
	.byte $bf
@ref143:
	.byte $bf
@ref144:
	.byte $bf
@ref145:
	.byte $bf
@ref146:
	.byte $bf
@ref147:
	.byte $bf
@ref148:
	.byte $bf
@ref149:
	.byte $bf
@ref150:
	.byte $bf
@ref151:
	.byte $bf
@ref152:
	.byte $bf
@ref153:
	.byte $bf
@ref154:
	.byte $bf
@ref155:
	.byte $bf
@ref156:
	.byte $bf
@ref157:
	.byte $bf
@ref158:
	.byte $bf
@ref159:
	.byte $bf
@ref160:
	.byte $bf
@ref161:
	.byte $bf
@ref162:
	.byte $bf
@ref163:
	.byte $bf
@ref164:
	.byte $bf
@ref165:
	.byte $bf
@ref166:
	.byte $bf
@ref167:
	.byte $bf
@ref168:
	.byte $bf
@ref169:
	.byte $bf
	.byte $fd
	.word @song0ch4loop


; gameplay
@song1ch0:
	.byte $fb,$03
@song1ch0loop:
@ref170:
	.byte $7f,$92,$09,$8d,$78,$09,$8d,$71,$09,$8d,$7f,$09,$8d,$78,$09,$8d
	.byte $71,$09,$85,$7f,$04,$83,$00
@ref171:
	.byte $09,$83,$00,$78,$04,$83,$00,$09,$83,$00,$71,$04,$83,$00,$09,$85
	.byte $00,$83,$00,$7f,$09,$8d,$78,$09,$85,$7f,$04,$8d,$7f,$04,$85
@ref172:
	.byte $76,$88,$26,$81,$72,$26,$81,$76,$22,$81,$72,$22,$81,$76,$1d,$81
	.byte $72,$1d,$81,$76,$22,$81,$72,$22,$81,$76,$1d,$81,$72,$1d,$81,$76
	.byte $1a,$81,$72,$1a,$81,$76,$1d,$81,$72,$1d,$81,$76,$1a,$81,$72,$1a
	.byte $81,$76,$16,$81,$72,$16,$89,$73,$16,$81,$71,$16,$81,$71,$80,$2a
	.byte $72,$2b,$73,$2c,$74,$2d
@ref173:
	.byte $92,$09,$83,$00,$78,$04,$83,$00,$09,$83,$00,$71,$04,$83,$00,$09
	.byte $85,$00,$83,$00,$7f,$09,$8d,$78,$09,$85,$7f,$04,$8d,$7f,$04,$85
@ref174:
	.byte $7f,$09,$8d,$78,$09,$8d,$71,$09,$8d,$7f,$09,$8d,$78,$09,$8d,$71
	.byte $09,$85,$7f,$04,$83,$00
	.byte $ff,$19
	.word @ref171
@ref176:
	.byte $76,$88,$26,$81,$72,$26,$81,$76,$22,$81,$72,$22,$81,$76,$21,$81
	.byte $72,$21,$81,$76,$22,$81,$72,$22,$81,$76,$21,$81,$72,$21,$81,$76
	.byte $1f,$81,$72,$1f,$81,$76,$1a,$81,$72,$1a,$81,$76,$1f,$81,$72,$1f
	.byte $81,$76,$1a,$81,$72,$1a,$89,$73,$1a,$81,$71,$1a,$87,$00
	.byte $ff,$19
	.word @ref173
	.byte $ff,$0f
	.word @ref174
	.byte $ff,$19
	.word @ref171
@ref180:
	.byte $75,$98,$24,$8d,$71,$24,$85,$75,$28,$8d,$71,$28,$85,$75,$2a,$8d
	.byte $72,$2a,$8d,$71,$2a,$8d
@ref181:
	.byte $72,$a0,$1b,$8b,$00,$1b,$81,$00,$81,$12,$8b,$00,$12,$81,$00,$81
	.byte $17,$81,$18,$81,$17,$85,$12,$8b,$00,$12,$81,$00,$85,$29,$2a
@ref182:
	.byte $75,$98,$2b,$85,$2a,$85,$2b,$85,$2a,$2b,$2a,$81,$28,$85,$24,$85
	.byte $2a,$8d,$72,$2a,$8b,$00,$71,$2a,$8b,$00
@ref183:
	.byte $87,$72,$a0,$17,$85,$18,$85,$15,$85,$1c,$85,$18,$1b,$18,$81,$17
	.byte $8b,$00,$71,$17,$72,$85,$00,$95
@ref184:
	.byte $78,$98,$2d,$8d,$73,$2d,$8d,$71,$2d,$8d,$00,$8d,$78,$2b,$8d,$73
	.byte $2b,$85,$78,$29,$85
@ref185:
	.byte $78,$28,$85,$2b,$85,$28,$2b,$28,$89,$74,$28,$2b,$28,$89,$71,$28
	.byte $2b,$28,$91,$78,$25,$8d,$74,$25,$85
@ref186:
	.byte $78,$26,$8d,$74,$26,$85,$78,$25,$85,$26,$85,$28,$85,$2b,$8d,$29
	.byte $8d,$26,$8d
@ref187:
	.byte $78,$25,$81,$26,$81,$28,$85,$74,$25,$81,$26,$81,$28,$85,$71,$25
	.byte $81,$26,$81,$28,$85,$00,$95,$78,$21,$8d,$74,$21,$85
@ref188:
	.byte $78,$2d,$8d,$73,$2d,$8d,$71,$2d,$85,$78,$2b,$85,$78,$30,$85,$78
	.byte $2e,$8d,$71,$2e,$85,$78,$2d,$85,$78,$2b,$85
@ref189:
	.byte $78,$2d,$8d,$74,$2d,$8d,$71,$2d,$a5,$78,$25,$8d,$74,$25,$85
@ref190:
	.byte $78,$26,$8d,$74,$26,$8d,$71,$26,$8d,$00,$ad
@ref191:
	.byte $b3,$72,$94,$1f,$83,$00,$21,$83,$00,$24,$83,$00,$72,$26,$83,$00
	.byte $27,$83,$00,$2d,$00
@ref192:
	.byte $df
@ref193:
	.byte $df
@ref194:
	.byte $df
@ref195:
	.byte $d7,$74,$8c,$26,$85
@ref196:
	.byte $74,$28,$85,$2b,$85,$2a,$85,$2b,$81,$2a,$81,$28,$85,$26,$85,$28
	.byte $85,$24,$85,$66,$87,$60,$1c,$85,$21,$85,$24,$85
@ref197:
	.byte $74,$28,$8d,$2a,$85,$26,$8d,$21,$85,$2a,$81,$2b,$81,$2a,$95,$28
	.byte $85,$26,$85
@ref198:
	.byte $74,$28,$85,$2d,$83,$00,$2d,$8d,$66,$8d,$00,$72,$60,$2d,$89,$00
	.byte $81,$71,$2d,$89,$00,$91
@ref199:
	.byte $d7,$74,$26,$85
	.byte $ff,$19
	.word @ref196
	.byte $ff,$12
	.word @ref197
	.byte $ff,$11
	.word @ref198
@ref203:
	.byte $71,$92,$0e,$85,$72,$0f,$81,$0e,$81,$73,$0c,$83,$00,$74,$09,$85
	.byte $76,$07,$85,$78,$09,$83,$00,$7a,$03,$85,$7b,$04,$85,$7c,$09,$83
	.byte $00,$7d,$09,$85,$7e,$0c,$85,$7f,$0d,$85
@ref204:
	.byte $76,$5f,$98,$2d,$85,$2b,$85,$2d,$85,$29,$85,$26,$85,$71,$29,$85
	.byte $76,$2d,$85,$2b,$85,$2d,$85,$26,$85,$29,$85,$2c,$85
@ref205:
	.byte $76,$2d,$8d,$73,$2d,$8d,$71,$2d,$8d,$76,$28,$8d,$73,$28,$8d,$71
	.byte $28,$8d
@ref206:
	.byte $76,$2b,$85,$29,$85,$2b,$85,$27,$85,$24,$85,$71,$27,$85,$76,$2b
	.byte $85,$29,$85,$2b,$85,$24,$85,$27,$85,$2a,$85
@ref207:
	.byte $76,$2b,$8d,$73,$2b,$8d,$71,$2b,$8d,$76,$26,$8d,$73,$26,$8d,$71
	.byte $26,$8d
@ref208:
	.byte $71,$8a,$20,$83,$71,$85,$72,$20,$83,$72,$85,$71,$20,$81,$71,$83
	.byte $76,$98,$24,$8d,$29,$8d,$73,$29,$8d,$71,$29,$85,$76,$27,$85
@ref209:
	.byte $76,$26,$81,$29,$81,$26,$85,$73,$26,$81,$29,$81,$26,$85,$71,$26
	.byte $81,$29,$81,$26,$85,$00,$95,$76,$24,$8d,$72,$24,$85
@ref210:
	.byte $76,$24,$8d,$73,$24,$8d,$71,$24,$8d,$71,$9c,$1d,$89,$72,$8b,$72
	.byte $8a,$1b,$89,$71,$8b
@ref211:
	.byte $76,$88,$24,$81,$72,$24,$81,$76,$20,$81,$72,$20,$81,$76,$1f,$81
	.byte $72,$1f,$81,$76,$20,$81,$72,$20,$81,$76,$1f,$81,$72,$1f,$81,$76
	.byte $1d,$81,$72,$1d,$81,$76,$1c,$81,$72,$1c,$81,$76,$1d,$81,$72,$1d
	.byte $81,$76,$20,$81,$72,$20,$81,$76,$23,$81,$72,$23,$81,$76,$26,$81
	.byte $72,$26,$81,$76,$28,$81,$73,$28,$81
	.byte $ff,$0f
	.word @ref170
	.byte $ff,$19
	.word @ref171
	.byte $fd
	.word @song1ch0loop

; gameplay
@song1ch1:
@song1ch1loop:
@ref214:
	.byte $df
@ref215:
	.byte $db,$72,$88,$24,$76,$25
@ref216:
	.byte $76,$2e,$81,$72,$2e,$81,$76,$29,$81,$72,$29,$81,$76,$26,$81,$72
	.byte $26,$81,$76,$29,$81,$72,$29,$81,$76,$26,$81,$72,$26,$81,$76,$22
	.byte $81,$72,$22,$81,$76,$26,$81,$72,$26,$81,$76,$22,$81,$72,$22,$81
	.byte $76,$1d,$81,$72,$1d,$91,$7f,$92,$04,$83,$00
@ref217:
	.byte $74,$8a,$16,$85,$73,$87,$71,$87,$73,$0a,$85,$72,$87,$71,$83,$00
	.byte $81,$76,$80,$3a,$81,$6c,$10,$83,$5f,$00,$85,$72,$3a,$81,$6c,$10
	.byte $83,$5f,$00,$85,$71,$3a,$81,$6c,$10,$83,$5f,$00,$85
@ref218:
	.byte $df
@ref219:
	.byte $db,$73,$88,$20,$76,$21
@ref220:
	.byte $76,$22,$81,$72,$22,$81,$76,$1f,$81,$72,$1f,$81,$76,$1a,$81,$72
	.byte $1a,$81,$76,$1f,$81,$72,$1f,$81,$76,$1a,$81,$72,$1a,$81,$76,$16
	.byte $81,$72,$16,$81,$76,$13,$81,$72,$13,$81,$76,$16,$81,$72,$16,$81
	.byte $76,$13,$81,$72,$13,$89,$71,$92,$09,$85,$7f,$04,$83,$00
@ref221:
	.byte $74,$9c,$1f,$85,$72,$87,$71,$87,$73,$13,$85,$72,$87,$71,$83,$00
	.byte $81,$76,$80,$37,$81,$6c,$10,$83,$5f,$00,$85,$72,$37,$81,$6c,$10
	.byte $83,$5f,$00,$85,$71,$37,$81,$6c,$10,$83,$5f,$00,$85
@ref222:
	.byte $df
@ref223:
	.byte $af,$71,$8c,$39,$8d,$72,$8f,$74,$8f
@ref224:
	.byte $75,$98,$15,$8d,$71,$15,$85,$75,$18,$8d,$71,$18,$85,$75,$17,$8d
	.byte $72,$17,$8d,$71,$17,$83,$00,$7f,$92,$04,$85
@ref225:
	.byte $09,$83,$00,$78,$04,$83,$00,$09,$83,$00,$71,$04,$83,$00,$09,$85
	.byte $00,$83,$00,$7f,$09,$8d,$78,$09,$85,$7f,$04,$8d,$7f,$04,$83,$00
@ref226:
	.byte $75,$98,$18,$85,$1b,$85,$18,$85,$17,$18,$17,$81,$15,$85,$10,$85
	.byte $17,$8d,$72,$17,$8b,$00,$71,$17,$8b,$00
@ref227:
	.byte $92,$09,$83,$00,$78,$04,$83,$00,$09,$83,$00,$71,$04,$83,$00,$09
	.byte $85,$00,$83,$00,$7f,$09,$8d,$7f,$04,$8d,$7f,$04,$8b,$00
@ref228:
	.byte $72,$9e,$26,$85,$24,$85,$21,$85,$1d,$85,$1a,$85,$1d,$85,$1f,$85
	.byte $1a,$85,$1d,$85,$1f,$85,$22,$85,$24,$85
@ref229:
	.byte $72,$25,$85,$22,$85,$21,$85,$1c,$85,$1a,$85,$1c,$81,$1a,$81,$19
	.byte $85,$1a,$85,$1c,$85,$1f,$85,$21,$85,$22,$83,$00
@ref230:
	.byte $72,$22,$85,$21,$85,$1f,$85,$1a,$85,$1f,$85,$21,$85,$22,$85,$21
	.byte $85,$26,$85,$22,$85,$21,$85,$1f,$85
@ref231:
	.byte $72,$1c,$85,$19,$85,$17,$85,$15,$85,$10,$85,$15,$85,$17,$85,$19
	.byte $85,$1c,$85,$21,$85,$1c,$85,$21,$85
@ref232:
	.byte $72,$26,$85,$24,$85,$21,$85,$1d,$85,$1a,$85,$1d,$85,$1f,$85,$1a
	.byte $85,$1d,$85,$1f,$85,$22,$85,$24,$85
	.byte $ff,$1b
	.word @ref229
	.byte $ff,$18
	.word @ref230
@ref235:
	.byte $72,$28,$85,$26,$81,$28,$26,$22,$85,$21,$85,$1f,$85,$1a,$85,$94
	.byte $1f,$83,$00,$21,$83,$00,$24,$83,$00,$74,$26,$83,$00,$27,$83,$00
	.byte $2d,$83,$00
@ref236:
	.byte $df
@ref237:
	.byte $df
@ref238:
	.byte $df
@ref239:
	.byte $d9,$72,$8c,$26,$83
@ref240:
	.byte $83,$72,$28,$85,$2b,$85,$2a,$85,$2b,$81,$2a,$81,$28,$85,$26,$85
	.byte $28,$85,$24,$85,$66,$87,$60,$1c,$85,$21,$85,$24,$81
@ref241:
	.byte $83,$72,$28,$8d,$2a,$85,$26,$8d,$21,$85,$2a,$81,$2b,$81,$2a,$95
	.byte $28,$85,$26,$81
@ref242:
	.byte $83,$72,$28,$85,$2d,$85,$2d,$8d,$66,$8f,$60,$00,$a9
@ref243:
	.byte $d7,$74,$94,$23,$85
@ref244:
	.byte $74,$24,$8d,$26,$85,$23,$85,$66,$87,$60,$1f,$85,$21,$85,$1c,$85
	.byte $1e,$85,$1f,$85,$1e,$85,$1c,$85
@ref245:
	.byte $74,$18,$85,$1a,$85,$6d,$20,$21,$85,$1f,$8d,$5f,$1e,$83,$00,$1a
	.byte $8d,$6d,$20,$23,$85,$5f,$21,$8d,$23,$85
@ref246:
	.byte $74,$24,$85,$26,$83,$00,$28,$8d,$66,$8d,$00,$72,$60,$28,$89,$00
	.byte $81,$71,$28,$89,$00,$91
@ref247:
	.byte $bf,$71,$6b,$01,$8c,$2d,$85,$71,$87,$72,$87,$74,$87
@ref248:
	.byte $76,$5f,$98,$15,$85,$13,$85,$15,$85,$11,$85,$0e,$85,$71,$11,$85
	.byte $76,$15,$85,$13,$85,$15,$85,$0e,$85,$11,$85,$14,$85
@ref249:
	.byte $76,$15,$8d,$73,$15,$8d,$71,$15,$8d,$76,$10,$8d,$73,$94,$21,$85
	.byte $6d,$33,$22,$85,$21,$81,$22,$21,$1c,$85
@ref250:
	.byte $73,$5f,$1f,$85,$72,$83,$71,$83,$76,$98,$13,$85,$0f,$85,$0c,$85
	.byte $71,$0f,$85,$76,$13,$85,$11,$85,$13,$85,$0c,$85,$0f,$85,$12,$85
@ref251:
	.byte $76,$13,$8d,$73,$13,$8d,$71,$13,$8d,$76,$0e,$8d,$73,$0e,$85,$72
	.byte $94,$1f,$85,$6d,$33,$1d,$85,$1b,$85
@ref252:
	.byte $73,$5f,$1f,$83,$72,$85,$71,$83,$72,$9c,$24,$83,$72,$85,$71,$24
	.byte $81,$76,$98,$0c,$8d,$11,$8d,$73,$11,$8d,$71,$11,$85,$76,$0f,$85
@ref253:
	.byte $76,$0e,$81,$11,$81,$0e,$85,$73,$0e,$81,$11,$81,$0e,$85,$71,$0e
	.byte $81,$11,$81,$0e,$85,$00,$95,$76,$0c,$8d,$72,$0c,$85
@ref254:
	.byte $76,$0c,$8d,$73,$0c,$85,$76,$a4,$14,$85,$18,$85,$1d,$85,$00,$85
	.byte $71,$9c,$1d,$89,$72,$8b,$72,$8a,$1b,$89,$88,$2a,$2b
@ref255:
	.byte $76,$2c,$81,$72,$2c,$81,$76,$29,$81,$72,$29,$81,$76,$25,$81,$72
	.byte $25,$81,$76,$29,$81,$72,$29,$81,$76,$25,$81,$72,$25,$81,$76,$24
	.byte $81,$72,$24,$81,$76,$23,$81,$72,$23,$81,$76,$26,$81,$72,$26,$81
	.byte $76,$29,$81,$72,$29,$81,$76,$28,$81,$72,$28,$81,$76,$2c,$81,$72
	.byte $2c,$81,$76,$2f,$81,$72,$2f,$81
@ref256:
	.byte $df
@ref257:
	.byte $df
	.byte $fd
	.word @song1ch1loop

; gameplay
@song1ch2:
@song1ch2loop:
@ref258:
	.byte $6c,$60,$92,$28,$5f,$15,$ab,$6c,$60,$28,$5f,$15,$a1,$00,$6c,$60
	.byte $28,$5f,$10,$81,$00
@ref259:
	.byte $6c,$60,$28,$5f,$15,$a9,$00,$6c,$60,$28,$5f,$15,$93,$6c,$60,$28
	.byte $5f,$10,$8b,$6c,$60,$28,$5f,$10,$81,$00
@ref260:
	.byte $6c,$60,$28,$5f,$15,$ab,$6c,$60,$28,$5f,$15,$a1,$00,$6c,$60,$28
	.byte $5f,$10,$81,$00
	.byte $ff,$0e
	.word @ref259
	.byte $ff,$0b
	.word @ref260
	.byte $ff,$0e
	.word @ref259
	.byte $ff,$0b
	.word @ref260
	.byte $ff,$0e
	.word @ref259
	.byte $ff,$0b
	.word @ref260
	.byte $ff,$0e
	.word @ref259
	.byte $ff,$0b
	.word @ref260
	.byte $ff,$0e
	.word @ref259
	.byte $ff,$0b
	.word @ref260
	.byte $ff,$0e
	.word @ref259
@ref272:
	.byte $6c,$60,$28,$5f,$8c,$16,$8b,$6d,$45,$22,$85,$00,$8b,$00,$6c,$60
	.byte $92,$28,$5f,$11,$81,$00,$6c,$60,$28,$5f,$8c,$16,$93,$6c,$60,$92
	.byte $28,$5f,$8c,$16,$93
@ref273:
	.byte $6c,$60,$92,$28,$5f,$80,$15,$9b,$00,$83,$00,$6c,$60,$92,$28,$5f
	.byte $10,$81,$00,$6c,$60,$28,$5f,$80,$15,$93,$6c,$60,$92,$28,$5f,$10
	.byte $8b,$6c,$60,$28,$5f,$10,$81,$00
@ref274:
	.byte $6c,$60,$28,$5f,$80,$13,$a1,$00,$6c,$60,$92,$28,$5f,$1a,$81,$00
	.byte $6c,$60,$28,$5f,$80,$1f,$8b,$6c,$05,$87,$5f,$1a,$8d,$6c,$60,$92
	.byte $28,$5f,$0e,$81,$00
@ref275:
	.byte $6c,$60,$28,$5f,$80,$15,$9b,$00,$83,$00,$6c,$60,$92,$28,$5f,$10
	.byte $81,$00,$6c,$60,$28,$5f,$80,$15,$93,$6c,$60,$92,$28,$5f,$10,$8b
	.byte $6c,$60,$28,$5f,$10,$81,$00
	.byte $ff,$12
	.word @ref272
	.byte $ff,$14
	.word @ref273
	.byte $ff,$12
	.word @ref274
@ref279:
	.byte $6c,$60,$28,$5f,$80,$13,$a1,$00,$6c,$60,$92,$28,$5f,$0e,$81,$00
	.byte $6c,$60,$28,$5f,$80,$11,$93,$6c,$60,$92,$28,$5f,$80,$10,$91,$00
@ref280:
	.byte $6c,$60,$92,$28,$5f,$16,$81,$00,$6c,$60,$28,$5f,$15,$8b,$6c,$60
	.byte $28,$5f,$15,$8b,$6c,$60,$28,$5f,$10,$83,$6c,$60,$28,$5f,$80,$13
	.byte $8b,$6c,$60,$92,$28,$5f,$15,$89,$00,$8f
@ref281:
	.byte $df
@ref282:
	.byte $6c,$60,$28,$5f,$16,$81,$00,$6c,$60,$28,$5f,$15,$8b,$6c,$60,$28
	.byte $5f,$15,$8b,$6c,$60,$28,$5f,$10,$83,$6c,$60,$28,$5f,$80,$13,$8b
	.byte $6c,$60,$92,$28,$5f,$15,$89,$00,$8f
@ref283:
	.byte $df
	.byte $ff,$15
	.word @ref282
@ref285:
	.byte $df
	.byte $ff,$15
	.word @ref282
@ref287:
	.byte $df
	.byte $ff,$15
	.word @ref282
@ref289:
	.byte $df
@ref290:
	.byte $6c,$60,$28,$5f,$16,$81,$00,$6c,$60,$28,$5f,$15,$8b,$6c,$60,$28
	.byte $5f,$15,$8b,$6c,$60,$28,$5f,$10,$83,$6c,$60,$28,$5f,$80,$13,$8b
	.byte $6c,$60,$92,$28,$5f,$15,$8b,$6c,$60,$28,$5f,$18,$83,$6c,$60,$28
	.byte $5f,$1a,$81,$00
@ref291:
	.byte $6c,$60,$28,$5f,$1a,$83,$6c,$60,$28,$5f,$1b,$6c,$60,$28,$5f,$1a
	.byte $6c,$60,$28,$5f,$18,$81,$00,$6c,$60,$28,$5f,$15,$83,$6c,$60,$28
	.byte $5f,$13,$83,$6c,$60,$28,$5f,$15,$81,$00,$6c,$60,$28,$5f,$0f,$83
	.byte $6c,$60,$28,$5f,$10,$83,$6c,$60,$28,$5f,$15,$81,$00,$6c,$60,$28
	.byte $5f,$09,$83,$6c,$60,$28,$5f,$0c,$83,$6c,$60,$28,$5f,$0d,$83
@ref292:
	.byte $6c,$60,$28,$5f,$82,$0e,$8b,$6d,$70,$1a,$83,$00,$a8,$32,$85,$35
	.byte $85,$39,$83,$00,$6c,$60,$92,$28,$5f,$82,$11,$8b,$6d,$70,$1a,$83
	.byte $00,$a8,$2d,$85,$32,$85,$35,$85
@ref293:
	.byte $6c,$60,$92,$28,$5f,$82,$0d,$8b,$6d,$70,$15,$83,$00,$a8,$2d,$85
	.byte $31,$85,$34,$85,$6c,$60,$92,$28,$5f,$82,$15,$8b,$6c,$03,$87,$6c
	.byte $60,$92,$28,$5f,$82,$09,$83,$00,$85,$6c,$60,$92,$28,$5f,$82,$09
	.byte $83
@ref294:
	.byte $6c,$60,$92,$28,$5f,$82,$0c,$8b,$6d,$70,$18,$83,$00,$a8,$30,$85
	.byte $33,$85,$37,$83,$00,$6c,$60,$92,$28,$5f,$82,$0f,$8b,$6d,$70,$18
	.byte $83,$00,$a8,$2b,$85,$30,$85,$33,$85
@ref295:
	.byte $6c,$60,$92,$28,$5f,$82,$0b,$8b,$6d,$70,$13,$83,$00,$a8,$2b,$85
	.byte $2f,$85,$32,$85,$6c,$60,$92,$28,$5f,$82,$13,$8b,$6c,$03,$87,$6c
	.byte $60,$92,$28,$5f,$82,$07,$83,$00,$85,$6c,$60,$92,$28,$5f,$82,$07
	.byte $83
@ref296:
	.byte $6c,$60,$92,$28,$5f,$14,$83,$6c,$60,$28,$5f,$08,$83,$6c,$60,$28
	.byte $5f,$08,$83,$6c,$60,$28,$5f,$14,$83,$6c,$60,$28,$5f,$08,$83,$6c
	.byte $60,$28,$5f,$08,$83,$6c,$60,$28,$5f,$14,$8b,$6d,$70,$20,$85,$a8
	.byte $30,$85,$33,$85,$38,$85
@ref297:
	.byte $6c,$60,$92,$28,$5f,$82,$13,$8b,$6d,$70,$23,$83,$00,$a8,$2b,$85
	.byte $2f,$85,$32,$85,$6c,$60,$92,$28,$5f,$82,$1f,$8b,$6c,$03,$87,$6c
	.byte $60,$92,$28,$5f,$82,$13,$83,$00,$85,$6c,$60,$92,$28,$5f,$82,$07
	.byte $83
@ref298:
	.byte $6c,$60,$92,$28,$5f,$82,$11,$8b,$6d,$70,$20,$85,$5f,$a8,$35,$85
	.byte $38,$85,$3c,$85,$6c,$60,$92,$28,$5f,$14,$a1,$00,$6c,$60,$28,$5f
	.byte $0c,$81,$00
@ref299:
	.byte $6c,$60,$28,$5f,$11,$ab,$6c,$60,$28,$5f,$10,$8b,$6c,$60,$28,$5f
	.byte $10,$8b,$6c,$60,$28,$5f,$82,$10,$8b
	.byte $ff,$0b
	.word @ref258
	.byte $ff,$0e
	.word @ref259
	.byte $fd
	.word @song1ch2loop

; gameplay
@song1ch3:
@song1ch3loop:
@ref302:
	.byte $74,$a6,$0c,$85,$72,$0c,$85,$74,$0c,$8d,$72,$0c,$8d,$74,$0c,$85
	.byte $72,$0c,$85,$74,$0c,$8d,$72,$0c,$85,$72,$0c,$81,$73,$0c,$81
@ref303:
	.byte $74,$0c,$85,$72,$0c,$85,$74,$0c,$8d,$72,$0c,$8d,$74,$0c,$85,$72
	.byte $0c,$85,$74,$0c,$8d,$72,$0c,$85,$72,$0c,$81,$73,$0c,$81
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $ff,$14
	.word @ref303
	.byte $fd
	.word @song1ch3loop

; gameplay
@song1ch4:
@song1ch4loop:
@ref346:
	.byte $df
@ref347:
	.byte $df
@ref348:
	.byte $df
@ref349:
	.byte $df
@ref350:
	.byte $df
@ref351:
	.byte $df
@ref352:
	.byte $df
@ref353:
	.byte $df
@ref354:
	.byte $df
@ref355:
	.byte $df
@ref356:
	.byte $df
@ref357:
	.byte $df
@ref358:
	.byte $df
@ref359:
	.byte $df
@ref360:
	.byte $df
@ref361:
	.byte $df
@ref362:
	.byte $df
@ref363:
	.byte $df
@ref364:
	.byte $df
@ref365:
	.byte $df
@ref366:
	.byte $df
@ref367:
	.byte $df
@ref368:
	.byte $df
@ref369:
	.byte $df
@ref370:
	.byte $df
@ref371:
	.byte $df
@ref372:
	.byte $df
@ref373:
	.byte $df
@ref374:
	.byte $df
@ref375:
	.byte $df
@ref376:
	.byte $df
@ref377:
	.byte $df
@ref378:
	.byte $df
@ref379:
	.byte $df
@ref380:
	.byte $df
@ref381:
	.byte $df
@ref382:
	.byte $df
@ref383:
	.byte $df
@ref384:
	.byte $df
@ref385:
	.byte $df
@ref386:
	.byte $df
@ref387:
	.byte $df
@ref388:
	.byte $df
@ref389:
	.byte $df
	.byte $fd
	.word @song1ch4loop


; victory
@song2ch0:
	.byte $fb,$02
@ref390:
	.byte $7f,$92,$25,$85,$24,$85,$25,$85,$21,$81,$00,$81,$1c,$81,$00,$81
	.byte $21,$81,$00,$81,$23,$85,$20,$85,$1c,$85,$26,$85,$25,$85,$23,$85
@song2ch0loop:
@ref391:
	.byte $7a,$8c,$39,$81,$6c,$10,$85,$5f,$00,$83,$72,$39,$81,$6c,$10,$83
	.byte $7f,$5f,$a2,$1c,$95,$7f,$21,$89,$28,$93,$00,$7f,$28,$89
@ref392:
	.byte $8b,$7f,$2a,$89,$27,$93,$00,$73,$27,$87,$00,$7f,$23,$89,$25,$87
	.byte $00,$26,$89
@ref393:
	.byte $89,$00,$73,$26,$89,$00,$87,$00,$71,$26,$89,$7f,$21,$93,$00,$7f
	.byte $23,$8f,$00,$83
@ref394:
	.byte $7f,$25,$83,$23,$83,$25,$87,$00,$73,$25,$83,$23,$83,$25,$87,$00
	.byte $71,$25,$83,$23,$83,$25,$87,$00,$97
@ref395:
	.byte $97,$7f,$1c,$93,$00,$7f,$21,$89,$25,$93,$00,$7f,$23,$89
@ref396:
	.byte $89,$00,$7f,$25,$87,$00,$1e,$93,$00,$76,$1e,$87,$00,$7f,$1e,$89
	.byte $20,$89,$21,$89
@ref397:
	.byte $89,$00,$76,$21,$93,$00,$71,$21,$87,$00,$7f,$21,$89,$1e,$89,$21
	.byte $87,$00,$25,$89
@ref398:
	.byte $89,$00,$7f,$26,$89,$25,$87,$00,$23,$93,$00,$76,$23,$93,$00,$71
	.byte $23,$89
	.byte $fd
	.word @song2ch0loop

; victory
@song2ch1:
@ref399:
	.byte $7f,$92,$09,$95,$0d,$95,$0b,$8d,$10,$85,$04,$85,$06,$85,$08,$85
@song2ch1loop:
@ref400:
	.byte $92,$09,$95,$78,$5f,$a2,$19,$87,$00,$7f,$92,$09,$81,$00,$09,$81
	.byte $00,$78,$a2,$1c,$89,$21,$93,$00,$78,$23,$89
@ref401:
	.byte $8b,$78,$23,$89,$1e,$87,$00,$7f,$92,$09,$81,$00,$7f,$09,$81,$00
	.byte $71,$a2,$1e,$89,$78,$1e,$89,$1c,$87,$00,$1e,$89
@ref402:
	.byte $89,$00,$71,$1e,$89,$00,$87,$00,$7f,$92,$09,$81,$00,$09,$81,$00
	.byte $78,$a2,$1d,$ad
@ref403:
	.byte $78,$1c,$93,$00,$73,$1c,$87,$00,$7f,$92,$09,$81,$00,$7f,$09,$81
	.byte $00,$71,$94,$1c,$87,$00,$a3
@ref404:
	.byte $97,$78,$5f,$a2,$19,$87,$00,$7f,$92,$09,$81,$00,$09,$81,$00,$78
	.byte $a2,$1c,$89,$21,$93,$00,$1e,$89
@ref405:
	.byte $97,$78,$1b,$87,$00,$7f,$92,$09,$81,$00,$7f,$09,$81,$00,$8b,$78
	.byte $a2,$1b,$89,$17,$89,$1a,$89
@ref406:
	.byte $a1,$00,$92,$0e,$81,$00,$0e,$81,$00,$94,$1e,$89,$a2,$1a,$89,$1e
	.byte $89,$21,$89
@ref407:
	.byte $8b,$23,$89,$21,$87,$00,$20,$93,$00,$71,$20,$93,$00,$8b
	.byte $fd
	.word @song2ch1loop

; victory
@song2ch2:
@ref408:
	.byte $92,$39,$81,$00,$89,$39,$00,$39,$81,$3d,$81,$00,$81,$39,$81,$00
	.byte $81,$3d,$81,$00,$81,$40,$81,$42,$81,$40,$81,$42,$81,$40,$81,$42
	.byte $81,$40,$81,$42,$81,$40,$81,$42,$81,$43,$81,$44,$81
@song2ch2loop:
@ref409:
	.byte $6c,$45,$82,$28,$5f,$15,$85,$00,$21,$81,$00,$21,$81,$00,$15,$87
	.byte $00,$21,$81,$00,$2d,$81,$00,$6c,$45,$28,$5f,$15,$85,$00,$21,$81
	.byte $00,$21,$81,$00,$10,$87,$00,$6c,$45,$28,$5f,$15,$87
@ref410:
	.byte $89,$00,$21,$81,$00,$21,$81,$00,$15,$87,$00,$21,$81,$00,$2d,$81
	.byte $00,$6c,$45,$28,$5f,$15,$85,$00,$21,$81,$00,$21,$81,$00,$6c,$45
	.byte $34,$5f,$1c,$85,$00,$5f,$1a,$87,$00
@ref411:
	.byte $6c,$45,$28,$5f,$15,$85,$00,$21,$81,$00,$21,$81,$00,$15,$87,$00
	.byte $21,$81,$00,$2d,$81,$00,$6c,$45,$28,$5f,$15,$85,$00,$21,$81,$00
	.byte $21,$81,$00,$10,$87,$00,$6c,$45,$28,$5f,$15,$87
	.byte $ff,$22
	.word @ref410
	.byte $ff,$23
	.word @ref411
	.byte $ff,$22
	.word @ref410
@ref415:
	.byte $6c,$45,$28,$5f,$0e,$85,$00,$15,$81,$00,$85,$1a,$87,$00,$15,$81
	.byte $00,$1a,$81,$00,$6c,$45,$28,$5f,$0e,$85,$00,$1a,$81,$00,$1a,$81
	.byte $00,$0e,$87,$00,$1a,$81,$00,$1a,$81,$00
@ref416:
	.byte $6c,$45,$28,$5f,$10,$85,$00,$1c,$81,$00,$1c,$81,$00,$10,$87,$00
	.byte $1c,$81,$00,$28,$81,$00,$6c,$45,$28,$5f,$10,$85,$00,$1c,$81,$00
	.byte $1c,$81,$00,$6c,$45,$28,$5f,$10,$85,$00,$6c,$45,$28,$5f,$10,$85
	.byte $00
	.byte $fd
	.word @song2ch2loop

; victory
@song2ch3:
@ref417:
	.byte $78,$88,$0c,$85,$74,$0c,$85,$74,$0c,$85,$78,$0c,$85,$74,$0c,$85
	.byte $74,$0c,$85,$78,$0c,$85,$74,$0c,$85,$74,$0c,$85,$78,$92,$09,$83
	.byte $7a,$07,$83,$7c,$05,$83,$7c,$02,$83
@song2ch3loop:
@ref418:
	.byte $7f,$80,$0f,$89,$78,$88,$0a,$81,$7a,$0e,$81,$7a,$0e,$81,$7b,$92
	.byte $09,$89,$78,$88,$0a,$81,$00,$7a,$0e,$81,$00,$7f,$92,$0f,$89,$78
	.byte $88,$0a,$81,$00,$7a,$0e,$81,$00,$7f,$92,$09,$89,$7f,$80,$0f,$89
@ref419:
	.byte $8b,$78,$88,$0a,$81,$7a,$0e,$81,$7a,$0e,$81,$7f,$92,$09,$89,$78
	.byte $88,$0a,$81,$00,$7a,$0e,$81,$00,$7f,$92,$0f,$89,$78,$0a,$81,$00
	.byte $7a,$0e,$81,$00,$7f,$09,$89,$78,$0a,$81,$00,$7a,$0e,$81,$00
	.byte $ff,$1c
	.word @ref418
	.byte $ff,$1f
	.word @ref419
	.byte $ff,$1c
	.word @ref418
	.byte $ff,$1f
	.word @ref419
	.byte $ff,$1c
	.word @ref418
	.byte $ff,$1f
	.word @ref419
	.byte $fd
	.word @song2ch3loop

; victory
@song2ch4:
@ref426:
	.byte $df
@song2ch4loop:
@ref427:
	.byte $df
@ref428:
	.byte $df
@ref429:
	.byte $df
@ref430:
	.byte $df
@ref431:
	.byte $df
@ref432:
	.byte $df
@ref433:
	.byte $df
@ref434:
	.byte $df
	.byte $fd
	.word @song2ch4loop


; mirror
@song3ch0:
	.byte $fb,$05
@song3ch0loop:
@ref435:
	.byte $9f
	.byte $fd
	.word @song3ch0loop

; mirror
@song3ch1:
@song3ch1loop:
@ref436:
	.byte $9f
	.byte $fd
	.word @song3ch1loop

; mirror
@song3ch2:
@song3ch2loop:
@ref437:
	.byte $6c,$40,$88,$2a,$00,$83,$6c,$40,$2b,$00,$93
	.byte $fd
	.word @song3ch2loop

; mirror
@song3ch3:
@song3ch3loop:
@ref438:
	.byte $9f
	.byte $fd
	.word @song3ch3loop

; mirror
@song3ch4:
@song3ch4loop:
@ref439:
	.byte $9f
	.byte $fd
	.word @song3ch4loop
