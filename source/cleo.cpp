#include "stdafx.h"
#include "cleo.h"
#include <windows.h>
#include <shlobj.h>


namespace CLEO
{
    CCleoInstance CleoInstance;
    CCleoInstance& GetInstance() { return CleoInstance; }

    std::string ResolveCleoPath(const char* path)
    {
        if (path == nullptr)
        {
            return {};
        }

        if (strlen(path) < 2 || path[1] != ':')
        {
            return path;
        }

        if (path[0] < '0' || path[0] > '2')
        {
            return path; // outside supported range
        }

        std::string resolved(MAX_PATH, '\0');

        if (path[0] == '1')
        {
            SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, resolved.data());
            resolved.resize(strlen(resolved.data()));
            resolved += "\\GTA San Andreas User Files";
            resolved += path + 2; // original path without '1:' prefix
            return resolved;
        }

        // game root directory
        GetModuleFileNameA(NULL, resolved.data(), MAX_PATH);
        resolved.resize(strlen(resolved.data()));
        
        if (path[0] == '2') // cleo directory
        {
            resolved += "\\cleo";
        }
        else if (path[0] == '3') // cleo modules directory
        {
            resolved += "\\cleo\\cleo_modules";
        }

        resolved += path + 2; // original path without 'X:' prefix
        return resolved;
    }

    void __declspec(naked) CCleoInstance::OnUpdateGameLogics()
    {
        //GetInstance().UpdateGameLogics(); // !
        GetInstance().SoundSystem.Update();
        static DWORD dwFunc;
        dwFunc = (DWORD)(GetInstance().UpdateGameLogics);
        _asm jmp dwFunc
    }
}
