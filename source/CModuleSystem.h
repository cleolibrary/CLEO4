#pragma once
#include <map>

namespace CLEO
{
	class CModuleSystem
	{
		std::map<std::string, CModule> modules;

	public:
		const ModuleExport* GetExport(const char* moduleName, const char* exportName);
	};

	struct ModuleExport
	{
		std::string name;
		int label = 0;

		void Clear();
		bool LoadFromFile(std::ifstream& file);
	};

	class CModule
	{
		std::string filePath;
		BYTE* data = nullptr;
		std::map<std::string, ModuleExport> exports;

	public:
		CModule() = default;
		~CModule();

		void Clear();
		bool LoadFromFile(const char* path);
		const ModuleExport* GetExport(const char* name);
	};
}

