#include <cstdio>
#include <shlobj.h>
#include "CLEO.h"

class IniFiles {
public:
	IniFiles() {
		//check cleo version
		if (CLEO_GetVersion() >= CLEO_VERSION) {
			// register opcodes
			CLEO_RegisterOpcode(0x0AF0, Script_InifileGetInt);
			CLEO_RegisterOpcode(0x0AF1, Script_InifileWriteInt);
			CLEO_RegisterOpcode(0x0AF2, Script_InifileGetFloat);
			CLEO_RegisterOpcode(0x0AF3, Script_InifileWriteFloat);
			CLEO_RegisterOpcode(0x0AF4, Script_InifileReadString);
			CLEO_RegisterOpcode(0x0AF5, Script_InifileWriteString);
		}
		else
			MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IniFiles.cleo", MB_ICONERROR);
	}

	static void EnsureDirExists(const char* filePath)
	{
		if (!filePath || !*filePath) return;
		char dir[MAX_PATH];
		strncpy(dir, filePath, MAX_PATH - 1);
		dir[MAX_PATH - 1] = '\0';
		char* p = strrchr(dir, '\\');
		if (!p) p = strrchr(dir, '/');
		if (p)
		{
			*p = '\0';
			char* start = dir;
			if (strlen(start) > 2 && start[1] == ':')
				start += 3;
			for (char* s = start; *s; s++)
			{
				if (*s == '\\' || *s == '/')
				{
					*s = '\0';
					CreateDirectoryA(dir, NULL);
					*s = '\\';
				}
			}
			CreateDirectoryA(dir, NULL);
		}
	}

	static bool IsPathWritable(const char* path)
	{
		if (!path || !*path) return false;
		char probe[MAX_PATH];
		strncpy(probe, path, MAX_PATH - 1);
		probe[MAX_PATH - 1] = '\0';
		char* p = strrchr(probe, '\\');
		if (!p) p = strrchr(probe, '/');
		if (p) *(p + 1) = '\0';
		else strcpy(probe, ".\\");

		sprintf(probe + strlen(probe), "__cleo_ini_probe_%lu.tmp", GetCurrentProcessId());
		HANDLE hProbe = CreateFileA(probe, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		if (hProbe != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hProbe);
			return true;
		}
		return false;
	}

	static bool GetUserFilesPath(const char* relativePath, char* dst)
	{
		if (!relativePath || !*relativePath || !dst) return false;
		char myDocs[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, myDocs)))
		{
			// Skip leading slashes in relativePath if any
			while (*relativePath == '\\' || *relativePath == '/') relativePath++;
			sprintf(dst, "%s\\GTA San Andreas User Files\\%s", myDocs, relativePath);
			return true;
		}
		return false;
	}

	static char* MakeFullPath(const char *path, char *dst)
	{
		if (!dst) return dst;
		if (!path || !*path)
		{
			*dst = '\0';
			return dst;
		}

		if (strlen(path) > 1 && path[1] == ':')
		{
			strncpy(dst, path, MAX_PATH - 1);
			dst[MAX_PATH - 1] = '\0';
		}
		else
		{
			//get current working directory
			GetCurrentDirectoryA(MAX_PATH, dst);
			size_t len = strlen(dst);
			if (len > 0 && dst[len - 1] != '\\' && dst[len - 1] != '/')
			{
				strncat(dst, "\\", MAX_PATH - len - 1);
			}
			strncat(dst, path, MAX_PATH - strlen(dst) - 1);
		}
		return dst;
	}

	static char* ResolveIniReadPath(const char* path, char* dst)
	{
		MakeFullPath(path, dst);
		if (path && *path && (strlen(path) < 2 || path[1] != ':') && path[0] != '\\' && path[0] != '/')
		{
			char userPath[MAX_PATH];
			if (GetUserFilesPath(path, userPath))
			{
				if (GetFileAttributesA(userPath) != INVALID_FILE_ATTRIBUTES)
				{
					if (GetFileAttributesA(dst) == INVALID_FILE_ATTRIBUTES || !IsPathWritable(dst))
					{
						strncpy(dst, userPath, MAX_PATH - 1);
						dst[MAX_PATH - 1] = '\0';
						return dst;
					}
				}
			}
		}
		return dst;
	}

	static BOOL SafeWritePrivateProfileString(const char* sectionName, const char* key, const char* value, const char* path)
	{
		if (!path || !*path) return FALSE;

		char iniPath[MAX_PATH];
		MakeFullPath(path, iniPath);

		BOOL result = WritePrivateProfileStringA(sectionName, key, value, iniPath);
		if (result) return TRUE;

		if ((strlen(path) < 2 || path[1] != ':') && path[0] != '\\' && path[0] != '/')
		{
			char userPath[MAX_PATH];
			if (GetUserFilesPath(path, userPath))
			{
				EnsureDirExists(userPath);
				if (GetFileAttributesA(userPath) == INVALID_FILE_ATTRIBUTES && GetFileAttributesA(iniPath) != INVALID_FILE_ATTRIBUTES)
				{
					if (CopyFileA(iniPath, userPath, TRUE))
					{
						SetFileAttributesA(userPath, FILE_ATTRIBUTE_NORMAL);
					}
				}
				return WritePrivateProfileStringA(sectionName, key, value, userPath);
			}
		}
		return FALSE;
	}

	static OpcodeResult WINAPI Script_InifileGetInt(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF0=4,%4d% = get_int_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char iniPath[MAX_PATH];
		char path[100];
		char sectionName[100];
		char key[100];
		int result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		// Resolve path taking into account UAC and User Files
		ResolveIniReadPath(path, iniPath);

		result = GetPrivateProfileInt(sectionName, key, 0x80000000, iniPath);
		CLEO_SetIntOpcodeParam(thread, result);
		CLEO_SetThreadCondResult(thread, result != 0x80000000);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileWriteInt(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
		****************************************************************/
	{
		char path[100];
		char sectionName[100];
		char key[100];
		DWORD value;
		char strValue[100];
		BOOL result;

		value = CLEO_GetIntOpcodeParam(thread);
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		result = SafeWritePrivateProfileString(sectionName, key, _itoa(value, strValue, 10), path);
		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileGetFloat(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF2=4,%4d% = get_float_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char iniPath[MAX_PATH];
		char path[100];
		char sectionName[100];
		char key[100];
		float value = 0.0f;
		char strValue[100];
		BOOL result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		// Resolve path taking into account UAC and User Files
		ResolveIniReadPath(path, iniPath);

		result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);
		if (result)
		{
			value = (float)atof(strValue);
			CLEO_SetFloatOpcodeParam(thread, value);
		}
		else
			CLEO_SkipOpcodeParams(thread, 1);

		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileWriteFloat(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
		****************************************************************/
	{
		char path[100];
		char sectionName[100];
		char key[100];
		float value;
		char strValue[100];
		BOOL result;

		value = CLEO_GetFloatOpcodeParam(thread);
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		sprintf(strValue, "%g", value);

		result = SafeWritePrivateProfileString(sectionName, key, strValue, path);
		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileReadString(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char iniPath[MAX_PATH];
		char path[100];
		char sectionName[100];
		char key[100];
		char strValue[100];
		char *strptr;
		BOOL result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		// Resolve path taking into account UAC and User Files
		ResolveIniReadPath(path, iniPath);

		result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);
		if (result)
		{
			switch (CLEO_GetOperandType(thread))
			{
			case globalVarVString:
			case localVarVString:
			case globalVarSString:
			case localVarSString:
				CLEO_WriteStringOpcodeParam(thread, strValue);
				break;
			default:
				strptr = (char *)CLEO_GetIntOpcodeParam(thread);
				strcpy(strptr, strValue);
			}
		}
		else
			CLEO_SkipOpcodeParams(thread, 1);

		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileWriteString(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%
		****************************************************************/
	{
		char path[100];
		char sectionName[100];
		char key[100];
		char strValue[100];
		char *strptr;
		BOOL result;

		switch (CLEO_GetOperandType(thread))
		{
		case globalVarVString:
		case localVarVString:
		case globalVarSString:
		case localVarSString:
			CLEO_ReadStringPointerOpcodeParam(thread, strValue, sizeof(strValue));
			break;
		default:
			strptr = (char *)CLEO_GetIntOpcodeParam(thread);
			strcpy(strValue, strptr);
		}
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		result = SafeWritePrivateProfileString(sectionName, key, strValue, path);

		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}
} iniFiles;
