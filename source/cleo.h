#ifndef __CLEO_H
#define __CLEO_H

#include "CCodeInjector.h"
#include "CGameVersionManager.h"
#include "CDebug.h"
#include "CDmaFix.h"
#include "CGameMenu.h"
#include "CModuleSystem.h"
#include "CPluginSystem.h"
#include "CScriptEngine.h"
#include "CCustomOpcodeSystem.h"
#include "CTextManager.h"
#include "CSoundSystem.h"
#include "FileEnumerator.h"
#include "crc32.h"

namespace CLEO
{
    class CCleoInstance
    {
        bool			m_bStarted;

    public:
        CDmaFix					DmaFix;
        CGameMenu				GameMenu;
        CCodeInjector			CodeInjector;
        CGameVersionManager		VersionManager;
        CScriptEngine			ScriptEngine;
        CTextManager			TextManager;
        CCustomOpcodeSystem		OpcodeSystem;
        CModuleSystem			ModuleSystem;
        CSoundSystem			SoundSystem;
        CPluginSystem			PluginSystem;
        //CLegacy				Legacy;

        CCleoInstance()
        {
            m_bStarted = false;
        }

        virtual ~CCleoInstance()
        {
            Stop();
        }

        void(__cdecl * UpdateGameLogics)();
        static void __cdecl OnUpdateGameLogics();

        void Start()
        {
            CreateDirectory("cleo", NULL);
            CreateDirectory("cleo/cleo_modules", NULL);
            CreateDirectory("cleo/cleo_plugins", NULL);
            CreateDirectory("cleo/cleo_saves", NULL);
            CreateDirectory("cleo/cleo_text", NULL);
            CodeInjector.OpenReadWriteAccess();		// must do this earlier to ensure plugins write access on init
            GameMenu.Inject(CodeInjector);
            DmaFix.Inject(CodeInjector);
            UpdateGameLogics = VersionManager.TranslateMemoryAddress(MA_UPDATE_GAME_LOGICS_FUNCTION);
            CodeInjector.ReplaceFunction(&OnUpdateGameLogics, VersionManager.TranslateMemoryAddress(MA_CALL_UPDATE_GAME_LOGICS));
            TextManager.Inject(CodeInjector);
            SoundSystem.Inject(CodeInjector);
            OpcodeSystem.Inject(CodeInjector);
            ScriptEngine.Inject(CodeInjector);
        }

        void Stop()
        {
            if (!m_bStarted) return;
        }
    };

    CCleoInstance& GetInstance();

    // get absolute path. Supports predefined CLEO paths starting with '[digit]:'
    std::string ResolvePath(const char* path, const char* workDir = nullptr);
}

#endif
