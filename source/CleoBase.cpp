#include "stdafx.h"
#include "CleoBase.h"
#include <filesystem>


namespace CLEO
{
    CCleoInstance CleoInstance;
    CCleoInstance& GetInstance() { return CleoInstance; }

    inline CCleoInstance::CCleoInstance()
    {
        m_bStarted = false;
    }

    inline CCleoInstance::~CCleoInstance()
    {
        Stop();
    }

    void __declspec(naked) CCleoInstance::OnUpdateGameLogics()
    {
        //GetInstance().UpdateGameLogics(); // !
        GetInstance().SoundSystem.Update();
        static DWORD dwFunc;
        dwFunc = (DWORD)(GetInstance().UpdateGameLogics);
        _asm jmp dwFunc
    }

    void CCleoInstance::Start()
    {
        if (m_bStarted) return; // already started

        ConfigFilename = std::filesystem::current_path().append("cleo\\.cleo_config.ini").string();

        CreateDirectory("cleo", NULL);
        CreateDirectory("cleo/cleo_modules", NULL);
        CreateDirectory("cleo/cleo_saves", NULL);
        CreateDirectory("cleo/cleo_text", NULL);
        CodeInjector.OpenReadWriteAccess(); // must do this earlier to ensure plugins write access on init
        GameMenu.Inject(CodeInjector);
        DmaFix.Inject(CodeInjector);
        UpdateGameLogics = VersionManager.TranslateMemoryAddress(MA_UPDATE_GAME_LOGICS_FUNCTION);
        CodeInjector.ReplaceFunction(&OnUpdateGameLogics, VersionManager.TranslateMemoryAddress(MA_CALL_UPDATE_GAME_LOGICS));
        TextManager.Inject(CodeInjector);
        SoundSystem.Inject(CodeInjector);
        OpcodeSystem.Inject(CodeInjector);
        ScriptEngine.Inject(CodeInjector);

        CodeInjector.ReplaceFunction(OnDrawingFinished, 0x00734640); // nullsub_63 - originally something like renderDebugStuff?

        m_bStarted = true;
    }

    void CCleoInstance::Stop()
    {
        if (!m_bStarted) return;

        m_bStarted = false;
    }

    void CCleoInstance::AddCallback(eCallbackId id, void* func)
    {
        m_callbacks[id].insert(func);
    }

    const std::set<void*>& CCleoInstance::GetCallbacks(eCallbackId id)
    {
        return m_callbacks[id];
    }

    void WINAPI CLEO_RegisterCallback(eCallbackId id, void* func)
    {
        GetInstance().AddCallback(id, func);
    }

    void __cdecl CCleoInstance::OnDrawingFinished()
    {
        // execute callbacks
        for (void* func : GetInstance().GetCallbacks(eCallbackId::DrawingFinished))
        {
            typedef void WINAPI callback(void);
            ((callback*)func)();
        }
    }
}

