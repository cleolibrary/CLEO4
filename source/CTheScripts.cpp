#include "stdafx.h"
#include "CTheScripts.h"
#include "cleo.h"
//#include "CScriptEngine.h"


bool CRunningScript::IsCustom()
{
    auto cs = reinterpret_cast<CLEO::CCustomScript*>(this);
    return cs->IsCustom();
}

const char* CRunningScript::GetScriptFileDir()
{
    if (IsCustom())
    {
        return reinterpret_cast<CLEO::CCustomScript*>(this)->GetScriptFileDir();
    }

    return CLEO::GetInstance().ScriptEngine.MainScriptFileDir.c_str();
}

void CRunningScript::SetScriptFileDir(const char* directory)
{
    if (IsCustom())
    {
        reinterpret_cast<CLEO::CCustomScript*>(this)->SetScriptFileDir(directory);
        return;
    }

    CLEO::GetInstance().ScriptEngine.MainScriptFileDir = directory;
}

const char* CRunningScript::GetScriptFileName()
{
    if (IsCustom())
    {
        return reinterpret_cast<CLEO::CCustomScript*>(this)->GetScriptFileName();
    }

    return CLEO::GetInstance().ScriptEngine.MainScriptFileName.c_str();
}

void CRunningScript::SetScriptFileName(const char* filename)
{
    if (IsCustom())
    {
        reinterpret_cast<CLEO::CCustomScript*>(this)->SetScriptFileName(filename);
        return;
    }

    CLEO::GetInstance().ScriptEngine.MainScriptFileName = filename;
}

