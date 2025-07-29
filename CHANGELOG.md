## 5.2.0
- added script execution log feature. See **cleo\\.cleo_script_log.xml** after game crash or typing **CLOG** cheat code
- added check for preceding **gosub** call in **0AA1 ([return_if_false](https://library.sannybuilder.com/#/sa/script/extensions/CLEO/0AA1))**
- added limit for memory allocated per script (see `cleo_plugins\SA.MemoryOperations.ini`). Exceeding the limit will cause a warning in game
- added listing of remaining memory blocks allocated by scripts to the cleo.log when closing or starting new game
- added support of string pointers as output type in **0AD4 ([scan_string](https://library.sannybuilder.com/#/sa/script/extensions/CLEO/0AD4))**
- added more checks for correctness of format string in **0AD4 ([scan_string](https://library.sannybuilder.com/#/sa/script/extensions/CLEO/0AD4))**
- fixed false positive `Code execution past script end` error messages when calling functions from CLEO modules
- added error message when the script silently crashes in SAMP due to the script problem
- fixed problem with texture sprites corruption, now scripts can safely use sprites from multiple TXDs
- new SDK method: `CLEO_CallNativeOpcode`. See SDK documentation: https://github.com/cleolibrary/CLEO5/wiki/CLEO-5-SDK

## 5.1.0
- fixed collision of GXT texts hook with SAMP and other mods
- rewritten script drawing handling code. Logic moved from **CLEO.asi** to **Text.cleo** plugin
- relocated several opcodes from CLEO core into new **GameEntities.cleo** plugin
- fixed **0ADD ([spawn_vehicle_by_cheating](https://library.sannybuilder.com/#/sa/script/extensions/CLEO/0ADD))** crashing when invalid model id was used
- new CLEO plugin: **Input.cleo**
- new opcode **2080 ([is_key_just_pressed](https://library.sannybuilder.com/#/sa/script/extensions/input/2080))**
- new opcode **2081 ([get_key_pressed_in_range](https://library.sannybuilder.com/#/sa/script/extensions/input/2081))**
- new opcode **2082 ([get_key_just_pressed_in_range](https://library.sannybuilder.com/#/sa/script/extensions/input/2082))**
- new opcode **2083 ([emulate_key_press](https://library.sannybuilder.com/#/sa/script/extensions/input/2083))**
- new opcode **2084 ([emulate_key_release](https://library.sannybuilder.com/#/sa/script/extensions/input/2084))**
- new opcode **2085 ([get_controller_key](https://library.sannybuilder.com/#/sa/script/extensions/input/2085))**
- new opcode **2086 ([get_key_name](https://library.sannybuilder.com/#/sa/script/extensions/input/2086))**
- added warning message to commands **0AC2-0AC5** if used with non-3d audio streams
- call function/method commands **0AA5-0AA7** are now setting logical result
- limited "previously called opcode" info in script error messages to show just commands of that script
- now more script error messages include previously executed command name
- added missing **terminate_this_script** command error message for scripts compiled with extra SCM info

## 5.0.3
- added **GxtHook.cleo** plugin to ignored list
- fixed audio streams stopping when volume was set to 0.0
- fixed audio streams potentially not starting to play if still decoding or downloading
- volume of audio streams with type None and UserInterface no longer affected by screen fades

## 5.0.2
- updated BASS library
- fixed Doppler effect calculations for 3d audio streams
- fixed sound glitches of 3d audio streams when playback starts or the camera mode changes
- fixed possible crashes when car/object/ped with attached 3d audio stream is deleted
- fixed CLEO sound artifacts when moving camera with mouse
- updated distance decay for 3d audio streams to match behavior of in-game sounds
- implemented proper support for 3d audio streams source size
- CLEO sounds volume is now affected by wide screen and fades in same way as in-game sounds
- updated "Audio_Demo" example script
- reverted muting 'music' type audio streams for game speeds other than default introduced in 5.0.1
- allow storing string result at static address (e.g. `get_name_of_vehicle_model 400 0xC16F98`)
- fixed path resolution issues when the game is installed in symlink or RAM Disk directory
- fixed audio playback not working in SAMP

## 5.0.1
- cleo.log file moved back to the game root (as in CLEO4)
- restored behavior of '.\\' prefix for scripts in legacy compat mode
- fixed **0AB9 ([get_audio_stream_state](https://library.sannybuilder.com/#/sa/audio/0AB9))** not always returning Stopped, despite playback ended
- fixed **2507 ([get_audio_stream_progress](https://library.sannybuilder.com/#/sa/audio/2507))** sometimes returning values smaller than 1.0, despite playback ended
- fixed progress set with **2508 ([set_audio_stream_progress](https://library.sannybuilder.com/#/sa/audio/2508))** being ignored by stopped streams
- added new opcode **250B ([get_audio_stream_progress_seconds](https://sannybuilder.com/lib/sa/script/audio/250B))**
- added new opcode **250C ([set_audio_stream_progress_seconds](https://sannybuilder.com/lib/sa/script/audio/250C))**
- fixed **0AA3 ([free_dynamic_library](https://library.sannybuilder.com/#/sa/script/extensions/CLEO/0AA3))** clearing reference to libraries loaded more than once

## 5.0.0
- support for CLEO modules feature https://github.com/sannybuilder/dev/issues/264
- new [Audio](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/Audio) plugin
  - audio related opcodes moved from CLEO core into separated plugin
  - CLEO's audio now obey game's volume settings
  - implemented Doppler effect for 3d audio streams (fast moving sound sources)
  - CLEO's audio now follows game speed changes
  - sound device can be now manually selected in .ini file
  - new opcode **2500 ([is_audio_stream_playing](https://library.sannybuilder.com/#/sa/audio/2500))**
  - new opcode **2501 ([get_audio_stream_duration](https://library.sannybuilder.com/#/sa/audio/2501))**
  - new opcode **2502 ([get_audio_stream_speed](https://library.sannybuilder.com/#/sa/audio/2502))**
  - new opcode **2503 ([set_audio_stream_speed](https://library.sannybuilder.com/#/sa/audio/2503))**
  - new opcode **2504 ([set_audio_stream_volume_with_transition](https://library.sannybuilder.com/#/sa/audio/2504))**
  - new opcode **2505 ([set_audio_stream_speed_with_transition](https://library.sannybuilder.com/#/sa/audio/2505))**
  - new opcode **2506 ([set_audio_stream_source_size](https://library.sannybuilder.com/#/sa/audio/2506))**
  - new opcode **2507 ([get_audio_stream_progress](https://library.sannybuilder.com/#/sa/audio/2507))**
  - new opcode **2508 ([set_audio_stream_progress](https://library.sannybuilder.com/#/sa/audio/2508))**
  - new opcode **2509 ([get_audio_stream_type](https://library.sannybuilder.com/#/sa/audio/2509))**
  - new opcode **250A ([set_audio_stream_type](https://library.sannybuilder.com/#/sa/audio/250A))**
- new [DebugUtils](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/DebugUtils) plugin
  - new opcode **00C3 ([debug_on](https://library.sannybuilder.com/#/sa/debug/00C3))**
  - new opcode **00C4 ([debug_off](https://library.sannybuilder.com/#/sa/debug/00C4))**
  - new opcode **2100 ([breakpoint](https://library.sannybuilder.com/#/sa/debug/2100))**
  - new opcode **2101 ([trace](https://library.sannybuilder.com/#/sa/debug/2101))**
  - new opcode **2102 ([log_to_file](https://library.sannybuilder.com/#/sa/debug/2102))**
  - implemented support of opcodes **0662**, **0663** and **0664** (original Rockstar's script debugging opcodes. See DebugUtils.ini)
- new [FileSystemOperations](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/FileSystemOperations) plugin
  - forbidden scripts from accessing and changing any files outside game root or game settings directory
  - file related opcodes moved from CLEO core into separated plugin
  - opcode **0A9E ([write_to_file](https://library.sannybuilder.com/#/sa/file/0A9E))** now supports literal numbers and strings
  - opcode **0AD7 ([read_string_from_file](https://library.sannybuilder.com/#/sa/file/0AD7))** no longer includes new line character(s) at end of read string
  - fixed bug causing file stream opcodes not working correctly when read-write modes are used
  - fixed buffer overflows in file stream read opcodes
  - added/fixed support of all file stream opcodes in legacy mode (Cleo3)
  - new opcode **2300 ([get_file_position](https://library.sannybuilder.com/#/sa/file/2300))**
  - new opcode **2301 ([read_block_from_file](https://library.sannybuilder.com/#/sa/file/2301))**
  - new opcode **2302 ([write_block_to_file](https://library.sannybuilder.com/#/sa/file/2302))**
  - new opcode **2303 ([resolve_filepath](https://library.sannybuilder.com/#/sa/file/2303))**
  - new opcode **2304 ([get_script_filename](https://library.sannybuilder.com/#/sa/file/2304))**
- new [Math](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/Math) plugin
  - math related opcodes moved from CLEO core into separated plugin
  - new opcode **2700 ([is_bit_set](https://library.sannybuilder.com/#/sa/math/2700))**
  - new opcode **2701 ([set_bit](https://library.sannybuilder.com/#/sa/math/2701))**
  - new opcode **2702 ([clear_bit](https://library.sannybuilder.com/#/sa/math/2702))**
  - new opcode **2703 ([toggle_bit](https://library.sannybuilder.com/#/sa/math/2703))**
  - new opcode **2704 ([is_truthy](https://library.sannybuilder.com/#/sa/math/2704))**
  - new opcode **2705 ([pick_random_int](https://library.sannybuilder.com/#/sa/math/2705))**
  - new opcode **2706 ([pick_random_float](https://library.sannybuilder.com/#/sa/math/2706))**
  - new opcode **2707 ([pick_random_text](https://library.sannybuilder.com/#/sa/math/2707))**
  - new opcode **2708 ([random_chance](https://library.sannybuilder.com/#/sa/math/2708))**
- new [MemoryOperations](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/MemoryOperations) plugin
  - memory related opcodes moved from CLEO core into separated plugin
  - validation of input and output parameters for all opcodes
  - opcode **0A8C ([write_memory](https://library.sannybuilder.com/#/sa/memory/0A8C))** now supports strings
  - new opcode **2400 ([copy_memory](https://library.sannybuilder.com/#/sa/memory/2400))**
  - new opcode **2401 ([read_memory_with_offset](https://library.sannybuilder.com/#/sa/memory/2401))**
  - new opcode **2402 ([write_memory_with_offset](https://library.sannybuilder.com/#/sa/memory/2402))**
  - new opcode **2403 ([forget_memory](https://library.sannybuilder.com/#/sa/memory/2403))**
  - new opcode **2404 ([get_script_struct_just_created](https://library.sannybuilder.com/#/sa/memory/2404))**
  - new opcode **2405 ([is_script_running](https://library.sannybuilder.com/#/sa/memory/2405))**
  - new opcode **2406 ([get_script_struct_from_filename](https://library.sannybuilder.com/#/sa/memory/2406))**
  - new opcode **2407 ([is_memory_equal](https://library.sannybuilder.com/#/sa/memory/2407))**
  - new opcode **2408 ([terminate_script](https://library.sannybuilder.com/#/sa/memory/2408))**
- new [Text](https://github.com/cleolibrary/CLEO5/tree/master/cleo_plugins/Text) plugin
  - text related opcodes moved from CLEO core into separated plugin
  - new opcode **2600 ([is_text_empty](https://library.sannybuilder.com/#/sa/text/2600))**
  - new opcode **2601 ([is_text_equal](https://library.sannybuilder.com/#/sa/text/2601))**
  - new opcode **2602 ([is_text_in_text](https://library.sannybuilder.com/#/sa/text/2602))**
  - new opcode **2603 ([is_text_prefix](https://library.sannybuilder.com/#/sa/text/2603))**
  - new opcode **2604 ([is_text_suffix](https://library.sannybuilder.com/#/sa/text/2604))**
  - new opcode **2605 ([display_text_formatted](https://library.sannybuilder.com/#/sa/text/2605))**
  - new opcode **2606 ([load_fxt](https://library.sannybuilder.com/#/sa/text/2606))**
  - new opcode **2607 ([unload_fxt](https://library.sannybuilder.com/#/sa/text/2607))**
  - new opcode **2608 ([get_text_length](https://library.sannybuilder.com/#/sa/text/2608))**
  - new opcode **2609 ([add_text_label_formatted](https://library.sannybuilder.com/#/sa/text/2609))**
- new and updated opcodes
  - implemented support for **memory pointer string** arguments for all game's native opcodes
  - **0B1E ([sign_extend](https://library.sannybuilder.com/#/sa/bitwise/0B1E))**
  - **0DD5 ([get_game_platform](https://library.sannybuilder.com/#/sa/CLEO/0DD5))**
  - **2000 ([get_cleo_arg_count](https://library.sannybuilder.com/#/sa/CLEO/2000))**
  - **2002 ([cleo_return_with](https://library.sannybuilder.com/#/sa/CLEO/2002))**
  - **2003 ([cleo_return_fail](https://library.sannybuilder.com/#/sa/CLEO/2003))**
  - **2800 ([delete_section_from_ini_file](https://library.sannybuilder.com/#/sa/ini/2800))**
  - **2801 ([delete_key_from_ini_file](https://library.sannybuilder.com/#/sa/ini/2801))**
  - 'argument count' parameter of **0AB1 (cleo_call)** is now optional. `cleo_call @LABEL args 0` can be written as `cleo_call @LABEL`
  - 'argument count' parameter of **0AB2 (cleo_return)** is now optional. `cleo_return 0` can be written as `cleo_return`
  - SCM functions can return string literals and string variables
  - SCM functions **(0AB1)** now keep their own GOSUB's call stack
  - fixed bug in **0AD4 ([scan_string](https://library.sannybuilder.com/#/sa/text/2604))** causing data overruns when reading strings longer than target variable
  - fixed result register not being cleared before function call in opcodes **0AA7** and **0AA8**
  - fixed **0ABA ([terminate_all_custom_scripts_with_this_name](https://library.sannybuilder.com/#/sa/CLEO/0ABA))** terminating only first found script
- changes in file operations
  - file paths can now use 'virtual absolute paths'. Use prefix in file path strings to access predefined locations:
    - `root:\` for _game root_ directory
    - `user:\` for _game save files_ directory
    - `.\` for _this script file_ directory
    - `cleo:\` for _CLEO_ directory
    - `modules:\` for _CLEO\cleo_modules_ directory
  - rewritten opcode **0A99 (set_current_directory)**. Now it no longer affects internal game state or current directory in other scripts
- improved error handling
  - more detailed error messages in multiple scenarios
  - some errors now cause the script to pause, instead of crashing the game
- updated included Silent's ASI Loader to version 1.3

#### Bug Fixes
- fixed error in **004E (terminate_this_script)** allowing to run multiple missions
- fixed handling of strings longer than 128 characters causing errors in some cases
- fixed error in handling of first string argument in **0AF5 (write_string to_ini_file)**
- fixed resolution dependent aspect ratio of CLEO text in main menu
- fixed clearing mission locals when new CLEO mission is started
- when reading less than 4 bytes with **0A9D (readfile)** now remaining bytes of the target variable are set to zero
- fixed invalid 7 characters length limit of **0AAA (get_script_struct_named)**
- fixed an undefined behavior caused by **0459 (terminate_all_scripts_with_this_name)** when the name matches a custom script

#### SDK and plugins
- now all opcodes in range **0-7FFF** can be registered by plugins
- plugins moved to _cleo\cleo_plugins_ directory
- new SDK methods:
  - CLEO_GetVersionStr
  - CLEO_RegisterCommand
  - CLEO_RegisterCallback
  - CLEO_GetVarArgCount
  - CLEO_PeekIntOpcodeParam
  - CLEO_PeekFloatOpcodeParam
  - CLEO_PeekPointerToScriptVariable
  - CLEO_SkipUnusedVarArgs
  - CLEO_ReadParamsFormatted
  - CLEO_ReadStringParamWriteBuffer
  - CLEO_GetOpcodeParamsArray
  - CLEO_GetParamsHandledCount
  - CLEO_IsScriptRunning
  - CLEO_TerminateScript
  - CLEO_GetScriptVersion
  - CLEO_SetScriptVersion
  - CLEO_GetScriptInfoStr
  - CLEO_GetScriptFilename
  - CLEO_GetScriptWorkDir
  - CLEO_SetScriptWorkDir
  - CLEO_ResolvePath
  - CLEO_ListDirectory
  - CLEO_ListDirectoryFree
  - CLEO_GetGameDirectory
  - CLEO_GetUserDirectory
  - CLEO_GetScriptByName
  - CLEO_GetScriptByFilename
  - CLEO_GetScriptDebugMode
  - CLEO_SetScriptDebugMode
  - CLEO_Log

#### CLEO internal
- introduced unit test scripts
- project migrated to VS 2022
- configured game debugging settings
- plugins moved into single solution
- configured automatic releases on GitHub
- added setup_env.bat script

## Older
For previous changes, see [CLEO4 changelog](https://github.com/cleolibrary/CLEO4/blob/master/CHANGELOG.md)

## Special Thanks
- **123nir** for the v5.0.0-alpha testing, troubleshooting and valuable bug reports
- **Hamal** for the v5.0.0-beta testing, troubleshooting and valuable bug reports

