#pragma once
#include <list>
#include <algorithm>
#include <windows.h>
#include "FileEnumerator.h"
#include "CDebug.h"

namespace CLEO
{
    class CPluginSystem
    {
        std::list<HMODULE> plugins;

    public:
        CPluginSystem()
        {
            TRACE("Loading plugins...");
            FilesWalk("cleo\\cleo_plugins", ".cleo", [this](const char *filename)
            {
                TRACE("Loading plugin '%s'", filename);
                HMODULE hlib = LoadLibrary(filename);
                if (!hlib)
                {
                    LOG_WARNING("Error loading plugin '%s'", filename);
                }
                else plugins.push_back(hlib);
            });
        }

        ~CPluginSystem()
        {
            std::for_each(plugins.begin(), plugins.end(), FreeLibrary);
        }

        inline size_t GetNumPlugins() { return plugins.size(); }
    };
}
