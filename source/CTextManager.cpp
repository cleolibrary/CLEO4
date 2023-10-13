#include "stdafx.h"

#include "CTextManager.h"
#include "CleoBase.h"
#include "FileEnumerator.h"
#include <fstream>
#include <sstream>
#include <direct.h>

namespace CLEO
{
    CText *gameTexts;
    char *cheatString;
    BYTE *mpackNumber;

    void(__cdecl * _PrintHelp)(const char *, bool sound, bool permanent, bool brief);
    void(__cdecl * _PrintBig)(const char *, unsigned time, unsigned style);
    void(__cdecl * _Print) (const char *, unsigned time, bool flag1, bool flag2);
    void(__cdecl * _PrintNow) (const char *, unsigned time, bool flag1, bool flag2);
    const char* (__fastcall * CText__Get)(CText*, int dummy, const char*);
    DWORD _CText__TKey__locate;

    char message_buf_big[7][MAX_STR_LEN];
    char message_buf_low[MAX_STR_LEN];
    char message_buf_high[MAX_STR_LEN];

    const char * __fastcall CText__TKey__locate(CText__TKey *key, int dummy, const char *gxt, bool& found)
    {
        const char * result;
        _asm
        {
            mov ecx, key
            push found
            push gxt
            call _CText__TKey__locate
            mov result, eax
        }
        return result;
    }

    void PrintHelp(const char *text, bool bPermanent, bool bBeep, bool bAddBrief)
    {
        _PrintHelp(text, bBeep, bPermanent, bAddBrief);
    }

    void ClearHelp()
    {
        _PrintHelp(nullptr, false, false, false);
    }

    void PrintBig(const char *text, unsigned time, unsigned style)
    {
        strcpy(message_buf_big[style - 1], text);
        _PrintBig(message_buf_big[style - 1], time, style - 1);
    }

    void Print(const char *text, unsigned time)
    {
        strcpy(message_buf_low, text);
        _Print(message_buf_low, time, false, false);
    }

    void PrintNow(const char *text, unsigned time)
    {
        strcpy(message_buf_high, text);
        _PrintNow(message_buf_high, time, false, false);
    }

    bool TestCheat(const char* cheat)
    {
        char *c = cheatString;
        char buf[30];
        strcpy(buf, cheat);
        char *s = _strrev(buf);
        if (_strnicmp(s, c, strlen(s))) return false;
        cheatString[0] = 0;
        return true;
    }

    const char * __fastcall CText__locate(CText *text, int dummy, const char *gxt)
    {
        bool bFound;
        const char *szResult;

        if ((*gxt == '\0') || (*gxt == ' ')) return "";

        szResult = GetInstance().TextManager.LocateFxt(gxt);
        if (szResult) return szResult;

        szResult = CText__TKey__locate(&text->tkeyMain, 0, gxt, bFound);

        if (!bFound)
        {
            if (text->missionTableLoaded || *mpackNumber || text->haveTabl)
            {
                szResult = CText__TKey__locate(&text->tkeyMission, 0, gxt, bFound);
                if (!bFound)	 return "";
                //else TRACE("Failed to find used text label '%s'", gxt);
            }
        }
        return szResult;
    }

    CTextManager::CTextManager() : fxts(1, crc32FromUpcaseStdString)
    {
        // parse FXT files
        FilesWalk("cleo\\cleo_text", ".fxt", [this](const char *fname)
        {
            TRACE("Parsing FXT file %s", fname);
            try
            {
                std::ifstream stream(fname);
                ParseFxtFile(stream);
            }
            catch (std::exception& ex)
            {
                LOG_WARNING("Loading of FXT file '%s' failed: \n%s", fname, ex.what());
            }
        });
    }

    const char* CTextManager::Get(const char* key)
    {
        return CText__Get(gameTexts, 0, key);
    }

    bool CTextManager::AddFxt(const char *key, const char *value, bool dynamic)
    {
        // TODO: replace this part with in-place construction of FxtEntry,
        // when it will be implemented in libstdc++
        // ^^ me iz noob and doesnt can use GNU

        auto fxt = fxts.find(key);

        if (fxt != fxts.end())
        {
            if (!dynamic || fxt->second->is_static)
            {
                LOG_WARNING("Attempting to add FXT \'%s\' - FAILED (GXT conflict)", key, value);
                return false;
            }

            fxt->second->text = value;
        }
        else
        {
            std::string str = key;
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
            fxts[str.c_str()] = new FxtEntry(value, !dynamic);

            TRACE("Added FXT[%s]", str.c_str());
        }
        return true;
    }

    bool CTextManager::RemoveFxt(const char *key)
    {
        TRACE("Deleting FXT[%s]", key);
        return fxts.erase(key) != 0;
    }

    const char *CTextManager::LocateFxt(const char *key)
    {
        std::string str = key;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        const_fxt_iterator found = fxts.find(str);
        if (found == fxts.end()) return nullptr;
        return found->second->text.c_str();
    }

    void CTextManager::ClearDynamicFxts()
    {
        TRACE("Deleting dynamic fxts...");
        //		size_t count = 0, total = fxts.size();
        for (auto it = fxts.begin(); it != fxts.end();)
        {
            if (!it->second->is_static)
            {
                delete it->second;
                fxts.erase(it++);
                //				++count;
            }
            else ++it;
        }
        //		TRACE("Deleting finished, %d elements erased, %d elements left",
        //			count, total - count);
    }

    CTextManager::~CTextManager()
    {
        TRACE("Deleting fxts...");
        size_t count = 0;
        for (auto it = fxts.begin(); it != fxts.end();)
        {
            delete it->second;
            fxts.erase(it++);
            ++count;
        }
        //		TRACE("Deleting finished, %d elements erased", count);
    }

    void CTextManager::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting TextManager...");
        CGameVersionManager& gvm = GetInstance().VersionManager;
        _PrintHelp = gvm.TranslateMemoryAddress(MA_TEXT_BOX_FUNCTION);
        _PrintBig = gvm.TranslateMemoryAddress(MA_STYLED_TEXT_FUNCTION);
        _Print = gvm.TranslateMemoryAddress(MA_TEXT_LOW_PRIORITY_FUNCTION);
        _PrintNow = gvm.TranslateMemoryAddress(MA_TEXT_HIGH_PRIORITY_FUNCTION);
        _CText__TKey__locate = gvm.TranslateMemoryAddress(MA_CTEXT_TKEY_LOCATE_FUNCTION);
        gameTexts = gvm.TranslateMemoryAddress(MA_GAME_TEXTS);
        cheatString = gvm.TranslateMemoryAddress(MA_CHEAT_STRING);
        mpackNumber = gvm.TranslateMemoryAddress(MA_MPACK_NUMBER);
        CText__Get = gvm.TranslateMemoryAddress(MA_CALL_CTEXT_LOCATE);
        inj.InjectFunction(CText__locate, CText__Get);
    }

    CTextManager::FxtEntry::FxtEntry(const char *_text, bool _static) : text(_text), is_static(_static)
    {
    }

    void CTextManager::ParseFxtFile(std::istream& stream)
    {
        static char buf[0x100];
        char *key_iterator, *value_iterator, *value_start, *key_start;
        stream.exceptions(std::ios::badbit);

        while (true)
        {
            if (stream.eof()) break;
            stream.getline(buf, sizeof(buf));
            if (stream.fail()) break;
            // parse extracted line	
            key_start = key_iterator = buf;
            while (*key_iterator)
            {
                if (*key_iterator == '#')	// start of comment
                    break;
                if (*key_iterator == '/' && key_iterator[1] == '/')
                    break;
                if (isspace(*key_iterator))
                {
                    *key_iterator = '\0';
                    // while (isspace(*++key_iterator)) ; // skip leading spaces
                    value_start = value_iterator = key_iterator + 1;
                    while (*value_iterator)
                    {
                        // start of comment
                        if (*value_iterator == '#' || (*key_iterator == '/' && key_iterator[1] == '/'))
                        {
                            *value_iterator = '\0';
                            break;
                        }
                        break;
                        value_iterator++;
                    }
                    // register found fxt entry
                    AddFxt(key_start, value_start, false);
                    break;
                }
                key_iterator++;
            }
        }
    }
}
