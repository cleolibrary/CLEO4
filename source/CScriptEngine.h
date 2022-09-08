#pragma once
#include "CCodeInjector.h"
#include "CCustomOpcodeSystem.h"

namespace CLEO
{
    const char cleo_dir[] = "./cleo";
    const char cs_mask[] = "./*.cs";
    const char cs4_mask[] = "./*.cs4";
    const char cs3_mask[] = "./*.cs3";

    class CCustomScript : public CRunningScript
    {
        friend class CScriptEngine;
        friend struct ScmFunction;
        friend struct ThreadSavingInfo;

        DWORD dwChecksum;
        bool bSaveEnabled;
        bool bOK;
        DWORD LastSearchPed, LastSearchCar, LastSearchObj;
        CLEO_Version CompatVer;
        BYTE UseTextCommands;
        int NumDraws;
        int NumTexts;
		CCustomScript *parentThread;
		std::list<CCustomScript*> childThreads;
        std::list<RwTexture*> script_textures;
        std::vector<BYTE> script_draws;
        std::vector<BYTE> script_texts;

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
        inline WORD GetScmFunction() { return MemRead<WORD>(reinterpret_cast<BYTE*>(this) + 0xDD); }
        inline void SetScmFunction(WORD id) { MemWrite<WORD>(reinterpret_cast<BYTE*>(this) + 0xDD, id); }
        inline void SetNotFlag(bool b) { NotFlag = b; }
        inline char GetNotFlag() { return NotFlag; }
        inline void IsCustom(bool b) { MemWrite<BYTE>(reinterpret_cast<BYTE*>(this) + 0xDF, b); }
        inline bool IsCustom() { return MemRead<bool>(reinterpret_cast<BYTE*>(this) + 0xDF); }
        inline bool IsOK() { return bOK; }
        inline void enable_saving(bool en = true) { bSaveEnabled = en; }
        inline void SetCompatibility(CLEO_Version ver) { CompatVer = ver; }
        inline CLEO_Version GetCompatibility() { return CompatVer; }
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
    };

    class CScriptEngine : VInjectible
    {
        friend class CCustomScript;
        std::list<CCustomScript *> CustomScripts;
        std::list<CCustomScript *> ScriptsWaitingForDelete;
        std::set<unsigned long> InactiveScriptHashes;
        CCustomScript *CustomMission;

        CCustomScript			*	LoadScript(const char *szFilePath);

    public:
        static SCRIPT_VAR			CleoVariables[0x400];
        inline CCustomScript		*	GetCustomMission() { return CustomMission; }
        void							LoadCustomScripts(bool bMode = false);
        void							SaveState();
        CRunningScript			*	FindScriptNamed(const char *);
        CCustomScript			*	FindCustomScriptNamed(const char*);
        void							AddCustomScript(CCustomScript*);
        void							RemoveCustomScript(CCustomScript*);
        void							RemoveAllCustomScripts();
        void							UnregisterAllScripts();
        void							ReregisterAllScripts();
        inline size_t				WorkingScriptsCount() { return CustomScripts.size(); }
        virtual void					Inject(CCodeInjector&);

        CScriptEngine()
        {
            CustomMission = nullptr;
        }

        ~CScriptEngine()
        {
            TRACE("Unloading scripts...");
            RemoveAllCustomScripts();
        }

        void DrawScriptStuff(char bBeforeFade);
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

