#pragma once
#include "..\cleo_sdk\CLEO.h"
#include "..\cleo_sdk\CLEO_Utils.h"
#include <CTheScripts.h>

namespace CLEO
{
    // check for extra SCM data at the end of script block
    static DWORD GetExtraInfoSize(BYTE* scriptData, DWORD size)
    {
        static constexpr char SB_Footer_Sig[] = {'_', '_', 'S', 'B', 'F', 'T', 'R', '\0'};

        if (size < (sizeof(SB_Footer_Sig) + sizeof(DWORD))) return 0; // not enough data for signature + size

        auto ptr = scriptData + size; // data end
        ptr -= sizeof(SB_Footer_Sig);
        if (memcmp(ptr, &SB_Footer_Sig, sizeof(SB_Footer_Sig))) return 0; // signature not present

        ptr -= sizeof(DWORD);
        auto codeSize = *reinterpret_cast<DWORD*>(ptr);
        if (codeSize > size) return 0; // error: reported size greater than script block

        return size - codeSize;
    }

    // get pointer to arbitrary global or local variable
    static SCRIPT_VAR* GetScriptVar(CLEO::CRunningScript* script, bool global, size_t index)
    {
        SCRIPT_VAR* vars;
        if (global)
            vars = (SCRIPT_VAR*)CLEO_GetScmMainData();
        else
            vars = script->IsMission() ? (SCRIPT_VAR*)CTheScripts::LocalVariablesForCurrentMission : script->LocalVar;

        return &vars[index];
    }

    struct ScriptParamInfo
    {
        eDataType type       = eDataType::DT_INVALID;
        eArrayType arrayType = eArrayType::AT_NONE;
        WORD varIndex        = 0;
        WORD arrayIndexVar   = 0; // index of variable storing array index
        WORD arraySize       = 0;
        WORD arrayFlags      = 0;

        SCRIPT_VAR value = {0};
        size_t stringLen = 0;

        ScriptParamInfo() = default;

        // Create from script
        // Advances instruction pointer forward
        // Ommits all param processing functions/hooks
        ScriptParamInfo(CLEO::CRunningScript* script)
        {
            // type
            type      = script->PeekDataType();
            arrayType = script->PeekArrayType();
            script->IncPtr(sizeof(eDataType));

            // type properties
            if (arrayType != eArrayType::AT_NONE)
            {
                varIndex      = script->ReadVarIndex();
                arrayIndexVar = script->ReadArrayIndexVarIndex();
                arraySize     = script->ReadArraySize();
                arrayFlags    = script->ReadArrayFlags();

                if (arrayFlags & ATF_INDEX_GLOBAL) arrayIndexVar /= 4; // stored as byte offset
            }
            else if (IsVariable(type) || IsVarString(type))
            {
                varIndex = script->ReadVarIndex();
            }

            // value
            size_t varStride; // num of variable slots
            switch (type)
            {
            case DT_VAR_TEXTLABEL:
            case DT_VAR_TEXTLABEL_ARRAY:
            case DT_LVAR_TEXTLABEL:
            case DT_LVAR_TEXTLABEL_ARRAY:
                varStride = 2;
                break;

            case DT_VAR_STRING:
            case DT_VAR_STRING_ARRAY:
            case DT_LVAR_STRING:
            case DT_LVAR_STRING_ARRAY:
                varStride = 4;
                break;

            case DT_VAR:
            case DT_VAR_ARRAY:
            case DT_LVAR:
            case DT_LVAR_ARRAY:
                varStride = 1;
                break;

            default:
                varStride = 0;
            }

            bool isGlobalVar = false;
            switch (type)
            {
            // immediate numbers
            case DT_BYTE:
                value.dwParam = script->ReadByte();
                break;
            case DT_WORD:
                value.dwParam = script->ReadWord();
                break;
            case DT_DWORD:
                value.dwParam = script->ReadDword();
                break;
            case DT_FLOAT:
                value.dwParam = script->ReadDword();
                break; // 4 bytes

            // immediate texts
            case DT_TEXTLABEL:
                value.pParam = script->CurrentIP;
                stringLen    = strnlen(value.pcParam, 8);
                script->IncPtr(8);
                break;

            case DT_STRING:
                value.pParam = script->CurrentIP;
                stringLen    = strnlen(value.pcParam, 16);
                script->IncPtr(16);
                break;

            case DT_VARLEN_STRING:
                stringLen    = script->ReadByte();
                value.pParam = script->CurrentIP;
                script->IncPtr(stringLen);
                break;

            // global variables
            case DT_VAR:
            case DT_VAR_ARRAY:
            case DT_VAR_TEXTLABEL:
            case DT_VAR_TEXTLABEL_ARRAY:
            case DT_VAR_STRING:
            case DT_VAR_STRING_ARRAY:
                isGlobalVar = true;
                varIndex /= 4; // stored as byte offset
                [[fallthrough]];

            // local variables
            case DT_LVAR:
            case DT_LVAR_ARRAY:
            case DT_LVAR_TEXTLABEL:
            case DT_LVAR_TEXTLABEL_ARRAY:
            case DT_LVAR_STRING:
            case DT_LVAR_STRING_ARRAY: {
                SCRIPT_VAR* var;
                if (arrayType == AT_NONE)
                {
                    var = GetScriptVar(script, isGlobalVar, varIndex);
                }
                else
                {
                    auto idxVar = GetScriptVar(script, arrayFlags & ATF_INDEX_GLOBAL, arrayIndexVar);
                    var         = GetScriptVar(script, isGlobalVar, varIndex + idxVar->nParam * varStride);
                }

                if (IsVarString(type))
                {
                    value.pParam = var; // store pointer to variable contents
                    stringLen    = strnlen(value.pcParam, varStride * sizeof(SCRIPT_VAR));
                }
                else
                    value = *var;
            }
            break;
            }
        }

        ~ScriptParamInfo() = default;

        // generalized data type
        eDataType GetBaseType() const
        {
            switch (type)
            {
            case DT_BYTE:
            case DT_WORD:
            case DT_DWORD:
                return DT_DWORD; // int

            case DT_FLOAT:
                return DT_FLOAT; // float

            case DT_VAR:
            case DT_LVAR:
                return DT_VAR; // any 32 bit

            case DT_VAR_ARRAY:
            case DT_LVAR_ARRAY:
                if (arrayType == AT_INT)
                    return DT_DWORD; // int
                else if (arrayType == AT_FLOAT)
                    return DT_FLOAT; // float
                else
                    return DT_VAR; // unknown

            case DT_TEXTLABEL:
            case DT_VAR_TEXTLABEL:
            case DT_LVAR_TEXTLABEL:
            case DT_VAR_TEXTLABEL_ARRAY:
            case DT_LVAR_TEXTLABEL_ARRAY:
                return DT_TEXTLABEL; // 8 char string

            case DT_STRING:
            case DT_VAR_STRING:
            case DT_LVAR_STRING:
            case DT_VAR_STRING_ARRAY:
            case DT_LVAR_STRING_ARRAY:
                return DT_STRING; // 16 char string

            default:
                return type;
            }
        }

        std::string_view GetText() const
        {
            if (IsImmString(type))
            {
                return std::string_view(value.pcParam, stringLen);
            }

            if (IsVarString(type))
            {
                if (value.pParam == nullptr)
                    return "NULL_PTR";
                else if (value.dwParam < MinValidAddress)
                    return "INVALID_PTR";
                else
                    return {value.pcParam, strnlen(value.pcParam, 255)};
            }

            return "INVALID_TYPE"; // not a string type param
        }

        std::string ValueToString()
        {
            std::string result;
            ValueToString(result);
            return result;
        }

        // append to given string
        void ValueToString(std::string& dest)
        {
            auto baseType = GetBaseType();

            switch (baseType)
            {
            case DT_DWORD: // integers
            case DT_VAR:   // unspecified 32 bit
                if (abs(value.nParam) >= MinValidAddress)
                {
                    dest += "0x";
                    StringAppendHex(dest, value.nParam);
                }
                else
                    StringAppendNum(dest, value.nParam);
                break;

            // float
            case DT_FLOAT:
                StringAppendFloat(dest, value.fParam);
                break;

            // short string
            case DT_TEXTLABEL:
                dest += '\'';
                dest += GetText();
                dest += '\'';
                break;

            // long string
            case DT_STRING:
                dest += '\"';
                dest += GetText();
                dest += '\"';
                break;
            }
        }
    };
} // namespace CLEO
