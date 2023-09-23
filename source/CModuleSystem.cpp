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
	std::string path(moduleName);
	NormalizePath(path);

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
	NormalizePath(normalizedPath);

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

	auto p = CLEO::ResolvePath(path); // actual absolute path
	try
	{
		for (auto& it : std::filesystem::recursive_directory_iterator(p))
		{
			auto& filePath = it.path();
			if (filePath.extension() == ".s")
			{
				result &= LoadFile(filePath.string().c_str());
			}
		}
	}
	catch (const std::exception& ex)
	{
		TRACE("Error while iterating CLEO Modules: %s", ex.what());
		return false;
	}

	return result;
}

bool CModuleSystem::LoadCleoModules()
{
	return LoadDirectory("3:\\"); // cleo\cleo_modules
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

void CLEO::CModuleSystem::NormalizePath(std::string& path)
{
	for (char& c : path)
	{
		// standarize path separators
		if (c == '/')
			c = '\\';

		// lower case
		c = std::tolower(c);
	};
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
	const char Segment_First_Instruction[] = { 0x02, 0x00, 0x01 }; // jump, param type
	const char Segment_Magic[] = { 0xFF, 0x7F, 0xFE, 0x00, 0x00 }; // Rockstar custom header magic
	const char Header_Signature_Module_Exports[] = { 'E', 'X', 'P', 'T' }; // CLEO's module header signature
#pragma warning ( pop )

	// read first instruction
#pragma pack(push, 1)
	struct
	{
		char firstInstruction[3];
		int jumpAddress;
		char magic[5];
	} segment;
#pragma pack(pop)

	file.read((char*)&segment, sizeof(segment));
	if (file.fail())
	{
		TRACE("Module '%s' file header read error", path);
		return false;
	}

	// verify segment data
	if (std::memcmp(segment.firstInstruction, Segment_First_Instruction, sizeof(Segment_First_Instruction)) != 0 ||
		segment.jumpAddress >= 0 || // jump labels should be negative values
		std::memcmp(segment.magic, Segment_Magic, sizeof(Segment_Magic)) != 0) // not a custom header
	{
		TRACE("Module '%s' load error. Custom segment not present", path);
		return false;
	}
	segment.jumpAddress = abs(segment.jumpAddress); // turn label into actual file offset

	// process custom headers
#pragma pack(push, 1)
	struct
	{
		char signature[4];
		int size;
	} header;
#pragma pack(pop)

	bool result = false; // no custom header found yet
	while (file.tellg() < segment.jumpAddress)
	{
		file.read((char*)&header, sizeof(header));
		if (file.fail() ||
			file.tellg() > segment.jumpAddress) // read past the segment end
		{ 
			TRACE("Module '%s' load error. Invalid custom header", path);
			return false;
		}

		auto headerEndPos = file.tellg();
		headerEndPos += header.size;

		// CLEO Module Exports
		if (std::memcmp(header.signature, Header_Signature_Module_Exports, sizeof(Header_Signature_Module_Exports)) == 0)
		{
			if (headerEndPos > segment.jumpAddress)
			{
				TRACE("Module '%s' load error. Invalid size of exports header", path);
				return false;
			}

			while (true)
			{
				ModuleExport e;

				if (!e.LoadFromFile(file) ||
					!file.good() ||
					file.tellg() > headerEndPos)
				{
					if (e.name.empty())
					{
						TRACE("Module '%s' export load error.", path);
					}
					else
					{
						TRACE("Module's '%s' export '%s' load error.", path, e.name.c_str());
					}
					return false;
				}

				exports[e.name] = std::move(e); // move to container
				result = true; // something useful loaded

				if (file.tellg() == headerEndPos)
				{
					break; // all exports done
				}
			}
		}
		else // other unknown header type
		{
			file.seekg(headerEndPos, file.beg);
			if (file.fail())
			{
				TRACE("Module '%s' load error. Error while skipping unknown header type", path);
				return false;
			}
		}
	}

	if (!file.good())
	{
		TRACE("Module '%s' read error", path);
		return false;
	}

	if (!result) // no usable elements found. No point to keeping this module
	{
		TRACE("Module '%s' skipped. Nothing found", path);
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
	auto normalized = std::string(name);
	ModuleExport::NormalizeName(normalized);

	auto it = exports.find(normalized);
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
	NormalizeName(name);

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

void CModuleSystem::CModule::ModuleExport::NormalizeName(std::string& name)
{
	for (auto& ch : name)
	{
		ch = std::tolower(ch);
	}
}
