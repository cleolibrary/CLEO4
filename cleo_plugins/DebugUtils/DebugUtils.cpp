#include <windows.h> // keyboard
#include <deque>
#include <map>
#include <fstream>
#include <sstream>

#include "CMessages.h"
#include "CTimer.h"

#include "CLEO.h"
#include "CLEO_Utils.h"
#include "ScreenLog.h"
#include "ScriptLog.h"

using namespace CLEO;
using namespace plugin;

class DebugUtils
{
  public:
    static ScreenLog screenLog;

    struct PausedScriptInfo
    {
        CRunningScript* ptr;
        std::string msg;
        PausedScriptInfo(CRunningScript* ptr, const char* msg) : ptr(ptr), msg(msg) {}
    };
    static std::deque<PausedScriptInfo> pausedScripts;

    static ScriptLog scriptLog;

    // limits for processing after which script is considered hanging
    static size_t configLimitCommand;
    static size_t configLimitTime;

    // breakpoint continue keys
    static const int KeyFirst    = VK_F5;
    static const size_t KeyCount = 8; // F5 to F12
    static bool keysReleased;         // none of continue keys was pressed during previous frame

    static std::map<std::string, std::ofstream> logFiles;

    DebugUtils()
    {
        if (!PluginCheckCleoVersion()) return;

        auto config        = GetConfigFilename();
        configLimitCommand = GetPrivateProfileInt("Limits", "Command", 2000000, config.c_str()); // 2 milion commands
        configLimitTime    = GetPrivateProfileInt("Limits", "Time", 5, config.c_str());          // 5 seconds

        // register opcodes
        CLEO_RegisterOpcode(0x00C3, Opcode_DebugOn);
        CLEO_RegisterOpcode(0x00C4, Opcode_DebugOff);
        CLEO_RegisterOpcode(0x2100, Opcode_Breakpoint);
        CLEO_RegisterOpcode(0x2101, Opcode_Trace);
        CLEO_RegisterOpcode(0x2102, Opcode_LogToFile);

        // original Rockstar's script debugging opcodes
        if (GetPrivateProfileInt("General", "LegacyDebugOpcodes", 0, config.c_str()) != 0)
        {
            CLEO_RegisterOpcode(0x0662, Opcode_PrintString);
            CLEO_RegisterOpcode(0x0663, Opcode_PrintInt);
            CLEO_RegisterOpcode(0x0664, Opcode_PrintFloat);
        }

        // register event callbacks
        CLEO_RegisterCallback(eCallbackId::GameBegin, OnGameBegin);
        CLEO_RegisterCallback(eCallbackId::Log, OnLog);
        CLEO_RegisterCallback(eCallbackId::DrawingFinished, OnDrawingFinished);
        CLEO_RegisterCallback(eCallbackId::ScriptProcessBefore, OnScriptProcessBefore);
        CLEO_RegisterCallback(eCallbackId::ScriptOpcodeProcessBefore, OnScriptOpcodeProcessBefore);
        CLEO_RegisterCallback(eCallbackId::ScriptsFinalize, OnScriptsFinalize);
    }

    ~DebugUtils()
    {
        CLEO_UnregisterCallback(eCallbackId::GameBegin, OnGameBegin);
        CLEO_UnregisterCallback(eCallbackId::Log, OnLog);
        CLEO_UnregisterCallback(eCallbackId::DrawingFinished, OnDrawingFinished);
        CLEO_UnregisterCallback(eCallbackId::ScriptProcessBefore, OnScriptProcessBefore);
        CLEO_UnregisterCallback(eCallbackId::ScriptOpcodeProcessBefore, OnScriptOpcodeProcessBefore);
        CLEO_UnregisterCallback(eCallbackId::ScriptsFinalize, OnScriptsFinalize);
    }

    // ---------------------------------------------- event callbacks -------------------------------------------------

    static void WINAPI OnGameBegin(DWORD saveSlot) { screenLog.Clear(); }

    static void WINAPI OnScriptsFinalize()
    {
        pausedScripts.clear();
        logFiles.clear(); // close all
    }

    static void WINAPI OnDrawingFinished()
    {
        // use ingame function as GetKeyState might look like keylogger to some AV software
        auto GTA_GetKeyState = (SHORT(__stdcall*)(int))0x0081E64C;

        // log messages
        screenLog.Draw();

        // draw active breakpoints list
        if (!pausedScripts.empty() && (CTimer::m_FrameCounter & 0xE) != 0) // flashing
        {
            for (size_t i = 0; i < pausedScripts.size(); i++)
            {
                std::ostringstream ss;
                ss << "Script '" << pausedScripts[i].ptr->GetName() << "' breakpoint";

                if (!pausedScripts[i].msg.empty()) // named breakpoint
                {
                    ss << " '" << pausedScripts[i].msg << "'";
                }

                if (i < KeyCount)
                {
                    ss << " (F" << 5 + i << ")";
                }

                screenLog.DrawLine(ss.str().c_str(), i);
            }

            // for some reason last string on print list is always drawn incorrectly
            // Walkaround: add one extra dummy line then
            screenLog.DrawLine("_~n~_~n~_", 500);
        }

        // update keys state
        if (!keysReleased)
        {
            keysReleased = true;
            for (size_t i = 0; i < KeyCount; i++)
            {
                auto state = GTA_GetKeyState(KeyFirst + i);
                if (state & 0x8000) // key down
                {
                    keysReleased = false;
                    break;
                }
            }
        }
        else // ready for next press
        {
            const size_t count = std::min(pausedScripts.size(), KeyCount);
            for (size_t i = 0; i < count; i++)
            {
                auto state = GTA_GetKeyState(KeyFirst + i);
                if (state & 0x8000) // key down
                {
                    keysReleased = false;

                    if (!CTimer::m_CodePause)
                    {
                        std::stringstream ss;
                        ss << "Script breakpoint ";
                        if (!pausedScripts[i].msg.empty())
                            ss << "'" << pausedScripts[i].msg << "' "; // TODO: restore color if custom was used in name
                        ss << "released in '" << pausedScripts[i].ptr->GetName() << "'";
                        CLEO_Log(eLogLevel::Debug, ss.str().c_str());
                    }

                    if (CTimer::m_CodePause)
                    {
                        CLEO_Log(eLogLevel::Debug, "Game unpaused");
                        CTimer::m_CodePause = false;
                    }

                    pausedScripts.erase(pausedScripts.begin() + i);

                    break; // breakpoint continue
                }
            }
        }
    }

    static bool WINAPI OnScriptProcessBefore(CRunningScript* thread)
    {
        for (const auto& s : pausedScripts)
        {
            if (s.ptr == thread)
            {
                return false; // script paused, do not process
            }
        }

        return true;
    }

    static OpcodeResult WINAPI OnScriptOpcodeProcessBefore(CRunningScript* thread, DWORD opcode)
    {
        // script per render frame commands limit
        if (configLimitCommand > 0 && scriptLog.CurrScriptCommandCount() > configLimitCommand)
        {
            // add comma separators into huge numbers
            std::string limitStr;
            auto limit = configLimitCommand;
            while (limit >= 1000)
            {
                limitStr = StringPrintf(",%03d%s", limit % 1000, limitStr.c_str());
                limit /= 1000;
            }
            limitStr = StringPrintf("%d%s", limit, limitStr.c_str());

            SHOW_ERROR(
                "Over %s commands executed in a single frame by script %s \nTo prevent the game from freezing, CLEO "
                "suspended this script.\n\nTo supress this error, increase 'Command' property in %s.ini file and "
                "restart the game.",
                limitStr.c_str(), ScriptInfoStr(thread).c_str(), TARGET_NAME
            );
            return thread->Suspend();
        }

        // script per frame time execution limit
        if (scriptLog.CurrScriptCommandCount() % 1000 == 0) // check once every 1000 commands
        {
            if (configLimitTime > 0 && scriptLog.CurrScriptElapsedSeconds() > configLimitTime)
            {
                SHOW_ERROR(
                    "Over %d seconds of lag in a single frame by script %s \nTo prevent the game from freezing, CLEO "
                    "suspended this script.\n\nTo supress this error, increase 'Time' property in %s.ini file and "
                    "restart the game.",
                    configLimitTime, ScriptInfoStr(thread).c_str(), TARGET_NAME
                );
                return thread->Suspend();
            }
        }

        return OR_NONE;
    }

    static void WINAPI OnLog(eLogLevel level, const char* msg) { screenLog.Add(level, msg); }

    // ---------------------------------------------- opcodes -------------------------------------------------

    // 00C3=0, debug_on
    static OpcodeResult __stdcall Opcode_DebugOn(CRunningScript* thread)
    {
        CLEO_SetScriptDebugMode(thread, true);

        return OR_CONTINUE;
    }

    // 00C4=0, debug_off
    static OpcodeResult __stdcall Opcode_DebugOff(CRunningScript* thread)
    {
        CLEO_SetScriptDebugMode(thread, false);

        return OR_CONTINUE;
    }

    // 2100=-1, breakpoint ...
    static OpcodeResult __stdcall Opcode_Breakpoint(CRunningScript* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipUnusedVarArgs(thread);
            return OR_CONTINUE;
        }

        bool blocking    = true; // pause entire game logic
        std::string name = "";

        // bool param - blocking
        auto paramType = thread->PeekDataType();
        if (paramType == DT_BYTE)
        {
            blocking = CLEO_GetIntOpcodeParam(thread) != 0;
        }

        paramType = thread->PeekDataType();
        if (paramType == eDataType::DT_END)
        {
            thread->IncPtr(); // consume arguments terminator
        }
        else // breakpoint formatted name string
        {
            OPCODE_READ_PARAM_STRING_FORMATTED(nameStr);
            CMessages::InsertPlayerControlKeysInString(nameStr);
            name = nameStr;
        }

        pausedScripts.emplace_back(thread, name.c_str());

        std::stringstream ss;
        ss << "Script breakpoint";
        if (!name.empty()) ss << " '" << name << "'";
        ss << " captured in '" << thread->GetName() << "'";
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        if (blocking)
        {
            CLEO_Log(eLogLevel::Debug, "Game paused");
            CTimer::m_CodePause = true;
        }

        return OR_INTERRUPT;
    }

    // 2101=-1, trace %1s% ...
    static OpcodeResult __stdcall Opcode_Trace(CRunningScript* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipUnusedVarArgs(thread);
            return OR_CONTINUE;
        }

        OPCODE_READ_PARAM_STRING_FORMATTED(message);
        CMessages::InsertPlayerControlKeysInString(message);

        CLEO_Log(eLogLevel::Debug, message);
        return OR_CONTINUE;
    }

    // 2102=-1, log_to_file %1s% timestamp %2d% text %3s% ...
    static OpcodeResult __stdcall Opcode_LogToFile(CRunningScript* thread)
    {
        auto filestr = CLEO_ReadStringOpcodeParam(thread);

        // normalized absolute filepath
        std::string filename(MAX_PATH, '\0');
        const size_t len = strlen(filestr);
        for (size_t i = 0; i < len; i++)
        {
            if (filestr[i] == '/')
                filename[i] = '\\';
            else
                filename[i] = std::tolower(filestr[i]);
        }
        CLEO_ResolvePath(thread, filename.data(), MAX_PATH);
        filename.resize(strlen(filename.data())); // clip to actual cstr len

        auto it = logFiles.find(filename);
        if (it == logFiles.end()) // not opened yet
        {
            it = logFiles
                     .emplace(
                         std::piecewise_construct, std::make_tuple(filename),
                         std::make_tuple(filename, std::ios_base::app)
                     )
                     .first;
        }

        auto& file = it->second;
        if (!file.good())
        {
            std::ostringstream ss;
            ss << "Failed to open log file '" << filename << "'";
            CLEO_Log(eLogLevel::Error, ss.str().c_str());

            CLEO_SkipUnusedVarArgs(thread);
            return OR_CONTINUE;
        }

        // time stamp
        if (CLEO_GetIntOpcodeParam(thread) != 0)
        {
            SYSTEMTIME t;
            GetLocalTime(&t);
            static char szBuf[64];
            sprintf_s(
                szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond,
                t.wMilliseconds
            );
            file << szBuf;
        }

        OPCODE_READ_PARAM_STRING_FORMATTED(message);
        CMessages::InsertPlayerControlKeysInString(message);

        file << message << std::endl;

        return OR_CONTINUE;
    }

    // 0662=1, printstring %1s%
    static OpcodeResult __stdcall Opcode_PrintString(CRunningScript* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipOpcodeParams(thread, 1);
            return OR_CONTINUE;
        }

        auto text = CLEO_ReadStringOpcodeParam(thread);

        CLEO_Log(eLogLevel::Debug, text);

        return OR_CONTINUE;
    }

    // 0663=1, printint %1s% %2d%
    static OpcodeResult __stdcall Opcode_PrintInt(CRunningScript* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipOpcodeParams(thread, 2);
            return OR_CONTINUE;
        }

        auto text  = CLEO_ReadStringOpcodeParam(thread);
        auto value = CLEO_GetIntOpcodeParam(thread);

        std::ostringstream ss;
        ss << text << ": " << value;
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        return OR_CONTINUE;
    }

    // 0664=1, printfloat %1s% %2f%
    static OpcodeResult __stdcall Opcode_PrintFloat(CRunningScript* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipOpcodeParams(thread, 2);
            return OR_CONTINUE;
        }

        auto text  = CLEO_ReadStringOpcodeParam(thread);
        auto value = CLEO_GetFloatOpcodeParam(thread);

        std::ostringstream ss;
        ss << text << ": " << value;
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        return OR_CONTINUE;
    }
} DebugUtils;

ScreenLog DebugUtils::screenLog = {};
std::deque<DebugUtils::PausedScriptInfo> DebugUtils::pausedScripts;
ScriptLog DebugUtils::scriptLog = {};
size_t DebugUtils::configLimitCommand;
size_t DebugUtils::configLimitTime;
bool DebugUtils::keysReleased = true;
std::map<std::string, std::ofstream> DebugUtils::logFiles;
