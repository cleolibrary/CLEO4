#include "stdafx.h"
#include "CleoBase.h"
#include "CDebug.h"

class Starter
{
    static Starter dummy;
    Starter()
    {
        auto gv = CLEO::GetInstance().VersionManager.GetGameVersion();
        TRACE("Started on game of version: %s",
            (gv == CLEO::GV_US10) ? "SA 1.0 us" :
            (gv == CLEO::GV_EU11) ? "SA 1.01 eu" :
            (gv == CLEO::GV_EU10) ? "SA 1.0 eu" :
            (gv == CLEO::GV_STEAM) ? "SA 3.0 steam" :
            "<!unknown!>");

        if (gv != CLEO::GV_US10 && gv != CLEO::GV_EU11 && gv != CLEO::GV_EU10 && gv != CLEO::GV_STEAM)
            SHOW_ERROR(
                "Unknown game version.\n"
                "The list of all supported executables:\n\n"
                "  1) gta_sa.exe, original 1.0 us, 14 405 632 bytes;\n"
                "  2) gta_sa.exe, public no-dvd 1.0 us, 14 383 616 bytes;\n"
                "  3) gta_sa_compact.exe, listener's executable, 5 189 632 bytes;\n"
                "  4) gta_sa.exe, original 1.01 eu, 14 405 632 bytes;\n"
                "  5) gta_sa.exe, public no-dvd 1.01 eu, 15 806 464 bytes;\n"
                "  6) gta_sa.exe, 1C localization, 15 806 464 bytes;\n"
                "  7) gta_sa.exe, original 1.0 eu, unknown size;\n"
                "  8) gta_sa.exe, public no-dvd 1.0eu, 14 386 176 bytes;\n"
                "  9) gta_sa.exe, original 3.0 steam executable, unknown size;"
                " 10) gta_sa.exe, decrypted 3.0 steam executable, 5 697 536 bytes."
            );

        CLEO::GetInstance().Start();
    }

    ~Starter()
    {
        CLEO::GetInstance().Stop();
    }
};

Starter Starter::dummy;

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
