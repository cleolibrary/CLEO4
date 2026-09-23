#include "stdafx.h"
#include "CUacHelper.h"
#include <shlobj.h>
#include <direct.h>
#include <sys/stat.h>

namespace CLEO
{
    bool CUacHelper::IsElevated()
    {
        HANDLE hToken = NULL;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        {
            TOKEN_ELEVATION elevation;
            DWORD dwSize = sizeof(TOKEN_ELEVATION);
            if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
            {
                CloseHandle(hToken);
                return elevation.TokenIsElevated != 0;
            }
            CloseHandle(hToken);
        }
        return false;
    }

    bool CUacHelper::IsDirectoryWritable(const char* dirPath)
    {
        char probePath[MAX_PATH];
        if (dirPath && *dirPath && strcmp(dirPath, ".") != 0)
        {
            size_t len = strlen(dirPath);
            if (dirPath[len - 1] == '\\' || dirPath[len - 1] == '/')
                sprintf(probePath, "%s__cleo_probe_%lu.tmp", dirPath, GetCurrentProcessId());
            else
                sprintf(probePath, "%s/__cleo_probe_%lu.tmp", dirPath, GetCurrentProcessId());
        }
        else
        {
            sprintf(probePath, "./__cleo_probe_%lu.tmp", GetCurrentProcessId());
        }

        HANDLE hFile = CreateFileA(probePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFile);
            return true;
        }
        return false;
    }

    const std::string& CUacHelper::GetUserFilesDirectory()
    {
        static std::string s_userDir;
        if (s_userDir.empty())
        {
            char path[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path)))
            {
                s_userDir = std::string(path) + "\\GTA San Andreas User Files\\";
                EnsureDirectoryExists(s_userDir);
            }
            else
            {
                s_userDir = ".\\";
            }
        }
        return s_userDir;
    }

    bool CUacHelper::EnsureDirectoryExists(const std::string& dirPath)
    {
        if (dirPath.empty()) return false;

        DWORD attr = GetFileAttributesA(dirPath.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
            return true;

        char tmp[MAX_PATH];
        strncpy(tmp, dirPath.c_str(), sizeof(tmp));
        tmp[sizeof(tmp) - 1] = '\0';

        size_t len = strlen(tmp);
        if (len > 0 && (tmp[len - 1] == '\\' || tmp[len - 1] == '/'))
            tmp[len - 1] = '\0';

        char* start = tmp;
        if (strlen(start) > 2 && start[1] == ':')
            start += 3; // Skip "C:\"

        for (char* p = start; *p; p++)
        {
            if (*p == '\\' || *p == '/')
            {
                *p = '\0';
                CreateDirectoryA(tmp, NULL);
                *p = '\\';
            }
        }
        return CreateDirectoryA(tmp, NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
    }

    std::string CUacHelper::GetCleoSavesDirectory(bool forWriting)
    {
        if (forWriting)
        {
            if (IsDirectoryWritable("cleo") || IsDirectoryWritable("cleo/cleo_saves"))
            {
                return "cleo/cleo_saves";
            }
            std::string userSaves = GetUserFilesDirectory() + "cleo_saves";
            EnsureDirectoryExists(userSaves);
            return userSaves;
        }
        else
        {
            return "cleo/cleo_saves";
        }
    }

    std::string CUacHelper::ResolveSaveFilePath(int nSlot, bool forWriting)
    {
        char defaultPath[MAX_PATH];
        sprintf(defaultPath, "./cleo/cleo_saves/cs%d.sav", nSlot);

        if (!forWriting)
        {
            std::string userSave = GetUserFilesDirectory() + "cleo_saves\\cs" + std::to_string(nSlot) + ".sav";
            bool userExists = (GetFileAttributesA(userSave.c_str()) != INVALID_FILE_ATTRIBUTES);

            // If game directory is not writable and user save exists, prefer user save
            if (userExists && !IsDirectoryWritable("cleo/cleo_saves") && !IsDirectoryWritable("cleo"))
                return userSave;

            if (GetFileAttributesA(defaultPath) != INVALID_FILE_ATTRIBUTES)
                return defaultPath;

            if (userExists)
                return userSave;

            return defaultPath;
        }
        else
        {
            if (IsDirectoryWritable("cleo/cleo_saves") || (EnsureDirectoryExists("cleo/cleo_saves") && IsDirectoryWritable("cleo/cleo_saves")))
            {
                return defaultPath;
            }

            std::string userSavesDir = GetUserFilesDirectory() + "cleo_saves";
            EnsureDirectoryExists(userSavesDir);
            return userSavesDir + "\\cs" + std::to_string(nSlot) + ".sav";
        }
    }

    std::string CUacHelper::ResolveWritablePath(const char* relativePath)
    {
        if (!relativePath || !*relativePath) return "";

        if (relativePath[0] == '\\' || relativePath[0] == '/' || (strlen(relativePath) > 1 && relativePath[1] == ':'))
            return relativePath;

        if (IsDirectoryWritable("."))
            return relativePath;

        std::string target = GetUserFilesDirectory() + relativePath;
        size_t lastSlash = target.find_last_of("\\/");
        if (lastSlash != std::string::npos)
        {
            EnsureDirectoryExists(target.substr(0, lastSlash));
        }
        return target;
    }

    std::string CUacHelper::ResolveReadablePath(const char* relativePath)
    {
        if (!relativePath || !*relativePath) return "";

        if (relativePath[0] == '\\' || relativePath[0] == '/' || (strlen(relativePath) > 1 && relativePath[1] == ':'))
            return relativePath;

        std::string userPath = GetUserFilesDirectory() + relativePath;
        bool userExists = (GetFileAttributesA(userPath.c_str()) != INVALID_FILE_ATTRIBUTES);

        // If game directory is not writable and user copy exists, prefer user copy
        if (userExists && !IsDirectoryWritable("."))
            return userPath;

        if (GetFileAttributesA(relativePath) != INVALID_FILE_ATTRIBUTES)
            return relativePath;

        if (userExists)
            return userPath;

        return relativePath;
    }

    std::string CUacHelper::ResolveLogFilePath()
    {
        if (IsDirectoryWritable("."))
            return "cleo.log";

        return GetUserFilesDirectory() + "cleo.log";
    }
}
