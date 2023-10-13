#include "ScreenLog.h"
#include "Utils.h"
#include "CLEO.h"
#include "CTimer.h"
#include <windows.h> // keyboard
#include <deque>
#include <map>
#include <fstream>
#include <sstream>

using namespace CLEO;

class DebugUtils
{
public:
    static ScreenLog screenLog;

    struct PausedScriptInfo 
    { 
        CScriptThread* ptr;
        std::string msg;
        PausedScriptInfo(CScriptThread* ptr, const char* msg) : ptr(ptr), msg(msg) {}
    };
    static std::deque<PausedScriptInfo> pausedScripts;

    // breakpoint continue keys
    static const int KeyFirst = VK_F5;
    static const size_t KeyCount = 8; // F5 to F12
    static bool keysReleased; // none of continue keys was pressed during previous frame

    static std::map<std::string, std::ofstream> logFiles;

    DebugUtils()
    {
        auto cleoVer = CLEO_GetVersion();
        if (cleoVer >= CLEO_VERSION)
        {
            auto config = GetConfigFilename();

            // register opcodes
            CLEO_RegisterOpcode(0x00C3, Opcode_DebugOn);
            CLEO_RegisterOpcode(0x00C4, Opcode_DebugOff);
            CLEO_RegisterOpcode(0x00CC, Opcode_Breakpoint);
            CLEO_RegisterOpcode(0x00CD, Opcode_Trace);
            CLEO_RegisterOpcode(0x00CE, Opcode_LogToFile);

            // original Rockstar's script debugging opcodes
            if(GetPrivateProfileInt("General", "LegacyDebugOpcodes", 0, config.c_str()) != 0)
            {
                CLEO_RegisterOpcode(0x0662, Opcode_PrintString);
                CLEO_RegisterOpcode(0x0663, Opcode_PrintInt);
                CLEO_RegisterOpcode(0x0664, Opcode_PrintFloat);
            }

            // register event callbacks
            CLEO_RegisterCallback(eCallbackId::ScriptsFinalize, OnScriptsFinalize);
            CLEO_RegisterCallback(eCallbackId::ScriptDraw, OnScriptDraw);
            CLEO_RegisterCallback(eCallbackId::MenuDraw, OnMenuDraw);
            CLEO_RegisterCallback(eCallbackId::ScriptProcess, OnScriptProcess);
            CLEO_RegisterCallback(eCallbackId::Log, OnLog);
        }
        else
        {
            std::string err(128, '\0');
            sprintf(err.data(), "This plugin requires version %X or later! \nCurrent version of CLEO is %X.", CLEO_VERSION >> 8, cleoVer >> 8);
            MessageBox(HWND_DESKTOP, err.data(), "DebugUtils.cleo", MB_SYSTEMMODAL | MB_ICONERROR);
        }
    }

    // ---------------------------------------------- event callbacks -------------------------------------------------

    static void WINAPI OnScriptsFinalize()
    {
        pausedScripts.clear();
        logFiles.clear(); // close all
    }

    static void WINAPI OnScriptDraw(bool beforeFade)
    {
        if (beforeFade) return; // skip drawing before fade pass

        OnMenuDraw();
    }

    static void WINAPI OnMenuDraw()
    {
        // log messages
        screenLog.Draw();

        // draw active breakpoints list
        for (size_t i = 0; i < pausedScripts.size(); i++)
        {
            std::ostringstream ss;
            ss << "Script '" << pausedScripts[i].ptr->GetName() << "' breakpoint";

            if(!pausedScripts[i].msg.empty()) // named breakpoint
            {
                ss << " '" << pausedScripts[i].msg << "'";
            }

            if(i < KeyCount)
            {
                ss << " (F" << 5 + i << ")";
            }

            screenLog.DrawLine(ss.str().c_str(), i);
        }

        // update keys state
        if(!keysReleased)
        {
            keysReleased = true;
            for (size_t i = 0; i < KeyCount; i++)
            {
                auto state = GetKeyState(KeyFirst + i);
                if (state & 0x8000) // pressed
                {
                    keysReleased = false;
                    break;
                }
            }
        }
        else // ready for next press
        {
            for (size_t i = 0; i < pausedScripts.size(); i++)
            {
                if (keysReleased && i < KeyCount)
                {
                    auto state = GetKeyState(KeyFirst + i);
                    if (state & 0x8000) // pressed
                    {
                        keysReleased = false;

                        std::stringstream ss;
                        ss << "Script breakpoint ";
                        if (!pausedScripts[i].msg.empty()) ss << "'" << pausedScripts[i].msg << "' ";
                        ss << "released in '" << pausedScripts[i].ptr->GetName() << "'";
                        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

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
    }

    static bool WINAPI OnScriptProcess(CScriptThread* thread)
    {
        for (size_t i = 0; i < pausedScripts.size(); i++)
        {
            if (pausedScripts[i].ptr == thread)
            {
                return false; // script paused, do not process
            }
        }

        return true;
    }

    static void WINAPI OnLog(eLogLevel level, const char* msg)
    {
        screenLog.Add(level, msg);
    }

    // ---------------------------------------------- opcodes -------------------------------------------------

    // 00C3=0, debug_on
    static OpcodeResult WINAPI Opcode_DebugOn(CScriptThread* thread)
    {
        CLEO_SetScriptDebugMode(thread, true);

        return OR_CONTINUE;
    }

    // 00C4=0, debug_off
    static OpcodeResult WINAPI Opcode_DebugOff(CScriptThread* thread)
    {
        CLEO_SetScriptDebugMode(thread, false);

        return OR_CONTINUE;
    }

    // 00CC=-1, breakpoint ...
    static OpcodeResult WINAPI Opcode_Breakpoint(CScriptThread* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            SkipUnusedParams(thread);
            return OR_CONTINUE;
        }

        bool blocking = true; // pause entire game logic
        std::string name = "";

        // bool param - blocking
        auto paramType = CLEO_GetOperandType(thread);
        if(paramType == DT_BYTE)
        {
            blocking = CLEO_GetIntOpcodeParam(thread) != 0;
        }

        paramType = CLEO_GetOperandType(thread);
        if (paramType == eDataType::DT_END)
        {
            thread->IncPtr(); // consume arguments terminator
        }
        else // breakpoint formatted name string
        {
            auto format = CLEO_ReadStringOpcodeParam(thread);
            name = CLEO_ReadParamsFormatted(thread, format);
        }

        pausedScripts.emplace_back(thread, name.c_str());

        std::stringstream ss;
        ss << "Script breakpoint";
        if (!name.empty()) ss << " '" << name << "'";
        ss << " captured in '" << thread->GetName() << "'";
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        if(blocking)
        {
            CLEO_Log(eLogLevel::Debug, "Game paused");
            CTimer::m_CodePause = true;
        }

        return OR_CONTINUE;
    }

    // 00CD=-1, trace %1s% ...
    static OpcodeResult WINAPI Opcode_Trace(CScriptThread* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            SkipUnusedParams(thread);
            return OR_CONTINUE;
        }

        auto format = CLEO_ReadStringOpcodeParam(thread);
        auto message = CLEO_ReadParamsFormatted(thread, format);

        CLEO_Log(eLogLevel::Debug, message);
        return OR_CONTINUE;
    }

    // 00CE=-1, log_to_file %1s% timestamp %2d% text %3s% ...
    static OpcodeResult WINAPI Opcode_LogToFile(CScriptThread* thread)
    {
        auto filestr = CLEO_ReadStringOpcodeParam(thread);

        std::string filename(MAX_PATH, '\0');
        const size_t len = strlen(filestr);
        for(size_t i = 0; i < len; i++)
        {
            if(filestr[i] == '/')
                filename[i] = '\\';
            else
                filename[i] = std::tolower(filestr[i]);
        }
        CLEO_ResolvePath(thread, filename.data(), MAX_PATH);
        filename.resize(strlen(filename.data())); // clip to actual cstr len

        auto it = logFiles.find(filename);
        if(it == logFiles.end()) // not opened yet
        {
            it = logFiles.emplace(std::piecewise_construct, std::make_tuple(filename), std::make_tuple(filename, std::ios_base::app)).first;
        }

        auto& file = it->second;
        if(!file.good())
        {
            std::ostringstream ss;
            ss << "Failed to open log file '" << filename << "'";
            CLEO_Log(eLogLevel::Error, ss.str().c_str());

            SkipUnusedParams(thread);
            return OR_CONTINUE;
        }

        // time stamp
        if(CLEO_GetIntOpcodeParam(thread) != 0)
        {
            SYSTEMTIME t;
            GetLocalTime(&t);
            static char szBuf[64];
            sprintf(szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
            file << szBuf;
        }

        auto format = CLEO_ReadStringOpcodeParam(thread);
        auto message = CLEO_ReadParamsFormatted(thread, format);

        file << message << std::endl;

        return OR_CONTINUE;
    }

    // 0662=1, printstring %1s%
    static OpcodeResult WINAPI Opcode_PrintString(CScriptThread* thread)
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
    static OpcodeResult WINAPI Opcode_PrintInt(CScriptThread* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipOpcodeParams(thread, 2);
            return OR_CONTINUE;
        }

        auto text = CLEO_ReadStringOpcodeParam(thread);
        auto value = CLEO_GetIntOpcodeParam(thread);

        std::ostringstream ss;
        ss << text << ": " << value;
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        return OR_CONTINUE;
    }

    // 0664=1, printfloat %1s% %2f%
    static OpcodeResult WINAPI Opcode_PrintFloat(CScriptThread* thread)
    {
        if (!CLEO_GetScriptDebugMode(thread))
        {
            CLEO_SkipOpcodeParams(thread, 2);
            return OR_CONTINUE;
        }

        auto text = CLEO_ReadStringOpcodeParam(thread);
        auto value = CLEO_GetFloatOpcodeParam(thread);

        std::ostringstream ss;
        ss << text << ": " << value;
        CLEO_Log(eLogLevel::Debug, ss.str().c_str());

        return OR_CONTINUE;
    }
} DebugUtils;

ScreenLog DebugUtils::screenLog = {};
std::deque<DebugUtils::PausedScriptInfo> DebugUtils::pausedScripts;
bool DebugUtils::keysReleased = true;
std::map<std::string, std::ofstream> DebugUtils::logFiles;

