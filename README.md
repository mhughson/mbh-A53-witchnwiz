# Witch n' Wiz Action53 NESDev Compo 2020/21 Edition
This is the version of Witch n' Wiz (www.witchnwiz.com) that was entered into the NESDev competition in March 2021, and won 2nd place. It is *not* the full retail version of the game released in November 2021.

For the NESDev Competition, entries must be compatible with the Action 53 Mapper (Mapper 28), which may not be straight forward depending on you technical backround. Witch n' Wiz was targetting Mapper 1 (MMC1) prior to the competition, and the transition was not straight forward for me. My hope is that this project can be a starting point for others wanting to create NES games in C for the compo.

# Credits (beyond actual game credits)

* Shiru (neslib, famitone)
* nesdoug (nesdoug library)
* norill (donut integration, and general Mapper 28 support)
* JRoatch (donut compression library)

# Compiling

Currently only Windows is supported. Requires Python to be installed and for the "python" command to work. You can test this by opening command prompt and simply typing "python" without quotes. If this is unrecognized, you need to install Python.)=

1. In VSCode open root folder of project.
2. Press Ctrl + Shift + B to open the "Built Task to Run" prompt.
3. Select "Build All". This will build all aspects of the game (music, sound, art, code).
4. ROM will be found in "game/BUILD/main.nes"

I recommend that you associate ".nes" files with your prefered emulator. This will allow you to use the "Build & Run" commands to automatically open the game after building.

# Things of Note

1. The Root directory is almost entirely the compiler for the game (CC65 toolchain). The game itself can be found in the "game" sub-directory.
2. Compiling the project happens in compile_master.bat.
3. CHR data is split arcross multiple banks (chrrom_bank0.chr, chrrom_bank1.chr, ...).
4. You can add addtional banks of CHR in compile_master.bat.