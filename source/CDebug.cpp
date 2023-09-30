#include "stdafx.h"
#include "CDebug.h"
#include "cleo.h"
#include "CFont.h"
#include "CFileMgr.h"

using namespace CLEO;

CDebug Debug;

void Error(const char *szStr)
{
    MessageBox(nullptr, szStr, "CLEO error", MB_ICONERROR | MB_OK);
    Debug.Trace(LogLevel::Error, "[Error] Exiting with error: %s", szStr);
    //exit(1);
}

void Warning(const char *szStr)
{
    MessageBox(nullptr, szStr, "CLEO warning", MB_ICONWARNING | MB_OK);
    Debug.Trace(LogLevel::Error, "[Warning] %s", szStr);
    //exit(1);
}

inline CDebug::CDebug() : m_hFile(szLogFileName)
{
    screenLog.Init();
    Trace("Log started.");
}

inline CDebug::~CDebug()
{
    Trace("Log finished.");
}

void CDebug::Trace(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    Trace(LogLevel::Default, format, args);
    va_end(args);
}

inline void CDebug::Trace(LogLevel level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    Trace(level, format, args);
    va_end(args);
}

void CDebug::Trace(LogLevel level, const char* format, va_list args)
{
    SYSTEMTIME t;
    static char szBuf[1024];

    GetLocalTime(&t);
    sprintf(szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    char* stampEnd = szBuf + strlen(szBuf);

    vsprintf(stampEnd, format, args);

    screenLog.Add(stampEnd, level);

    m_hFile << szBuf << std::endl;

    OutputDebugString(szBuf);
    OutputDebugString("\n");
}

void ScreenLog::Init()
{
    entries.clear();

    // load config file
    std::string configFile = CFileMgr::ms_rootDirName;
    if (!configFile.empty()) configFile += "\\";
    configFile += "cleo\\cleo.ini";

    level = (LogLevel)GetPrivateProfileInt("ScreenLog", "Level", (UINT)LogLevel::None, configFile.c_str());
    maxMessages = GetPrivateProfileInt("ScreenLog", "MessagesMax", 40, configFile.c_str());
    timeDisplay = std::chrono::milliseconds(GetPrivateProfileInt("ScreenLog", "MessageTime", 6000, configFile.c_str()));
    fontSize = 0.01f * GetPrivateProfileInt("ScreenLog", "FontSize", 60, configFile.c_str());
}

void ScreenLog::Add(const char* msg, LogLevel level)
{
    if (level > this->level)
    {
        return;
    }

    entries.emplace_front(msg, level);

    if (entries.size() > maxMessages)
    {
        entries.resize(maxMessages);
    }
}

void ScreenLog::Draw()
{
    // clean up expired entries
    const auto now = std::chrono::system_clock::now();
    const auto maxTime = timeDisplay + timeFadeout;
    for (size_t i = 0; i < entries.size(); i++)
    {
        if ((now - entries[i].time) > maxTime)
        {
            // first expired entry. All following are even older
            entries.resize(i);
        }
    }

    if (entries.empty())
    {
        return; // nothing to print
    }

    CFont::SetBackground(false, false);
    CFont::SetWrapx(99999999.0f); // no line wrap
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetEdge(1);
    CFont::SetProportional(true);

    const float aspect = (float)RsGlobal.maximumWidth / RsGlobal.maximumHeight;
    float sizeX = fontSize * 0.55f * static_cast<float>(RsGlobal.maximumWidth) / 640.0f / aspect;
    float sizeY = fontSize * static_cast<float>(RsGlobal.maximumHeight) / 448.0f;
    CFont::SetScale(sizeX, sizeY);

    CFont::SetOrientation(ALIGN_LEFT);
    float posX = 15.0f * sizeX;
    float posY = 0.0f;

    for (size_t i = 0; i < entries.size(); i++)
    {
        auto& entry = entries[i];

        unsigned char alpha = 255;
        auto elapsed = now - entries[i].time;
        if (elapsed > timeDisplay)
        {
            elapsed -= timeDisplay;
            float fadeProgress = (float)std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / timeFadeout.count();
            std::clamp(fadeProgress, 0.0f, 1.0f);
            fadeProgress = 1.0f - fadeProgress; // fade out
            alpha = 0xFF * fadeProgress;
        }
        
        auto color = fontColor[(size_t)entry.level];
        alpha = min(alpha, color.a);
        color.a = alpha;

        CFont::SetColor(color);

        alpha /= 2; // border is same text rendered many times
        CFont::SetDropColor(CRGBA(0, 0, 0, alpha));

        CFont::PrintString(posX, posY, entry.msg.c_str());

        // count this message lines
        size_t lines = 1;
        size_t pos = 0;
        while ((pos = entry.msg.find("~n~", pos)) != std::string::npos)
        {
            lines++;
            pos += 3; // pattern length
        }
        lines += std::count(entry.msg.begin(), entry.msg.end(), '\n');

        posY += 18.0f * sizeY * lines;
    }
}
