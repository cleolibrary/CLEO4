#include "ScriptLog.h"
#include "CLEO_Utils.h"
#include "crc32.h"       // from CLEO core
#include "ScriptUtils.h" // from CLEO core
#include <CCheat.h>
#include <CGame.h>
#include <CHud.h>
#include <CMenuManager.h>
#include <CRunningScript.h>
#include <CTheScripts.h>
#include <CTimer.h>
#include <memory>
#include <string>

using namespace CLEO;

ScriptLog* ScriptLog::g_Instance = nullptr;

ScriptLog::ScriptLog()
{
    assert(g_Instance == nullptr);
    g_Instance = this;

    m_logFilePath = CLEO_GetGameDirectory();
    m_logFilePath += "\\cleo\\.cleo_script_log.xml";
    LogFileDelete();

    CLEO_RegisterCallback(eCallbackId::GameBegin, callbackGameBegin);
    CLEO_RegisterCallback(eCallbackId::GameProcessBefore, callbackGameProcessBefore);
    CLEO_RegisterCallback(eCallbackId::GameProcessAfter, callbackGameProcessAfter);
    CLEO_RegisterCallback(eCallbackId::ScriptProcessBefore, callbackScriptProcessBefore);
    CLEO_RegisterCallback(eCallbackId::ScriptProcessAfter, callbackScriptProcessAfter);
    CLEO_RegisterCallback(eCallbackId::ScriptOpcodeProcessBefore, callbackScriptOpcodeProcessBefore);
    CLEO_RegisterCallback(eCallbackId::ScriptOpcodeProcessAfter, callbackScriptOpcodeProcessAfter);
    CLEO_RegisterCallback(eCallbackId::DrawingFinished, callbackDrawingFinished);
    CLEO_RegisterCallback(eCallbackId::MainWindowFocus, callbackMainWindowFocus);

    LoadConfig();

    std::string path = CLEO_GetGameDirectory();
    path += "\\cleo\\.config\\sa.json";
    m_opcodeDatabase.LoadCommands(path.c_str());

    path = CLEO_GetGameDirectory();
    path += "\\cleo\\.config\\enums.json";
    m_opcodeDatabase.LoadEnums(path.c_str());

    m_logBuffer.reserve(Initial_Buff_Size);

    // TODO: start file writer thread
}

ScriptLog::~ScriptLog()
{
    // TODO: stop file writer thread

    m_patchSetConditionResult.Apply(); // undo hook

    CLEO_UnregisterCallback(eCallbackId::GameBegin, callbackGameBegin);
    CLEO_UnregisterCallback(eCallbackId::GameProcessBefore, callbackGameProcessBefore);
    CLEO_UnregisterCallback(eCallbackId::GameProcessAfter, callbackGameProcessAfter);
    CLEO_UnregisterCallback(eCallbackId::ScriptProcessBefore, callbackScriptProcessBefore);
    CLEO_UnregisterCallback(eCallbackId::ScriptProcessAfter, callbackScriptProcessAfter);
    CLEO_UnregisterCallback(eCallbackId::ScriptOpcodeProcessBefore, callbackScriptOpcodeProcessBefore);
    CLEO_UnregisterCallback(eCallbackId::ScriptOpcodeProcessAfter, callbackScriptOpcodeProcessAfter);
    CLEO_UnregisterCallback(eCallbackId::DrawingFinished, callbackDrawingFinished);
    CLEO_UnregisterCallback(eCallbackId::MainWindowFocus, callbackMainWindowFocus);

    LogWriteFile(); // flush to file on exit or crash
}

size_t ScriptLog::CurrScriptCommandCount() const
{
    return m_currScriptCommandCount;
}

size_t ScriptLog::CurrScriptElapsedSeconds() const
{
    return (clock() - m_currScriptStartTime) / CLOCKS_PER_SEC;
}

void ScriptLog::LoadConfig(bool keepState)
{
    auto config = GetConfigFilename();

    if (!keepState)
    {
        switch (GetPrivateProfileInt("ScriptLog", "Enabled", 0, config.c_str()))
        {
        case 0:
            state = LoggingState::Disabled;
            break;

        case 1:
        default:
            state = LoggingState::OnCrash;
            break;

        case 2:
            state = LoggingState::Full;
            break;
        }
    }

    int size    = GetPrivateProfileInt("ScriptLog", "MaxSize", 250, config.c_str());
    size        = std::max(size, 0);
    maxFileSize = size;
    maxFileSize *= 1024 * 1024; // to MB

    logCustomScriptsOnly = GetPrivateProfileInt("ScriptLog", "OnlyCustomScripts", -1, config.c_str());
    logDebugScriptsOnly  = GetPrivateProfileInt("ScriptLog", "OnlyDebugScripts", false, config.c_str()) != 0;
    logOffsets           = GetPrivateProfileInt("ScriptLog", "Offsets", 0, config.c_str()) != 0;
    logOpcodes           = GetPrivateProfileInt("ScriptLog", "Opcodes", 0, config.c_str()) != 0;
}

void ScriptLog::SetCurrScript(CLEO::CRunningScript* script)
{
    // close previous script's log block
    if (m_currScript != nullptr && script != m_currScript && state != LoggingState::Disabled && m_currScriptLogging)
    {
        if (m_currScriptCommandCount > 0)
        {
            if (m_processingGame) LogAppend(Block_Indent);
            LogAppend(Block_Indent);
            LogAppend("Executed in ");
            LogAppendNum(size_t(clock() - m_currScriptStartTime) * 1000 / CLOCKS_PER_SEC);
            LogAppend(" ms\n");
        }

        if (m_processingGame) LogAppend(Block_Indent);
        LogAppend("</script>\n");
    }

    // find index of this script in active scripts queue
    int scriptIdx = -1;
    int i         = 0;
    for (auto s = (CLEO::CRunningScript*)CTheScripts::pActiveScripts; s; s = s->Next)
    {
        i++; // 1-based display
        if (s == script)
        {
            scriptIdx = i;
            break;
        }
    }

    // update m_currScriptLogging state
    if (script)
    {
        m_currScriptLogging = true;

        // custom scripts
        if (m_currScriptLogging && !script->IsCustom() && scriptIdx != -1) // native scripts in active scripts queue
        {
            switch (logCustomScriptsOnly)
            {
            case -1:
                m_currScriptLogging = m_customMain;
                break; // automatic: log if not defaul main.scm
            case 1:
                m_currScriptLogging = false;
                break; // only custom
                // default: // log all
            }
        }

        // debug mode scripts
        if (m_currScriptLogging && logDebugScriptsOnly)
        {
            m_currScriptLogging =
                CLEO_GetScriptDebugMode(script) ||                      // debug mode on
                (*(WORD*)script->GetBytePointer()) == COMMAND_DEBUG_ON; // debug mode just about to be enabled
        }
    }
    else
        m_currScriptLogging = false;

    // start new script's log block
    if (script != nullptr && script != m_currScript && state != LoggingState::Disabled && m_currScriptLogging)
    {
        // script file path
        static std::string filename; // keep for performance

        auto name = CLEO_GetScriptFilename(script);
        if (name != nullptr)
        {
            filename = ".\\";
            filename.resize(MAX_PATH);
            CLEO_ResolvePath(script, filename.data(), filename.size());
            filename.resize(strlen(filename.c_str()));
            if (!filename.empty()) filename.push_back('\\');
            filename += name;
            FilepathNormalize(filename);

            if (StringStartsWith(filename, CLEO_GetUserDirectory(), false))
            {
                FilepathRemoveParent(filename, CLEO_GetUserDirectory());
                filename = "user:\\" + filename;
            }
            else
                FilepathRemoveParent(filename, CLEO_GetGameDirectory());
        }
        else
        {
            filename = "?";
        }

        LogAppend('\n');
        if (m_processingGame) LogAppend(Block_Indent);
        LogAppend("<script idx='");
        LogAppendNum(scriptIdx);

        LogAppend("' name='");
        LogAppend(script->GetName());

        LogAppend("' file='");
        LogAppend(filename);
        LogAppend("'");

        if (script->IsCustom()) LogAppend(" custom='true'");
        if (script->IsMission()) LogAppend(" mission='true'");
        if (CLEO_GetScriptDebugMode(script)) LogAppend(" debug='true'");

        auto ver = CLEO_GetScriptVersion(script);
        if (ver != eCLEO_Version::CLEO_VER_CUR)
        {
            LogAppend(" compat='");
            switch (ver)
            {
            case eCLEO_Version::CLEO_VER_3:
                LogAppend("CLEO3");
                break;
            case eCLEO_Version::CLEO_VER_4:
                LogAppend("CLEO4");
                break;
            case eCLEO_Version::CLEO_VER_5:
                LogAppend("CLEO5");
                break;
            default:
                LogAppendHex(ver);
                break;
            }
            LogAppend("'");
        }

        LogAppend(">");

        // call stack info
        auto cleoStack = CLEO_GetCleoCallStackSize(script);
        if (cleoStack || script->SP)
        {
            LogAppend('\n');
            if (m_processingGame) LogAppend(Block_Indent);
            LogAppend(Block_Indent);
            LogAppend("Call Stacks depth - cleo_call: ");
            LogAppendNum(cleoStack);
            LogAppend(", gosub: ");
            LogAppendNum(script->SP);

            bool savingEnabled = !script->IsCustom() || false; // TODO: check CLEO save enabled for custom script
            if (savingEnabled && cleoStack)
            {
                LogAppend(" \\ BUG: Scrip saving enabled. Saving during cleo_call would result in errors!");
            }

            bool wastedBustedCheck = script->IsMission() && script->bWastedBustedCheck;
            if (wastedBustedCheck && cleoStack)
            {
                LogAppend(
                    " \\ BUG: Mission death-arrest-check active. Executing death-arrest procedure during cleo_call "
                    "would result in errors!"
                );
            }
            else if (wastedBustedCheck && script->SP == 0) // Rockstar's bug?
            {
                LogAppend(
                    " \\ BUG: Mission death-arrest-check requires at least one GOSUB level. Executing death-arrest "
                    "procedure now would result in errors!"
                );
            }
        }

        // 'wait' command in progress?
        if (script->WakeTime > CTimer::m_snTimeInMilliseconds)
        {
            LogAppend('\n');
            if (m_processingGame) LogAppend(Block_Indent);
            LogAppend(Block_Indent);
            LogAppend("Sleeping for ");
            LogAppendNum(script->WakeTime - CTimer::m_snTimeInMilliseconds);
            LogAppend(" ms more");
        }

        LogAppend('\n');
    }

    // update/reset script stats
    if (script != m_currScript || script == nullptr)
    {
        m_currScript             = script;
        m_currScriptStartTime    = clock();
        m_currScriptCommandCount = 0;
        m_prevCommand            = 0xFFFF;
    }
}

void ScriptLog::LogLine(const std::string_view& str)
{
    // TODO: thread lock
    m_logBuffer += '\n';
    m_logBuffer += str;
    // TODO: thread unlock
}

void ScriptLog::LogFormattedLine(const char* format, ...)
{
    static std::string line; // keep for performance
    line.clear();

    va_list args;

    va_start(args, format);
    auto len = std::vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (len <= 0) return; // empty or encoding error

    line.resize(len);

    va_start(args, format);
    std::vsnprintf(line.data(), len + 1, format, args);
    va_end(args);

    LogLine(line);
}

inline void ScriptLog::LogAppend(char ch)
{
    // TODO: thread lock
    m_logBuffer += ch;
    // TODO: thread unlock
}

void ScriptLog::LogAppend(const std::string_view& str)
{
    // TODO: thread lock
    m_logBuffer += str;
    // TODO: thread unlock
}

inline void ScriptLog::LogAppendNum(int number, int padLen)
{
    // TODO: thread lock
    StringAppendNum(m_logBuffer, number, padLen);
    // TODO: thread unlock
}

inline void ScriptLog::LogAppendHex(int number, int padLen)
{
    // TODO: thread lock
    StringAppendHex(m_logBuffer, number, padLen);
    // TODO: thread unlock
}

inline void ScriptLog::LogAppendFloat(float number, int padLen)
{
    // TODO: thread lock
    StringAppendFloat(m_logBuffer, number, padLen);
    // TODO: thread unlock
}

inline void ScriptLog::LogAppendSpace()
{
    // TODO: thread lock
    if (!m_logBuffer.empty() && m_logBuffer.back() != ' ' && m_logBuffer.back() != '\t')
    {
        m_logBuffer += ' ';
    }
    // TODO: thread unlock
}

bool ScriptLog::LogAppendScriptParam(
    CLEO::CRunningScript* script, const OpcodeInfoDatabase::Command* command, size_t paramIdx, bool logName,
    bool logVariable, bool logValue
)
{
    bool hasName = false;
    if (logName)
    {
        if (!command->arguments[paramIdx].name.empty())
        {
            hasName = true;
            LogAppend('{');
            LogAppend(command->arguments[paramIdx].name);
            LogAppend('}');
        }
    }

    ScriptParamInfo paramInfo(script);

    bool isGlobalVar;
    switch (paramInfo.type)
    {
    case DT_VAR:
    case DT_VAR_ARRAY:
    case DT_VAR_TEXTLABEL:
    case DT_VAR_TEXTLABEL_ARRAY:
    case DT_VAR_STRING:
    case DT_VAR_STRING_ARRAY:
        isGlobalVar = true;
        break;

    default:
        isGlobalVar = false;
    }

    bool hasVariable = false;
    if (logVariable)
    {
        switch (paramInfo.type)
        {
        case DT_END:
            /*hasVariable = true;
            if (hasName) LogAppend(' ';
            LogAppend("ArgsEnd";*/
            break;

        case DT_VAR:
        case DT_VAR_ARRAY:
        case DT_VAR_TEXTLABEL:
        case DT_VAR_TEXTLABEL_ARRAY:
        case DT_VAR_STRING:
        case DT_VAR_STRING_ARRAY:
        case DT_LVAR:
        case DT_LVAR_ARRAY:
        case DT_LVAR_TEXTLABEL:
        case DT_LVAR_TEXTLABEL_ARRAY:
        case DT_LVAR_STRING:
        case DT_LVAR_STRING_ARRAY:
            hasVariable = true;
            if (hasName) LogAppend(' ');
            if (isGlobalVar) LogAppend('$');
            LogAppendNum(paramInfo.varIndex);
            if (!isGlobalVar) LogAppend('@');

            if (paramInfo.arrayType != eArrayType::AT_NONE)
            {
                auto idxVar = GetScriptVar(script, paramInfo.arrayFlags & ATF_INDEX_GLOBAL, paramInfo.arrayIndexVar);

                LogAppend("[ ");
                if (paramInfo.arrayFlags & ATF_INDEX_GLOBAL) LogAppend('$');
                LogAppendNum(paramInfo.arrayIndexVar);
                if ((paramInfo.arrayFlags & ATF_INDEX_GLOBAL) == 0) LogAppend('@');
                LogAppend('(');
                if (abs(idxVar->nParam) >= MinValidAddress)
                {
                    LogAppend("0x");
                    LogAppendHex(idxVar->dwParam);
                }
                else
                    LogAppendNum(idxVar->nParam);
                LogAppend(") ]");
            }
            break;
        }
    }

    // no value to print
    switch (paramInfo.type)
    {
    case DT_END:
        logValue = false;
    }

    if (logValue)
    {
        if (hasVariable)
            LogAppend('(');
        else if (hasName)
            LogAppend(' ');

        // pick presentation style according to param type declared in the mode
        switch (command->arguments[paramIdx].type)
        {
        case OpcodeInfoDatabase::CommandArgumentType::Other: // enum or class
        {
            auto e = m_opcodeDatabase.GetEnum(command->arguments[paramIdx].typeNameLower.c_str());
            if (e)
            {
                if (e->IsNumeric())
                {
                    if (!IsImmString(paramInfo.type) && !IsVarString(paramInfo.type))
                    {
                        auto entry = e->GetEntryName(paramInfo.value.nParam);
                        if (entry)
                        {
                            LogAppend(e->name);
                            LogAppend('.');
                            LogAppend(entry);
                        }
                        else
                        {
                            // TODO: thread lock
                            paramInfo.ValueToString(m_logBuffer); // print according to param type in script
                            // TODO: thread unlock
                        }
                    }
                    else
                    {
                        // TODO: thread lock
                        paramInfo.ValueToString(m_logBuffer); // print according to param type in script
                        // TODO: thread unlock
                    }
                }
                else // text enums
                {
                    auto entry = e->GetEntryName(std::string(paramInfo.GetText()).c_str());
                    if (entry)
                    {
                        LogAppend(e->name);
                        LogAppend('.');
                        LogAppend(entry);
                    }
                    else
                    {
                        // TODO: thread lock
                        paramInfo.ValueToString(m_logBuffer); // print according to param type in script
                        // TODO: thread unlock
                    }
                }
            }
            else // class or unknown enum
            {
                // TODO: thread lock
                paramInfo.ValueToString(m_logBuffer); // print according to param type in script
                // TODO: thread unlock
            }
            break;
        }

            // case OpcodeInfoDatabase::CommandArgumentType::Any:
            // case OpcodeInfoDatabase::CommandArgumentType::Arguments:

        case OpcodeInfoDatabase::CommandArgumentType::Bool:
            switch (paramInfo.value.nParam)
            {
            case 0:
                LogAppend("false");
                break;
            case 1:
                LogAppend("true");
                break;
            default:
                LogAppendNum(paramInfo.value.nParam);
                break;
            }
            break;

        case OpcodeInfoDatabase::CommandArgumentType::Float:
            LogAppendFloat(paramInfo.value.fParam);
            break;

        case OpcodeInfoDatabase::CommandArgumentType::GxtKey:
        case OpcodeInfoDatabase::CommandArgumentType::ZoneKey:
        case OpcodeInfoDatabase::CommandArgumentType::String:
        case OpcodeInfoDatabase::CommandArgumentType::String128:
            LogAppend((paramInfo.GetBaseType() == DT_TEXTLABEL) ? '\'' : '"');
            LogAppend(paramInfo.GetText());
            LogAppend((paramInfo.GetBaseType() == DT_TEXTLABEL) ? '\'' : '"');
            break;

        case OpcodeInfoDatabase::CommandArgumentType::Int:
            if (paramInfo.GetBaseType() == DT_DWORD || paramInfo.GetBaseType() == DT_VAR)
            {
                // TODO: thread lock
                paramInfo.ValueToString(m_logBuffer);
                // TODO: thread unlock
            }
            else // expected integer, got something else. Print as hex
            {
                LogAppend("0x");
                LogAppendHex(paramInfo.value.nParam);
            }
            break;

        case OpcodeInfoDatabase::CommandArgumentType::Label:
            LogAppend("@LABEL_");
            LogAppendNum(abs(paramInfo.value.nParam));
            break;

        case OpcodeInfoDatabase::CommandArgumentType::ModelAny:
        case OpcodeInfoDatabase::CommandArgumentType::ModelChar:
        case OpcodeInfoDatabase::CommandArgumentType::ModelObject:
        case OpcodeInfoDatabase::CommandArgumentType::ModelVehicle:
            LogAppendNum(paramInfo.value.nParam);
            break;

        case OpcodeInfoDatabase::CommandArgumentType::ScriptId:
            LogAppend("0x");
            LogAppendHex(paramInfo.value.nParam);
            break;

        default: {
            // TODO: thread lock
            paramInfo.ValueToString(m_logBuffer); // print according to param type in script
            // TODO: thread unlock
        }
        }

        if (hasVariable) LogAppend(')');
    }

    return isGlobalVar;
}

void ScriptLog::LogWriteFile(bool forceUpdate)
{
    static size_t fileSize = 0;

    // close stream so system and applications notice it was updated
    if (forceUpdate)
    {
        m_logFile.flush();
        m_logFile.close();
    }

    // TODO: thread lock
    // std::swap(m_logBuffer, m_logFileBuffer);
    // unlock
    std::string& m_logFileBuffer = m_logBuffer; // no double buffering in single thread

    if (m_logFileBuffer.empty())
    {
        return; // empty
    }

    if (maxFileSize > 0 && fileSize > maxFileSize)
    {
        LogFileDelete();
        LOG_WARNING(0, "CLEO script log file cleared");
    }

    if (!m_logFile.is_open())
    {
        m_logFile.clear(); // clear flags
        m_logFile.open(m_logFilePath.c_str(), std::ios::app);
    }

    if (m_logFile.is_open())
    {
        m_logFile << m_logFileBuffer;
        fileSize = (size_t)m_logFile.tellp();
    }
    else
    {
        LOG_WARNING(0, "Failed to open script log file!");
    }

    m_logBuffer.clear();
}

void ScriptLog::LogFileDelete()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }

    // rename then remove
    // fixes lag when overwriting old file
    auto newName = m_logFilePath;
    StringAppendHex(newName, GetTickCount());
    if (std::rename(m_logFilePath.c_str(), newName.c_str()) == 0)
    {
        std::remove(newName.c_str());
    }
}

void __fastcall ScriptLog::HOOK_SetConditionResult(CLEO::CRunningScript* script, int dummy, bool state)
{
    if (script->NotFlag) state = !state;

    g_Instance->m_conditionResultUpdated = true;
    g_Instance->m_conditionResultValue   = state;

    // restoring and calling orignal function code is more performance costly than reimplementing it here

    if (script->LogicalOp == eLogicalOperation::NONE)
    {
        script->bCondResult = state;
    }
    else if (script->LogicalOp >= eLogicalOperation::ANDS_1 && script->LogicalOp < eLogicalOperation::AND_END)
    {
        script->bCondResult &= state;
        --script->LogicalOp;
    }
    else if (script->LogicalOp >= eLogicalOperation::ORS_1 && script->LogicalOp < eLogicalOperation::OR_END)
    {
        script->bCondResult |= state;
        --script->LogicalOp;
    }
}

void ScriptLog::OnGameBegin(DWORD saveSlot)
{
    if (state == LoggingState::Disabled) return;

    SetCurrScript(nullptr);

    LoadConfig(true);

    // time stamp
    SYSTEMTIME t;
    GetLocalTime(&t);
    char timeStamp[32];
    sprintf_s(
        timeStamp, "%02d/%02d/%04d %02d:%02d:%02d.%03d", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond,
        t.wMilliseconds
    );

    if (CGame::bMissionPackGame)
    {
        // find name of the mission pack
        const char* packName = nullptr;
        for (const auto& pack : FrontEndMenuManager.m_MissionPacks)
        {
            if (pack.id == FrontEndMenuManager.m_nSelectedMissionPack)
            {
                packName = pack.name;
                break;
            }
        }

        LogFormattedLine(
            "<newGameSession mPack='%d' mPackName='%s' time='%s'/>", FrontEndMenuManager.m_nSelectedMissionPack,
            packName ? packName : "???", timeStamp
        );

        m_customMain = true;
    }
    else
    {
        // verify main.scm
        const size_t Orig_Main_Size        = 200000;
        const size_t Orig_Main_Code_Offset = 55976; // skip global variables
        const size_t Orig_Main_Hash        = 0xbd4e2fcf;

        auto hash =
            crc32((BYTE*)CTheScripts::ScriptSpace + Orig_Main_Code_Offset, Orig_Main_Size - Orig_Main_Code_Offset);
        m_customMain = hash != Orig_Main_Hash; // hash of original main.scm

        if (saveSlot != -1)
        {
            LogFormattedLine(
                "<newGameSession saveSlot='%d' mainScript='%s' time='%s'/>",
                saveSlot + 1, // 1-based index
                m_customMain ? "modified" : "original", timeStamp
            );
        }
        else
        {
            LogFormattedLine(
                "<newGameSession mainScript='%s' time='%s'/>", m_customMain ? "modified" : "original", timeStamp
            );
        }
    }

    LogAppend('\n'); // separator
}

void ScriptLog::OnGameProcessBefore()
{
    if (!CTimer::m_UserPause && !CTimer::m_CodePause) m_processingGame = true;

    // 'CLOG' cheat
    if (_strnicmp(CCheat::m_CheatString, "golc", 4) == 0) // reversed order
    {
        CCheat::m_CheatString[0] = '\0'; // consume the cheat

        state = (state == LoggingState::Full) ? LoggingState::OnCrash : LoggingState::Full; // toggle
        if (state == LoggingState::Full) LoadConfig(true); // refresh config from ini file

        if (state == LoggingState::Full)
        {
            CHud::SetHelpMessage("CLEO script log: ~g~~h~~h~~h~ON~s~", true, false, false);
            LogLine("Log: ON\n");
        }
        else
        {
            CHud::SetHelpMessage("CLEO script log: ~r~~h~~h~~h~OFF~s~", true, false, false);
            LogLine("Log: OFF\n");
            LogWriteFile(); // flush
        }
    }

    if (state == LoggingState::Disabled) return;

    // count active scripts
    if (m_processingGame)
    {
        size_t scriptCount = 0;
        auto next          = CTheScripts::pActiveScripts;
        while (next)
        {
            scriptCount++;
            next = next->m_pNext;
        }

        LogAppend("\n<gameProcess frame='");
        LogAppendNum(CTimer::m_FrameCounter);
        LogAppend("' activeScripts='");
        LogAppendNum(scriptCount);
        LogAppend("'>");
    }
}

void ScriptLog::OnGameProcessAfter()
{
    SetCurrScript(nullptr);

    if (state != LoggingState::Disabled)
    {
        if (m_processingGame) LogAppend("</gameProcess>\n");
    }

    m_processingGame = false;
}

bool ScriptLog::OnScriptProcessBefore(CLEO::CRunningScript* script)
{
    SetCurrScript(script);
    return true;
}

void ScriptLog::OnScriptProcessAfter(CLEO::CRunningScript* script)
{
    SetCurrScript(nullptr);
}

OpcodeResult ScriptLog::OnScriptOpcodeProcessBefore(CLEO::CRunningScript* script, DWORD opcode)
{
    SetCurrScript(script);
    m_currScriptCommandCount++;

    if (state == LoggingState::Disabled || !m_currScriptLogging) return OR_NONE;

    m_currCommandReturnParams = nullptr;
    m_conditionResultUpdated  = false;
    m_conditionResultExpected =
        script->NotFlag || m_prevCommand == COMMAND_ANDOR || script->LogicalOp != eLogicalOperation::NONE;
    if (opcode == 0x0AB1) m_conditionResultExpected = false; // cleo_call: condition result set on return

    auto oriIP = script->CurrentIP;

    if (m_processingGame) LogAppend(Block_Indent); // game processing block indentation
    if (m_currScript) LogAppend(Block_Indent);     // script processing block indentation

    // script offset
    if (logOffsets)
    {
        auto offset = CLEO_GetScriptBaseRelativeOffset(script, script->CurrentIP);
        offset -= sizeof(WORD); // command opcode was already consumed

        auto digits = offset <= 0 ? 1 : (int)(log10(offset) + 1);
        for (int i = 7 - digits; i > 0; i--)
            LogAppend(' '); // pad to constant width
        LogAppend('{');
        LogAppendNum(offset);
        LogAppend("}   ");
    }

    // command opcode
    if (logOpcodes)
    {
        LogAppend('{');
        LogAppendHex(opcode | (script->NotFlag ? 0x8000 : 0), 4);
        LogAppend(":} ");
    }

    // if block conditions indentation
    if (m_prevCommand == COMMAND_ANDOR || script->LogicalOp != eLogicalOperation::NONE) LogAppend(Script_Indent);

    // gosub scope indentation
    for (auto i = script->SP; i > 0; i--)
        LogAppend(Script_Indent);

    // cleo_call scope indentation
    for (auto i = CLEO_GetCleoCallStackSize(script); i > 0; i--)
        LogAppend(Script_Indent);

    // negation
    if (script->NotFlag) LogAppend("not ");

    auto command = m_opcodeDatabase.GetCommand((uint16_t)opcode);

    if (command == nullptr) // not documented in database?
    {
        LogAppend("command_");
        LogAppendHex(opcode, 4);
        m_prevCommand = (WORD)opcode;
        return OR_NONE;
    }

    // return parameters as "x, y = command ..."
    auto returnArgCount = command->arguments.size() - command->inputArguments;
    if (returnArgCount > 0)
    {
        for (size_t i = 0; i < command->arguments.size(); i++)
        {
            if (i < command->inputArguments)
            {
                if (command->arguments[i].type == OpcodeInfoDatabase::CommandArgumentType::Arguments)
                {
                    // when varArg param is present in command SB always compiles all following params as members of the
                    // varArg list we need to count all provided params and skip only those which are actualy optional
                    BYTE* argsIP = script->CurrentIP;
                    int args     = 0;
                    while (true)
                    {
                        ScriptParamInfo paramInfo(script); // just skip
                        if (paramInfo.type != eDataType::DT_END)
                            args++;
                        else
                            break;
                    }

                    // skip optional args
                    script->CurrentIP = argsIP;
                    args -= command->arguments.size() - (i + 1); // substract expected remaining params
                    while (args > 0)
                    {
                        ScriptParamInfo paramInfo(script); // just skip
                        args--;
                    }
                }
                else
                    ScriptParamInfo paramInfo(script); // just skip
            }
            else
            {
                if (i == command->inputArguments)
                    m_currCommandReturnParams = script->CurrentIP; // keep pointer to return params start

                LogAppendSpace();
                LogAppendScriptParam(script, command, i, returnArgCount > 1, true, false);
            }
        }
        script->CurrentIP = oriIP; // restore original position

        LogAppend(" = ");
    }
    else if (!command->oper.empty() && !command->arguments.empty() && !command->IsComparison())
    {
        m_currCommandReturnParams = oriIP; // operator result is first param
    }

    // command name
    if (command->oper.empty())
    {
        LogAppend(command->nameLower);
    }

    // command params
    if (opcode == COMMAND_ANDOR)
    {
        ScriptParamInfo paramInfo(script);
        if (paramInfo.value.nParam >= eLogicalOperation::ORS_1)
            LogAppend(" or");
        else if (paramInfo.value.nParam >= eLogicalOperation::ANDS_1)
            LogAppend(" and");
    }
    else
    {
        for (size_t i = 0; i < command->inputArguments; i++)
        {
            LogAppendSpace();

            // operator
            if (!command->oper.empty())
            {
                if (command->inputArguments == 1 && i == 0) // %param
                {
                    LogAppend(command->oper);
                }
                else if (command->inputArguments == 2 && i == 1) // param % param
                {
                    LogAppend(command->oper);
                    if (!command->IsComparison() && command->oper.find('=') == std::string::npos)
                        LogAppend('='); // param %= param
                    LogAppend(" ");
                }
            }

            if (command->arguments[i].type != OpcodeInfoDatabase::CommandArgumentType::Arguments)
            {
                LogAppendScriptParam(script, command, i, true, true, true);
            }
            else // varArgs
            {
                bool first = true;
                auto type  = script->PeekDataType();
                while (type != eDataType::DT_END)
                {
                    if (!first) LogAppend(" ");
                    LogAppendScriptParam(script, command, i, first, true, true);

                    first = false;
                    type  = script->PeekDataType();
                }
                script->IncPtr(sizeof(eDataType)); // var args terminator
            }
        }
    }

    script->CurrentIP = oriIP; // restore original position
    m_prevCommand     = (WORD)opcode;
    return OR_NONE;
}

CLEO::OpcodeResult ScriptLog::OnScriptOpcodeProcessAfter(
    CLEO::CRunningScript* script, DWORD opcode, CLEO::OpcodeResult result
)
{
    if (state == LoggingState::Disabled || !m_currScriptLogging) return result;

    auto command = m_opcodeDatabase.GetCommand((uint16_t)opcode);
    if (!command)
    {
        LogAppend('\n');
        return result;
    }

    bool hasComment    = false;
    bool hasReturnVal  = false;
    bool warnGlobalVar = false;

    // print values of return arguments
    if (m_currCommandReturnParams != nullptr)
    {
        auto oriIp        = script->CurrentIP;
        script->CurrentIP = m_currCommandReturnParams;

        size_t i     = command->oper.empty() ? command->inputArguments : 0;
        size_t count = command->oper.empty() ? command->arguments.size() : 1;
        for (; i < count; i++)
        {
            if (!hasReturnVal)
                LogAppend(hasComment ? " -> " : " // -> ");
            else
                LogAppend(", "); // separator
            hasComment = true;

            auto isGlobalVar = LogAppendScriptParam(script, command, i, false, false, true);
            hasReturnVal     = true;

            if (isGlobalVar && script->IsCustom()) warnGlobalVar = true;
        }
        script->CurrentIP = oriIp;
    }

    // condition result value
    if (m_conditionResultUpdated)
    {
        if (!hasReturnVal)
            LogAppend(hasComment ? " -> " : " // -> ");
        else
            LogAppend(", "); // separator
        hasComment = true;

        LogAppend("condition: ");
        LogAppend(m_conditionResultValue ? "true" : "false");
        hasReturnVal = true;
    }

    // extra annotations
    if (command->isNop)
    {
        LogAppend(hasComment ? ", " : " // ");
        hasComment = true;
        LogAppend("NOP command");
    }

    if (m_conditionResultExpected && !m_conditionResultUpdated &&
        opcode != 0x0AB1) // assume cleo_call always sets condition result
    {
        LogAppend(hasComment ? ", " : " // ");
        hasComment = true;
        LogAppend("BUG: non-logical command");
    }

    if (warnGlobalVar)
    {
        LogAppend(hasComment ? ", " : " // ");
        hasComment = true;
        LogAppend("BUG?: Custom Script writes global variable");
    }

    LogAppend('\n');

    return result;
}

void ScriptLog::OnDrawingFinished()
{
    // late initialization
    if (!m_initialized)
    {
        m_patchSetConditionResult =
            MemPatchJump(gaddrof(::CRunningScript::UpdateCompareFlag), &HOOK_SetConditionResult);

        m_initialized = true;
    }

    // this should be done just at the end of scripts processing
    // but PluginSDK, SAMP etc. call single scripts/callbacks outside of the processing queue
    // make sure current script does not persists to next render frame
    SetCurrScript(nullptr);

    if (state == LoggingState::Full) LogWriteFile();

    if (state != LoggingState::Full) // keep only current frame
    {
        // TODO: thread lock
        m_logBuffer.clear();
        // TODO: thread unlock
    }
}

void ScriptLog::OnMainWindowFocus(bool active)
{
    if (active == false && state == LoggingState::Full)
    {
        LogWriteFile(true); // flush
    }
}
