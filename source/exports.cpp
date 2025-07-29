#include "stdafx.h"
#include "CleoBase.h"
#include "ScmFunction.h"

namespace CLEO
{
    extern "C"
    {
        DWORD WINAPI CLEO_GetVersion()
        {
            return CLEO_VERSION;
        }

        LPCSTR WINAPI CLEO_GetVersionStr()
        {
            return CLEO_VERSION_STR;
        }

        eGameVersion WINAPI CLEO_GetGameVersion()
        {
            return DetermineGameVersion();
        }

        BOOL WINAPI CLEO_RegisterOpcode(WORD opcode, CustomOpcodeHandler callback)
        {
            return CCustomOpcodeSystem::RegisterOpcode(opcode, callback);
        }

        BOOL WINAPI CLEO_RegisterCommand(const char* commandName, CustomOpcodeHandler callback)
        {
            WORD opcode = CleoInstance.OpcodeInfoDb.GetOpcode(commandName);
            if (opcode == 0xFFFF)
            {
                LOG_WARNING(0, "Failed to register opcode [%s]! Command name not found in the database.", commandName);
                return false;
            }

            return CCustomOpcodeSystem::RegisterOpcode(opcode, callback);
        }

        OpcodeResult WINAPI CLEO_CallNativeOpcode(CRunningScript* script, WORD opcode)
        {
            return CCustomOpcodeSystem::CallNativeOpcode(script, opcode);
        }

        void WINAPI CLEO_RegisterCallback(eCallbackId id, void* func)
        {
            CleoInstance.AddCallback(id, func);
        }

        void WINAPI CLEO_UnregisterCallback(eCallbackId id, void* func)
        {
            CleoInstance.RemoveCallback(id, func);
        }

        void WINAPI CLEO_AddScriptDeleteDelegate(FuncScriptDeleteDelegateT func)
        {
            CleoInstance.OpcodeSystem.scriptDeleteDelegate += func;
        }

        void WINAPI CLEO_RemoveScriptDeleteDelegate(FuncScriptDeleteDelegateT func)
        {
            CleoInstance.OpcodeSystem.scriptDeleteDelegate -= func;
        }

        BOOL WINAPI CLEO_IsScriptRunning(const CLEO::CRunningScript* thread)
        {
            return CleoInstance.ScriptEngine.IsActiveScriptPtr(thread);
        }

        void WINAPI CLEO_GetScriptInfoStr(CLEO::CRunningScript* thread, bool currLineInfo, char* buf, DWORD bufSize)
        {
            if (thread == nullptr || buf == nullptr || bufSize < 2)
            {
                return; // invalid param
            }

            auto text = reinterpret_cast<CCustomScript*>(thread)->GetInfoStr(currLineInfo);

            if (text.length() >= bufSize) text.resize(bufSize - 1); // and terminator character

            std::memcpy(buf, text.c_str(), text.length() + 1); // with terminator
        }

        void WINAPI CLEO_GetScriptParamInfoStr(int idexOffset, char* buf, DWORD bufSize)
        {
            auto curr = idexOffset - 1 + CleoInstance.OpcodeSystem.handledParamCount;
            auto name = CleoInstance.OpcodeInfoDb.GetArgumentName(CleoInstance.OpcodeSystem.lastOpcode, curr);

            curr++; // 1-based argument index display

            std::string msg;
            if (name != nullptr)
                msg = StringPrintf("#%d \"%s\"", curr, name);
            else
                msg = StringPrintf("#%d", curr);

            strncpy_s(buf, bufSize, msg.c_str(), bufSize);
        }

        DWORD WINAPI CLEO_GetScriptBaseRelativeOffset(const CRunningScript* script, const BYTE* codePos)
        {
            auto base = (BYTE*)script->BaseIP;
            if (base == 0) base = CleoInstance.ScriptEngine.scmBlock;

            if (script->IsMission() && !script->IsCustom())
            {
                if (codePos >= CleoInstance.ScriptEngine.missionBlock)
                {
                    // we are in mission code buffer
                    // native missions are loaded from script file into mission block area

                    // add offset of this mission within source script file
                    codePos += ((DWORD*)CTheScripts::MultiScriptArray)[CleoInstance.ScriptEngine.missionIndex];
                }
                else
                {
                    base = CleoInstance.ScriptEngine.scmBlock; // seems that mission uses main scm code
                }
            }

            return codePos - base;
        }

        eCLEO_Version WINAPI CLEO_GetScriptVersion(const CRunningScript* thread)
        {
            if (thread->IsCustom())
                return reinterpret_cast<const CCustomScript*>(thread)->GetCompatibility();
            else
                return CleoInstance.ScriptEngine.NativeScriptsVersion;
        }

        void WINAPI CLEO_SetScriptVersion(CRunningScript* thread, eCLEO_Version version)
        {
            if (thread->IsCustom())
                ((CCustomScript*)thread)->SetCompatibility(version);
            else
                CleoInstance.ScriptEngine.NativeScriptsVersion = version;
        }

        LPCSTR WINAPI CLEO_GetScriptFilename(const CRunningScript* thread)
        {
            if (!CleoInstance.ScriptEngine.IsValidScriptPtr(thread))
            {
                return nullptr;
            }

            auto cs = (CCustomScript*)thread;
            return cs->GetScriptFileName();
        }

        LPCSTR WINAPI CLEO_GetScriptWorkDir(const CRunningScript* thread)
        {
            auto cs = (CCustomScript*)thread;
            return cs->GetWorkDir();
        }

        void WINAPI CLEO_SetScriptWorkDir(CRunningScript* thread, const char* path)
        {
            auto cs = (CCustomScript*)thread;
            cs->SetWorkDir(path);
        }

        DWORD WINAPI CLEO_GetCleoCallStackSize(CRunningScript* thread)
        {
            ScmFunction* scmFunc = ScmFunction::Get(thread->GetScmFunction());
            return scmFunc ? 1 + scmFunc->GetCallStackSize() : 0;
        }

        void WINAPI CLEO_SetThreadCondResult(CLEO::CRunningScript* thread, BOOL result)
        {
            ((::CRunningScript*)thread)->UpdateCompareFlag(result != FALSE); // CRunningScript from Plugin SDK
        }

        void WINAPI CLEO_ThreadJumpAtLabelPtr(CLEO::CRunningScript* thread, int offset)
        {
            thread->SetIp(offset < 0 ? thread->GetBasePointer() - offset : CleoInstance.ScriptEngine.scmBlock + offset);
        }

        void WINAPI CLEO_TerminateScript(CLEO::CRunningScript* thread)
        {
            CleoInstance.ScriptEngine.RemoveScript(thread);
        }

        int WINAPI CLEO_GetOperandType(const CLEO::CRunningScript* thread)
        {
            return (int)thread->PeekDataType();
        }

        DWORD WINAPI CLEO_GetVarArgCount(CLEO::CRunningScript* thread)
        {
            return GetVarArgCount(thread);
        }

        SCRIPT_VAR* opcodeParams;
        SCRIPT_VAR* missionLocals;
        CRunningScript* staticThreads;

        BYTE* WINAPI CLEO_GetScmMainData()
        {
            return CleoInstance.ScriptEngine.scmBlock;
        }

        SCRIPT_VAR* WINAPI CLEO_GetOpcodeParamsArray()
        {
            return CLEO::opcodeParams;
        }

        BYTE WINAPI CLEO_GetParamsHandledCount()
        {
            return CleoInstance.OpcodeSystem.handledParamCount;
        }

        SCRIPT_VAR* WINAPI CLEO_GetPointerToScriptVariable(CLEO::CRunningScript* thread)
        {
            return CScriptEngine::GetScriptParamPointer(thread);
        }

        void WINAPI CLEO_RetrieveOpcodeParams(CLEO::CRunningScript* thread, int count)
        {
            CScriptEngine::GetScriptParams(thread, count);
        }

        DWORD WINAPI CLEO_GetIntOpcodeParam(CLEO::CRunningScript* thread)
        {
            CScriptEngine::GetScriptParams(thread, 1);
            return opcodeParams[0].dwParam;
        }

        float WINAPI CLEO_GetFloatOpcodeParam(CLEO::CRunningScript* thread)
        {
            CScriptEngine::GetScriptParams(thread, 1);
            return opcodeParams[0].fParam;
        }

        LPCSTR WINAPI CLEO_ReadStringOpcodeParam(CLEO::CRunningScript* thread, char* buff, int buffSize)
        {
            static char internal_buff[MAX_STR_LEN + 1]; // and terminator
            if (!buff)
            {
                buff = internal_buff;
                // allow user's length limit
                buffSize = (buffSize > 0) ? std::min<int>(buffSize, sizeof(internal_buff)) : sizeof(internal_buff);
            }

            auto result = ReadStringParam(thread, buff, buffSize);
            return (result != nullptr) ? buff : nullptr;
        }

        LPCSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CLEO::CRunningScript* thread, char* buff, int buffSize)
        {
            static char internal_buff[MAX_STR_LEN + 1]; // and terminator
            bool userBuffer = buff != nullptr;
            if (!userBuffer)
            {
                buff = internal_buff;
                // allow user's length limit
                buffSize = (buffSize > 0) ? std::min<int>(buffSize, sizeof(internal_buff)) : sizeof(internal_buff);
            }

            return ReadStringParam(thread, buff, buffSize);
        }

        void WINAPI CLEO_ReadStringParamWriteBuffer(
            CLEO::CRunningScript* thread, char** outBuf, int* outBufSize, BOOL* outNeedsTerminator
        )
        {
            if (thread == nullptr || outBuf == nullptr || outBufSize == nullptr || outNeedsTerminator == nullptr)
            {
                LOG_WARNING(
                    thread, "Invalid argument of CLEO_ReadStringParamWriteBuffer in script %s",
                    ((CCustomScript*)thread)->GetInfoStr().c_str()
                );
                return;
            }

            auto target         = GetStringParamWriteBuffer(thread);
            *outBuf             = target.data;
            *outBufSize         = target.size;
            *outNeedsTerminator = target.needTerminator;
        }

        char* WINAPI CLEO_ReadParamsFormatted(CLEO::CRunningScript* thread, const char* format, char* buf, int bufSize)
        {
            static char internal_buf[MAX_STR_LEN * 4];
            if (!buf)
            {
                buf     = internal_buf;
                bufSize = sizeof(internal_buf);
            }
            if (!bufSize) bufSize = MAX_STR_LEN;

            if (ReadFormattedString(thread, buf, bufSize, format) == -1) // error?
            {
                return nullptr; // error
            }

            return buf;
        }

        DWORD WINAPI CLEO_PeekIntOpcodeParam(CLEO::CRunningScript* thread)
        {
            // store state
            auto param = CLEO::opcodeParams[0];
            auto ip    = thread->CurrentIP;
            auto count = CleoInstance.OpcodeSystem.handledParamCount;

            CScriptEngine::GetScriptParams(thread, 1);
            DWORD result = CLEO::opcodeParams[0].dwParam;

            // restore state
            thread->CurrentIP                           = ip;
            CleoInstance.OpcodeSystem.handledParamCount = count;
            CLEO::opcodeParams[0]                       = param;

            return result;
        }

        float WINAPI CLEO_PeekFloatOpcodeParam(CLEO::CRunningScript* thread)
        {
            // store state
            auto param = CLEO::opcodeParams[0];
            auto ip    = thread->CurrentIP;
            auto count = CleoInstance.OpcodeSystem.handledParamCount;

            CScriptEngine::GetScriptParams(thread, 1);
            float result = CLEO::opcodeParams[0].fParam;

            // restore state
            thread->CurrentIP                           = ip;
            CleoInstance.OpcodeSystem.handledParamCount = count;
            CLEO::opcodeParams[0]                       = param;

            return result;
        }

        SCRIPT_VAR* WINAPI CLEO_PeekPointerToScriptVariable(CLEO::CRunningScript* thread)
        {
            // store state
            auto ip    = thread->CurrentIP;
            auto count = CleoInstance.OpcodeSystem.handledParamCount;

            auto result = CScriptEngine::GetScriptParamPointer(thread);

            // restore state
            thread->CurrentIP                           = ip;
            CleoInstance.OpcodeSystem.handledParamCount = count;

            return result;
        }

        void WINAPI CLEO_SkipOpcodeParams(CLEO::CRunningScript* thread, int count)
        {
            if (count < 1) return;

            for (int i = 0; i < count; i++)
            {
                switch (thread->ReadDataType())
                {
                case DT_VAR:
                case DT_LVAR:
                case DT_VAR_STRING:
                case DT_LVAR_STRING:
                case DT_VAR_TEXTLABEL:
                case DT_LVAR_TEXTLABEL:
                    thread->IncPtr(2);
                    break;
                case DT_VAR_ARRAY:
                case DT_LVAR_ARRAY:
                case DT_VAR_TEXTLABEL_ARRAY:
                case DT_LVAR_TEXTLABEL_ARRAY:
                case DT_VAR_STRING_ARRAY:
                case DT_LVAR_STRING_ARRAY:
                    thread->IncPtr(6);
                    break;
                case DT_BYTE:
                    // case DT_END: // should be only skipped with var args dediacated functions
                    thread->IncPtr();
                    break;
                case DT_WORD:
                    thread->IncPtr(2);
                    break;
                case DT_DWORD:
                case DT_FLOAT:
                    thread->IncPtr(4);
                    break;
                case DT_VARLEN_STRING:
                    thread->IncPtr((int)1 + *thread->GetBytePointer()); // as unsigned! length byte + string data
                    break;

                case DT_TEXTLABEL:
                    thread->IncPtr(8);
                    break;
                case DT_STRING:
                    thread->IncPtr(16);
                    break;
                }
            }

            CleoInstance.OpcodeSystem.handledParamCount += count;
        }

        void WINAPI CLEO_SkipUnusedVarArgs(CLEO::CRunningScript* thread)
        {
            SkipUnusedVarArgs(thread);
        }

        void WINAPI CLEO_RecordOpcodeParams(CLEO::CRunningScript* thread, int count)
        {
            CScriptEngine::SetScriptParams(thread, count);
        }

        void WINAPI CLEO_SetIntOpcodeParam(CLEO::CRunningScript* thread, DWORD value)
        {
            CLEO::opcodeParams[0].dwParam = value;
            CScriptEngine::SetScriptParams(thread, 1);
        }

        void WINAPI CLEO_SetFloatOpcodeParam(CLEO::CRunningScript* thread, float value)
        {
            CLEO::opcodeParams[0].fParam = value;
            CScriptEngine::SetScriptParams(thread, 1);
        }

        void WINAPI CLEO_WriteStringOpcodeParam(CLEO::CRunningScript* thread, const char* str)
        {
            if (!WriteStringParam(thread, str))
                LOG_WARNING(
                    thread, "%s in script %s", CCustomOpcodeSystem::lastErrorMsg.c_str(),
                    ((CCustomScript*)thread)->GetInfoStr().c_str()
                );
        }

        BOOL WINAPI CLEO_GetScriptDebugMode(const CLEO::CRunningScript* thread)
        {
            return reinterpret_cast<const CCustomScript*>(thread)->GetDebugMode();
        }

        void WINAPI CLEO_SetScriptDebugMode(CLEO::CRunningScript* thread, BOOL enabled)
        {
            reinterpret_cast<CCustomScript*>(thread)->SetDebugMode(enabled);
        }

        CLEO::CRunningScript* WINAPI
        CLEO_CreateCustomScript(CLEO::CRunningScript* fromThread, const char* filePath, int label)
        {
            return (CLEO::CRunningScript*)CleoInstance.ScriptEngine.CreateCustomScript(fromThread, filePath, label);
        }

        CLEO::CRunningScript* WINAPI CLEO_GetLastCreatedCustomScript()
        {
            return CleoInstance.ScriptEngine.LastScriptCreated;
        }

        CLEO::CRunningScript* WINAPI
        CLEO_GetScriptByName(const char* threadName, BOOL standardScripts, BOOL customScripts, DWORD resultIndex)
        {
            return CleoInstance.ScriptEngine.FindScriptNamed(threadName, standardScripts, customScripts, resultIndex);
        }

        CLEO::CRunningScript* WINAPI CLEO_GetScriptByFilename(const char* path, DWORD resultIndex)
        {
            return CleoInstance.ScriptEngine.FindScriptByFilename(path, resultIndex);
        }

        DWORD WINAPI CLEO_GetScriptTextureById(CLEO::CRunningScript* thread, int id)
        {
            HMODULE textPlugin = GetModuleHandleA("SA.Text.cleo");
            if (textPlugin == nullptr)
            {
                return (DWORD) nullptr;
            }

            auto GetScriptTexture =
                (RwTexture * (__cdecl*)(CLEO::CRunningScript*, DWORD)) GetProcAddress(textPlugin, "GetScriptTexture");
            if (GetScriptTexture == nullptr)
            {
                return (DWORD) nullptr;
            }

            return (DWORD)GetScriptTexture(thread, id);
        }

        DWORD WINAPI CLEO_GetInternalAudioStream(CLEO::CRunningScript* unused, DWORD audioStreamPtr)
        {
            return *(DWORD*)(audioStreamPtr + 0x4); // CAudioStream->streamInternal
        }

        // void WINAPI CLEO_StringListFree(StringList list)
        // void WINAPI CLEO_ResolvePath(CLEO::CRunningScript* thread, char* inOutPath, DWORD pathMaxLen)
        // StringList WINAPI CLEO_ListDirectory(CLEO::CRunningScript* thread, const char* searchPath, BOOL listDirs,
        // BOOL listFiles) defined in CleoBase.h

        LPCSTR WINAPI CLEO_GetGameDirectory()
        {
            return Filepath_Game.c_str();
        }

        LPCSTR WINAPI CLEO_GetUserDirectory()
        {
            return Filepath_User.c_str();
        }

        void WINAPI CLEO_Log(eLogLevel level, const char* msg)
        {
            Debug.Trace(level, msg);
        }
    }
} // namespace CLEO