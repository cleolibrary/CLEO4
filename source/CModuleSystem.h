#pragma once
#include <map>

namespace CLEO
{
	struct ScriptDataRef
	{
		char* base = nullptr; // script's base data
		int offset = 0; // address within the script

		bool Valid() const
		{
			return base != nullptr;
		}
	};

	class CModuleSystem
	{
	public:
		void Clear();

		const ScriptDataRef GetExport(const char* moduleName, const char* exportName);

		bool LoadFile(const char* const path); // single file
		bool LoadDirectory(const char* const path); // all modules in directory
		bool LoadCleoModules(); // all in cleo\cleo_modules

		bool Reload(); // reload already loaded modules. Not safe if any module code is currently in exectuion!

	private:
		static void NormalizePath(std::string& path);

		class CModule
		{
			struct ModuleExport
			{
				std::string name;
				int offset = 0; // address within module's data

				void Clear();
				bool LoadFromFile(std::ifstream& file);

				static void NormalizeName(std::string& name);
			};

			std::string filepath;
			std::vector<char> data;
			std::map<std::string, ModuleExport> exports;

		public:
			void Clear();
			const char* GetFilepath() const;
			bool LoadFromFile(const char* path);
			const ScriptDataRef GetExport(const char* name);
		};

		std::map<std::string, CModule> modules;
	};
}

