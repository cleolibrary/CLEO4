#pragma once
#include "CLEO.h"
#include "CRGBA.h"
#include <deque>
#include <string>

using namespace CLEO;

class ScreenLog
{
public:
    ScreenLog();

    void Init();
    void Add(eLogLevel level, const char* msg);
    void Draw();
    void DrawLine(const char* msg, size_t row = 0);

private:
    eLogLevel level;
    size_t maxMessages;
    float fontSize;
    DWORD timeDisplay;
    DWORD timeFadeout;

    const CRGBA fontColor[4] = { // colors for eLogLevel
        CRGBA(0xDD, 0xDD, 0xDD, 0xF0), // None
        CRGBA(0xFF, 0x30, 0x30, 0xF0), // Error
        CRGBA(0xFF, 0xEE, 0x30, 0xF0), // User
        CRGBA(0xDD, 0xDD, 0xDD, 0xF0), // Default
    };

    struct Entry
    {
        eLogLevel level;
        std::string msg;
        DWORD endTime;

        Entry() :
            level(eLogLevel::Default),
            msg(""),
            endTime(0)
        {
        }

        Entry(eLogLevel level, const char* msg, DWORD endTime) :
            level(level),
            endTime(endTime)
        {
            if(msg != nullptr)
            {
                auto len = strlen(msg);
                this->msg.reserve(len);

                for(size_t i = 0; i < len; i++)
                {
                    char c = msg[i];

                    if(c == '\n')
                        this->msg += "~n~";
                    else
                        this->msg.push_back(c);
                }
            }
        }
    };

    static size_t CountLines(std::string& msg);
    static DWORD GetTime();

    std::deque<Entry> entries;
    float scrollOffset;
};