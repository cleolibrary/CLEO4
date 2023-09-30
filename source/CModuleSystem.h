#pragma once
#include <atomic>
#include <filesystem>
#include <map>
#include <mutex>
#include <thread>

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
		const ScriptDataRef GetExport(std::string modulePath, std::string_view exportName);

		bool LoadFile(const char* const path); // single file
		bool LoadDirectory(const char* const path); // all modules in directory
		bool LoadCleoModules(); // all in cleo\cleo_modules

		// marking modules usage
		void AddModuleRef(const char* baseIP);
		void ReleaseModuleRef(const char* baseIP);

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

			std::string filepath; // source file
			std::vector<char> data;
			std::map<std::string, ModuleExport> exports;

			// hot reloading when source file modified
			std::atomic<int> refCount = 0;
			std::filesystem::file_time_type fileTime; // last write time of source file
			void Update();
			std::atomic<bool> updateActive = true;
			std::atomic<bool> updateNeeded = false;
			std::mutex updateMutex;
			std::thread updateThread;

		public:
			CModule();
			~CModule();

			void Clear();
			const char* GetFilepath() const;
			bool LoadFromFile(const char* path);
			const ScriptDataRef GetExport(std::string name);
		};

		std::map<std::string, CModule> modules;
	};
}

