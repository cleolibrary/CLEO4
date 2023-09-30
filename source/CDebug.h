#pragma once
#include <deque>
#include <mutex>
#include <string>
#include <chrono>

#define TRACE __noop

#ifdef DEBUGIT
#undef TRACE
#define TRACE(a,...) {Debug.Trace(a, __VA_ARGS__);}
#endif

enum class LogLevel
{
    None,
    Error,
    User,
    Default
};

const char szLogFileName[] = "cleo.log";

class ScreenLog
{
    std::mutex mutex;

public:
    void Init();
    void Add(const char* msg, LogLevel level = LogLevel::Default);
    void Draw();

private:
    LogLevel level;
    size_t maxMessages;
    float fontSize;

    const RGBA fontColor[4] = {
        RGBA(0xDD, 0xDD, 0xDD, 0xEE), // None
        RGBA(0xFF, 0x30, 0x30, 0xEE), // Error
        RGBA(0xFF, 0xEE, 0x30, 0xEE), // User
        RGBA(0xDD, 0xDD, 0xDD, 0xEE), // Default
    };

    std::chrono::milliseconds timeDisplay = std::chrono::milliseconds(6000);
    std::chrono::milliseconds timeFadeout = std::chrono::milliseconds(3000);

    struct Entry
    {
        LogLevel level;
        std::string msg;
        std::chrono::system_clock::time_point time;

        Entry() :
            level(LogLevel::Default),
            msg(""),
            time(std::chrono::system_clock::now())
        {
        }

        Entry(const char* message, LogLevel level = LogLevel::Default) :
            level(level),
            msg(message),
            time(std::chrono::system_clock::now())
        {
        }
    };
    std::deque<Entry> entries;
};

class CDebug
{
#ifdef DEBUGIT
    std::ofstream m_hFile;

public:
    CDebug();
    ~CDebug();

    void Trace(const char* format, ...);
    void Trace(LogLevel level, const char* format, ...);
    void Trace(LogLevel level, const char* format, va_list args);
    
    void Draw() { screenLog.Draw(); }

private:
    ScreenLog screenLog;
#endif
};

extern CDebug Debug;
void Warning(const char *);
void Error(const char *);
