#include <cstdio>
#include "CLEO.h"

using namespace CLEO;

class IniFiles
{
public:
	IniFiles()
	{
		//check cleo version
		if (CLEO_GetVersion() >= CLEO_VERSION)
		{
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

	static OpcodeResult WINAPI Script_InifileGetInt(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF0=4,%4d% = get_int_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		int result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		result = GetPrivateProfileInt(sectionName, key, 0x80000000, path);
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
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		DWORD value;
		char strValue[100];
		BOOL result;

		value = CLEO_GetIntOpcodeParam(thread);
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		result = WritePrivateProfileString(sectionName, key, _itoa(value, strValue, 10), path);
		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileGetFloat(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF2=4,%4d% = get_float_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		float value = 0.0f;
		char strValue[100];
		BOOL result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), path);
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
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		float value;
		char strValue[100];
		BOOL result;

		value = CLEO_GetFloatOpcodeParam(thread);
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		sprintf(strValue, "%g", value);

		result = WritePrivateProfileString(sectionName, key, strValue, path);
		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI Script_InifileReadString(CScriptThread* thread)
		/****************************************************************
		Opcode Format
		0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
		****************************************************************/
	{
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		char strValue[100];
		char *strptr;
		BOOL result;

		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), path);
		if (result)
		{
			switch (CLEO_GetOperandType(thread))
			{
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
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
		char path[MAX_PATH];
		char sectionName[100];
		char key[100];
		char strValue[100];
		BOOL result;

		CLEO_ReadStringPointerOpcodeParam(thread, strValue, sizeof(strValue));
		CLEO_ReadStringPointerOpcodeParam(thread, path, sizeof(path));
		CLEO_ReadStringPointerOpcodeParam(thread, sectionName, sizeof(sectionName));
		CLEO_ReadStringPointerOpcodeParam(thread, key, sizeof(key));

		CLEO_ResolvePath(thread, path, sizeof(path)); // convert to absolute path

		result = WritePrivateProfileString(sectionName, key, strValue, path);

		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}
} iniFiles;
