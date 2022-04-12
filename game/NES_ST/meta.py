#!/usr/bin/python3

# written by Doug Fraker 2018
# this file parses a binary, uncompressed nametable file,
# including attribute bytes from NES Screen Tool (.nam file)
# converts to a c array of metatile data (metatiles.txt)

# arrange the tiles in 2x2 (16px x 16 px) left to right,
# top to bottom, up to 64 metatiles, and select a palette for each
# the first 0,0,0,0 metatile (not counting the 0th one)
# will stop the parsing, and end the output

# later, take a screenshot of the metatiles, crop in GIMP, 
# use "Tiled" to arrange the metatiles into usable data.
# include or copy/paste metatiles.txt to allow the nesdoug
# metatile engine to convert them into tiles

import sys
import json
import os

print("Processing metatiles...")

# Get this working with build script from VS.
os.chdir(os.getcwd() + "/NES_ST")

try:
    sys.argv[1]
except (IndexError, NameError):
	print ("\n!!ERROR: expected filename as an argument!!")
	filename = input("Enter name of metasprite file now: ")
else:	
	filename = sys.argv[1]

try:
    sys.argv[2]
except (IndexError, NameError):
	print ("\n!!ERROR: expected filename of tileset as an argument!!")
	tilesetfilename = input("Enter name of tileset file now: ")
else:	
	tilesetfilename = sys.argv[2]

oldfile = open(filename, 'rb')
tilesetfile = open(tilesetfilename, 'r')
newfile = open('../meta_tiles.h', 'w')  # warning, this will overwrite old file !!!!!!!!!!!!!!!!!!!!!

tilesetjson = json.load(tilesetfile)
#print(tilesetjson["tiles"][0]["properties"][0]["value"])

# tiles [ meta tile index ] . properties [ flag index ] . value
# if tilesetjson["tiles"][0]["properties"][0]["value"] == True:
#     print("its true!")

oldfile.seek(0,2)
file_size = oldfile.tell()
if(file_size != 1024):
	print("ERROR: wrong file size, should be 1024, but is \n")
	print(file_size)
	oldfile.close
	newfile.close
	exit()

	
newfile.write("const unsigned char metatiles[]={\n")


a = [0] * 128 # array of 64 ints
b = 0

def CombineFlags(props):

    flgs = 0
    for i in range (0, 8):
        # tiles [ meta tile index ] . properties [ flag index ] . value
        if props[i]["value"] == True:
            flgs |= 1 << i

    return flgs

def ParseBits(b):
	global d1
	global d2
	global d3
	global d4
	
	d1 = b & 0x03
	d2 = (b & 0x0c) >> 2
	d3 = (b & 0x30) >> 4
	d4 = (b & 0xc0) >> 6
	

# 0 1 16 17
# load all the attribute bits into an array, separated
oldfile.seek(960)
j = 0
for i in range (0, 32):
	b = ord(oldfile.read(1))
	ParseBits(b)
	a[(i*2)+j] = d1
	a[(i*2)+j+1] = d2
	a[(i*2)+j+16] = d3
	a[(i*2)+j+17] = d4
	if ((i+1) % 8) == 0:
		j += 16
	
# print(a)	# debugging

e1 = 0
e2 = 0
e3 = 0
e4 = 0
e5 = 0
# print 4 tile bytes, then an attribute byte, loop

def metatile_print(e1,e2,e3,e4,e5,e6):
	newfile.write("\t" + str(e1) + ", ")
	newfile.write(str(e2) + ", ")
	newfile.write(str(e3) + ", ")
	newfile.write(str(e4) + ",  ")
	newfile.write(str(e5) + ", ")
	newfile.write(str(e6) + ",\n")
	

j = 0
for i in range (0, 128):
	oldfile.seek(j)
	e1 = ord(oldfile.read(1))
	e2 = ord(oldfile.read(1))
	j = j + 32
	oldfile.seek(j)
	e3 = ord(oldfile.read(1))
	e4 = ord(oldfile.read(1))
	#if i != 0 and e1 == 0 and e2 == 0 and e3 == 0 and e4 == 0:
	#	break
	if ((i+1) % 16) == 0:
		j = j + 2
	else:
		j = j - 30
	e5 = a[i]
	# tiles [ meta tile index ] . properties [ flag index ] . value
	metatile_print(e1,e2,e3,e4,e5, CombineFlags(tilesetjson["tiles"][i]["properties"]))
	
	
# delete that last comma, back it up	
z = newfile.tell()
z = z - 3
newfile.seek(z)

newfile.write("\n};\n\n")


print("done")
oldfile.close
newfile.close


