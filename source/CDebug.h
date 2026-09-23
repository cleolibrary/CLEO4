#pragma once
#include "CUacHelper.h"

#define TRACE __noop

#ifdef DEBUGIT
#undef TRACE
#define TRACE(a,...) {Debug.Trace(a, __VA_ARGS__);}
#endif

class CDebug
{
#ifdef DEBUGIT
    std::ofstream m_hFile;
#endif

public:
#ifdef DEBUGIT

    CDebug() : m_hFile(CLEO::CUacHelper::ResolveLogFilePath().c_str())
    {
        Trace("Log started.");
    }

    ~CDebug()
    {
        Trace("Log finished.");
    }

    void Trace(const char *format, ...)
    {
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
