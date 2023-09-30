#pragma once
#include <mutex>

#define TRACE __noop

#ifdef DEBUGIT
#undef TRACE
#define TRACE(a,...) {Debug.Trace(a, __VA_ARGS__);}
#endif

const char szLogFileName[] = "cleo.log";

class CDebug
{
    std::mutex mutex;

#ifdef DEBUGIT
    std::ofstream m_hFile;
#endif

public:
#ifdef DEBUGIT

    CDebug() : m_hFile(szLogFileName)
    {
        Trace("Log started.");

#ifdef _DEBUG
        Trace("CLEO v%s DEBUG", CLEO_VERSION_DOT_STR);
#elif
        Trace("CLEO v%s", CLEO_VERSION_DOT_STR);
#endif
    }

    ~CDebug()
    {
        Trace("Log finished.");
    }

    void Trace(const char *format, ...)
    {
        std::lock_guard<std::mutex> guard(mutex);

        SYSTEMTIME			t;
        static char			szBuf[1024];

        GetLocalTime(&t);
        sprintf(szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        va_list arg;
        va_start(arg, format);
        vsprintf(szBuf + strlen(szBuf), format, arg);
        va_end(arg);
        m_hFile << szBuf << std::endl;
        OutputDebugString(szBuf);
        OutputDebugString("\n");
    }
#endif
};

extern CDebug Debug;
void Warning(const char *);
void Error(const char *);
