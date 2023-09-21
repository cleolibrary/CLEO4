#include "stdafx.h"
#include "cleo.h"
//#include <windows.h>
//#include <shlobj.h>

#include <filesystem>

namespace CLEO
{
    CCleoInstance CleoInstance;
    CCleoInstance& GetInstance() { return CleoInstance; }

    std::string ResolvePath(const char* path, const char* workDir)
    {
        if (path == nullptr)
        {
            return {};
        }

        std::string result;
        if (strlen(path) < 2 || path[1] != ':') // does not start with drive letter
        {
            if (workDir != nullptr)
            {
                result = std::string(workDir) + '\\' + path;
            }
            else
            {
                // application's current working dir. Can be set with 0A99
                result = std::string(MAX_PATH, '\0');
                _getcwd(result.data(), MAX_PATH);
                result.resize(strlen(result.data()));

                result.push_back('\\');
                result.append(path);
            }
        }
        else
        {
            result = path;
        }

        return result;

        // TODO: CLEO virtual paths. Enable later
        // predefined CLEO paths starting with '[digit]:'
        /*if (result.length() < 2 || result[1] != ':' ||
            result[0] < '0' || result[0] > '3') // supported range
        {
            return result; // not predefined path prefix found
        }

        std::string resolved(MAX_PATH, '\0');

        if (result[0] == '1') // 1: game saves
        {
            SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, resolved.data());
            resolved.resize(strlen(resolved.data()));
            resolved += "\\GTA San Andreas User Files";
            resolved += &result[2]; // original path without '1:' prefix
            return resolved;
        }

        // 0: game root directory
        GetModuleFileNameA(NULL, resolved.data(), MAX_PATH);
        resolved.resize(strlen(resolved.data()));
        resolved = std::filesystem::path(resolved).parent_path().string(); // remove executable name
        
        if (result[0] == '2') // 2: cleo directory
        {
            resolved += "\\cleo";
        }
        else if (result[0] == '3') // 3: cleo modules directory
        {
            resolved += "\\cleo\\cleo_modules";
        }

        resolved += &result[2]; // original path without 'X:' prefix
        return resolved;*/
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
