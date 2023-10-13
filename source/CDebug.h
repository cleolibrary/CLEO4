#pragma once
#include <mutex>

#define TRACE(a,...) {Debug.Trace(CLEO::eLogLevel::Default, a, __VA_ARGS__);}
#define LOG_WARNING(a,...) {Debug.Trace(CLEO::eLogLevel::Error, a, __VA_ARGS__);}
#define SHOW_ERROR(a,...) {Debug.Error(a, __VA_ARGS__);}

const char szLogFileName[] = "cleo.log";

class CDebug
{
public:
    CDebug() : m_hFile(szLogFileName)
    {
        Trace(CLEO::eLogLevel::Default, "Log started.");

#ifdef _DEBUG
        Trace(CLEO::eLogLevel::Default, "CLEO v%s DEBUG", CLEO_VERSION_STR);
#else
        Trace(CLEO::eLogLevel::Default, "CLEO v%s", CLEO_VERSION_STR);
#endif
    }

    ~CDebug()
    {
        Trace(CLEO::eLogLevel::Default, "Log finished.");
    }
    
    void Trace(CLEO::eLogLevel level, const char* format, ...);
    void Error(const char* format, ...);
    
private:
    std::mutex mutex;
    std::ofstream m_hFile;
    const char* TraceVArg(CLEO::eLogLevel level, const char* format, va_list args);
};

extern CDebug Debug;
