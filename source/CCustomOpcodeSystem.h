#pragma once
#include "CCodeInjector.h"
#include "CDebug.h"
#include "CSoundSystem.h"
#include <direct.h>
#include <set>

namespace CLEO
{
    typedef OpcodeResult(__stdcall * CustomOpcodeHandler)(CRunningScript*);
    void ResetScmFunctionStore();
    bool is_legacy_handle(DWORD dwHandle);
    FILE * convert_handle_to_file(DWORD dwHandle);

    extern const char* (__cdecl* GetUserDirectory)();
    extern void(__cdecl* ChangeToUserDir)();
    extern void(__cdecl* ChangeToProgramDir)(const char*);

    class CCustomOpcodeSystem : public VInjectible
    {
    public:
        static const size_t LastOriginalOpcode = 0x0A4E; // GTA SA
        static const size_t LastCustomOpcode = 0x7FFF;

        // most recently processed
        static CRunningScript* lastScript;
        static WORD lastOpcode;
        static WORD* lastOpcodePtr;
        static WORD lastCustomOpcode;
        
        void FinalizeScriptObjects();

        CCustomOpcodeSystem();
        virtual void Inject(CCodeInjector& inj);
        ~CCustomOpcodeSystem()
        {
            TRACE("Last opcode executed %04X", lastOpcode);
        }

        static bool RegisterOpcode(WORD opcode, CustomOpcodeHandler callback);

        static OpcodeResult ErrorSuspendScript(CRunningScript* thread); // suspend script execution forever

    private:
        friend OpcodeResult __stdcall opcode_0A9A(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0A9B(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AA2(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AA3(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AC8(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AC9(CRunningScript *pScript);

        std::set<DWORD> m_hFiles;
        std::set<HMODULE> m_hNativeLibs;
        std::set<void*> m_pAllocations;

        typedef OpcodeResult(__thiscall* _OpcodeHandler)(CRunningScript* thread, WORD opcode);

        static const size_t OriginalOpcodeHandlersCount = (LastOriginalOpcode / 100) + 1; // 100 opcodes peer handler
        static _OpcodeHandler originalOpcodeHandlers[OriginalOpcodeHandlersCount]; // backuped when patching

        static const size_t CustomOpcodeHandlersCount = (LastCustomOpcode / 100) + 1; // 100 opcodes peer handler
        static _OpcodeHandler customOpcodeHandlers[CustomOpcodeHandlersCount]; // original + new opcodes

        static OpcodeResult __fastcall customOpcodeHandler(CRunningScript* thread, int dummy, WORD opcode); // universal CLEO's opcode handler

        static CustomOpcodeHandler customOpcodeProc[LastCustomOpcode + 1]; // procedure for each opcode
    };

    extern void(__thiscall * ProcessScript)(CRunningScript*);
    
    char* ReadStringParam(CRunningScript* thread, char* buf = nullptr, DWORD bufSize = 0);
    bool WriteStringParam(CRunningScript* thread, const char* str);
    int ReadFormattedString(CRunningScript* thread, char* buf, DWORD bufSize, const char* format);
    void SkipUnusedParameters(CRunningScript* thread); // for var-args opcodes
}
