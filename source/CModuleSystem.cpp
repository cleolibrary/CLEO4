#include "stdafx.h"
#include "cleo.h"
#include "CModuleSystem.h"

#include <fstream>

using namespace CLEO;


CModule::~CModule()
{
	if (data != nullptr)
	{
		delete data;
		data = nullptr;
	}
}

void CLEO::CModule::Clear()
{
	filePath.clear();

	if (data != nullptr)
	{
		delete data;
		data = nullptr;
	}

	exports.clear();
}

bool CModule::LoadFromFile(const char* path)
{
	Clear();

	std::ifstream file(path, std::ios::binary);
	if (!file.good())
	{
		TRACE("Failed to open module file '%s'", path);
		return false;
	}

	const char Header_First_Instruction[] = { 0x20, 0x00, 0x10 }; // jump, param type
	const char Header_Magic[] = { 0xFF, 0x7F, 0xFE, 0x00, 0x00 }; // Rockstar custom header magic
	const char Header_Module_Signature[] = { 'E', 'X', 'P', 'T' }; // CLEO's module header signature
	struct
	{
		char firstInstruction[3];
		int jumpAddress;
		char magic[5];
		char signature[4];
		int size;
	} header;

	file.read((char*)&header, sizeof(header));
	if (!file.good())
	{
		TRACE("Module '%s' header read error", path);
		return false;
	}

	// verify header data
	if (std::memcmp(header.firstInstruction, Header_First_Instruction, sizeof(Header_First_Instruction)) != 0 ||
		std::memcmp(header.magic, Header_Magic, sizeof(Header_Magic)) != 0 ||
		std::memcmp(header.signature, Header_Module_Signature, sizeof(Header_Module_Signature)) != 0 ||
		header.size <= 0)
	{
		TRACE("Module '%s' load error. Invalid module file", path);
		return false;
	}

	auto endPos = file.tellg();
	endPos += header.size;

	while (true)
	{
		ModuleExport e;

		if (!e.LoadFromFile(file) || 
			!file.good() ||
			file.tellg() > endPos)
		{
			if (e.name.empty())
			{
				TRACE("Module '%s' export load error.", path);
			}
			else
			{
				TRACE("Module's '%s' export '%s' load error.", path);
			}
			return false;
		}

		exports[e.name] = std::move(e); // move to container

		if (file.tellg() == endPos)
		{
			break; // all exports done
		}
	}

	return true;
}

const ModuleExport* CModule::GetExport(const char* name)
{
	// TODO: normalize export name?

	auto it = exports.find(name);
	if (it == exports.end())
	{
		return nullptr;
	}

	return &(it->second);
}

const ModuleExport* CModuleSystem::GetExport(const char* moduleName, const char* exportName)
{
	// TODO: get current working dir
	std::string path = "";

	size_t len = strlen(moduleName);
	if (path.empty())
	{
		path.reserve(len);
	}
	else
	{
		path.reserve(len + 1);
		path.push_back('/'); // path separator
	}

	path += moduleName;

	std::transform(path.begin(), path.end(), path.begin(), std::tolower);
	std::replace(path.begin(), path.end(), '\\', '/'); // standarize path separator
	// TODO: resolve "../" in path
	
	auto it = modules.find(path);
	if (it == modules.end())
	{
		// module not loaded yet?
		CModule module;

		if (!module.LoadFromFile(path.c_str()))
		{
			return nullptr;
		}

		modules[path] = std::move(module); // move to container
	}

	return it->second.GetExport(exportName);
}

void CLEO::ModuleExport::Clear()
{
	name.clear();
	label = 0;
}

bool CLEO::ModuleExport::LoadFromFile(std::ifstream& file)
{
	if (!file.good())
	{
		return false;
	}

	// name
	std::getline(file, name, '\0');
	if (!file.good())
	{
		return false;
	}

	// address
	file.read((char*)&label, 4);
	if (!file.good())
	{
		return false;
	}

	// input arg count
	unsigned char inParamCount;
	file.read((char*)&inParamCount, 1);
	if (!file.good())
	{
		return false;
	}

	// skip input argument types info
	file.seekg(inParamCount, file._Seekcur);
	if (!file.good())
	{
		return false;
	}

	// return value count
	unsigned char outParamCount;
	file.read((char*)&outParamCount, 1);
	if (!file.good())
	{
		return false;
	}

	// skip return value types info
	file.seekg(outParamCount, file._Seekcur);
	if (!file.good())
	{
		return false;
	}

	return true; // done
}
