#pragma once
#include "CCodeInjector.h"
#include "CDebug.h"
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
}
