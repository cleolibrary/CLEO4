## 4.5.0

- introduced CLEO modules feature
- introduced DebugUtils plugin
- new opcode 00C3 (debug_on)
- new opcode 00C4 (debug_off)
- new opcode 00CC (breakpoint)
- new opcode 00CD (trace)
- new opcode 00CE (log_to_file)
- new opcode 0DD5 (get_game_platform)
- new opcode 2000 (resolve_filepath)
- new opcode 2001 (get_script_name)
- implemented support of opcodes 0662, 0663 and 0664 (original Rockstar's script debugging opcodes. See DebugUtils.ini)
- opcodes 0AAB, 0AE4, 0AE5, 0AE1, 0AE2 and 0AE3 moved from CLEO to File plugin. Adding "{$USE FILE}" might be required to compile some scripts
- introduced 'virtual absolute paths'. Use prefix in file path strings to access predefined locations: "0:\" game root, "1:\" game save files directory, "2:\" this script file directory, "3:\" cleo folder, "4:\" cleo\cleo_modules
- added more detailed error messages in some scenarios
- on some errors instead of crashing the game just invalid script is paused
- 0AB1 (cleo_call) and 0AB2 (cleo_return) scope now saves and restores GOSUB's call stack
- when reading less than 4 bytes with 0A9D (readfile) now remaining bytes of the target variable are set to zero
- fixed error in 004E (terminate_this_script) allowing to run multiple missions
- 'argument count' parameter of 0AB1 (cleo_call) is now optional. 'cleo_call @LABEL args 0' can be written as 'cleo_call @LABEL'
- 'argument count' parameter of 0AB2 (cleo_return) is now optional. 'cleo_return 0' can be written as 'cleo_return'
- fixed handling of strings longer than 128 characters causing errors in some cases
- fixed error in handling of first string argument in 0AF5 (write_string to_ini_file)
- fixed resolution dependent aspect ratio of CLEO text in main menu
#### SDK AND PLUGINS
- now all opcodes in range 0-7FFF can be registered by plugins
- plugins moved to cleo\cleo_plugins directory
- new SDK method: CLEO_ResolvePath
- new SDK method: CLEO_RegisterCallback
- new SDK method: CLEO_GetScriptVersion
- new SDK method: CLEO_GetScriptDebugMode
- new SDK method: CLEO_SetScriptDebugMode
- new SDK method: CLEO_Log
- new SDK method: CLEO_ReadParamsFormatted	
#### CLEO internal
- updated project settings
- updated general methods for getting and setting string parameters
- rewritten Current Working Directory (editable with 0A99) handling. CWD changes no longer affects internal game's processes and are not globally shared among all scripts
- updated opcodes handling

## 4.4.4

- added string arguments support to 0AB1 (cleo_call)
- fix an issue when PRINT_STRING and PRINT_BIG_STRING would overwrite each other (see https://github.com/cleolibrary/CLEO4/issues/80)
- update BASS.dll to the latest to solve some issues with audio in game (see https://github.com/cleolibrary/CLEO4/issues/70)
- added support of variable length arguments longer than 128 characters

## 4.4.3

- added correct support of condition result to opcodes 0AB1 0AB2. Fixes possible bugs when 0AB1 are used in multi conditional if statements. 
- set condition result in 0ADA and 0AD8

## 4.4.2

- fix eventual crash when using the game's user track player radio station (see https://github.com/cleolibrary/CLEO4/issues/38 for details)
- fix 0AAA opcode not working with scripts from main.scm

## 4.4.1

- fix some issues with audio stream output #61 (by @GeTechG)
- compatibility with latest plugin-sdk

## 4.4.0

- Dropped Windows XP support

## 4.3.24

- Added the export of some functions required for new version of the CLEO+ plugin, and can be used in other plugins: CLEO_GetScriptTextureById, CLEO_GetInternalAudioStream, CLEO_CreateCustomScript, CLEO_GetLastCreatedCustomScript, CLEO_AddScriptDeleteDelegate, CLEO_RemoveScriptDeleteDelegate.
- Fixed sounds not pausing when unfocus (thanks to dkluin).
- Opcodes for finding entities (0AE1, 0AE2, 0AE3) now use a distance check with optimized performance, and ignore the distance limitation if the argument sent is greater than 1000.0.
- Opcode for finding peds (0AE1) now makes it possible to send "-1" in the "pass_deads" parameter to ignore all checks and return literally all peds.
- Opcode for car number of gears (0AB7) now returns from vehicle class itself instead of using model and handling arrays — now compatible with f92la and IndieVehicles.
- Opcodes for blip target coordinates (0AB6), car name (0ADB) and spawn (0ADD) are now compatible with f92la.
- Now the full version is shown in the SDK and menu text.

## 4.3.23

- Now you can use string pointer in the file address parameter for .ini files opcodes.
- Fixed the 0ABA opcode causing heap corruption.
- Fixed shared variables not reset correctly. Which caused malfunctions in mods that use them and you play a new game or load game in a slot without the variables.

## 4.3.22

- Now creates cleo, cleo/cleo_saves and cleo/cleo_text directories on startup if they do not exist
- Fix to issue with 0AE9 not returning result

## 4.3.21

- Fixed operand type IDs in CLEO.h
- Added 'extern' to variables declared in CLEO.h
- Fix to issue with 0AB1 in missions not storing mission locals

## 4.3.19-20

- Fixed issue with 0AB1 passing incorrect variable scope in missions
- Updated SDK version

## 4.3.17-18

- Fixed potential future problem with 0AB0 which used methods with undefined behaviour
- Fixed incorrect method used for 0AB7

## 4.3.16

- Fixed bugs with CLEO saves when saved scripts ended
- Prevented crashing when invalid audiostream handles are used

## 4.3.15

- Improvemed compatibility fix for opcodes 0AE1, 0AE2 and 0AE3 with incorrect find_next usage

## 4.3.14

- Fixed 0AAA only returning custom scripts
- Fixed many things which use the 'SCM Block' or 'Mission Local Storage' space
- Fixed parameters being passed to script local storage instead of mission local storage through 0A94
- Fixed potential problems with iteration through the script queues (may cause rare and hard to trace bugs)

## 4.3.13

- Fixed crashing when starting a new game after a game has already started with CLEO scripts installed
- Possibly fixed other issues with starting a game with CLEO scripts installed

## 4.3.12.1

- Un-did the 'Scripts no longer load prematurely' fix as it caused scripts to not load certain circumstances (like before CLEO 4)
- Included 'cleo_text' folder in installation

## 4.3.12

- Fixed string parameter skipping in 'SkipOpcodeParams' used by CLEO plugins
- 0AC8 now returns a NULL value to the output var if allocation failed (as it did before 4.3a)
- 0AC9 now checks the memory was allocated by 0AC8 before attempting to free it
- FXT references are now case insensitive (as they were before 4.3a)
- File operations now check the input handle isn't null (as it seems was the way before 4.3a)
- 'Loaded mission' status now reset on new/loaded game (as it was before 4.3a)
- Scripts no longer load prematurely (like before 4.3a)
- Resolved conflicts with other menu hooks such as 'HUME'
- Other minor tweaks

## 4.3.11

- Fixed crash with 0ADA in scripts beginning with an opcode ending in '00'

## 4.3.10

- Improvements to opcodes 0AE1, 0AE2 and 0AE3 - now loops around the pool even when the 'find_next' flag isn't used correctly
- Fixed 0AD2 not returning peds targetted with the mouse, while targetting with a pad worked

## 4.3.9

- Will now be able to start a CLEO mission after recently finishing a standard mission
- Will no longer error & terminate when scripts fail to open and instead simply log the error
- Will no longer terminate on warnings
- No longer includes paths in automatically generated script names (e.g. cleo\dir\demo.cs is now named 'demo.cs' and not 'dir\dem')
- Improved handling of script load errors

## 4.3.8

- Fixed crash which would occur when missions were ended with 004E

## 4.3.7

- Custom missions launched by CLEO scripts now inherit their compatibility mode - possibly fixing incompatibilities with mods using custom missions
- The current directory set by 0A99 is now script-dependant and only affects running CLEO scripts (not the entire game or the main.scm)
- Text and texture/sprite draws are now script-dependant (doesn't affect main.scm scripts)

## 4.3.0

- Replaced code which dynamically allocated and deallocated memory for script parameters every time 0AA5-0AA8 were called with static arrays
- Removed a script execution loop replacement which wasn't used for anything important and weirdly only worked with 1.0US that caused crashes with script logging plugins
- Added support for Steam (v3) versions of gta_sa.exe
- Prevented the local storage from being initialized in SCM functions when the script is in CLEO 3 compatibility mode ('.cs3' extension)

### Updates to behaviour of following opcodes:

#### 0A99

CHANGE_DIRECTORY can now correctly change to the program directory

#### 0A9A

OPEN_FILE now uses a 'legacy' mode when passing an integer as the mode parameter for compatibility of CLEO file handles and SA file handles
Note that you should really not pass CLEO file handles to game functions. However, this legacy mode now ensures that the handles are compatible.
Other file functions have also been updated ensuring that game file handles are passed to relevant game functions.
It is recommended to not rely on passing files to game functions and instead use CLEO 4's in-built file functions in future.

#### 0AD1

CALL now accepts string input, which is passed as a string pointer following string convention

#### 0AD4

SCAN_STRING now returns a condition result

#### 0AE6

FIND_FIRST_FILE now accepts string array output

#### 0AE3

FIND_ALL_RANDOM_OBJECTS_IN_SPHERE now ensures no fading objects are returned and returns -1 instead of 0 on failure

#### 0AE2

FIND_ALL_RANDOM_CARS_IN_SPHERE now ensures no script vehicles or fading vehicle are returned and returns -1 instead of 0 on failure

#### 0AE1

FIND_ALL_RANDOM_CHARS_IN_SPHERE now ensures no script characters or fading characters are returned and returns -1 instead of 0 on failure

#### 0ADF

ADD_TEXT_LABEL now updates existing text labels if they already exist

#### 0AD6

IS_END_OF_FILE_REACHED now returns true if a file error occured

#### 0AD2

GET_CHAR_PLAYER_IS_TARGETING now returns -1 instead of 0 when no target is found

#### 0AB5

STORE_CLOSEST_ENTITIES now ensures no script entities or fading entities are returned and ensures the player ped is not returned
