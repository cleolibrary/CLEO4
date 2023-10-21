#pragma once
#include "CLEO.h"
#include "CFileMgr.h"
#include <string>

// plugin's config file
static std::string GetConfigFilename()
{
    std::string configFile = CFileMgr::ms_rootDirName;
    if (!configFile.empty() && configFile.back() != '\\') configFile.push_back('\\');

    configFile += "cleo\\cleo_plugins\\DebugUtils.ini";

    return configFile;
}

