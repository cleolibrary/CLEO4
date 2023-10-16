#include "stdafx.h"
#include "CGameMenu.h"
#include "CleoBase.h"
#include "CDebug.h"
#include "CFont.h"
#include "plugin.h"
#include <sstream>

namespace CLEO
{
    CMenuManager* MenuManager;

    DWORD CTexture__DrawInRect; // original address
    void CTexture_DrawInRect(void* pTexture, RwRect2D* rect, RwRGBA* colour)
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
        CTexture_DrawInRect(texture, rect, color); // call original

        CFont::SetBackground(false, false);
        CFont::SetWrapx(640.0f);
        CFont::SetFontStyle(FONT_MENU);
        CFont::SetProportional(true);
        CFont::SetOrientation(ALIGN_LEFT);

        CFont::SetColor({ 0xAD, 0xCE, 0xC4, 0xFF });
        CFont::SetEdge(1);
        CFont::SetDropColor({ 0x00, 0x00, 0x00, 0xFF });

        const float fontSize = 0.5f / 448.0f;
        const float aspect = (float)RsGlobal.maximumWidth / RsGlobal.maximumHeight;
        const float subtextHeight = 0.75f; // factor of first line height
        float sizeX = fontSize * 0.5f / aspect * RsGlobal.maximumWidth;
        float sizeY = fontSize * RsGlobal.maximumHeight;

        float posX = 25.0f * sizeX; // left margin
        float posY = RsGlobal.maximumHeight - 15.0f * sizeY; // bottom margin

        auto cs_count = GetInstance().ScriptEngine.WorkingScriptsCount();
        auto plugin_count = GetInstance().PluginSystem.GetNumPlugins();
        if (cs_count || plugin_count)
        {
            posY -= 15.0f * sizeY; // add space for bottom text
        }

        // draw CLEO version text
        std::ostringstream text;
        text << "CLEO v" << CLEO_VERSION_STR;
#ifdef _DEBUG
        text << " ~r~~h~DEBUG";
#endif
        CFont::SetScale(sizeX, sizeY);
        CFont::PrintString(posX, posY - 15.0f * sizeY, text.str().c_str());

        // draw plugins / scripts text
        if (cs_count || plugin_count)
        {
            text.str(""); // clear
            if (plugin_count) text << plugin_count << (plugin_count > 1 ? " plugins" : " plugin");
            if (cs_count && plugin_count) text << " / ";
            if (cs_count) text << cs_count << (cs_count > 1 ? " scripts" : " script");

            posY += 15.0f * sizeY; // line feed
            sizeX *= subtextHeight;
            sizeY *= subtextHeight;
            CFont::SetScale(sizeX, sizeY);
            CFont::PrintString(posX, posY - 15.0f * sizeY, text.str().c_str());
        }
    }

    void CGameMenu::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting MenuStatusNotifier...");
        CGameVersionManager& gvm = GetInstance().VersionManager;
        MenuManager = gvm.TranslateMemoryAddress(MA_MENU_MANAGER);

        inj.MemoryReadOffset(gvm.TranslateMemoryAddress(MA_CALL_CTEXTURE_DRAW_BG_RECT).address + 1, CTexture__DrawInRect, true);
        inj.ReplaceFunction(OnDrawMenuBackground, gvm.TranslateMemoryAddress(MA_CALL_CTEXTURE_DRAW_BG_RECT));
    }
}
