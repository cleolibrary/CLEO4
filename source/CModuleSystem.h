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

		// registers module reference. Needs to be released with ReleaseModuleRef
		const ScriptDataRef GetExport(const char* moduleName, const char* exportName);

		bool LoadFile(const char* const path); // single file
		bool LoadDirectory(const char* const path); // all modules in directory
		bool LoadCleoModules(); // all in cleo\cleo_modules

		// marking modules usage
		void AddModuleRef(const char* baseIP);
		void ReleaseModuleRef(const char* baseIP);

		bool Reload(); // reload already loaded modules that are not currently in use

	private:
		static void NormalizePath(std::string& path);

		class CModule
		{
			friend class CModuleSystem;

			struct ModuleExport
			{
				std::string name;
				int offset = 0; // address within module's data

				void Clear();
				bool LoadFromFile(std::ifstream& file);

				static void NormalizeName(std::string& name);
			};

			int refCount = 0;
			std::string filepath;
			std::vector<char> data;
			std::map<std::string, ModuleExport> exports;

		public:
			void Clear();
			const char* GetFilepath() const;
			bool LoadFromFile(const char* path);
			bool Reload();
			const ScriptDataRef GetExport(const char* name);
		};

		std::map<std::string, CModule> modules;
	};
}

