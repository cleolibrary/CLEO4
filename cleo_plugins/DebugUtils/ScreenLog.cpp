#include "ScreenLog.h"
#include "Utils.h"
#include "CFont.h"
#include "CTimer.h"

ScreenLog::ScreenLog()
{
    scrollOffset = 0.0f;

    Init();
}

void ScreenLog::Init()
{
    // load settings from ini file
    auto config = GetConfigFilename();

    level = (eLogLevel)GetPrivateProfileInt("ScreenLog", "Level", (UINT)eLogLevel::None, config.c_str());
    maxMessages = GetPrivateProfileInt("ScreenLog", "MessagesMax", 40, config.c_str());
    timeDisplay = GetPrivateProfileInt("ScreenLog", "MessageTime", 6000, config.c_str());
    timeFadeout = 2000;
    fontSize = 0.01f * GetPrivateProfileInt("ScreenLog", "FontSize", 60, config.c_str());
}

void ScreenLog::Add(eLogLevel level, const char* msg)
{
    if (level > this->level)
    {
        return;
    }

    // calculate end time
    auto duration = DWORD(0.2f * timeDisplay);
    duration += DWORD(0.8f * timeDisplay * strlen(msg) / 40); // assume 40 characters as baseline
    duration = min(duration, 3 * timeDisplay);
    
    /*auto startTime = GetTime();
    if(!entries.empty()) startTime = entries.front().endTime;
    entries.emplace_front(level, msg, startTime + duration);*/

    auto endTime = GetTime() + duration;
    if (!entries.empty()) endTime = max(endTime, entries.front().endTime + 200);
    entries.emplace_front(level, msg, endTime);

    if (entries.size() > maxMessages)
    {
        entries.resize(maxMessages);
    }

    // update scroll pos
    float sizeY = fontSize * RsGlobal.maximumHeight / 448.0f;
    size_t lines = CountLines(std::string(msg));
    scrollOffset += 18.0f * lines * sizeY;
}

void ScreenLog::Draw()
{
    // scroll animation
    static DWORD prevTime;
    DWORD currTime = GetTickCount(); // game independent
    if (scrollOffset > 0.001f)
    {
        float delta = 0.01f * (currTime - prevTime);
        scrollOffset *= max(0.9f - delta, 0.0f);
    }
    else
        scrollOffset = 0.0f;
    prevTime = currTime;

    const auto now = GetTime(); // miliseconds

    // clean up expired entries
    while(!entries.empty())
    {
        if(entries.back().endTime + timeFadeout < now)
            entries.pop_back();
        else
            break;
    }

    if (entries.empty())
    {
        scrollOffset = 0.0f;
        return; // nothing to print
    }

    CFont::SetBackground(false, false);
    CFont::SetWrapx(99999999.0f); // no line wrap
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetEdge(1);
    CFont::SetProportional(true);

    const float aspect = (float)RsGlobal.maximumWidth / RsGlobal.maximumHeight;
    float sizeX = fontSize * 0.55f * RsGlobal.maximumWidth / 640.0f / aspect;
    float sizeY = fontSize * RsGlobal.maximumHeight / 448.0f;
    CFont::SetScale(sizeX, sizeY);

    CFont::SetOrientation(ALIGN_LEFT);
    float posX = 15.0f * sizeX;
    float posY = 7.0f * sizeY - scrollOffset;

    for (size_t i = 0; i < entries.size(); i++)
    {
        auto& entry = entries[i];

        BYTE alpha = 255;
        if (entry.endTime < now)
        {
            auto elapsed = now - entry.endTime;
            float fadeProgress = (float)elapsed / timeFadeout;
            fadeProgress = std::clamp(fadeProgress, 0.0f, 1.0f);
            fadeProgress = 1.0f - fadeProgress; // fade out
            fadeProgress = sqrtf(fadeProgress);
            alpha = (BYTE)(fadeProgress * 0xFF);
        }
        else if(entry.endTime > (now + 4 * timeDisplay))
        {
             entry.endTime = now + 4 * timeDisplay;
        }

        auto color = fontColor[(size_t)entry.level];
        alpha = min(alpha, color.a);
        color.a = alpha;

        CFont::SetColor(color);

        alpha = std::clamp(int(alpha * alpha) / 255, 0, 255); // corrected for fadeout
        CFont::SetDropColor(CRGBA(0, 0, 0, alpha));

        CFont::PrintString(posX, posY, entry.msg.c_str());

        size_t lines = CountLines(entry.msg);
        posY += 18.0f * sizeY * lines;
    }
}

void ScreenLog::DrawLine(const char* msg, size_t row)
{
    CFont::SetBackground(false, false);
    CFont::SetWrapx(99999999.0f); // no line wrap
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetEdge(1);
    CFont::SetProportional(true);

    const float aspect = (float)RsGlobal.maximumWidth / RsGlobal.maximumHeight;
    float sizeX = fontSize * 0.55f * RsGlobal.maximumWidth / 640.0f / aspect;
    float sizeY = fontSize * RsGlobal.maximumHeight / 448.0f;
    CFont::SetScale(sizeX, sizeY);

    CFont::SetOrientation(ALIGN_RIGHT);
    float posX = (float)RsGlobal.maximumWidth - 15.0f * sizeX;
    
    //if(FrontEndMenuManager.m_bHudOn)
    float posY = 0.25f * RsGlobal.maximumHeight;
    posY += 18.0f * sizeY * row;

    auto color = fontColor[(size_t)eLogLevel::Error];
    CFont::SetColor(color);
    CFont::SetDropColor(CRGBA(0, 0, 0, color.a));

    CFont::PrintString(posX, posY, msg);
}

size_t ScreenLog::CountLines(std::string& msg)
{
    size_t lines = 1;

    size_t pos = 0;
    while ((pos = msg.find("~n~", pos)) != std::string::npos)
    {
        lines++;
        pos += 3; // pattern length
    }

    lines += std::count(msg.begin(), msg.end(), '\n');

    return lines;
}

DWORD ScreenLog::GetTime()
{
    //return GetTickCount();
    return CTimer::m_snPreviousTimeInMillisecondsNonClipped;
}

