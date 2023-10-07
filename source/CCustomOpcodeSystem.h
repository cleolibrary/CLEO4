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
        friend OpcodeResult __stdcall opcode_0A9A(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0A9B(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AA2(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AA3(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AC8(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AC9(CRunningScript *pScript);

    public:
        std::set<DWORD> m_hFiles;
        std::set<HMODULE> m_hNativeLibs;
        std::set<void *> m_pAllocations;

        void FinalizeScriptObjects();

        virtual void Inject(CCodeInjector& inj);
        ~CCustomOpcodeSystem()
        {
            //TRACE("Last opcode executed %04X at %s:%d", last_opcode, last_thread, last_off);
        }
    };

    extern void(__thiscall * ProcessScript)(CRunningScript*);
    
    char* ReadStringParam(CRunningScript* thread, char* buf, BYTE size);
    bool WriteStringParam(CRunningScript* thread, const char* str);
    int ReadFormattedString(CRunningScript* thread, char* outputStr, size_t len, const char* format);
    void SkipUnusedParameters(CRunningScript* thread); // for var-args opcodes
}
