#include "stdafx.h"
#include "CleoBase.h"
#include "CDebug.h"
#include "CCodeInjector.h"

namespace CLEO
{
    void CCodeInjector::OpenReadWriteAccess()
    {
        if (bAccessOpen) return;

        auto dwLoadOffset = static_cast<memory_pointer>(GetModuleHandle(nullptr));

        // Unprotect image - make .text and .rdata section writeable
        auto pImageBase = (BYTE *)dwLoadOffset;
        auto pDosHeader = (PIMAGE_DOS_HEADER)dwLoadOffset;
        auto pNtHeader = (PIMAGE_NT_HEADERS)(pImageBase + pDosHeader->e_lfanew);
        auto pSection = IMAGE_FIRST_SECTION(pNtHeader);

        for (int i = pNtHeader->FileHeader.NumberOfSections; i; i--, pSection++)
        {
            if (!strcmp((char*)pSection->Name, ".text") || !strcmp((char*)pSection->Name, ".rdata"))
            {
                DWORD dwPhysSize = (pSection->Misc.VirtualSize + 4095) & ~4095;
                TRACE("Unprotecting memory region '%s': 0x%08X (size: 0x%08X)",
                    pSection->Name,
                    (DWORD)pSection->VirtualAddress,
                    (DWORD)dwPhysSize
                );
                DWORD oldProtect, newProtect = (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
                if (!VirtualProtect(pImageBase + pSection->VirtualAddress, dwPhysSize, newProtect, &oldProtect))
                    SHOW_ERROR("Virtual protect error");
            }
        }

        bAccessOpen = true;
    }

    void CCodeInjector::CloseReadWriteAccess()
    {
        if (!bAccessOpen) return;

        auto dwLoadOffset = static_cast<memory_pointer>(GetModuleHandle(nullptr));

        // Unprotect image - make .text and .rdata section writeable
        auto pImageBase = (BYTE *)dwLoadOffset;
        auto pDosHeader = (PIMAGE_DOS_HEADER)dwLoadOffset;
        auto pNtHeader = (PIMAGE_NT_HEADERS)(pImageBase + pDosHeader->e_lfanew);
        auto pSection = IMAGE_FIRST_SECTION(pNtHeader);

        for (int i = pNtHeader->FileHeader.NumberOfSections; i; i--, pSection++)
        {
            if (!strcmp((char*)pSection->Name, ".text") || !strcmp((char*)pSection->Name, ".rdata"))
            {
                DWORD dwPhysSize = (pSection->Misc.VirtualSize + 4095) & ~4095;
                TRACE("Reprotecting memory region '%s': 0x%08X (size: 0x%08X)",
                    pSection->Name,
                    (DWORD)pSection->VirtualAddress,
                    (DWORD)dwPhysSize
                );
                DWORD oldProtect, newProtect = (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
                if (!VirtualProtect(pImageBase + pSection->VirtualAddress, dwPhysSize, newProtect, &oldProtect))
                    SHOW_ERROR("Virtual protect error");
            }
        }

        bAccessOpen = false;
    }
}
