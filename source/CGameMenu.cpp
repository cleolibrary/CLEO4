#include "stdafx.h"
#include "CGameMenu.h"
#include "CleoBase.h"
#include "CDebug.h"
#include <sstream>

namespace CLEO
{
    void(__cdecl	* TextDraw)				(float x, float y, const char* text);
    void(__cdecl	* SetTextAlign)			(BYTE nAlign);
    void(__cdecl	* SetTextFont)			(BYTE nFont);
    void(__cdecl* SetTextEdge)			(char nEdge);
    void(__cdecl	* SetLetterSize)			(float w, float h);
    void(__cdecl	* SetLetterColor)		(RGBA color);

    // thiscalls dont work with memory_pointer operator overload template
    DWORD	CMenuManager__ScaleX;
    DWORD	CMenuManager__ScaleY;
    DWORD	CTexture__DrawInRect;
    //void	(__thiscall	* CTexture__DrawInRect)	(void *texture, RwRect2D *rect, RwRGBA color);

    CMenuManager *	MenuManager;

    float CGameMenu_ScaleX(CMenuManager *pMenu, float w)
    {
        float fReturn;
        _asm
        {
            mov ecx, pMenu
            push w
            call CMenuManager__ScaleX
            fstp fReturn
        }
        return fReturn;
    }

    float CGameMenu_ScaleY(CMenuManager *pMenu, float w)
    {
        float fReturn;
        _asm
        {
            mov ecx, pMenu
            push w
            call CMenuManager__ScaleY
            fstp fReturn
        }
        return fReturn;
    }

    void CTexture_DrawInRect(void *pTexture, RwRect2D *rect, RwRGBA *colour)
    {
        _asm
        {
            push colour
            push rect
            mov ecx, pTexture
            call CTexture__DrawInRect
        }
    }

    void __fastcall OnDrawMenuBackground(void *texture, int dummy, RwRect2D *rect, RwRGBA *color)
    {
        CTexture_DrawInRect(texture, rect, color);
        auto cs_count = GetInstance().ScriptEngine.WorkingScriptsCount();
        auto plugin_count = GetInstance().PluginSystem.GetNumPlugins();
        std::ostringstream cleo_text;
        cleo_text << "CLEO v" << CLEO_VERSION_STR;
#ifdef _DEBUG
        cleo_text << " DEBUG";
#endif
        SetTextAlign(1);
        SetTextFont(2);
        if (SetTextEdge) SetTextEdge(1);
        SetLetterSize(CGameMenu_ScaleX(MenuManager, 0.23f), CGameMenu_ScaleY(MenuManager, 0.4f));

        SetLetterColor(RGBA(/*0xE1, 0xE1, 0xE1, 0xFF*/0xAD, 0xCE, 0xC4, 0xFF));
        TextDraw(CGameMenu_ScaleX(MenuManager, 6.0f), CGameMenu_ScaleY(MenuManager, 428.0f), cleo_text.str().c_str());
        cleo_text.str("");

        if (cs_count || plugin_count)
        {
            if (plugin_count) cleo_text << plugin_count << (plugin_count > 1 ? " plugins" : " plugin");
            if (cs_count && plugin_count) cleo_text << " / ";
            if (cs_count) cleo_text << cs_count << (cs_count > 1 ? " scripts" : " script");
            //plugin_text << plugin_count << (plugin_count > 1 ? " plugins" : " plugin");
            SetTextAlign(1);
            //SetTextFont(2);

            SetLetterSize(CGameMenu_ScaleX(MenuManager, 0.18f), CGameMenu_ScaleY(MenuManager, 0.34f));

            SetLetterColor(RGBA(/*0xE1, 0xE1, 0xE1, 0xFF*/0xAD, 0xCE, 0xC4, 0xFF));
            TextDraw(CGameMenu_ScaleX(MenuManager, 6.0f), CGameMenu_ScaleY(MenuManager, 436.0f), cleo_text.str().c_str());
        }

        // execute callbacks
        auto& cleo = GetInstance();
        if (cleo.IsStarted())
        {
            for (void* func : cleo.GetCallbacks(eCallbackId::MenuDraw))
            {
                typedef void WINAPI callback(void);
                ((callback*)func)();
            }
        }
    }

    void CGameMenu::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting MenuStatusNotifier...");
        CGameVersionManager& gvm = GetInstance().VersionManager;
        MenuManager = gvm.TranslateMemoryAddress(MA_MENU_MANAGER);
        TextDraw = gvm.TranslateMemoryAddress(MA_DRAW_TEXT_FUNCTION);
        SetTextAlign = gvm.TranslateMemoryAddress(MA_SET_TEXT_ALIGN_FUNCTION);
        SetTextFont = gvm.TranslateMemoryAddress(MA_SET_TEXT_FONT_FUNCTION);
        SetTextEdge = gvm.TranslateMemoryAddress(MA_SET_TEXT_EDGE_FUNCTION);

        // Meh...
        CMenuManager__ScaleX = gvm.TranslateMemoryAddress(MA_CMENU_SCALE_X_FUNCTION);
        CMenuManager__ScaleY = gvm.TranslateMemoryAddress(MA_CMENU_SCALE_Y_FUNCTION);

        SetLetterSize = gvm.TranslateMemoryAddress(MA_SET_LETTER_SIZE_FUNCTION);
        SetLetterColor = gvm.TranslateMemoryAddress(MA_SET_LETTER_COLOR_FUNCTION);

        inj.MemoryReadOffset(gvm.TranslateMemoryAddress(MA_CALL_CTEXTURE_DRAW_BG_RECT).address + 1, CTexture__DrawInRect, true);
        //gvm.TranslateMemoryAddress(MA_CTEXTURE_DRAW_IN_RECT_FUNCTION);
        inj.ReplaceFunction(OnDrawMenuBackground, gvm.TranslateMemoryAddress(MA_CALL_CTEXTURE_DRAW_BG_RECT));
    }
}
