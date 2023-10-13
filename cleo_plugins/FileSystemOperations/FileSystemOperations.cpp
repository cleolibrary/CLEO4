#include "plugin.h"
#include "CLEO.h"

#include <set>

using namespace CLEO;
using namespace plugin;

class FileSystemOperations 
{
public:
    static std::set<HANDLE> m_hFileSearches;

    static void WINAPI OnFinalizeScriptObjects()
    {
        // clean up file searches
        for (auto handle : m_hFileSearches) FindClose(handle);
        m_hFileSearches.clear();
    }

    FileSystemOperations() 
    {
        auto cleoVer = CLEO_GetVersion();
        if (cleoVer >= CLEO_VERSION)
        {
            //register opcodes
            CLEO_RegisterOpcode(0x0AAB, Script_FS_FileExists);
            CLEO_RegisterOpcode(0x0AE4, Script_FS_DirectoryExists);
            CLEO_RegisterOpcode(0x0AE5, Script_FS_CreateDirectory);
            CLEO_RegisterOpcode(0x0AE6, Script_FS_FindFirstFile);
            CLEO_RegisterOpcode(0x0AE7, Script_FS_FindNextFile);
            CLEO_RegisterOpcode(0x0AE8, Script_FS_FindClose);

            CLEO_RegisterOpcode(0x0B00, Script_FS_DeleteFile);
            CLEO_RegisterOpcode(0x0B01, Script_FS_DeleteDirectory);
            CLEO_RegisterOpcode(0x0B02, Script_FS_MoveFile);
            CLEO_RegisterOpcode(0x0B03, Script_FS_MoveDir);
            CLEO_RegisterOpcode(0x0B04, Script_FS_CopyFile);
            CLEO_RegisterOpcode(0x0B05, Script_FS_CopyDir);

            // register event callbacks
            CLEO_RegisterCallback(eCallbackId::ScriptsFinalize, OnFinalizeScriptObjects);
        }
        else
        {
            std::string err(128, '\0');
            sprintf(err.data(), "This plugin requires version %X or later! \nCurrent version of CLEO is %X.", CLEO_VERSION >> 8, cleoVer >> 8);
            MessageBox(HWND_DESKTOP, err.data(), "FileSystemOperations.cleo", MB_SYSTEMMODAL | MB_ICONERROR);
        }
    }

    static std::string ReadPathParam(CRunningScript* thread)
    {
        std::string path(MAX_STR_LEN, '\0');
        CLEO_ReadStringOpcodeParam(thread, path.data(), MAX_STR_LEN);
        CLEO_ResolvePath(thread, path.data(), MAX_STR_LEN);
        path.resize(strlen(path.c_str()));
        return path;
    }

    // 0AAB=1, file_exists %1s%
    static OpcodeResult WINAPI Script_FS_FileExists(CRunningScript* thread)
    {
        auto filename = ReadPathParam(thread);

        DWORD fAttr = GetFileAttributes(filename.c_str());
        bool exists = (fAttr != INVALID_FILE_ATTRIBUTES) && !(fAttr & FILE_ATTRIBUTE_DIRECTORY);

        CLEO_SetThreadCondResult(thread, exists);
        return OR_CONTINUE;
    }

    // 0AE4=1, directory_exist %1s%
    static OpcodeResult WINAPI Script_FS_DirectoryExists(CRunningScript* thread)
    {
        auto filename = ReadPathParam(thread);

        DWORD fAttr = GetFileAttributes(filename.c_str());
        bool exists = (fAttr != INVALID_FILE_ATTRIBUTES) && (fAttr & FILE_ATTRIBUTE_DIRECTORY);

        CLEO_SetThreadCondResult(thread, exists);
        return OR_CONTINUE;
    }

    // 0AE5=1, create_directory %1s% //IF and SET
    static OpcodeResult WINAPI Script_FS_CreateDirectory(CRunningScript* thread)
    {
        auto filename = ReadPathParam(thread);

        bool result = CreateDirectory(filename.c_str(), NULL) != 0;

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }

    // 0AE6=3, %2d% = find_first_file %1s% get_filename_to %3s% //IF and SET
    static OpcodeResult WINAPI Script_FS_FindFirstFile(CRunningScript* thread)
    {
        auto filename = ReadPathParam(thread);
        WIN32_FIND_DATA ffd = { 0 };
        HANDLE handle = FindFirstFile(filename.c_str(), &ffd);

        CLEO_SetIntOpcodeParam(thread, (DWORD)handle);

        if (handle != INVALID_HANDLE_VALUE)
        {
            m_hFileSearches.insert(handle);

            CLEO_WriteStringOpcodeParam(thread, ffd.cFileName);
            CLEO_SetThreadCondResult(thread, true);
        }
        else
        {
            CLEO_SkipOpcodeParams(thread, 1);
            CLEO_SetThreadCondResult(thread, false);
        }
        return OR_CONTINUE;
    }

    // 0AE7=2,%2s% = find_next_file %1d% //IF and SET
    static OpcodeResult WINAPI Script_FS_FindNextFile(CRunningScript* thread)
    {
        auto handle = (HANDLE)CLEO_GetIntOpcodeParam(thread);

        WIN32_FIND_DATA ffd = { 0 };
        if (FindNextFile(handle, &ffd))
        {
            CLEO_WriteStringOpcodeParam(thread, ffd.cFileName);
            CLEO_SetThreadCondResult(thread, true);
        }
        else
        {
            CLEO_SkipOpcodeParams(thread, 1);
            CLEO_SetThreadCondResult(thread, false);
        }
        return OR_CONTINUE;
    }

    // 0AE8=1, find_close %1d%
    static OpcodeResult WINAPI Script_FS_FindClose(CRunningScript* thread)
    {
        auto handle = (HANDLE)CLEO_GetIntOpcodeParam(thread);
        FindClose(handle);
        m_hFileSearches.erase(handle);
        return OR_CONTINUE;
    }

    // 0B00=1, delete_file %1s% //IF and SET
    static OpcodeResult WINAPI Script_FS_DeleteFile(CScriptThread* thread)
    {
        auto filename = ReadPathParam(thread);

        CLEO_SetThreadCondResult(thread, DeleteFile(filename.c_str()));

        return OR_CONTINUE;
    }

    static BOOL DeleteDir(const char *path)
    {
        char mask[MAX_PATH];
        HANDLE hSearch = NULL;
        WIN32_FIND_DATA wfd;
        char subPath[MAX_PATH];

        memset(&wfd, 0, sizeof(wfd));
        //search mask
        sprintf(mask, "%s\\*", path);

        //try to delete all inside first
        if ((hSearch = FindFirstFile(mask, &wfd)) != INVALID_HANDLE_VALUE)
        {
            do
            {
                //recursively delete subdirectories
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if ((strcmp(wfd.cFileName, "..") != 0) && (strcmp(wfd.cFileName, ".") != 0))
                    {
                        sprintf(subPath, "%s\\%s", path, wfd.cFileName);
                        if (!DeleteDir(subPath))
                            return FALSE;
                    }
                }
                else
                {
                    //remove read-only, system, hidden flags
                    sprintf(subPath, "%s\\%s", path, wfd.cFileName);
                    SetFileAttributes(subPath, FILE_ATTRIBUTE_NORMAL);
                    //delete file
                    if (!DeleteFile(subPath))
                        return FALSE;
                }


            } while (FindNextFile(hSearch, &wfd));
            FindClose(hSearch);
        }

        //delete empty directory
        return RemoveDirectory(path);
    }

    // 0B01=1, delete_directory %1s% with_all_files_and_subdirectories %2d% //IF and SET
    static OpcodeResult WINAPI Script_FS_DeleteDirectory(CScriptThread* thread)
    {
        auto dirpath = ReadPathParam(thread);
        int DeleteAllInsideFlag = CLEO_GetIntOpcodeParam(thread);

        BOOL result;
        if (DeleteAllInsideFlag)
        {
            //remove directory with all files and subdirectories
            result = DeleteDir(dirpath.c_str());
        }
        else
        {
            //try to remove as empty directory
            result = RemoveDirectory(dirpath.c_str());
        }

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }

    // 0B02=2, move_file %1s% to %2s% //IF and SET
    static OpcodeResult WINAPI Script_FS_MoveFile(CScriptThread* thread)
    {
        auto filepath = ReadPathParam(thread);
        auto newFilepath = ReadPathParam(thread);

        BOOL result = GetFileAttributes(filepath.c_str()) & FILE_ATTRIBUTE_DIRECTORY;
        if (!result)
            result = MoveFile(filepath.c_str(), newFilepath.c_str());

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }

    // 0B03=2, move_directory %1s% to %2s% //IF and SET
    static OpcodeResult WINAPI Script_FS_MoveDir(CScriptThread* thread)
    {
        auto filepath = ReadPathParam(thread);
        auto newFilepath = ReadPathParam(thread);

        BOOL result = GetFileAttributes(filepath.c_str()) & FILE_ATTRIBUTE_DIRECTORY;
        if (result)
            result = MoveFile(filepath.c_str(), newFilepath.c_str());

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }

    // 0B04=2, copy_file %1s% to %2s% //IF and SET
    static OpcodeResult WINAPI Script_FS_CopyFile(CScriptThread* thread)
    {
        auto filepath = ReadPathParam(thread);
        auto newFilepath = ReadPathParam(thread);

        BOOL result = CopyFile(filepath.c_str(), newFilepath.c_str(), FALSE);
        if (result)
        {
            // copy file attributes
            DWORD fattr = GetFileAttributes(filepath.c_str());
            SetFileAttributes(newFilepath.c_str(), fattr);
        }

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }

    static BOOL CopyDir(const char *path, const char *newPath)
    {
        char mask[MAX_PATH];
        HANDLE hSearch = NULL;
        WIN32_FIND_DATA wfd;
        char subPath[MAX_PATH], newSubPath[MAX_PATH];
        DWORD fattr;

        //create parent directory
        if (!CreateDirectory(newPath, NULL))
            return FALSE;

        memset(&wfd, 0, sizeof(wfd));
        //search mask
        sprintf(mask, "%s\\*", path);

        //copy all files and folders into new directory
        if ((hSearch = FindFirstFile(mask, &wfd)) != INVALID_HANDLE_VALUE)
        {
            do
            {
                sprintf(subPath, "%s\\%s", path, wfd.cFileName);
                sprintf(newSubPath, "%s\\%s", newPath, wfd.cFileName);
                //copy subdirectories
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if ((strcmp(wfd.cFileName, "..") != 0) && (strcmp(wfd.cFileName, ".") != 0))
                    {
                        if (!CopyDir(subPath, newSubPath))
                            return FALSE;
                    }
                }
                else
                {
                    //copy file into new directory
                    if (CopyFile(subPath, newSubPath, FALSE))
                    {
                        fattr = GetFileAttributes(subPath);
                        SetFileAttributes(newSubPath, fattr);
                    }
                    else	return FALSE;
                }


            } while (FindNextFile(hSearch, &wfd));
            FindClose(hSearch);
        }

        //return success
        return TRUE;
    }

    // 0B05=2, copy_directory %1d% to %2d% //IF and SET
    static OpcodeResult WINAPI Script_FS_CopyDir(CScriptThread* thread)
    {
        auto filepath = ReadPathParam(thread);
        auto newFilepath = ReadPathParam(thread);

        BOOL result = CopyDir(filepath.c_str(), newFilepath.c_str());

        CLEO_SetThreadCondResult(thread, result);
        return OR_CONTINUE;
    }
} fileSystemOperations;

std::set<HANDLE> FileSystemOperations::m_hFileSearches;
