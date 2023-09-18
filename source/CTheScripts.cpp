#include "stdafx.h"
#include "CTheScripts.h"
#include "cleo.h"


const char* CRunningScript::GetScriptFileDir() const
{
    return CLEO::GetInstance().ScriptEngine.MainScriptFileDir.c_str();
}

void CRunningScript::SetScriptFileDir(const char* directory)
{
    CLEO::GetInstance().ScriptEngine.MainScriptFileDir = directory;
}

const char* CRunningScript::GetScriptFileName() const
{
    return CLEO::GetInstance().ScriptEngine.MainScriptFileName.c_str();
}

void CRunningScript::SetScriptFileName(const char* filename)
{
    CLEO::GetInstance().ScriptEngine.MainScriptFileName = filename;
}

