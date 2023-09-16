#include "stdafx.h"
#include "cleo.h"
#include "CModuleSystem.h"

#include <filesystem>
#include <fstream>

using namespace CLEO;

void CModuleSystem::Clear()
{
	modules.clear();
}

const ScriptDataRef CModuleSystem::GetExport(const char* moduleName, const char* exportName)
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
		path.reserve(path.length() + len + 1);
		path.push_back('/'); // path separator
	}

	path += moduleName;
	normalizePath(path);

	auto it = modules.find(path);
	if (it == modules.end()) // module not loaded yet?
	{
		if (!LoadFile(path.c_str()))
		{
			return {};
		}

		// check if available now
		it = modules.find(path);
		if (it == modules.end())
		{
			return {};
		}
	}
	auto& module = it->second;

	return module.GetExport(exportName);
}

bool CModuleSystem::LoadFile(const char* path)
{
	CModule module;

	std::string normalizedPath(path);
	normalizePath(normalizedPath);

	if (!module.LoadFromFile(normalizedPath.c_str()))
	{
		return false;
	}

	modules[normalizedPath] = std::move(module); // move to container
	return true;
}

bool CModuleSystem::LoadDirectory(const char* path)
{
	bool result = true;

	for (auto& it : std::filesystem::recursive_directory_iterator(path))
	{
		auto& filePath = it.path();
		if (filePath.extension() == ".s")
		{
			result &= LoadFile(filePath.string().c_str());
		}
	}

	return result;
}

bool CModuleSystem::LoadCleoModules()
{
	return LoadDirectory("cleo/cleo_modules"); // TODO: select work dir
}

bool CModuleSystem::Reload()
{
	std::set<std::string> names;
	for (auto& it : modules)
	{
		names.insert(it.second.GetFilepath());
	}

	Clear();

	bool result = true;
	for (auto& name : names)
	{
		result &= LoadFile(name.c_str());
	}

	return result;
}

void CLEO::CModuleSystem::normalizePath(std::string& path)
{
	for (char& c : path)
	{
		// standarize path separators
		if (c == '\\')
			c = '/';

		// lower case
		c = std::tolower(c);
	};

	// TODO: resolve "../" in path
}

void CModuleSystem::CModule::Clear()
{
	filepath.clear();
	data.clear();
	exports.clear();
}

const char* CModuleSystem::CModule::GetFilepath() const
{
	return filepath.c_str();
}

bool CModuleSystem::CModule::LoadFromFile(const char* path)
{
	Clear();

	std::ifstream file(path, std::ios::binary);
	if (!file.good())
	{
		TRACE("Failed to open module file '%s'", path);
		return false;
	}

#pragma warning ( push )
#pragma warning ( disable: 4838 )
#pragma warning ( disable: 4309 )
	const char Header_First_Instruction[] = { 0x02, 0x00, 0x01 }; // jump, param type
	const char Header_Magic[] = { 0xFF, 0x7F, 0xFE, 0x00, 0x00 }; // Rockstar custom header magic
	const char Header_Module_Signature[] = { 'E', 'X', 'P', 'T' }; // CLEO's module header signature
#pragma warning ( pop )

#pragma pack(push, 1)
	struct
	{
		char firstInstruction[3];
		int jumpAddress;
		char magic[5];
		char signature[4];
		int size;
	} header;
#pragma pack(pop)

	file.read((char*)&header, sizeof(header));
	if (file.fail())
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

	if (!file.good())
	{
		TRACE("Module '%s' read error", path);
		return false;
	}

	// get file size
	file.seekg(0, file.end);
	auto size = (size_t)file.tellg();
	file.seekg(0, file.beg);

	// store file data
	data.resize(size);
	file.read(data.data(), size);
	if (file.fail())
	{
		return false;
	}

	return true;
}

const ScriptDataRef CModuleSystem::CModule::GetExport(const char* name)
{
	// TODO: normalize export name?

	auto it = exports.find(name);
	if (it == exports.end())
	{
		return {};
	}
	auto& exp = it->second;

	return { data.data(), exp.offset };
}

void CModuleSystem::CModule::ModuleExport::Clear()
{
	name.clear();
	offset = 0;
}

bool CModuleSystem::CModule::ModuleExport::LoadFromFile(std::ifstream& file)
{
	if (!file.good())
	{
		return false;
	}

	// name
	std::getline(file, name, '\0');
	if (file.fail() || name.length() >= 0xFF)
	{
		return false;
	}

	// address
	file.read((char*)&offset, 4);
	if (file.fail())
	{
		return false;
	}

	// input arg count
	unsigned char inParamCount;
	file.read((char*)&inParamCount, 1);
	if (file.fail())
	{
		return false;
	}

	// skip input argument types info
	file.seekg(inParamCount, file.cur);
	if (file.fail())
	{
		return false;
	}

	// return value count
	unsigned char outParamCount;
	file.read((char*)&outParamCount, 1);
	if (file.fail())
	{
		return false;
	}

	// skip return value types info
	file.seekg(outParamCount, file.cur);
	if (file.fail())
	{
		return false;
	}

	return true; // done
}
