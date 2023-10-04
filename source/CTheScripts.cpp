#include "stdafx.h"
#include "CTheScripts.h"
#include "cleo.h"
#include "CFileMgr.h"

using namespace CLEO;


bool CRunningScript::IsCustom() const
{
    auto cs = reinterpret_cast<const CCustomScript*>(this);
    return cs->IsCustom();
}

const char* CRunningScript::GetScriptFileDir() const
{
    if (IsCustom())
    {
        return reinterpret_cast<const CCustomScript*>(this)->GetScriptFileDir();
    }

    return GetInstance().ScriptEngine.MainScriptFileDir.c_str();
}

void CRunningScript::SetScriptFileDir(const char* directory)
{
    if (IsCustom())
    {
        reinterpret_cast<CCustomScript*>(this)->SetScriptFileDir(directory);
        return;
    }

    GetInstance().ScriptEngine.MainScriptFileDir = directory;
}

const char* CRunningScript::GetScriptFileName() const
{
    if (IsCustom())
    {
        return reinterpret_cast<const CCustomScript*>(this)->GetScriptFileName();
    }

    return GetInstance().ScriptEngine.MainScriptFileName.c_str();
}

void CRunningScript::SetScriptFileName(const char* filename)
{
    if (IsCustom())
    {
        reinterpret_cast<CCustomScript*>(this)->SetScriptFileName(filename);
        return;
    }

    GetInstance().ScriptEngine.MainScriptFileName = filename;
}

const char* CRunningScript::GetWorkDir() const
{
    if (IsCustom())
    {
        return reinterpret_cast<const CCustomScript*>(this)->GetWorkDir();
    }

    return GetInstance().ScriptEngine.MainScriptCurWorkDir.c_str();
}

void CRunningScript::SetWorkDir(const char* directory)
{
    if (IsCustom())
    {
        reinterpret_cast<CCustomScript*>(this)->SetWorkDir(directory);
        return;
    }

    GetInstance().ScriptEngine.MainScriptCurWorkDir = directory;
}

std::string CRunningScript::ResolvePath(const char* path, const char* customWorkDir) const
{
    if (path == nullptr)
    {
        return {};
    }

    std::string result;
    if (strlen(path) < 2 || path[1] != ':') // does not start with drive letter
    {
        result = (customWorkDir != nullptr) ? customWorkDir : GetWorkDir();
        result.push_back('\\');
        result += path;
    }
    else
    {
        result = path;
    }

    // predefined CLEO paths starting with '[digit]:'
    if (result.length() < 2 || result[1] != ':' ||
        result[0] < DIR_GAME[0] || result[0] > DIR_MODULES[0]) // supported range
    {
        return result; // not predefined path prefix found
    }

    if (result[0] == DIR_USER[0]) // saves/settings location
    {
        return std::string(GetUserDirectory()) + &result[2]; // original path without '1:' prefix;
    }

    if (result[0] == DIR_SCRIPT[0]) // current script location
    {
        return std::string(GetScriptFileDir()) + &result[2]; // original path without '2:' prefix;
    }

    // game root directory
    std::string resolved = CFileMgr::ms_rootDirName;

    if (result[0] == DIR_CLEO[0]) // cleo directory
    {
        resolved += "\\cleo";
    }
    else if (result[0] == DIR_MODULES[0]) // cleo modules directory
    {
        resolved += "\\cleo\\cleo_modules";
    }

    resolved += &result[2]; // original path without 'X:' prefix
    return resolved;
}

