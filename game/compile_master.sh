#!/bin/bash

audio="${AUDIO:-1}"
code="${CODE:-1}"
gfx="${GFX:-1}"
maps="${MAPS:-1}"

name="main"
PATH=../bin/:$PATH
a53enable=1

# Check dependencies
missed=0
for dep in wine python cc65 ca65 ld65; do
	which "$dep" > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "Missing dependency "$dep" in your PATH" >&2
		missed=$((missed+1))
	fi
done
if [ "$missed" -ne 0 ]; then
	echo "$missed missing dependencies" >&2
	exit 1
fi

set -ex

if [ "$audio" != "0" ]; then
	wine MUSIC/text2vol5.exe MUSIC/songs.txt -ca65
	wine MUSIC/nsf2data5.exe MUSIC/sounds.nsf -ca65
fi

if [ "$gfx" != "0" ]; then
	python gfx/xor.py chrrom_bank0.chr chrrom_bank1.chr 0xor1.chr
	python gfx/donut.py chrrom_bank0.chr -fo gfx/chrrom_bank0.d
	python gfx/donut.py 0xor1.chr -fo gfx/0xor1.d
	python gfx/donut.py chrrom_bank1.chr -fo gfx/chrrom_bank1.d
	python gfx/donut.py chrrom_bank2.chr -fo gfx/chrrom_bank2.d
fi

if [ "$maps" != "0" ]; then
	python NES_ST/meta.py test_meta_tiles.nam ../MAPS/nametable.json
	python MAPS/generate_maps_header.py
fi

if [ "$code" != "0" ]; then
	# -g adds debug information, but the end result .nes file is not
	# affected, so leave it in all the time.
	cc65 -g -Oirs ${name}.c --add-source
	cc65 -g -Oirs PRG0.c --add-source
	cc65 -g -Oirs PRG1.c --add-source
	cc65 -g -Oirs PRG4.c --add-source
	cc65 -g -Oirs PRG5.c --add-source
	cc65 -g -Oirs A53/bank_helpers.c --add-source
	ca65 crt0.s
	ca65 $name.s -g
	ca65 PRG0.s -g
	ca65 PRG1.s -g
	ca65 PRG4.s -g
	ca65 PRG5.s -g
	ca65 donut.s -g
	ca65 unpack.s -g
	ca65 A53/bank_helpers.s -g

	# -dbgfile does not impact the resulting .nes file.
	ld65 -C A53.cfg --dbgfile ${name}.dbg -o ${name}.nes crt0.o ${name}.o A53/bank_helpers.o donut.o unpack.o PRG0.o PRG1.o PRG4.o PRG5.o nes.lib -Ln labels.txt -m map.txt

	rm *.o
	rm A53/*.o

	mkdir -p BUILD/
	mv ${name}.nes BUILD/ 
	mv ${name}.dbg BUILD/ 
	mv labels.txt BUILD/ 
	mv ${name}.s BUILD/ 
	mv A53/bank_helpers.s BUILD/ 
	mv PRG0.s BUILD/ 
	mv PRG1.s BUILD/ 
	mv PRG4.s BUILD/
	mv PRG5.s BUILD/
fi
