#pragma once
#include "CLEO.h"
#include "CRGBA.h"
#include <deque>
#include <string>

using namespace CLEO;

class ScreenLog
{
public:
    static DWORD timeDisplay; // miliseconds

    ScreenLog();

    void Init();
    void Add(eLogLevel level, const char* msg);
    void Draw();
    void DrawLine(const char* msg, size_t row = 0);

private:
    eLogLevel level;
    size_t maxMessages;
    float fontSize;
    DWORD timeFadeout; // miliseconds

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
        size_t msgStartPos;
        float timeLeft;
        size_t repeats;

        static const size_t Repeat_Prefix_Len = 16; // extra characters for repeat count text

        Entry() :
            level(eLogLevel::Default),
            msg(""),
            timeLeft(0.0f),
            repeats(1)
        {
        }

        Entry(eLogLevel level, const char* msg) :
            level(level), 
            repeats(1)
        {
            if(msg != nullptr)
            {
                auto len = strlen(msg);
                this->msg.reserve(Repeat_Prefix_Len + len);

                // repeat prefix
                this->msg.resize(Repeat_Prefix_Len - 2);
                this->msg.push_back(':');
                this->msg.push_back(' ');
                msgStartPos = Repeat_Prefix_Len; // prefix not present

                // copy input message
                for(size_t i = 0; i < len; i++)
                {
                    const char c = msg[i];
                    switch(c)
                    {
                        case '\n':
                            this->msg += "~n~";
                            break;

                        // characters not represented correctly by game's font texture
                        case '{':
                        case '}':
                            this->msg.push_back('_');
                            break;
                            
                        default:
                            this->msg.push_back(c);
                    }
                }
            }

            ResetTime();
        }

        void Repeat()
        {
            ResetTime();
            repeats++;

            std::string prefix = "x" + std::to_string(repeats);
            msgStartPos = Repeat_Prefix_Len - 2 - prefix.length(); // and ": "
            msg.replace(msgStartPos, prefix.length(), prefix);
        }

        void ResetTime()
        {
            timeLeft = min(msg.length(), 200) * 0.08f; // 12 letters peer second reading speed
            timeLeft = max(timeLeft, 0.001f * ScreenLog::timeDisplay); // not shorter than defined in config
        }

        const char* GetMsg(bool prefix = true) const
        {
            return msg.c_str() + (prefix ? msgStartPos : Repeat_Prefix_Len);
        }

        bool operator==(const Entry& other) const
        {
            return level == other.level && !strcmp(GetMsg(false), other.GetMsg(false));
        }
    };

    static size_t CountLines(std::string& msg);
    static DWORD GetTime();

    std::deque<Entry> entries;
    float scrollOffset;
};