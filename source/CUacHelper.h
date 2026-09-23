#pragma once
#include <windows.h>
#include <string>

namespace CLEO
{
    class CUacHelper
    {
    public:
        // Returns true if the current process is running with Administrator privileges
        static bool IsElevated();

        // Returns true if files can be created and written in the specified directory (defaults to current dir ".")
        static bool IsDirectoryWritable(const char* dirPath = ".");

        // Gets the GTA San Andreas User Files path in Documents (e.g. C:\Users\...\Documents\GTA San Andreas User Files\)
        static const std::string& GetUserFilesDirectory();

        // Gets the directory for cleo_saves, preferring game dir if writable, or User Files if protected
        static std::string GetCleoSavesDirectory(bool forWriting = false);

        // Resolves the full path to a save file cs<slot>.sav, checking both locations on read and choosing safe location on write
        static std::string ResolveSaveFilePath(int nSlot, bool forWriting = false);

        // Ensures the directory and all parent directories exist
        static bool EnsureDirectoryExists(const std::string& dirPath);

        // Resolves relative write path if current directory is read-only
        static std::string ResolveWritablePath(const char* relativePath);

        // Resolves relative read path: checks game directory first, falls back to User Files
        static std::string ResolveReadablePath(const char* relativePath);

        // Resolves path for cleo.log
        static std::string ResolveLogFilePath();
    };
}
