#include "stdafx.h"
#include "CDebug.h"
#include "CleoBase.h"

CDebug Debug;
using namespace CLEO;

void CDebug::Trace(eLogLevel level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    TraceVArg(level, format, args);
    va_end(args);
}

const char* CDebug::TraceVArg(CLEO::eLogLevel level, const char* format, va_list args)
{
    std::lock_guard<std::mutex> guard(mutex);

    static char szBuf[1024];

    // time stamp
    SYSTEMTIME t;
    GetLocalTime(&t);
    sprintf(szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    char* stampEnd = szBuf + strlen(szBuf);

    // put params into format
    vsprintf(stampEnd, format, args);

    // output to file
    if(m_hFile.good())
        m_hFile << szBuf << std::endl;

    // output to console
#ifdef _DEBUG
    OutputDebugString(szBuf);
    OutputDebugString("\n");
#endif

    // output to callbacks
    auto& cleo = GetInstance();
    if (cleo.IsStarted())
    {
        for (void* func : cleo.GetCallbacks(eCallbackId::Log))
        {
            typedef void WINAPI callback(eLogLevel, const char*);
            ((callback*)func)(level, stampEnd);
        }
    }

    return stampEnd;
}

void CDebug::Error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    auto msg = TraceVArg(eLogLevel::Error, format, args);
    va_end(args);

    auto mainWnd = GetInstance().MainWnd;
    PostMessage(mainWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    ShowWindow(mainWnd, SW_MINIMIZE);
    MessageBox(mainWnd, msg, "CLEO error", MB_SYSTEMMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);
    PostMessage(mainWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    ShowWindow(mainWnd, SW_RESTORE);
}

extern "C" 
{
    void WINAPI CLEO_Log(eLogLevel level, const char* msg)
    {
        Debug.Trace(level, "%s", msg);
    }
}
