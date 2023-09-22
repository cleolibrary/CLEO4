#pragma once
#include "CCodeInjector.h"
#include "CDebug.h"
#include "CSoundSystem.h"
#include <direct.h>
#include <set>

namespace CLEO
{
    const size_t MAX_STR_LEN = 0xff; // max length of string type parameter
    enum OpcodeResult : char
    {
        OR_CONTINUE = 0,
        OR_INTERRUPT = 1
    };

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
        friend OpcodeResult __stdcall opcode_0AE6(CRunningScript *pScript);
        friend OpcodeResult __stdcall opcode_0AE8(CRunningScript *pScript);

    public:
        std::set<DWORD> m_hFiles;
        std::set<HMODULE> m_hNativeLibs;
        std::set<HANDLE> m_hFileSearches;
        std::set<void *> m_pAllocations;

        void FinalizeScriptObjects()
        {
            // clean up after opcode_0A99
            _chdir("");
            TRACE("Cleaning up script data... %u files, %u libs, %u file scans, %u allocations...",
                m_hFiles.size(), m_hNativeLibs.size(), m_hFileSearches.size(), m_pAllocations.size()
            );

            // clean up after opcode_0A9A
            for (auto i = m_hFiles.begin(); i != m_hFiles.end(); ++i)
            {
                if (!is_legacy_handle(*i))
                    fclose(convert_handle_to_file(*i));
            }
            m_hFiles.clear();

            // clean up after opcode_0AA2
            std::for_each(m_hNativeLibs.begin(), m_hNativeLibs.end(), FreeLibrary);
            m_hNativeLibs.clear();

            // clean up file searches
            std::for_each(m_hFileSearches.begin(), m_hFileSearches.end(), FindClose);
            m_hFileSearches.clear();

            // clean up after opcode_0AB1
            ResetScmFunctionStore();

            // clean up after opcode_0AC8
            std::for_each(m_pAllocations.begin(), m_pAllocations.end(), free);
            m_pAllocations.clear();
        }

        virtual void Inject(CCodeInjector& inj);
        ~CCustomOpcodeSystem()
        {
            //TRACE("Last opcode executed %04X at %s:%d", last_opcode, last_thread, last_off);
        }
    };

    extern void(__thiscall * ProcessScript)(CRunningScript*);

    // Exports
    extern "C"
    {
        // Define external symbols with MSVC decorating schemes
        BOOL WINAPI CLEO_RegisterOpcode(WORD opcode, CustomOpcodeHandler callback);
        DWORD WINAPI CLEO_GetIntOpcodeParam(CRunningScript* thread);
        float WINAPI CLEO_GetFloatOpcodeParam(CRunningScript* thread);
        void WINAPI CLEO_SetIntOpcodeParam(CRunningScript* thread, DWORD value);
        void WINAPI CLEO_SetFloatOpcodeParam(CRunningScript* thread, float value);
        LPSTR WINAPI CLEO_ReadStringOpcodeParam(CRunningScript* thread, char* buf, int size);
        LPSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CRunningScript* thread, char* buf, int size);
        void WINAPI CLEO_WriteStringOpcodeParam(CRunningScript* thread, LPCSTR str);
        void WINAPI CLEO_SetThreadCondResult(CRunningScript* thread, BOOL result);
        void WINAPI CLEO_SkipOpcodeParams(CRunningScript* thread, int count);
        void WINAPI CLEO_ThreadJumpAtLabelPtr(CRunningScript* thread, int labelPtr);
        int WINAPI CLEO_GetOperandType(CRunningScript* thread);
        void WINAPI CLEO_RetrieveOpcodeParams(CRunningScript* thread, int count);
        void WINAPI CLEO_RecordOpcodeParams(CRunningScript* thread, int count);
        SCRIPT_VAR* WINAPI CLEO_GetPointerToScriptVariable(CRunningScript* thread);
        RwTexture* WINAPI CLEO_GetScriptTextureById(CRunningScript* thread, int id);
        HSTREAM WINAPI CLEO_GetInternalAudioStream(CRunningScript* thread, CAudioStream* stream);
        CRunningScript* WINAPI CLEO_CreateCustomScript(CRunningScript* fromThread, const char* fileName, int label);
    }
}
