#pragma once
#include "stdafx.h"
#include "CCodeInjector.h"
#include "crc32.h"
#include <string>
#include <unordered_map>

namespace CLEO
{
    class CTextManager : VInjectible
    {
        class FxtEntry
        {
            friend class CTextManager;
            std::string text;
            bool is_static;
            FxtEntry(const char *_text, bool _static = false);
        };

        typedef std::unordered_map<std::string, FxtEntry *,
            decltype(&crc32FromUpcaseStdString)> fxt_map_type;
        typedef fxt_map_type::iterator fxt_iterator;
        typedef fxt_map_type::const_iterator const_fxt_iterator;
        fxt_map_type fxts;
    public:
        CTextManager();
        ~CTextManager();
        const char* Get(const char* key);
        bool AddFxt(const char *key, const char *value, bool dynamic = true);
        bool RemoveFxt(const char *key);
        // find fxt text by its key
        const char *LocateFxt(const char *key);
        // erase all fxts, added by scripts
        void ClearDynamicFxts();
        void ParseFxtFile(std::istream& stream);
        virtual void Inject(CCodeInjector& inj);
    };

    void PrintHelp(const char *text, bool bPermanent = false, bool bBeep = true, bool bAddBrief = false);
    void ClearHelp();
    void PrintBig(const char *text, unsigned time, unsigned style);
    void Print(const char *text, unsigned time);
    void PrintNow(const char *text, unsigned time);

    bool TestCheat(const char* cheat);
    extern CText * gameTexts;
    const char * __fastcall CText__locate(CText *text, int dummy, const char *gxt);
}
