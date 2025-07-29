// Some optional utilities usefull when creating CLEO plugins
//
// Add following lines to "Additional Include Directories" in project config:
//   $(PLUGIN_SDK_DIR)\plugin_sa\
//   $(PLUGIN_SDK_DIR)\shared\game\
//   $(PLUGIN_SDK_DIR)\plugin_sa\game_sa\
//
// Add following lines to "Preprocesor Definitions":
//   GTASA
//   TARGET_NAME=R"($(TargetName))"
//
// Depending on used functions, may require adding "CPools.cpp" from GTA Plugin SDK to the project files

#pragma once
#include "CLEO.h"
#include "CPools.h"   // from GTA Plugin SDK
#include "shellapi.h" // game window minimize/maximize support
#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <wtypes.h>

namespace CLEO
{
    /*
    TRACE(format,...) // log to file. Can be displayed on screen with change in DebugUtils.ini
    LOG_WARNING(script, format, ...) // warning text on screen and in log file. Not displayed for scripts in 'legacy'
    mode SHOW_ERROR(a,...) // message box, log to file

    Macros to use inside opcode handler functions. Performs types validation, printing warnings and suspending script on
    critical errors. Please mind those expand into multiple lines, so CAN NOT be used in places where single code line
    is expected! (like 'if' condition body without brackets)

    OPCODE_CONDITION_RESULT(value) // set command logical result

    OPCODE_SKIP_PARAMS(count) // ignore X params
    OPCODE_SKIP_VARARG_PARAMS(count) // ignore remaining variable arguments, including varArg terminator

    OPCODE_PEEK_PARAM_TYPE() // get param type without advancing the script
    OPCODE_PEEK_VARARG_COUNT() // get count of remaining variable arguments (not including varArg terminator)

    // reading opcode input arguments
    OPCODE_READ_PARAM_BOOL()
    OPCODE_READ_PARAM_INT8()
    OPCODE_READ_PARAM_UINT8()
    OPCODE_READ_PARAM_INT16()
    OPCODE_READ_PARAM_UINT16()
    OPCODE_READ_PARAM_INT()
    OPCODE_READ_PARAM_UINT()
    OPCODE_READ_PARAM_FLOAT()
    OPCODE_READ_PARAM_ANY32() // get raw data of any simple-type value (practically integers and floats)
    OPCODE_READ_PARAM_STRING(varName) // reads param and creates const char* variable named 'varName' with pointer to
    null-terminated string OPCODE_READ_PARAM_STRING_LEN(varName, maxLength) // same as above, but text length is clamped
    to maxLength OPCODE_READ_PARAM_STRING_FORMATTED(varName) // reads "format" string argument, then all var-args.
    Creates variable named 'varName' containing formatted text. Creates 'varNameOk' variable with pointer to the text,
    or nullptr if user provided invalid arguments OPCODE_READ_PARAMS_FORMATTED(format, varName) // reads all var-args
    and tries to put them into formatted string. Creates variable named 'varName' containing formatted text. Creates
    'varNameOk' variable with pointer to the text, or nullptr if user provided invalid arguments
    OPCODE_READ_PARAM_FILEPATH(varName) // reads param and creates const char* variable named 'varName' with pointer to
    resolved, null-terminated, filepath OPCODE_READ_PARAM_PTR() // read and validate memory address argument
    OPCODE_READ_PARAM_OBJECT_HANDLE() // read and validate game object handle
    OPCODE_READ_PARAM_PED_HANDLE() // read and validate character (ped/actor) handle
    OPCODE_READ_PARAM_PLAYER_ID() // read and validate player id
    OPCODE_READ_PARAM_VEHICLE_HANDLE() // read and validate vehicle handle

    // for opcodes with mixed params order, where 'strore_to' occurs before input arguments
    OPCODE_READ_PARAM_OUTPUT_VAR_INT() // get pointer to integer variable param to write result later
    OPCODE_READ_PARAM_OUTPUT_VAR_FLOAT() // get pointer to float variable param to write result later
    OPCODE_READ_PARAM_OUTPUT_VAR_ANY32() // get pointer to simple-type variable param to write result later
    OPCODE_READ_PARAM_OUTPUT_VAR_STRING() // returns instance of StringParamBufferInfo used to write string param later

    // writing opcode output/result data
    OPCODE_WRITE_PARAM_BOOL(value)
    OPCODE_WRITE_PARAM_INT8(value)
    OPCODE_WRITE_PARAM_UINT8(value)
    OPCODE_WRITE_PARAM_INT16(value)
    OPCODE_WRITE_PARAM_UINT16(value)
    OPCODE_WRITE_PARAM_INT(value)
    OPCODE_WRITE_PARAM_UINT(value)
    OPCODE_WRITE_PARAM_FLOAT(value)
    OPCODE_WRITE_PARAM_ANY32(value) // write raw data into simple-type variable (practically integers and floats)
    OPCODE_WRITE_PARAM_STRING(value)
    OPCODE_WRITE_PARAM_STRING_INFO(info, value) // write param using info object revceived from
    OPCODE_READ_PARAM_OUTPUT_VAR_STRING OPCODE_WRITE_PARAM_PTR(value) // memory address
    */

    static bool IsLegacyScript(CLEO::CRunningScript* thread)
    {
        return CLEO_GetScriptVersion(thread) < CLEO_VER_5;
    }

    static std::string StringPrintf(const char* format, ...)
    {
        va_list args;

        va_start(args, format);
        auto len = std::vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (len <= 0) return {}; // empty or encoding error

        std::string result;
        result.resize(len);

        va_start(args, format);
        std::vsnprintf(result.data(), result.length() + 1, format, args);
        va_end(args);

        return std::move(result);
    }

    static void StringAppendPrintf(std::string& dest, const char* format, ...)
    {
        va_list args;

        va_start(args, format);
        auto len = std::vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (len <= 0) return; // empty or encoding error

        size_t oriSize = dest.size();
        dest.resize(dest.size() + len);

        va_start(args, format);
        std::vsnprintf(dest.data() + oriSize, len + 1, format, args);
        va_end(args);
    }

    static void StringAppendNum(std::string& dest, int number, int padLen = 0)
    {
        static char buff[16];

        if (number < 0)
        {
            dest.push_back('-');
            padLen--;
            number = -number;
        }

        int i = 0;
        do
        {
            buff[i] = '0' + (number % 10);
            number /= 10;
            i++;
            padLen--;
        } while (number != 0 || padLen > 0);

        dest.reserve(dest.length() + i);
        while (i > 0)
        {
            i--;
            dest.push_back(buff[i]);
        }
    }

    static void StringAppendHex(std::string& dest, DWORD number, int padLen = 0)
    {
        static const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        static char buff[16];

        int i = 0;
        do
        {
            buff[i] = digits[number & 0xF];
            number >>= 4; // 4 bits peer hex digit
            i++;
            padLen--;
        } while (number != 0 || padLen > 0);

        dest.reserve(dest.length() + i);
        while (i > 0)
        {
            i--;
            dest.push_back(buff[i]);
        }
    }

    static void StringAppendFloat(std::string& dest, float number, int padLen = 0)
    {
        static char buff[32];

        int len = sprintf_s(buff, "%f", number);

        // cut trailing zeros
        if (len > 2)
        {
            int posNonZero = -1;
            for (auto i = len - 1; i >= 0; i--)
            {
                if (buff[i] == '.')
                {
                    if (posNonZero != -1)
                        len = posNonZero;
                    else
                        len = i + 1; // keep one zero after decimal point
                    len++;           // index to length
                    buff[len] = '\0';
                    break;
                }

                if (posNonZero == -1 && buff[i] != '0') posNonZero = i;
            }
        }

        padLen -= len;
        while (padLen > 0)
        {
            dest += ' ';
            padLen--;
        }

        dest += buff;
    }

    static bool StringStartsWith(const std::string_view str, const std::string_view prefix, bool caseSensitive = true)
    {
        if (str.length() < prefix.length())
        {
            return false;
        }

        if (caseSensitive)
        {
            return strncmp(str.data(), prefix.data(), prefix.length()) == 0;
        }
        else
        {
            return _strnicmp(str.data(), prefix.data(), prefix.length()) == 0;
        }
    }

    static bool StringEndsWith(const std::string_view str, const std::string_view suffix, bool caseSensitive = true)
    {
        if (str.length() < suffix.length())
        {
            return false;
        }

        if (caseSensitive)
        {
            return strncmp(&str.back() + 1 - suffix.length(), suffix.data(), suffix.length()) == 0;
        }
        else
        {
            return _strnicmp(&str.back() + 1 - suffix.length(), suffix.data(), suffix.length()) == 0;
        }
    }

    static void StringSplit(
        const std::string_view str, const std::string_view delimiters, std::vector<std::string>& output
    )
    {
        size_t prevPos = 0;
        while (true)
        {
            size_t pos = str.find_first_of(delimiters, prevPos);

            if (pos == std::string::npos)
            {
                if (strlen(str.data() + prevPos) > 0)
                {
                    output.emplace_back(str.data() + prevPos);
                }
                break;
            }
            else
            {
                if (pos - prevPos > 0)
                {
                    output.emplace_back(str.data() + prevPos, pos - prevPos);
                }
                prevPos = pos + 1;
            }
        }
    }

    // erase GTA's text formatting sequences like ~r~
    static void StringRemoveFormatting(std::string& str)
    {
        size_t pos = 0;
        while (true)
        {
            pos = str.find('~', pos);
            if (pos == std::string::npos ||
                (pos + 2) >= str.length()) // not enought characters left for formatting sequence
            {
                break;
            }

            if (str[pos + 2] != '~')
            {
                pos++;
                continue;
            }

            switch (str[pos + 1])
            {
            case 'n':
            case 'N':
                str.replace(pos, 3, "\n");
                break;

            default:
                str.erase(pos, 3);
                break;
            }
        }
    }

    static void StringToLower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
    }

    // remove white characters from left hand side of the string
    static void StringTrimLeft(std::string& str)
    {
        auto it = std::find_if(str.begin(), str.end(), [](char c) {
            return c != ' ' && c != '\t' && c != '\v' && c != '\r' && c != '\n' && c != '\f';
        });
        str.erase(str.begin(), it);
    }

    // remove white characters from right hand side of the string
    static void StringTrimRight(std::string& str)
    {
        auto it = std::find_if(str.rbegin(), str.rend(), [](char c) {
            return c != ' ' && c != '\t' && c != '\v' && c != '\r' && c != '\n' && c != '\f';
        });
        str.erase(it.base(), str.end());
    }

    // remove white characters at each end of the string
    static void StringTrim(std::string& str)
    {
        StringTrimRight(str);
        StringTrimLeft(str);
    }

    static std::string ScriptInfoStr(CLEO::CRunningScript* thread)
    {
        std::string info(1024, '\0');
        CLEO_GetScriptInfoStr(thread, true, info.data(), info.length());
        return std::move(info);
    }

    // Normalize filepath, collapse all parent directory references, trim path separators at front and back. Input
    // should be path without expandable %variables%
    static void FilepathNormalize(std::string& path, bool normalizeCase = true)
    {
        if (path.empty()) return;

        std::replace(path.begin(), path.end(), '/', '\\');

        // remove doubled separators
        size_t pos;
        while ((pos = path.find("\\\\")) != std::string::npos)
        {
            path.erase(pos, 1);
        }

        if (normalizeCase) StringToLower(path);

        // collapse references to parent directory
        const auto ParentRef    = "\\..\\";
        const auto ParentRefLen = 4;

        size_t refPos = path.find(ParentRef);
        while (refPos != std::string::npos && refPos > 0)
        {
            size_t parentPos = path.rfind('\\', refPos - 1); // find start of parent dir name

            if (parentPos == std::string::npos) // no more separators, so parent has to be root dir
            {
                parentPos = 0;
                refPos += 1; // remove following separator too
            }

            if (_strnicmp(path.c_str() + parentPos, "..\\", 3) == 0)
            {
                break; // parent directory is reference to parent directory too
            }

            path.replace(
                parentPos, (refPos - parentPos) + ParentRefLen - 1, ""
            ); // remove parent dir along with following \\..

            refPos = path.find(ParentRef); // find next
        }

        // trim separators
        while (path.front() == '\\')
            path.erase(0, 1);
        while (path.back() == '\\')
            path.pop_back();
    }

    // strip parent prefix from filepath if present
    // 1. FilepathRemoveParent("C:\game\cleo\1.cs", "C:\game") => cleo\1.cs
    // 2. FilepathRemoveParent("C:cleo\1.cs", "C:") => cleo\1.cs
    static void FilepathRemoveParent(std::string& path, const std::string_view base)
    {
        if (path.length() < base.length()) return; // can not hold that prefix
        if (!StringStartsWith(path, base, false)) return;
        if (path.length() > base.length() && path[base.length()] != '\\' && path[base.length() - 1] != ':')
            return; // just similar base

        auto to = base.length();
        if (path[to] == '\\')
        {
            // remove path separator too if present
            to += 1;
        }
        path.replace(0, to, "");
    }

    // get path without last file/directory element
    static const std::string_view FilepathGetParent(const std::string_view str)
    {
        auto separatorPos = str.find_last_of('\\');

        if (separatorPos == std::string::npos)
        {
            return {};
        }

        return std::string_view(str.data(), separatorPos);
    }

    // this plugin's config file
    static std::string GetConfigFilename()
    {
        std::string path = CLEO_GetGameDirectory();
        path += "\\cleo\\cleo_plugins\\";
        path += TARGET_NAME;
        path += ".ini";
        return path;
    }

    // does normalized file path points inside game directories? (game root or user files)
    static bool FilepathIsSafe(CLEO::CRunningScript* thread, const char* path)
    {
        if (strchr(path, '%') != nullptr)
        {
            return false; // do not allow paths containing expandable variables
        }

        std::string absolute;
        if (!std::filesystem::path(path).is_absolute())
        {
            absolute = CLEO_GetScriptWorkDir(thread);
            if (!absolute.empty())
            {
                absolute += '\\';
                absolute += path;
                FilepathNormalize(absolute, false);
                path = absolute.c_str();
            }
        }

        if (!StringStartsWith(path, CLEO_GetGameDirectory(), false) &&
            !StringStartsWith(path, CLEO_GetUserDirectory(), false))
        {
            if (StringStartsWith(path, "..")) // relative path trying to escape game's directory
                return false;
        }

        return true;
    }

    static bool IsObjectHandleValid(DWORD handle)
    {
        // get handle info
        auto flags = handle & 0xFF;
        auto index = handle >> 8;

        if (index >= (DWORD)CPools::ms_pObjectPool->m_nSize) return false; // index out of bounds

        if (CPools::ms_pObjectPool->m_byteMap[index].IntValue() != flags) return false; // flags mismatch

        return true;
    }

    static bool IsPedHandleValid(DWORD handle)
    {
        // get handle info
        auto flags = handle & 0xFF;
        auto index = handle >> 8;

        if (index >= (DWORD)CPools::ms_pPedPool->m_nSize) return false; // index out of bounds

        if (CPools::ms_pPedPool->m_byteMap[index].IntValue() != flags) return false; // flags mismatch

        return true;
    }

    static bool IsVehicleHandleValid(DWORD handle)
    {
        // get handle info
        auto flags = handle & 0xFF;
        auto index = handle >> 8;

        if (index >= (DWORD)CPools::ms_pVehiclePool->m_nSize) return false; // index out of bounds

        if (CPools::ms_pVehiclePool->m_byteMap[index].IntValue() != flags) return false; // flags mismatch

        return true;
    }

    static bool IsPlayerIdValid(int id)
    {
        switch (id)
        {
        case -1: // player in focus
        case 0:  // player 1
        case 1:  // player 2
            return true;
        }

        return false;
    }

    static const char* TraceVArg(CLEO::eLogLevel level, const char* format, va_list args)
    {
        static char szBuf[1024];
        vsprintf_s(szBuf, format, args); // put params into format
        CLEO_Log(level, szBuf);
        return szBuf;
    }

    static void Trace(CLEO::eLogLevel level, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        TraceVArg(level, format, args);
        va_end(args);
    }

    static void Trace(const CLEO::CRunningScript* thread, CLEO::eLogLevel level, const char* format, ...)
    {
        if (thread != nullptr && CLEO_GetScriptVersion(thread) < CLEO::eCLEO_Version::CLEO_VER_5)
        {
            return; // do not log this in older versions
        }

        va_list args;
        va_start(args, format);
        TraceVArg(level, format, args);
        va_end(args);
    }

    static void ShowError(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        auto msg = TraceVArg(CLEO::eLogLevel::Error, format, args);
        va_end(args);

        auto mainWnd    = (HWND*)0x00C8CF88; // PluginSDK: RsGlobal.ps->window
        auto style      = GetWindowLong(*mainWnd, GWL_STYLE);
        bool fullscreen = (style & (WS_BORDER | WS_CAPTION)) == 0;

        if (fullscreen)
        {
            PostMessage(*mainWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
            ShowWindow(*mainWnd, SW_MINIMIZE);
        }

        auto caption = StringPrintf("CLEO v%s", CLEO_GetVersionStr());
        MessageBoxA(NULL, msg, caption.c_str(), MB_SYSTEMMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);

        if (fullscreen)
        {
            PostMessage(*mainWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            ShowWindow(*mainWnd, SW_RESTORE);
        }
    }

    static bool PluginCheckCleoVersion()
    {
        auto ver = CLEO_GetVersion();

        if (ver < CLEO_VERSION)
        {
            ShowError(
                "%s.cleo plugin requires CLEO.asi version %s or later! \nCurrent version is %s", TARGET_NAME,
                CLEO_VERSION_STR, CLEO_GetVersionStr()
            );
            return false;
        }

        return true;
    }

    static std::string GetParamInfo(int offset = 0)
    {
        std::string info;
        info.resize(32);
        CLEO_GetScriptParamInfoStr(offset, info.data(), info.length());
        return info;
    }

    struct StringParamBufferInfo
    {
        char* data          = nullptr;
        int size            = 0;
        BOOL needTerminator = false;
    };

    class MemPatch
    {
        void* address = nullptr;
        std::vector<char> buffer;

      public:
        MemPatch() = default;

        MemPatch(void* src, size_t size) : address(src), buffer(size) { memcpy(buffer.data(), src, size); }

        inline void Apply() const
        {
            if (!buffer.empty())
            {
                memcpy(address, buffer.data(), buffer.size());
            }
        }

        inline void* GetAddress() const { return address; }
    };

    static MemPatch MemPatchJump(size_t position, void* jumpTarget)
    {
        MemPatch original((void*)position, 5);

        *(BYTE*)position = 0xE9; // asm: jmp
        position += sizeof(BYTE);

        *(DWORD*)position = (DWORD)jumpTarget - position - 4;

        return original;
    }

    static void* MemPatchCall(size_t position, void* newFunction)
    {
        *(BYTE*)position = 0xE8; // asm: call
        position += sizeof(BYTE);

        DWORD original    = *(DWORD*)position + position + 4;
        *(DWORD*)position = (DWORD)newFunction - position - 4;

        return (void*)original;
    }

    template <typename T> static StringList CreateStringList(const T& container)
    {
        StringList result;
        result.count   = 0;
        result.strings = nullptr;

        if (container.size() > 0)
        {
            result.strings = (char**)malloc(container.size() * sizeof(DWORD)); // array of pointers
            for (const std::string& s : container)
            {
                auto size = s.length() + 1; // and terminator character
                auto str  = (char*)malloc(size);
                memcpy(str, s.c_str(), size);

                result.strings[result.count] = str;
                result.count++;
            }
        }

        return result;
    }

#define TRACE(format, ...)                                                                                             \
    {                                                                                                                  \
        CLEO::Trace(CLEO::eLogLevel::Default, format, __VA_ARGS__);                                                    \
    }
#define LOG_WARNING(script, format, ...)                                                                               \
    {                                                                                                                  \
        CLEO::Trace(script, CLEO::eLogLevel::Error, format, __VA_ARGS__);                                              \
    }
#define SHOW_ERROR(a, ...)                                                                                             \
    {                                                                                                                  \
        CLEO::ShowError(a, __VA_ARGS__);                                                                               \
    }

#define COMPAT_MODE_TIP(MSG) MSG##" \n\nThis error can be ignored in legacy mode by changing script extension to '.cs4'"
#define SHOW_ERROR_COMPAT(a, ...)                                                                                      \
    {                                                                                                                  \
        CLEO::ShowError(COMPAT_MODE_TIP(a), __VA_ARGS__);                                                              \
    }

    const size_t MinValidAddress =
        0x10000; // used for validation of pointers received from scripts. First 64kb are for sure reserved by Windows.
#define OPCODE_VALIDATE_POINTER(x)                                                                                     \
    if ((size_t)x <= MinValidAddress)                                                                                  \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid '0x%X' pointer argument in script %s \nScript suspended.", x, ScriptInfoStr(thread).c_str()       \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_CONDITION_RESULT(value) CLEO_SetThreadCondResult(thread, value);

    // opcode param handling utils internal
    static SCRIPT_VAR* _paramsArray       = nullptr;
    static eDataType _lastParamType       = eDataType::DT_END;
    static eArrayType _lastParamArrayType = eArrayType::AT_NONE;

    static SCRIPT_VAR& _readParam(CRunningScript* thread)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        CLEO_RetrieveOpcodeParams(thread, 1);
        if (_paramsArray == nullptr) _paramsArray = CLEO_GetOpcodeParamsArray();
        return _paramsArray[0];
    }

    static SCRIPT_VAR& _readParamFloat(CRunningScript* thread)
    {
        auto& var = _readParam(thread);

        // people tend to use '0' instead '0.0' when providing literal float params in scripts
        // binary these are equal, so can be allowed
        if (var.dwParam == 0)
        {
            // pretend it was float type
            if (IsImmInteger(_lastParamType)) _lastParamType = eDataType::DT_FLOAT;
            if (_lastParamArrayType == eArrayType::AT_INT) _lastParamArrayType = eArrayType::AT_FLOAT;
        }

        return var;
    }

    static SCRIPT_VAR* _readParamVariable(CRunningScript* thread)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        return CLEO_GetPointerToScriptVariable(thread);
    }

    static StringParamBufferInfo _readParamStringInfo(CRunningScript* thread)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        StringParamBufferInfo result;
        CLEO_ReadStringParamWriteBuffer(thread, &result.data, &result.size, &result.needTerminator);
        return result;
    }

    static void _writeParamPtr(CRunningScript* thread, void* valuePtr)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        if (_paramsArray == nullptr) _paramsArray = CLEO_GetOpcodeParamsArray();
        _paramsArray[0].pParam = valuePtr;
        CLEO_RecordOpcodeParams(thread, 1);
    }

    template <typename T> static void _writeParam(CRunningScript* thread, T value)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        if (_paramsArray == nullptr) _paramsArray = CLEO_GetOpcodeParamsArray();
        _paramsArray[0].dwParam = 0;
        memcpy(&_paramsArray[0], &value, sizeof(T));
        CLEO_RecordOpcodeParams(thread, 1);
    }

    static inline bool _paramWasInt(bool output = false)
    {
        if (_lastParamArrayType != eArrayType::AT_NONE) return _lastParamArrayType == eArrayType::AT_INT;
        if (IsVariable(_lastParamType)) return true;
        if (!output && IsImmInteger(_lastParamType)) return true;
        return false;
    }

    static inline bool _paramWasFloat(bool output = false)
    {
        if (_lastParamArrayType != eArrayType::AT_NONE) return _lastParamArrayType == eArrayType::AT_FLOAT;
        if (IsVariable(_lastParamType)) return true;
        if (!output && IsImmFloat(_lastParamType)) return true;
        return false;
    }

    static inline bool _paramWasString(bool output = false)
    {
        if (_lastParamArrayType != eArrayType::AT_NONE)
        {
            return _lastParamArrayType == eArrayType::AT_STRING || _lastParamArrayType == eArrayType::AT_TEXTLABEL ||
                   _lastParamArrayType == eArrayType::AT_INT; // pointer to output buffer
        }

        if (IsVarString(_lastParamType)) return true;
        if (output && IsImmInteger(_lastParamType)) return true; // allow writing strings into const addresses
        if (!output && IsImmString(_lastParamType)) return true;

        // pointer to output buffer
        if (IsVariable(_lastParamType)) return true;

        return false;
    }

    static inline bool _paramWasVariable()
    {
        return IsVariable(_lastParamType);
    }

    static const char* _readParamText(CRunningScript* thread, char* buffer, size_t bufferSize)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        if (!_paramWasString())
        {
            SHOW_ERROR(
                "Input argument %s expected to be string, got %s in script %s\nScript suspended.",
                GetParamInfo(1).c_str(), ToKindStr(_lastParamType, _lastParamArrayType), ScriptInfoStr(thread).c_str()
            );
            thread->Suspend();
            _lastParamType = DT_INVALID; // mark error
            return nullptr;
        }

        auto str = CLEO_ReadStringPointerOpcodeParam(
            thread, buffer, bufferSize
        ); // returns pointer to source data whenever possible

        if (str == nullptr) // reading string failed
        {
            auto isVariableInt = IsVariable(_lastParamType) && (_lastParamArrayType == eArrayType::AT_NONE ||
                                                                _lastParamArrayType == eArrayType::AT_INT);
            if ((IsImmInteger(_lastParamType) || isVariableInt) && // pointer argument type?
                CLEO_GetOpcodeParamsArray()->dwParam <= MinValidAddress)
            {
                SHOW_ERROR(
                    "Invalid '0x%X' pointer of input string argument %s in script %s",
                    CLEO_GetOpcodeParamsArray()->dwParam, GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()
                );
            }
            else
            {
                // other error
                SHOW_ERROR(
                    "Invalid input argument %s in script %s\nScript suspended.", GetParamInfo().c_str(),
                    ScriptInfoStr(thread).c_str()
                );
            }

            thread->Suspend();
            _lastParamType = DT_INVALID; // mark error
            return nullptr;
        }

        return str;
    }

    static bool _writeParamText(CLEO::CRunningScript* thread, const StringParamBufferInfo& target, const char* str)
    {
        if (str != nullptr && (size_t)str <= MinValidAddress)
        {
            SHOW_ERROR(
                "Invalid '0x%X' source pointer of output string argument %s in script %s \nScript suspended.", str,
                GetParamInfo(1).c_str(), ScriptInfoStr(thread).c_str()
            );
            thread->Suspend();
            return false;
        }

        if ((size_t)target.data <= MinValidAddress)
        {
            SHOW_ERROR(
                "Invalid '0x%X' target pointer of output string argument in script %s \nScript suspended.", target.data,
                ScriptInfoStr(thread).c_str()
            );
            thread->Suspend();
            return false;
        }

        if (target.size == 0)
        {
            return true; // done
        }

        bool addTerminator = target.needTerminator;
        size_t buffLen     = target.size - addTerminator;
        size_t length      = str == nullptr ? 0 : strlen(str);

        if (buffLen > length) addTerminator = true; // there is space left for terminator

        length = std::min<size_t>(length, buffLen);
        if (length > 0) std::memcpy(target.data, str, length);
        if (addTerminator) target.data[length] = '\0';

        return true;
    }

    static bool _writeParamText(CRunningScript* thread, const char* str)
    {
        _lastParamType      = thread->PeekDataType();
        _lastParamArrayType = thread->PeekArrayType();

        if (str != nullptr && (size_t)str <= MinValidAddress)
        {
            SHOW_ERROR(
                "Invalid '0x%X' source pointer of output string argument %s in script %s \nScript suspended.", str,
                GetParamInfo(1).c_str(), ScriptInfoStr(thread).c_str()
            );
            thread->Suspend();
            return false;
        }

        if (!_paramWasString(true))
        {
            SHOW_ERROR(
                "Output argument %s expected to be variable string, got %s in script %s\nScript suspended.",
                GetParamInfo(1).c_str(), ToKindStr(_lastParamType, _lastParamArrayType), ScriptInfoStr(thread).c_str()
            );
            thread->Suspend();
            return false;
        }

        if (IsImmInteger(_lastParamType) || IsVariable(_lastParamType)) // pointer to buffer
        {
            auto ptr = CLEO_PeekIntOpcodeParam(thread);

            if ((size_t)ptr <= MinValidAddress)
            {
                SHOW_ERROR(
                    "Invalid '0x%X' pointer of output string argument %s in script %s \nScript suspended.", ptr,
                    GetParamInfo(1).c_str(), ScriptInfoStr(thread).c_str()
                );
                thread->Suspend();
                return false;
            }
        }

        StringParamBufferInfo info;
        CLEO_ReadStringParamWriteBuffer(thread, &info.data, &info.size, &info.needTerminator);
        return _writeParamText(thread, info, str); // done
    }

#define OPCODE_SKIP_PARAMS(_count) CLEO_SkipOpcodeParams(thread, _count)
#define OPCODE_SKIP_VARARG_PARAMS() CLEO_SkipUnusedVarArgs(thread)

#define OPCODE_PEEK_PARAM_TYPE() thread->PeekDataType()
#define OPCODE_PEEK_VARARG_COUNT() CLEO_GetVarArgCount(thread)

    // macros for reading opcode input params. Performs type validation, throws error and suspends script if user
    // provided invalid argument type TOD: add range checks for limited size types?

#define OPCODE_READ_PARAM_BOOL()                                                                                       \
    _readParam(thread).dwParam != false;                                                                               \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_INT8()                                                                                       \
    _readParam(thread).cParam;                                                                                         \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_UINT8()                                                                                      \
    _readParam(thread).ucParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_INT16()                                                                                      \
    _readParam(thread).wParam;                                                                                         \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_UINT16()                                                                                     \
    _readParam(thread).usParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_INT()                                                                                        \
    _readParam(thread).nParam;                                                                                         \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_UINT()                                                                                       \
    _readParam(thread).dwParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_FLOAT()                                                                                      \
    _readParamFloat(thread).fParam;                                                                                    \
    if (!IsLegacyScript(thread) && !_paramWasFloat())                                                                  \
    {                                                                                                                  \
        SHOW_ERROR_COMPAT(                                                                                             \
            "Input argument %s expected to be float, got %s in script %s\nScript suspended.", GetParamInfo().c_str(),  \
            CLEO::ToKindStr(_lastParamType, _lastParamArrayType), CLEO::ScriptInfoStr(thread).c_str()                  \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_ANY32()                                                                                      \
    _readParam(thread);                                                                                                \
    if (!_paramWasInt() && !_paramWasFloat())                                                                          \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be int or float, got %s in script %s\nScript suspended.",                   \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_STRING(_varName)                                                                             \
    char _buff_##_varName[MAX_STR_LEN + 1];                                                                            \
    const char*##_varName = _readParamText(thread, _buff_##_varName, MAX_STR_LEN + 1);                                 \
    if (!_paramWasString())                                                                                            \
    {                                                                                                                  \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    }

#define OPCODE_READ_PARAM_STRING_LEN(_varName, _maxLen)                                                                \
    char _buff_##_varName[_maxLen + 1];                                                                                \
    const char*##_varName = _readParamText(thread, _buff_##_varName, _maxLen + 1);                                     \
    if (##_varName != nullptr) ##_varName = _buff_##_varName;                                                          \
    if (!_paramWasString())                                                                                            \
    {                                                                                                                  \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    }

#define OPCODE_READ_PARAM_STRING_FORMATTED(_varName)                                                                   \
    char _buff_format_##_varName[MAX_STR_LEN + 1];                                                                     \
    const char* _format_##_varName = _readParamText(thread, _buff_format_##_varName, MAX_STR_LEN + 1);                 \
    if (!_paramWasString())                                                                                            \
    {                                                                                                                  \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    }                                                                                                                  \
    char _varName[2 * MAX_STR_LEN + 1];                                                                                \
    char* _varName##Ok = CLEO_ReadParamsFormatted(thread, _buff_format_##_varName, _varName, sizeof(_varName));        \
    if (_varName##Ok == nullptr)                                                                                       \
    {                                                                                                                  \
        SHOW_ERROR("Invalid formatted string in script %s \nScript suspended.", CLEO::ScriptInfoStr(thread).c_str());  \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAMS_FORMATTED(_format, _varName)                                                                \
    char _varName[2 * MAX_STR_LEN + 1];                                                                                \
    char* _varName##Ok = CLEO_ReadParamsFormatted(thread, _format, _varName, sizeof(_varName));                        \
    if (_varName##Ok == nullptr)                                                                                       \
    {                                                                                                                  \
        SHOW_ERROR("Invalid formatted string in script %s \nScript suspended.", CLEO::ScriptInfoStr(thread).c_str());  \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_FILEPATH(_varName)                                                                           \
    char _buff_##_varName[512];                                                                                        \
    const char*##_varName = _readParamText(thread, _buff_##_varName, 512);                                             \
    if (##_varName != nullptr) ##_varName = _buff_##_varName;                                                          \
    if (_paramWasString())                                                                                             \
        CLEO_ResolvePath(thread, _buff_##_varName, 512);                                                               \
    else                                                                                                               \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    if (!FilepathIsSafe(thread, ##_varName))                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Forbidden file path '%s' outside game directories in script %s \nScript suspended.", ##_varName,          \
            ScriptInfoStr(thread).c_str()                                                                              \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_PTR()                                                                                        \
    _readParam(thread).pParam;                                                                                         \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    else if (_paramsArray[0].dwParam <= MinValidAddress)                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid pointer '0x%X' input argument %s in script %s \nScript suspended.", _paramsArray[0].dwParam,      \
            GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()                                                      \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_OBJECT_HANDLE()                                                                              \
    _readParam(thread).dwParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    else if (!IsObjectHandleValid(_paramsArray[0].dwParam))                                                            \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid object handle '0x%X' input argument %s in script %s \nScript suspended.",                         \
            _paramsArray[0].dwParam, GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()                             \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_PED_HANDLE()                                                                                 \
    _readParam(thread).dwParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    else if (!IsPedHandleValid(_paramsArray[0].dwParam))                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid character handle '0x%X' input argument %s in script %s \nScript suspended.",                      \
            _paramsArray[0].dwParam, GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()                             \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_VEHICLE_HANDLE()                                                                             \
    _readParam(thread).dwParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    else if (!IsVehicleHandleValid(_paramsArray[0].dwParam))                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid vehicle handle '0x%X' input argument %s in script %s \nScript suspended.",                        \
            _paramsArray[0].dwParam, GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()                             \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_PLAYER_ID()                                                                                  \
    _readParam(thread).dwParam;                                                                                        \
    if (!_paramWasInt())                                                                                               \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Input argument %s expected to be integer, got %s in script %s\nScript suspended.",                        \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    else if (!IsPlayerIdValid(_paramsArray[0].dwParam))                                                                \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Invalid player id '0x%X' input argument %s in script %s \nScript suspended.", _paramsArray[0].dwParam,    \
            GetParamInfo().c_str(), ScriptInfoStr(thread).c_str()                                                      \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_OUTPUT_VAR_ANY32()                                                                           \
    _readParamVariable(thread);                                                                                        \
    if (!_paramWasVariable())                                                                                          \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int or float, got %s in script %s\nScript suspended.",         \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_OUTPUT_VAR_INT()                                                                             \
    (int*)_readParamVariable(thread);                                                                                  \
    if (!_paramWasVariable())                                                                                          \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_OUTPUT_VAR_FLOAT()                                                                           \
    (float*)_readParamVariable(thread);                                                                                \
    if (!_paramWasVariable())                                                                                          \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable float, got %s in script %s\nScript suspended.",                \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }                                                                                                                  \
    if (!IsLegacyScript(thread) && !_paramWasFloat(true))                                                              \
    {                                                                                                                  \
        SHOW_ERROR_COMPAT(                                                                                             \
            "Output argument %s expected to be variable float, got %s in script %s\nScript suspended.",                \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_READ_PARAM_OUTPUT_VAR_STRING()                                                                          \
    _readParamStringInfo(thread);                                                                                      \
    if (!_paramWasString(true))                                                                                        \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable string, got %s in script %s\nScript suspended.",               \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

    // macros for writing opcode output params. Performs type validation, throws error and suspends script if user
    // provided invalid argument type

#define OPCODE_WRITE_PARAM_BOOL(_value)                                                                                \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_INT8(_value)                                                                                \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_UINT8(_value)                                                                               \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_INT16(_value)                                                                               \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_UINT16(_value)                                                                              \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_INT(_value)                                                                                 \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_UINT(_value)                                                                                \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_ANY32(_value)                                                                               \
    _writeParam(thread, _value);                                                                                       \
    if (!_paramWasInt(true) && !_paramWasFloat(true))                                                                  \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be int or float variable, got %s in script %s\nScript suspended.",         \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_FLOAT(_value)                                                                               \
    _writeParam(thread, _value);                                                                                       \
    if (!IsLegacyScript(thread) && !_paramWasFloat(true))                                                              \
    {                                                                                                                  \
        SHOW_ERROR_COMPAT(                                                                                             \
            "Output argument %s expected to be variable float, got %s in script %s\nScript suspended.",                \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }

#define OPCODE_WRITE_PARAM_STRING(_value)                                                                              \
    if (!_writeParamText(thread, _value))                                                                              \
    {                                                                                                                  \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    }

#define OPCODE_WRITE_PARAM_VAR_STRING(_info, _value)                                                                   \
    if (!_writeParamText(thread, _info, _value))                                                                       \
    {                                                                                                                  \
        return OpcodeResult::OR_INTERRUPT;                                                                             \
    }

#define OPCODE_WRITE_PARAM_PTR(_value)                                                                                 \
    _writeParamPtr(thread, (void*)_value);                                                                             \
    if (!_paramWasInt(true))                                                                                           \
    {                                                                                                                  \
        SHOW_ERROR(                                                                                                    \
            "Output argument %s expected to be variable int, got %s in script %s\nScript suspended.",                  \
            GetParamInfo().c_str(), CLEO::ToKindStr(_lastParamType, _lastParamArrayType),                              \
            CLEO::ScriptInfoStr(thread).c_str()                                                                        \
        );                                                                                                             \
        return thread->Suspend();                                                                                      \
    }
} // namespace CLEO
