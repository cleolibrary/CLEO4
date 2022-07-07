#pragma once
#pragma warning(disable:4733)
#define DEBUGIT
#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <direct.h>
#include <list>
#include <vector>
#include <set>
#include <cstdint>

#include <game_sa/CPools.h>
#include <game_sa/CMenuManager.h>
#include <game_sa/CText.h>
#include <game/CRGBA.h>
#include <game_sa/cHandlingDataMgr.h>
#include <game_sa/CPlayerPed.h>
#include <game_sa/CRadar.h>

#include "CTheScripts.h"

enum CLEO_Version
{
    //CLEO_VER_1				= 0x0100,
    //CLEO_VER_2				= 0x0200,
    CLEO_VER_3 = 0x0300,
    CLEO_VER_4_MIN = 0x0400,
    CLEO_VER_4_2 = 0x0402,
    CLEO_VER_4_3 = 0x0403,
    CLEO_VER_4_4 = 0x0404,
    CLEO_VER_4 = CLEO_VER_4_4,

    CLEO_VERSION = CLEO_VER_4,
};
#define VERSION_LONG		0x04040000
#define VERSION_STRING	"4.4.0"

#define CPOOL_USE_HANDLE_ACCESS

//#define VALIDATE_SIZE(struc, size)	static_assert(sizeof(struc) == size, #struc " (Invalid Structure Size)")

#define NUM_SCAN_ENTITIES 16

using CPedPool = CPool<CPed, CCopPed>;
using CVehiclePool = CPool<CVehicle, CHeli>;
using CObjectPool = CPool<CObject, CCutsceneObject>;
using CBuildingPool = CPool<CBuilding>;
using RGBA = CRGBA;
using RwV3D = RwV3d;
struct RwRect2D;
using CHandling = cHandlingDataMgr;
using CMarker = tRadarTrace;

class CTexture
{
    RwTexture *texture;
};

// stolen from GTASA
class CTextDrawer
{
public:
    float		m_fScaleX;
    float		m_fScaleY;
    CRGBA		m_Colour;
    BYTE			m_bJustify;
    BYTE			m_bCenter;
    BYTE			m_bBackground;
    BYTE			m_bUnk1;
    float		m_fLineHeight;
    float		m_fLineWidth;
    CRGBA		m_BackgroundColour;
    BYTE			m_bProportional;
    CRGBA		m_EffectColour;
    BYTE			m_ucShadow;
    BYTE			m_ucOutline;
    BYTE			m_bDrawBeforeFade;
    BYTE			m_bAlignRight;
    int			m_nFont;
    float		m_fPosX;
    float		m_fPosY;
    char			m_szGXT[8];
    int			m_nParam1;
    int			m_nParam2;
};

VALIDATE_SIZE(CTextDrawer, 0x44);

enum
{
    VEHICLE_TYPE_AUTOMOBILE,
    VEHICLE_TYPE_PLANE,
    VEHICLE_TYPE_BIKE,
    VEHICLE_TYPE_HELI,
    VEHICLE_TYPE_BOAT,
    VEHICLE_TYPE_TRAIN,
    VEHICLE_TYPE_TRAILER,
    VEHICLE_TYPE_BMX,
    VEHICLE_TYPE_MTRUCK,
    VEHICLE_TYPE_QUAD,
    VEHICLE_TYPE_UNKNOWN = -1,
};

inline CEntity* GetWeaponTarget(CPed* pSelf)
{
    return reinterpret_cast<int>(pSelf->m_pTargetedObject) != -1 ? pSelf->m_pTargetedObject : nullptr;
}

inline bool IsAvailable(CPed* pSelf)
{
    return pSelf->m_nPedState != PEDSTATE_DIE && pSelf->m_nPedState != PEDSTATE_DEAD;
}

inline bool	IsWrecked(CVehicle* pSelf)
{
    return pSelf->m_nStatus == STATUS_WRECKED || pSelf->m_nVehicleFlags.bIsDrowning;
}

inline std::vector<BYTE> ReadFileToBytes(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    file.seekg(0, std::ios::end);
    const std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<BYTE> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
        std::istream_iterator<BYTE>(file),
        std::istream_iterator<BYTE>());

    return vec;
}

#define OP_NOP			0x90
#define OP_RET			0xC3
#define OP_CALL			0xE8
#define OP_JMP			0xE9
#define OP_JMPSHORT		0xEB
#include "Mem.h"
