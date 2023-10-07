#pragma once

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
        bool m_bStarted;
        std::map<eCallbackId, std::set<void*>> m_callbacks;

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

        CCleoInstance();
        virtual ~CCleoInstance();

        void Start();
        void Stop();

        void AddCallback(eCallbackId id, void* func);
        const std::set<void*>& GetCallbacks(eCallbackId id);

        void(__cdecl * UpdateGameLogics)();
        static void __cdecl OnUpdateGameLogics();
    };

    CCleoInstance& GetInstance();
}

