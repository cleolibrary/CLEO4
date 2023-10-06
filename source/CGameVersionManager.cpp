#include "stdafx.h"
#include "CGameVersionManager.h"

namespace CLEO
{
    memory_pointer MemoryAddresses[MA_TOTAL][GV_TOTAL] =
    {
        // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x0053E981,	memory_und, 0x0053E981, 0x0053EE21, 0x00551174 },		// MA_CALL_UPDATE_GAME_LOGICS,
        { 0x0053BEE0,	memory_und, 0x0053BEE0, 0x0053C380, 0x0054DE60 },		// MA_UPDATE_GAME_LOGICS_FUNCTION,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x008232D8,	memory_und, 0x00823318, 0x00824098, 0x0085C75E },		// MA_FOPEN_FUNCTION,
        { 0x0082318B,	memory_und, 0x008231CB, 0x00823F4B, 0x0085C396 },		// MA_FCLOSE_FUNCTION,
        { 0x008231DC,	memory_und, 0x0082321C, 0x00823F9C, 0x0085C680 },		// MA_FGETC_FUNCTION,
        { 0x00823798,	memory_und, 0x008237D8, 0x00824558, 0x0085D00C },		// MA_FGETS_FUNCTION,
        { 0x008262B8,	memory_und, 0x008262F8, 0x00826BA8, 0x008621F1 },		// MA_FPUTS_FUNCTION,
        { 0x00823521,	memory_und, 0x00823561, 0x008242E1, 0x0085CD04 },		// MA_FREAD_FUNCTION,
        { 0x00823674,	memory_und, 0x008236B4, 0x00824434, 0x0085CE7E },		// MA_FWRITE_FUNCTION,
        { 0x0082374F,	memory_und, 0x0082378F, 0x0082450F, 0x0085CF87 },		// MA_FSEEK_FUNCTION,
        { 0x00823A30,	memory_und, 0x00823A70, 0x008247F0, 0x0085D464 },		// MA_FPRINTF_FUNCTION,
        { 0x00826261,	memory_und, 0x008262A1, 0x00826B51, 0x00862183 },		// MA_FTELL_FUNCTION,
        { 0x00823E86,	memory_und, 0x00823EC6, 0x00824C46, 0x0085DDDD },		// MA_FFLUSH_FUNCTION,
        { 0x008262A2,	memory_und, 0x008262E2, 0x00826B92, 0x0085D193 },		// MA_FEOF_FUNCTION,
        { 0x008262AD,	memory_und, 0x008262ED, 0x00826B9D, 0x0085D1C2 },		// MA_FERROR_FUNCTION,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x00BA6748,	memory_und, 0x00BA6748, 0x00BA8DC8, 0x00C33100 },		// MA_MENU_MANAGER,
        { 0x0071A700,	memory_und, 0x0071A700, 0x0071AF30, 0x0073BF50 },		// MA_DRAW_TEXT_FUNCTION,
        { 0x00719610,	memory_und, 0x00719610, 0x00719E40, 0x0073ABC0 },		// MA_SET_TEXT_ALIGN_FUNCTION,
        { 0x00719490,	memory_und, 0x00719490, 0x00719CC0, 0x0073AA00 },		// MA_SET_TEXT_FONT_FUNCTION,
        { 0x00719590,	memory_und, memory_und, memory_und, 0x0073AB30 },		// MA_SET_TEXT_EDGE_FUNCTION,
        { 0x005733E0,	memory_und, 0x005733E0, 0x00573950, 0x005885C0 },		// MA_CMENU_SCALE_X_FUNCTION,
        { 0x00573410,	memory_und, 0x00573410, 0x00573980, 0x00588600 },		// MA_CMENU_SCALE_Y_FUNCTION,
        { 0x00719380,	memory_und, 0x00719380, 0x00719BB0, 0x0073A8D0 },		// MA_SET_LETTER_SIZE_FUNCTION,
        { 0x00719430,	memory_und, 0x00719430, 0x00719C60, 0x0073A970 },		// MA_SET_LETTER_COLOR_FUNCTION,
        { 0x00728350,	memory_und, 0x00728350, 0x00728B80, 0x0075D640 },		// MA_CTEXTURE_DRAW_IN_RECT_FUNCTION,
        { 0x0057B9FD,	memory_und, 0x0057B9FD, 0x0057BF71, 0x00591379 },		// MA_CALL_CTEXTURE_DRAW_BG_RECT,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x00464C00,	memory_und, 0x00464C00, 0x00464C80, 0x0046A490 },		// MA_ADD_SCRIPT_TO_QUEUE_FUNCTION,
        { 0x00464BD0,	memory_und, 0x00464BD0, 0x00464C50, 0x0046A460 },		// MA_REMOVE_SCRIPT_FROM_QUEUE_FUNCTION,
        { 0x00465AA0,	memory_und, 0x00465AA0, 0x00465B20, 0x0046B2C0 },		// MA_STOP_SCRIPT_FUNCTION,
        { 0x00465E60,	memory_und, 0x00465E60, 0x00465EE0, 0x0046B640 },		// MA_SCRIPT_OPCODE_HANDLER0_FUNCTION,
        { 0x00464080,	memory_und, 0x00464080, 0x00464100, 0x00469790 },		// MA_GET_SCRIPT_PARAMS_FUNCTION,
        { 0x00464500,	memory_und, 0x00464500, 0x00464580, 0x00469BF0 },		// MA_TRANSMIT_SCRIPT_PARAMS_FUNCTION,
        { 0x00464370,	memory_und, 0x00464370, 0x004643F0, 0x00469A70 },		// MA_SET_SCRIPT_PARAMS_FUNCTION,
        { 0x004859D0,	memory_und, 0x004859D0, 0x00485A50, 0x0048BF40 },		// MA_SET_SCRIPT_COND_RESULT_FUNCTION,
        { 0x00464700,	memory_und, 0x00464700, 0x00464780, 0x00469DE0 },		// MA_GET_SCRIPT_PARAM_POINTER1_FUNCTION,
        { 0x00463D50,	memory_und, 0x00463D50, 0x00463DD0, 0x00469420 },		// MA_GET_SCRIPT_STRING_PARAM_FUNCTION,
        { 0x00464790,	memory_und, 0x00464790, 0x00464810, 0x00469E80 },		// MA_GET_SCRIPT_PARAM_POINTER2_FUNCTION,
        { 0x00468D50,	memory_und, 0x00468D50, 0x00468DD0, 0x0046E440 },		// MA_INIT_SCM_FUNCTION,
        { 0x005D4C40,	memory_und, 0x005D4C40, 0x005D5420, 0x005F13E0 },		// MA_SAVE_SCM_DATA_FUNCTION,
        { 0x005D4FD0,	memory_und, 0x005D4FD0, 0x005D57B0, 0x005F1770 },		// MA_LOAD_SCM_DATA_FUNCTION,
        { 0x00B7CB84,	memory_und, 0x00B7CB84, 0x00B7F204, 0x00C0F538 },		// MA_GAME_TIMER,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x00A43C78,	memory_und, 0x00A43C78, 0x00A462F8, 0x00AB8DD0 },		// MA_OPCODE_PARAMS,
        { 0x00A49960,	memory_und, 0x00A49960, 0x00A4BFE0, 0x00ABEA90 },		// MA_SCM_BLOCK,
        { 0x00A48960,	memory_und, 0x00A48960, 0x00A4AFE0, 0x00ABDA90 },		// MA_MISSION_LOCALS,
        { 0x00A444B1,	memory_und, 0x00A444B1, 0x00A46B31, 0x00AB95EA },		// MA_MISSION_LOADED,
        { 0x00A7A6A0,	memory_und, 0x00A7A6A0, 0x00A7CD20, 0x00AEF7D0 },		// MA_MISSION_BLOCK,
        { 0x00A476AC,	memory_und, 0x00A476AC, 0x00A49D2C, 0x00ABC7DC },		// MA_ON_MISSION_FLAG,
        { 0x00A8B42C,	memory_und, 0x00A8B42C, 0x00A8DAAC, 0x00B00558 },		// MA_ACTIVE_THREAD_QUEUE,
        { 0x00A8B428,	memory_und, 0x00A8B428, 0x00A8DAA8, 0x00ABDA8C },		// MA_INACTIVE_THREAD_QUEUE,
        { 0x00A8B430,	memory_und, 0x00A8B430, 0x00A8DAB0, 0x00B00560 },		// MA_STATIC_THREADS,
        { 0x0053BDD7,	memory_und, 0x0053BDD7, memory_und, 0x0054DD49 },		// MA_CALL_INIT_SCM1,
        { 0x005BA340,	memory_und, 0x005BA340, memory_und, 0x005D8EE9 },		// MA_CALL_INIT_SCM2,
        { 0x005D4FD7,	memory_und, 0x005D4FD7, 0x005D57B7, 0x005F1777 },		// MA_CALL_INIT_SCM3,
        { 0x005D14D5,	memory_und, 0x005D14D5, 0x005D157C, 0x005EDBD4 },		// MA_CALL_SAVE_SCM_DATA,
        { 0x005D18F0,	memory_und, 0x005D18F0, 0x005D20D0, 0x005EE017 },		// MA_CALL_LOAD_SCM_DATA,
        { 0x004667DB,	memory_und, 0x004667DB, 0x0046685B, 0x0046BEFD },		// MA_OPCODE_004E,
        { 0x0046A21B,	memory_und,	0x0046A21B, 0x0046AE9B, 0x0046F9A8 },		// MA_CALL_PROCESS_SCRIPT
        { 0x00A94B68,	memory_und, 0x00A94B68,	0x00A971E8, 0x00B09C80 },		// MA_SCRIPT_SPRITE_ARRAY
        { 0x00464980,	memory_und, 0x00464980, 0x00465600, 0x0046A130 },		// MA_DRAW_SCRIPT_SPRITES
        { 0x0058C092,	memory_und, 0x0058C092, 0x0058D462, 0x0059A3F2 },		// MA_CALL_DRAW_SCRIPT_SPRITES
        { 0x00A92D68,	memory_und,	0x00A92D68, 0x00A953E8, 0x00B07E80 },		// MA_SCRIPT_DRAW_ARRAY
        { 0x00A44B5C,	memory_und, 0x00A44B5C,	0x00A471DC, 0x00AB9C8C },		// MA_NUM_SCRIPT_DRAWS
        { 0x00465A6F,	memory_und, 0x00465A6F, 0x00465AEF, 0x0046B291 },		// MA_CODE_JUMP_FOR_TXD_STORE
        { 0x00A44B67,	memory_und, 0x00A44B67, 0x00A471E7, 0x00AB95FA },		// MA_USE_TEXT_COMMANDS
        { 0x00A913E8,	memory_und,	0x00A913E8, 0x00A93A68, 0x00B06500 },		// MA_SCRIPT_TEXT_ARRAY
        { 0x00A44B68,	memory_und, 0x00A44B68,	0x00A471E8, 0x00AB9C98 },		// MA_NUM_SCRIPT_TEXTS
        { 0x0058FCE4,	memory_und, 0x0058FCE4, 0x005904B4, 0x0059E73C },		// MA_CALL_DRAW_SCRIPT_TEXTS_BEFORE_FADE
        { 0x0058D552,	memory_und, 0x0058D552, 0x0058DD22, 0x0059BAD4 },		// MA_CALL_DRAW_SCRIPT_TEXTS_AFTER_FADE

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x008A6168,	memory_und, 0x008A6168, 0x008A7450, 0x00913C20 },		// MA_OPCODE_HANDLER,
        { 0x00469FEE,	memory_und, 0x00469FEE, 0x0046A06E, 0x0046F75C },		// MA_OPCODE_HANDLER_REF,
        { 0x00B74490,	memory_und, 0x00B74490, 0x00B76B10, 0x00C01038 },		// MA_PED_POOL,
        { 0x00B74494,	memory_und, 0x00B74494, 0x00B76B14, 0x00C0103C },		// MA_VEHICLE_POOL,
        { 0x00B7449C,	memory_und, 0x00B7449C, 0x00B76B18, 0x00C01044 },		// MA_OBJECT_POOL,
        { 0x00744FB0,	memory_und, 0x00744FB0, 0x007457E0, 0x0077EDC0 },		// MA_GET_USER_DIR_FUNCTION,
        { 0x00538860,	memory_und, 0x00538860, 0x00538D00, 0x0054A730 },		// MA_CHANGE_TO_USER_DIR_FUNCTION,
        { 0x005387D0,	memory_und, 0x005387D0, 0x00538C70, 0x0054A680 },		// MA_CHANGE_TO_PROGRAM_DIR_FUNCTION,
        { 0x00569660,	memory_und, 0x00569660, 0x00569B00, 0x00583CB0 },		// MA_FIND_GROUND_Z_FUNCTION,
        { 0x00BA86F0,	memory_und, 0x00BA86F0, 0x00BAAD70, 0x00C36020 },		// MA_RADAR_BLIPS,
        { 0x00C2B9C8,	memory_und, 0x00C2B9C8, 0x00C2E188, 0x00CAC1E0 },		// MA_HANDLING,
        { 0x0056E210,	memory_und, 0x0056E210, 0x0056E6B0, 0x00563900 },		// MA_GET_PLAYER_PED_FUNCTION,
        { 0x00A9B0C8,	memory_und, 0x00A9B0C8, 0x00A9D748, 0x00B0FFD8 },		// MA_MODELS,
        { 0x0043A0B0,	memory_und, 0x0043A0B0, 0x0043A136, 0x0043D3D0 },		// MA_SPAWN_CAR_FUNCTION,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x00588BE0,	memory_und, 0x00588BE0, 0x005893B0, 0x00596980 },		// MA_TEXT_BOX_FUNCTION,
        { 0x0069F2B0,	memory_und, 0x0069F2B0, 0x0069FAD0, 0x006CBF40 },		// MA_STYLED_TEXT_FUNCTION,
        { 0x0069F0B0,	memory_und, 0x0069F0B0, 0x0069F8D0, 0x006CBD50 },		// MA_TEXT_LOW_PRIORITY_FUNCTION,
        { 0x0069F1E0,	memory_und, 0x0069F1E0, 0x0069FA00, 0x006CBE80 },		// MA_TEXT_HIGH_PRIORITY_FUNCTION,
        { 0x006A0000,	memory_und, 0x006A0000, 0x006A0820, 0x006CCC90 },		// MA_CTEXT_TKEY_LOCATE_FUNCTION,
        { 0x006A0050,	memory_und, 0x006A0050, 0x006A0870, 0x006CCCE0 },		// MA_CALL_CTEXT_LOCATE,
        { 0x00C1B340,	memory_und, 0x00C1B340, 0x00C1DB00, 0x00946CC8 },		// MA_GAME_TEXTS,
        { 0x00969110,	memory_und, 0x00969110, 0x0096B790, 0x009DE3F8 },		// MA_CHEAT_STRING,
        { 0x00B72910,	memory_und, 0x00B72910, 0x00B74F90, 0x00BFF370 },		// MA_MPACK_NUMBER,

                                                                                // GV_US10,		GV_US11,		GV_EU10,		GV_EU11,		GV_STEAM
        { 0x00745560,	memory_und, 0x00745560, 0x00745D90, 0x0077F3A0 },		// MA_CREATE_MAIN_WINDOW_FUNCTION,
        { 0x007487A8,	memory_und, 0x007487F8, 0x0074907C, 0x0078276D },		// MA_CALL_CREATE_MAIN_WINDOW,
        { 0x00B6F028,	memory_und, 0x00B6F028, 0x00B716A8, 0x00BFBBE0 },		// MA_CAMERA,
        { 0x00B7CB48,	memory_und, 0x00B7CB48, 0x00B7F1C8, 0x00C0F4F9 },		// MA_CODE_PAUSE,
        { 0x00B7CB49,	memory_und,	0x00B7CB49, 0x00B7F1C9, 0x00C0F4FA },		// MA_USER_PAUSE,
        { 0x00C1703C,	memory_und,	0x00C1703C, 0x00C197FC, 0x00CA3578 },		// MA_RW_CAMERA_PP,
        { 0x00748454,	memory_und,	0x007484A4, 0x00748D24, 0x0078240C },		// MA_DEF_WINDOW_PROC_PTR,
    };

    eGameVersion DetermineGameVersion()
    {
        if (MemRead<DWORD>(0x8A6168) == 0x8523A0) return GV_EU11;
        if (MemRead<DWORD>(0x8A4004) == 0x8339CA) return GV_US10;
        else if (MemRead<DWORD>(0x8A4004) == 0x833A0A) return GV_EU10;
        else if (MemRead<DWORD>(0x913000) == 0x8A5B0C) return GV_STEAM;
        return GV_UNK;
    }

    // converts memory address' identifier to actual memory pointer
    memory_pointer CGameVersionManager::TranslateMemoryAddress(eMemoryAddress addrId)
    {
        return MemoryAddresses[addrId][GetGameVersion()];
    }

    extern "C"
    {
        DWORD __stdcall CLEO_GetVersion()
        {
            return CLEO_VERSION;
        }

        eGameVersion __stdcall CLEO_GetGameVersion()
        {
            return DetermineGameVersion();
        }
    }
}
