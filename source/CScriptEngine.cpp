#include "stdafx.h"
#include "CleoBase.h"

namespace CLEO
{
    const char* __fastcall GetScriptStringParam(CRunningScript* thread, int dummy, char* buff, int buffLen)
    {
        if (buff == nullptr || buffLen < 0)
        {
            LOG_WARNING(0, "Invalid ReadStringParam input argument! Ptr: 0x%08X, Size: %d", buff, buffLen);
            CLEO_SkipOpcodeParams(thread, 1);
            return nullptr;
        }

        auto paramType = thread->PeekDataType();
        auto arrayType = thread->PeekArrayType();
        auto isVariableInt =
            IsVariable(paramType) && (arrayType == eArrayType::AT_NONE || arrayType == eArrayType::AT_INT);

        // integer address to text buffer
        if (IsImmInteger(paramType) || isVariableInt)
        {
            auto str = (char*)CLEO_PeekIntOpcodeParam(thread);
            CLEO_SkipOpcodeParams(thread, 1);

            if ((size_t)str <= MinValidAddress)
            {
                LOG_WARNING(
                    thread, "Invalid '0x%X' pointer of input string argument %s in script %s", str,
                    GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()
                );
                return nullptr; // error
            }

            auto len = std::min((int)strlen(str), buffLen);
            memcpy(buff, str, len);
            if (len < buffLen) buff[len] = '\0'; // add terminator if possible
            return str;                          // pointer to original data
        }
        else if (paramType == DT_VARLEN_STRING)
        {
            CleoInstance.OpcodeSystem.handledParamCount++;
            thread->IncPtr(1); // already processed paramType

            DWORD length = *thread->GetBytePointer(); // as unsigned byte!
            thread->IncPtr(1);                        // length info

            char* str = (char*)thread->GetBytePointer();
            thread->IncPtr(length); // text data

            memcpy(buff, str, std::min(buffLen, (int)length));
            if ((int)length < buffLen) buff[length] = '\0'; // add terminator if possible
            return buff;
        }
        else if (IsImmString(paramType))
        {
            thread->IncPtr(1); // already processed paramType
            auto str = (char*)thread->GetBytePointer();

            switch (paramType)
            {
            case DT_TEXTLABEL: {
                CleoInstance.OpcodeSystem.handledParamCount++;
                memcpy(buff, str, std::min(buffLen, 8));
                thread->IncPtr(8); // text data
                return buff;
            }

            case DT_STRING: {
                CleoInstance.OpcodeSystem.handledParamCount++;
                memcpy(buff, str, std::min(buffLen, 16));
                thread->IncPtr(16); // ext data
                return buff;
            }
            }
        }
        else if (IsVarString(paramType))
        {
            switch (paramType)
            {
            // short string variable
            case DT_VAR_TEXTLABEL:
            case DT_LVAR_TEXTLABEL:
            case DT_VAR_TEXTLABEL_ARRAY:
            case DT_LVAR_TEXTLABEL_ARRAY: {
                auto str = (char*)CScriptEngine::GetScriptParamPointer(thread);
                memcpy(buff, str, std::min(buffLen, 8));
                if (buffLen > 8) buff[8] = '\0'; // add terminator if possible
                return buff;
            }

            // long string variable
            case DT_VAR_STRING:
            case DT_LVAR_STRING:
            case DT_VAR_STRING_ARRAY:
            case DT_LVAR_STRING_ARRAY: {
                auto str = (char*)CScriptEngine::GetScriptParamPointer(thread);
                memcpy(buff, str, std::min(buffLen, 16));
                if (buffLen > 16) buff[16] = '\0'; // add terminator if possible
                return buff;
            }
            }
        }

        // unsupported param type
        LOG_WARNING(
            thread, "Argument %s expected to be string, got %s in script %s", GetParamInfo().c_str(),
            ToKindStr(paramType, arrayType), ScriptInfoStr(thread).c_str()
        );
        CLEO_SkipOpcodeParams(thread, 1); // try skip unhandled param
        return nullptr;                   // error
    }

    struct CleoSafeHeader
    {
        const static unsigned sign;
        unsigned signature;
        unsigned n_saved_threads;
        unsigned n_stopped_threads;
    };

    const unsigned CleoSafeHeader::sign = 0x31345653;

    struct ThreadSavingInfo
    {
        unsigned long hash;
        SCRIPT_VAR tls[32];
        unsigned timers[2];
        bool condResult;
        unsigned sleepTime;
        eLogicalOperation logicalOp;
        bool notFlag;
        ptrdiff_t ip_diff;
        char threadName[8];

        ThreadSavingInfo(CCustomScript* cs)
            : hash(cs->m_codeChecksum), condResult(cs->bCondResult), logicalOp(cs->LogicalOp),
              notFlag(cs->NotFlag != false), ip_diff(cs->CurrentIP - reinterpret_cast<BYTE*>(cs->BaseIP))
        {
            sleepTime =
                cs->WakeTime >= CTimer::m_snTimeInMilliseconds ? 0 : cs->WakeTime - CTimer::m_snTimeInMilliseconds;
            std::copy(cs->LocalVar, cs->LocalVar + 32, tls);
            std::copy(cs->Timers, cs->Timers + 2, timers);
            std::copy(cs->Name, cs->Name + 8, threadName);
        }

        void Apply(CCustomScript* cs)
        {
            std::copy(tls, tls + 32, cs->LocalVar);
            std::copy(timers, timers + 2, cs->Timers);
            std::copy(threadName, threadName + 8, cs->Name);
            cs->m_codeChecksum = hash;
            cs->bCondResult    = condResult;
            cs->WakeTime       = CTimer::m_snTimeInMilliseconds + sleepTime;
            cs->LogicalOp      = logicalOp;
            cs->NotFlag        = notFlag;
            cs->CurrentIP      = reinterpret_cast<BYTE*>(cs->BaseIP) + ip_diff;
            cs->EnableSaving(true);
        }

        ThreadSavingInfo() {}
    };

    SCRIPT_VAR CScriptEngine::CleoVariables[0x400];

    template <typename T> void inline ReadBinary(std::istream& s, T& buf)
    {
        s.read(reinterpret_cast<char*>(&buf), sizeof(T));
    }

    template <typename T> void inline ReadBinary(std::istream& s, T* buf, size_t size)
    {
        s.read(reinterpret_cast<char*>(buf), sizeof(T) * size);
    }

    template <typename T> void inline WriteBinary(std::ostream& s, const T& data)
    {
        s.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }

    template <typename T> void inline WriteBinary(std::ostream& s, const T* data, size_t size)
    {
        s.write(reinterpret_cast<const char*>(data), sizeof(T) * size);
    }

    void __cdecl CScriptEngine::HOOK_DrawScriptText(char beforeFade)
    {
        DrawScriptText_Orig(beforeFade);

        // run registered callbacks
        for (void* func : CleoInstance.GetCallbacks(eCallbackId::ScriptDraw))
        {
            typedef void WINAPI callback(bool);
            ((callback*)func)(beforeFade != 0);
        }
    }

    SCRIPT_VAR* CScriptEngine::GetScriptParamPointer(CRunningScript* thread)
    {
        auto type = DT_DWORD; // thread->PeekDataType(); // ignored in GetPointerToScriptVariable anyway
        auto ptr  = ((::CRunningScript*)thread)->GetPointerToScriptVariable(type);

        // TODO: hook game's GetPointerToScriptVariable so this is always incremented?
        CleoInstance.OpcodeSystem.handledParamCount++;
        return (SCRIPT_VAR*)ptr;
    }

    void CScriptEngine::GetScriptParams(CRunningScript* script, BYTE count)
    {
        ((::CRunningScript*)script)->CollectParameters(count);
        CleoInstance.OpcodeSystem.handledParamCount += count;
    }

    void CScriptEngine::SetScriptParams(CRunningScript* script, BYTE count)
    {
        ((::CRunningScript*)script)->StoreParameters(count);
        CleoInstance.OpcodeSystem.handledParamCount += count;
    }

    void CScriptEngine::DrawScriptText_Orig(char beforeFade)
    {
        if (beforeFade)
            CleoInstance.ScriptEngine.DrawScriptTextBeforeFade_Orig(beforeFade);
        else
            CleoInstance.ScriptEngine.DrawScriptTextAfterFade_Orig(beforeFade);
    }

    void __fastcall CScriptEngine::HOOK_ProcessScript(CLEO::CRunningScript* pScript)
    {
        CleoInstance.ScriptEngine.GameBegin(); // all initialized and ready to process scripts

        // run registered callbacks
        bool process = true;
        for (void* func : CleoInstance.GetCallbacks(eCallbackId::ScriptProcessBefore))
        {
            typedef bool WINAPI callback(CRunningScript*);
            process = process && ((callback*)func)(pScript);
        }

        if (process)
        {
            CleoInstance.ScriptEngine.ProcessScript_Orig(pScript);
        }

        // run registered callbacks
        for (void* func : CleoInstance.GetCallbacks(eCallbackId::ScriptProcessAfter))
        {
            typedef void WINAPI callback(CRunningScript*);
            ((callback*)func)(pScript);
        }
    }

    void __cdecl CScriptEngine::HOOK_SaveScmData()
    {
        TRACE("Saving scripts save data...");
        CleoInstance.ScriptEngine.SaveState();
        CleoInstance.ScriptEngine.UnregisterAllCustomScripts();
        CleoInstance.ScriptEngine.SaveScmData_Orig();
        CleoInstance.ScriptEngine.ReregisterAllCustomScripts();
    }

    void CScriptEngine::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting ScriptEngine: Phase 1");
        CGameVersionManager& gvm = CleoInstance.VersionManager;

        // Global Events crashfix
        // inj.MemoryWrite(0xA9AF6C, 0, 4);

        opcodeParams  = (SCRIPT_VAR*)ScriptParams; // from Plugin SDK's TheScripts.h
        missionLocals = (SCRIPT_VAR*)CTheScripts::LocalVariablesForCurrentMission;
        staticThreads = (CRunningScript*)CTheScripts::ScriptsArray;

        // Protect script dependencies
        inj.ReplaceFunction(
            HOOK_ProcessScript, gvm.TranslateMemoryAddress(MA_CALL_PROCESS_SCRIPT), &ProcessScript_Orig
        );

        inj.ReplaceFunction(
            HOOK_DrawScriptText, gvm.TranslateMemoryAddress(MA_CALL_DRAW_SCRIPT_TEXTS_AFTER_FADE),
            &DrawScriptTextAfterFade_Orig
        );
        inj.ReplaceFunction(
            HOOK_DrawScriptText, gvm.TranslateMemoryAddress(MA_CALL_DRAW_SCRIPT_TEXTS_BEFORE_FADE),
            &DrawScriptTextBeforeFade_Orig
        );

        inj.ReplaceFunction(HOOK_SaveScmData, gvm.TranslateMemoryAddress(MA_CALL_SAVE_SCM_DATA), &SaveScmData_Orig);
    }

    void CScriptEngine::InjectLate(CCodeInjector& inj)
    {
        TRACE("Injecting ScriptEngine: Phase 2");
        CGameVersionManager& gvm = CleoInstance.VersionManager;

        // limit adjusters support: get adresses from (possibly) patched references
        inj.MemoryRead(gvm.TranslateMemoryAddress(MA_SCM_BLOCK_REF), scmBlock);
        inj.MemoryRead(gvm.TranslateMemoryAddress(MA_MISSION_BLOCK_REF), missionBlock);
    }

    CScriptEngine::~CScriptEngine()
    {
        GameEnd();
    }

    CleoSafeHeader safe_header;
    ThreadSavingInfo* safe_info;
    unsigned long* stopped_info;
    std::unique_ptr<ThreadSavingInfo[]> safe_info_utilizer;
    std::unique_ptr<unsigned long[]> stopped_info_utilizer;

    void CScriptEngine::GameBegin()
    {
        auto& activeScriptsListHead =
            (CRunningScript*&)CTheScripts::pActiveScripts; // reference, but with type casted to CLEO's CRunningScript

        if (gameInProgress) return;                   // already started
        if (activeScriptsListHead == nullptr) return; // main gamescript not loaded yet
        gameInProgress = true;

        if (CGame::bMissionPackGame == 0) // regular main game
        {
            MainScriptFileDir  = Filepath_Game + "\\data\\script";
            MainScriptFileName = "main.scm";
        }
        else // mission pack
        {
            MainScriptFileDir  = Filepath_User + StringPrintf("\\MPACK\\MPACK%d", CGame::bMissionPackGame);
            MainScriptFileName = "scr.scm";
        }

        NativeScriptsDebugMode = GetPrivateProfileInt("General", "DebugMode", 0, Filepath_Config.c_str()) != 0;

        // global native scripts legacy mode
        int ver = GetPrivateProfileInt("General", "MainScmLegacyMode", 0, Filepath_Config.c_str());
        switch (ver)
        {
        case 3:
            NativeScriptsVersion = eCLEO_Version::CLEO_VER_3;
            break;
        case 4:
            NativeScriptsVersion = eCLEO_Version::CLEO_VER_4;
            break;
        default:
            NativeScriptsVersion = eCLEO_Version::CLEO_VER_CUR;
            ver                  = 0;
            break;
        }
        if (ver != 0) TRACE("Legacy mode for native scripts active: CLEO%d", ver);

        MainScriptCurWorkDir = Filepath_Game;

        CleoInstance.ModuleSystem.LoadCleoModules();
        LoadState(CleoInstance.saveSlot);

        // keep already loaded scripts at front of processing queue
        auto head = activeScriptsListHead;
        auto tail = head;
        while (tail->Next)
            tail = tail->Next;

        // load custom scripts as new list
        activeScriptsListHead = nullptr;
        LoadCustomScripts();

        // append custom scripts list to the back
        if (activeScriptsListHead != nullptr)
        {
            tail->Next                      = activeScriptsListHead;
            activeScriptsListHead->Previous = tail;
        }

        activeScriptsListHead = head; // restore original
    }

    void CScriptEngine::GameEnd()
    {
        if (!gameInProgress) return;
        gameInProgress = false;

        RemoveAllCustomScripts();
        CleoInstance.ModuleSystem.Clear();
        memset(CleoVariables, 0, sizeof(CleoVariables));
    }

    void CScriptEngine::LoadCustomScripts()
    {
        TRACE(""); // separator
        TRACE("Listing CLEO scripts:");

        std::set<std::string> found;

        auto processFileList = [&](StringList fileList) {
            for (DWORD i = 0; i < fileList.count; i++)
            {
                const auto ext = FS::path(fileList.strings[i]).extension();
                if (ext == cs_ext || ext == cs3_ext || ext == cs4_ext)
                {
                    TRACE(" %s", fileList.strings[i]);
                    found.emplace(fileList.strings[i]);
                }
            }
        };

        auto searchPattern = Filepath_Cleo + "\\*" + cs_ext;
        auto list          = CLEO_ListDirectory(nullptr, searchPattern.c_str(), false, true);
        processFileList(list);
        CLEO_StringListFree(list);

        searchPattern = Filepath_Cleo + "\\*" + cs3_ext;
        list          = CLEO_ListDirectory(nullptr, searchPattern.c_str(), false, true);
        processFileList(list);
        CLEO_StringListFree(list);

        searchPattern = Filepath_Cleo + "\\*" + cs4_ext;
        list          = CLEO_ListDirectory(nullptr, searchPattern.c_str(), false, true);
        processFileList(list);
        CLEO_StringListFree(list);

        if (!found.empty())
        {
            TRACE("Starting CLEO scripts...");

            for (const auto& path : found)
            {
                LoadScript(path.c_str());
            }
        }
        else
        {
            TRACE(" - nothing found");
        }
    }

    CCustomScript* CScriptEngine::LoadScript(const char* szFilePath)
    {
        auto cs = new CCustomScript(szFilePath);

        if (!cs || !cs->IsOk())
        {
            TRACE("Loading of custom script '%s' failed", szFilePath);
            if (cs) delete cs;
            return nullptr;
        }

        // check whether the script is in stop-list
        if (stopped_info)
        {
            for (size_t i = 0; i < safe_header.n_stopped_threads; ++i)
            {
                if (stopped_info[i] == cs->m_codeChecksum)
                {
                    TRACE("Custom script '%s' found in the stop-list", szFilePath);
                    InactiveScriptHashes.insert(stopped_info[i]);
                    delete cs;
                    return nullptr;
                }
            }
        }

        // check whether the script is in safe-list
        if (safe_info)
        {
            for (size_t i = 0; i < safe_header.n_saved_threads; ++i)
            {
                if (safe_info[i].hash == cs->GetCodeChecksum())
                {
                    TRACE("Custom script '%s' found in the safe-list", szFilePath);
                    safe_info[i].Apply(cs);
                    break;
                }
            }
        }

        AddCustomScript(cs);
        return cs;
    }

    CCustomScript* CScriptEngine::CreateCustomScript(CRunningScript* fromThread, const char* script_name, int label)
    {
        auto filename =
            reinterpret_cast<CCustomScript*>(fromThread)
                ->ResolvePath(script_name, DIR_CLEO); // legacy: default search location is game\cleo directory

        if (label != 0) // create from label
        {
            TRACE("Starting new custom script from thread named '%s' label 0x%08X", filename.c_str(), label);
        }
        else
        {
            TRACE("Starting new custom script '%s'", filename.c_str());
        }

        // if "label == 0" then "script_name" need to be the file name
        auto cs = new CCustomScript(filename.c_str(), false, fromThread, label);
        if (fromThread) fromThread->SetConditionResult(cs && cs->IsOk());
        if (cs && cs->IsOk())
        {
            AddCustomScript(cs);
            if (fromThread) ((::CRunningScript*)fromThread)->ReadParametersForNewlyStartedScript((::CRunningScript*)cs);
        }
        else
        {
            if (cs) delete cs;
            if (fromThread) SkipUnusedVarArgs(fromThread);
            LOG_WARNING(0, "Failed to load script '%s'", filename.c_str());
            return nullptr;
        }

        return cs;
    }

    void CScriptEngine::LoadState(int saveSlot)
    {
        memset(CleoVariables, 0, sizeof(CleoVariables));
        safe_info                   = nullptr;
        stopped_info                = nullptr;
        safe_header.n_saved_threads = safe_header.n_stopped_threads = 0;

        if (saveSlot == -1) return; // new game started

        auto saveFile = FS::path(Filepath_Cleo).append(StringPrintf("cleo_saves\\cs%d.sav", saveSlot)).string();

        // load cleo saving file
        try
        {
            TRACE(""); // separator
            TRACE("Loading cleo safe '%s'", saveFile.c_str());
            std::ifstream ss(saveFile.c_str(), std::ios::binary);
            if (ss.is_open())
            {
                ss.exceptions(std::ios::eofbit | std::ios::badbit | std::ios::failbit);
                ReadBinary(ss, safe_header);
                if (safe_header.signature != CleoSafeHeader::sign) throw std::runtime_error("Invalid file format");
                safe_info = new ThreadSavingInfo[safe_header.n_saved_threads];
                safe_info_utilizer.reset(safe_info);
                stopped_info = new unsigned long[safe_header.n_stopped_threads];
                stopped_info_utilizer.reset(stopped_info);
                ReadBinary(ss, CleoVariables, 0x400);
                ReadBinary(ss, safe_info, safe_header.n_saved_threads);
                ReadBinary(ss, stopped_info, safe_header.n_stopped_threads);
                for (size_t i = 0; i < safe_header.n_stopped_threads; ++i)
                    InactiveScriptHashes.insert(stopped_info[i]);
                TRACE(
                    "Finished. Loaded %u cleo variables, %u saved threads info, %u stopped threads info", 0x400,
                    safe_header.n_saved_threads, safe_header.n_stopped_threads
                );
            }
            else
            {
                memset(CleoVariables, 0, sizeof(CleoVariables));
            }
        }
        catch (std::exception& ex)
        {
            TRACE("Loading of cleo safe '%s' failed: %s", saveFile.c_str(), ex.what());
            safe_header.n_saved_threads = safe_header.n_stopped_threads = 0;
            memset(CleoVariables, 0, sizeof(CleoVariables));
        }
    }

    void CScriptEngine::SaveState()
    {
        try
        {
            std::list<CCustomScript*> savedThreads;
            std::for_each(CustomScripts.begin(), CustomScripts.end(), [this, &savedThreads](CCustomScript* cs) {
                if (cs->m_saveEnabled) savedThreads.push_back(cs);
            });

            CleoSafeHeader header = {CleoSafeHeader::sign, savedThreads.size(), InactiveScriptHashes.size()};

            char safe_name[MAX_PATH];
            sprintf_s(safe_name, "./cleo/cleo_saves/cs%d.sav", FrontEndMenuManager.m_nSelectedSaveGame);
            TRACE("Saving script engine state to the file '%s'", safe_name);

            CreateDirectory("cleo", NULL);
            CreateDirectory("cleo/cleo_saves", NULL);
            std::ofstream ss(safe_name, std::ios::binary);
            if (ss.is_open())
            {
                ss.exceptions(std::ios::failbit | std::ios::badbit);

                WriteBinary(ss, header);
                WriteBinary(ss, CleoVariables, 0x400);

                std::for_each(savedThreads.begin(), savedThreads.end(), [&savedThreads, &ss](CCustomScript* cs) {
                    ThreadSavingInfo savingInfo(cs);
                    WriteBinary(ss, savingInfo);
                });

                std::for_each(InactiveScriptHashes.begin(), InactiveScriptHashes.end(), [&ss](unsigned long hash) {
                    WriteBinary(ss, hash);
                });

                TRACE(
                    "Done. Saved %u cleo variables, %u saved threads, %u stopped threads", 0x400,
                    header.n_saved_threads, header.n_stopped_threads
                );
            }
            else
            {
                TRACE("Failed to write save file '%s'!", safe_name);
            }
        }
        catch (std::exception& ex)
        {
            TRACE("Saving failed. %s", ex.what());
        }
    }

    CRunningScript* CScriptEngine::FindScriptNamed(
        const char* threadName, bool standardScripts, bool customScripts, size_t resultIndex
    )
    {
        if (standardScripts)
        {
            for (auto script = (CRunningScript*)CTheScripts::pActiveScripts; script; script = script->GetNext())
            {
                if (script->IsCustom())
                {
                    // skip custom scripts in the queue, they are handled separately
                    continue;
                }
                if (_strnicmp(threadName, script->Name, sizeof(script->Name)) == 0)
                {
                    if (resultIndex == 0)
                        return script;
                    else
                        resultIndex--;
                }
            }
        }

        if (customScripts)
        {
            if (CustomMission)
            {
                if (_strnicmp(threadName, CustomMission->Name, sizeof(CustomMission->Name)) == 0)
                {
                    if (resultIndex == 0)
                        return CustomMission;
                    else
                        resultIndex--;
                }
            }

            for (auto it = CustomScripts.begin(); it != CustomScripts.end(); ++it)
            {
                auto cs = *it;
                if (_strnicmp(threadName, cs->Name, sizeof(cs->Name)) == 0)
                {
                    if (resultIndex == 0)
                        return cs;
                    else
                        resultIndex--;
                }
            }
        }

        return nullptr;
    }

    CRunningScript* CScriptEngine::FindScriptByFilename(const char* path, size_t resultIndex)
    {
        if (path == nullptr) return nullptr;

        auto pathLen     = strlen(path);
        auto CheckScript = [&](CRunningScript* script) {
            if (script == nullptr) return false;

            auto cs                = (CCustomScript*)script;
            std::string scriptPath = cs->GetScriptFileFullPath();

            if (scriptPath.length() < pathLen) return false;

            auto startPos = scriptPath.length() - pathLen;
            if (_strnicmp(path, scriptPath.c_str() + startPos, pathLen) == 0)
            {
                if (startPos > 0 && path[startPos - 1] != '\\') return false; // whole file/dir name must match

                return true;
            }

            return false;
        };

        // standard scripts
        for (auto script = (CRunningScript*)CTheScripts::pActiveScripts; script; script = script->GetNext())
        {
            if (CheckScript(script))
            {
                if (resultIndex == 0)
                    return script;
                else
                    resultIndex--;
            }
        }

        // custom scripts
        if (CheckScript(CustomMission))
        {
            if (resultIndex == 0)
                return CustomMission;
            else
                resultIndex--;
        }

        for (auto it = CustomScripts.begin(); it != CustomScripts.end(); ++it)
        {
            auto cs = *it;
            if (CheckScript(cs))
            {
                if (resultIndex == 0)
                    return cs;
                else
                    resultIndex--;
            }
        }

        return nullptr;
    }

    bool CScriptEngine::IsActiveScriptPtr(const CRunningScript* ptr) const
    {
        for (auto script = (CRunningScript*)CTheScripts::pActiveScripts; script != nullptr; script = script->GetNext())
        {
            if (script == ptr) return ptr->IsActive();
        }

        for (const auto script : CustomScripts)
        {
            if (script == ptr) return ptr->IsActive();
        }

        return false;
    }

    bool CScriptEngine::IsValidScriptPtr(const CRunningScript* ptr) const
    {
        for (auto script = (CRunningScript*)CTheScripts::pActiveScripts; script != nullptr; script = script->GetNext())
        {
            if (script == ptr) return true;
        }

        for (auto script = (CLEO::CRunningScript*)CTheScripts::pIdleScripts; script != nullptr;
             script      = script->GetNext())
        {
            if (script == ptr) return true;
        }

        for (const auto script : CustomScripts)
        {
            if (script == ptr) return true;
        }

        for (const auto script : ScriptsWaitingForDelete)
        {
            if (script == ptr) return true;
        }

        return false;
    }

    void CScriptEngine::AddCustomScript(CCustomScript* cs)
    {
        if (cs->IsMission())
        {
            TRACE("Registering custom mission named '%s'", cs->GetName().c_str());
            CustomMission = cs;
        }
        else
        {
            TRACE("Registering custom script named '%s'", cs->GetName().c_str());
            CustomScripts.push_back(cs);
        }

        cs->AddScriptToList((CRunningScript**)&CTheScripts::pActiveScripts);
        cs->SetActive(true);

        // run registered callbacks
        for (void* func : CleoInstance.GetCallbacks(eCallbackId::ScriptRegister))
        {
            typedef void WINAPI callback(CCustomScript*);
            ((callback*)func)(cs);
        }
    }

    void CScriptEngine::RemoveScript(CRunningScript* script)
    {
        if (script->IsMission()) CTheScripts::bAlreadyRunningAMissionScript = false;

        if (script->IsCustom())
        {
            RemoveCustomScript((CCustomScript*)script);
        }
        else // native script
        {
            auto cs = (CCustomScript*)script;
            cs->RemoveScriptFromList((CRunningScript**)&CTheScripts::pActiveScripts);
            cs->AddScriptToList((CRunningScript**)&CTheScripts::pIdleScripts);
            cs->ShutdownThisScript();
        }
    }

    void CScriptEngine::RemoveCustomScript(CCustomScript* cs)
    {
        // run registered callbacks
        for (void* func : CleoInstance.GetCallbacks(eCallbackId::ScriptUnregister))
        {
            typedef void WINAPI callback(CCustomScript*);
            ((callback*)func)(cs);
        }

        if (cs == CustomMission)
        {
            CustomMission                              = nullptr;
            CTheScripts::bAlreadyRunningAMissionScript = false; // on_mission
        }

        if (cs->m_parentScript)
        {
            cs->BaseIP = 0; // don't delete BaseIP if child thread
        }

        for (auto childThread : cs->m_childScripts)
        {
            RemoveScript(childThread);
        }

        cs->SetActive(false);
        cs->RemoveScriptFromList((CRunningScript**)&CTheScripts::pActiveScripts);
        CustomScripts.remove(cs);

        if (cs->m_saveEnabled && !cs->IsMission())
        {
            TRACE("Stopping custom script named '%s'", cs->GetName().c_str());
            InactiveScriptHashes.insert(cs->GetCodeChecksum());
        }
        else
        {
            TRACE("Unregistering custom %s named '%s'", cs->IsMission() ? "mission" : "script", cs->GetName().c_str());
            ScriptsWaitingForDelete.insert(cs);
        }
    }

    void CScriptEngine::RemoveAllCustomScripts(void)
    {
        TRACE("");
        TRACE("Unloading scripts...");

        if (CustomMission)
        {
            RemoveCustomScript(CustomMission);
        }

        while (!CustomScripts.empty())
        {
            RemoveCustomScript(CustomScripts.back());
        }

        for (auto& script : ScriptsWaitingForDelete)
        {
            TRACE(" Deleting inactive script named '%s'", script->GetName().c_str());
            delete script;
        }
        ScriptsWaitingForDelete.clear();
    }

    void CScriptEngine::UnregisterAllCustomScripts()
    {
        TRACE("Unregistering all custom scripts");
        std::for_each(CustomScripts.begin(), CustomScripts.end(), [this](CCustomScript* cs) {
            cs->RemoveScriptFromList((CRunningScript**)&CTheScripts::pActiveScripts);
            cs->SetActive(false);
        });
    }

    void CScriptEngine::ReregisterAllCustomScripts()
    {
        TRACE("Reregistering all custom scripts");
        std::for_each(CustomScripts.begin(), CustomScripts.end(), [this](CCustomScript* cs) {
            cs->AddScriptToList((CRunningScript**)&CTheScripts::pActiveScripts);
            cs->SetActive(true);
        });
    }
} // namespace CLEO
