#pragma once
#include "CLEO.h"
#include "CFileMgr.h"
#include <string>

// plugin's config file
static std::string GetConfigFilename()
{
    std::string configFile = CFileMgr::ms_rootDirName;
    if (!configFile.empty()) configFile += "\\";

    configFile += "cleo\\cleo_plugins\\DebugUtils.ini";

    return configFile;
}

// var-args opcodes
static void SkipUnusedParams(CScriptThread* thread)
{
    while (CLEO_GetOperandType(thread) != DT_END)
        CLEO_SkipOpcodeParams(thread, 1); // skip param

    thread->ReadDataByte(); // skip terminator
}

