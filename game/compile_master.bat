@echo off

:: Options
:: audio
:: code
:: run

::echo %audio%
::echo %code%
::echo %run%

set name="main"

set path=..\bin\;%path%

set CC65_HOME=..\

set a53enable=1

IF DEFINED audio (
	MUSIC\text2vol5.exe MUSIC\songs.txt -ca65
	MUSIC\nsf2data5.exe MUSIC\sounds.nsf -ca65
)

IF DEFINED gfx (
	python gfx\xor.py chrrom_bank0.chr chrrom_bank1.chr 0xor1.chr
	python gfx\donut.py chrrom_bank0.chr -fo gfx/chrrom_bank0.d
	python gfx\donut.py 0xor1.chr -fo gfx/0xor1.d
	python gfx\donut.py chrrom_bank1.chr -fo gfx/chrrom_bank1.d
	python gfx\donut.py chrrom_bank2.chr -fo gfx/chrrom_bank2.d
)

IF DEFINED maps (
	python NES_ST/meta.py test_meta_tiles.nam ..\maps\nametable.json
	python MAPS\generate_maps_header.py
)

IF DEFINED code (
	REM -g adds debug information, but the end result .nes file is not
	REM affected, so leave it in all the time.
	cc65 -g -Oirs %name%.c --add-source
	cc65 -g -Oirs PRG0.c --add-source
	cc65 -g -Oirs PRG1.c --add-source
	cc65 -g -Oirs PRG4.c --add-source
	cc65 -g -Oirs PRG5.c --add-source
	cc65 -g -Oirs A53\bank_helpers.c --add-source
	ca65 crt0.s
	ca65 %name%.s -g
	ca65 PRG0.s -g
	ca65 PRG1.s -g
	ca65 PRG4.s -g
	ca65 PRG5.s -g
	ca65 donut.s -g
	ca65 unpack.s -g
	ca65 A53\bank_helpers.s -g

	REM -dbgfile does not impact the resulting .nes file.
	ld65 -C A53.cfg --dbgfile %name%.dbg -o %name%.nes crt0.o %name%.o A53\bank_helpers.o donut.o unpack.o PRG0.o PRG1.o PRG4.o PRG5.o nes.lib -Ln labels.txt -m map.txt

	del *.o
	del A53\*.o

	mkdir BUILD\
	move /Y %name%.nes BUILD\ 
	move /Y %name%.dbg BUILD\ 
	move /Y labels.txt BUILD\ 
	move /Y %name%.s BUILD\ 
	move /Y A53\bank_helpers.s BUILD\ 
	move /Y PRG0.s BUILD\ 
	move /Y PRG1.s BUILD\ 
	move /Y PRG4.s BUILD\
	move /Y PRG5.s BUILD\
)

if DEFINED run (
	BUILD\%name%.nes
)

::set audio
::set code
::set run