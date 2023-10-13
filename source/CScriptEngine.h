#pragma once
#include "CCodeInjector.h"
#include "CCustomOpcodeSystem.h"

namespace CLEO
{
    const char cs_ext[] = ".cs";
    const char cs4_ext[] = ".cs4";
    const char cs3_ext[] = ".cs3";

    class CCustomScript : public CRunningScript
    {
        friend class CScriptEngine;
        friend class CCustomOpcodeSystem;
        friend struct ScmFunction;
        friend struct ThreadSavingInfo;

        DWORD dwChecksum;
        bool bSaveEnabled;
        bool bOK;
        DWORD LastSearchPed, LastSearchCar, LastSearchObj;
        eCLEO_Version CompatVer;
        BYTE UseTextCommands;
        int NumDraws;
        int NumTexts;
		CCustomScript *parentThread;
		std::list<CCustomScript*> childThreads;
        std::list<RwTexture*> script_textures;
        std::vector<BYTE> script_draws;
        std::vector<BYTE> script_texts;

        bool bDebugMode; // debug mode enabled?

        std::string scriptFileDir;
        std::string scriptFileName;
        std::string workDir;

    public:
		inline RwTexture* GetScriptTextureById(unsigned int id)
		{
			if (script_textures.size() > id)
			{
				auto it = script_textures.begin();
				std::advance(it, id);
				return *it;
			}
			return nullptr;
		}

        inline SCRIPT_VAR * GetVarsPtr() { return LocalVar; }
        inline bool IsOK() const { return bOK; }
        inline void enable_saving(bool en = true) { bSaveEnabled = en; }
        inline void SetCompatibility(eCLEO_Version ver) { CompatVer = ver; }
        inline eCLEO_Version GetCompatibility() const { return CompatVer; }
        inline DWORD& GetLastSearchPed() { return LastSearchPed; }
        inline DWORD& GetLastSearchVehicle() { return LastSearchCar; }
        inline DWORD& GetLastSearchObject() { return LastSearchObj; }
		CCustomScript(const char *szFileName, bool bIsMiss = false, CCustomScript *parent = nullptr, int label = 0);
        ~CCustomScript();

        void Process();
        void Draw(char bBeforeFade);

        void StoreScriptSpecifics();
        void RestoreScriptSpecifics();
        void StoreScriptTextures();
        void RestoreScriptTextures();
        void StoreScriptDraws();
        void RestoreScriptDraws();

        void StoreScriptCustoms();
        void RestoreScriptCustoms();

        // debug related utils enabled?
        bool GetDebugMode() const;
        void SetDebugMode(bool enabled);

        // absolute path to directory where script's source file is located
        const char* GetScriptFileDir() const;
        void SetScriptFileDir(const char* directory);

        // filename with type extension of script's source file
        const char* GetScriptFileName() const;
        void SetScriptFileName(const char* filename);

        // current working directory of this script. Can be changed ith 0A99
        const char* GetWorkDir() const;
        void SetWorkDir(const char* directory);

        // create absolute file path
        std::string ResolvePath(const char* path, const char* customWorkDir = nullptr) const;

        // get short info text about script
        std::string GetInfoStr(bool currLineInfo = true) const;
    };

    class CScriptEngine : VInjectible
    {
        friend class CCustomScript;
        std::list<CCustomScript *> CustomScripts;
        std::list<CCustomScript *> ScriptsWaitingForDelete;
        std::set<unsigned long> InactiveScriptHashes;
        CCustomScript *CustomMission;

        CCustomScript *LoadScript(const char *szFilePath);
    public:
        bool NativeScriptsDebugMode; // debug mode enabled?
        std::string MainScriptFileDir;
        std::string MainScriptFileName;
        std::string MainScriptCurWorkDir;

        static SCRIPT_VAR CleoVariables[0x400];

        CScriptEngine();
        ~CScriptEngine();
        
        virtual void Inject(CCodeInjector&);
        void Initialize(); // call after new game started
        void LoadCustomScripts(bool bMode = false);

        void SaveState();

        CRunningScript*	FindScriptNamed(const char *);
        CCustomScript*	FindCustomScriptNamed(const char*);
        void AddCustomScript(CCustomScript*);
        void RemoveCustomScript(CCustomScript*);
        void RemoveAllCustomScripts();
        void UnregisterAllScripts();
        void ReregisterAllScripts();

        void DrawScriptStuff(char bBeforeFade);

        inline CCustomScript* GetCustomMission() { return CustomMission; }
        inline size_t WorkingScriptsCount() { return CustomScripts.size(); }
    };

    extern void(__thiscall * AddScriptToQueue)(CRunningScript *, CRunningScript **queue);
    extern void(__thiscall * RemoveScriptFromQueue)(CRunningScript *, CRunningScript **queue);
    extern void(__thiscall * StopScript)(CRunningScript *);
    extern char(__thiscall * ScriptOpcodeHandler00)(CRunningScript *, WORD opcode);
    extern void(__thiscall * GetScriptParams)(CRunningScript *, int count);
    extern void(__thiscall * TransmitScriptParams)(CRunningScript *, CRunningScript *);
    extern void(__thiscall * SetScriptParams)(CRunningScript *, int count);
    extern void(__thiscall * SetScriptCondResult)(CRunningScript *, bool);
    extern SCRIPT_VAR * (__thiscall * GetScriptParamPointer1)(CRunningScript *);
    extern void(__thiscall * GetScriptStringParam)(CRunningScript *, char* buf, BYTE len);
    extern SCRIPT_VAR * (__thiscall * GetScriptParamPointer2)(CRunningScript *, int __unused__);

    inline SCRIPT_VAR * GetScriptParamPointer(CRunningScript *thread)
    {
        SCRIPT_VAR* ptr = GetScriptParamPointer2(thread, 0);
        return ptr;
    }

    extern "C" {
        extern SCRIPT_VAR *opcodeParams;
        extern SCRIPT_VAR *missionLocals;
        extern CRunningScript *staticThreads;
    }

    extern BYTE *scmBlock, *missionBlock;
    extern CCustomScript *lastScriptCreated;

	extern float VectorSqrMagnitude(CVector vector);
}

