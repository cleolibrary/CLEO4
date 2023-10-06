#pragma once
#include "stdafx.h"
#include "CDmaFix.h"
#include "CGameVersionManager.h"
#include "CleoBase.h"

namespace CLEO
{
    void CDmaFix::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting DmaFix...");
        CGameVersionManager& gvm = GetInstance().VersionManager;
        switch (gvm.GetGameVersion())
        {
        case GV_EU10:
        case GV_US10:
            inj.MemoryWrite<DWORD>(0x463CC5, 0x0824448B);
            inj.MemoryWrite<BYTE>(0x463CC9, 0x90);

            inj.Nop(0x4641C6, 3);
            inj.Nop(0x4641E8, 3);
            inj.Nop(0x4641FE, 3);

            inj.Nop(0x46447F, 3);
            inj.Nop(0x4644A1, 3);
            inj.Nop(0x4644B7, 3);
            break;
        case GV_EU11:
            inj.MemoryWrite<DWORD>(0x463D45, 0x0824448B);
            inj.MemoryWrite<BYTE>(0x463D49, 0x90);

            inj.Nop(0x464246, 3);
            inj.Nop(0x464268, 3);
            inj.Nop(0x46427E, 3);

            inj.Nop(0x4644FF, 3);
            inj.Nop(0x464521, 3);
            inj.Nop(0x464537, 3);
            break;
        case GV_STEAM:
            inj.MemoryWrite<DWORD>(0x469390, 0x0824448B);
            inj.MemoryWrite<BYTE>(0x469394, 0x90);

            inj.Nop(0x4698F6, 3);
            break;
        default:
            Error("CDmaFix::Inject(): Unimplemented game version.");
        }
    }
}
