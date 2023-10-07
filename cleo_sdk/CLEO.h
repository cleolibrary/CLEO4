/*
	CLEO 4.5 header file
	Copyright (c) 2023 Alien, Deji, Junior_Djjr, Miran
*/
#pragma once

#include <wtypes.h>

#define CLEO_VERSION_MAIN	4
#define CLEO_VERSION_MAJOR	5
#define CLEO_VERSION_MINOR	0

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 24)|(CLEO_VERSION_MAJOR << 16)|(CLEO_VERSION_MINOR << 8)) // 0x0v0v0v00

#define __TO_STR(x) #x
#define TO_STR(x) __TO_STR(x)
#define CLEO_VERSION_STR TO_STR(CLEO_VERSION_MAIN.CLEO_VERSION_MAJOR.CLEO_VERSION_MINOR) // "v.v.v"

namespace CLEO
{
// result of CLEO_GetScriptVersion
enum eCLEO_Version : DWORD
{
	CLEO_VER_3 = 0x03000000,
	CLEO_VER_4_MIN = 0x04000000,
	CLEO_VER_4_2 = 0x04020000,
	CLEO_VER_4_3 = 0x04030000,
	CLEO_VER_4_4 = 0x04040000,
	CLEO_VER_4 = CLEO_VER_4_4,
	CLEO_VER_CUR = CLEO_VERSION
};

// result of CLEO_GetGameVersion
enum eGameVersion : int
{
	GV_US10 = 0, // 1.0 us
	GV_US11 = 1, // 1.01 us - not supported
	GV_EU10 = 2, // 1.0 eu
	GV_EU11 = 3, // 1.01 eu
	GV_STEAM,
	GV_TOTAL,
	GV_UNK = -1 // any other
};

// operand types
enum eDataType : int
{
	DT_END,
	DT_DWORD, // imm32
	DT_VAR, // globalVar $
	DT_LVAR, // localVar @
	DT_BYTE, // imm8
	DT_WORD, // imm16
	DT_FLOAT, // imm32f
	DT_VAR_ARRAY, // globalArr $(,)
	DT_LVAR_ARRAY, // localArr @(,)
	DT_TEXTLABEL, // sstring ''
	DT_VAR_TEXTLABEL, // globalVarSString s$
	DT_LVAR_TEXTLABEL, // localVarSString @s
	DT_VAR_TEXTLABEL_ARRAY,
	DT_LVAR_TEXTLABEL_ARRAY,
	DT_VARLEN_STRING, // vstring ""
	DT_STRING,
	DT_VAR_STRING, // globalVarVString v$
	DT_LVAR_STRING, // localVarVString @v
	DT_VAR_STRING_ARRAY,
	DT_LVAR_STRING_ARRAY
};

const size_t MAX_STR_LEN = 0xff; // max length of string type parameter

union SCRIPT_VAR
{
	DWORD dwParam;
	short wParam;
	WORD usParam;
	BYTE ucParam;
	char cParam;
	bool bParam;
	int nParam;
	float fParam;
	void* pParam;
	char* pcParam;
};

enum eLogicalOperation : WORD
{
	NONE = 0, // just replace

	AND_2 = 1, // AND operation on results of next two conditional opcodes
	AND_3,
	AND_4,
	AND_5,
	AND_6,
	AND_7,
	AND_END,
	
	OR_2 = 21, // OR operation on results of next two conditional opcodes
	OR_3,
	OR_4,
	OR_5,
	OR_6,
	OR_7,
	OR_END,
};
static eLogicalOperation& operator--(eLogicalOperation& o)
{
	if (o == eLogicalOperation::NONE) return o; // can not be decremented anymore
	if (o == eLogicalOperation::OR_2) return o = eLogicalOperation::NONE;
	
	auto val = static_cast<WORD>(o); // to number
	val--;
	return o = static_cast<eLogicalOperation>(val);
}

// CLEO virtual path prefixes. Expandable with CLEO_ResolvePath
const char DIR_GAME[] = "0:"; // game root directory
const char DIR_USER[] = "1:"; // game save directory
const char DIR_SCRIPT[] = "2:"; // current script directory
const char DIR_CLEO[] = "3:"; // game\cleo directory
const char DIR_MODULES[] = "4:"; // game\cleo\modules directory

// argument of CLEO_RegisterCallback
enum class eCallbackId
{
	ScmInit1, // void WINAPI OnScmInit1();
	ScmInit2, // void WINAPI OnScmInit2();
	ScmInit3, // void WINAPI OnScmInit3();
	ScriptsLoaded, // void WINAPI OnScriptsLoaded();
	ScriptsFinalize, // void WINAPI OnScriptsFinalize();
	ScriptProcess, // bool WINAPI OnScriptProcess(CRunningScript* pScript, int); // return false to skip this script processing
	ScriptDraw, // void WINAPI OnScriptDraw(bool beforeFade);
};

typedef int SCRIPT_HANDLE;
typedef SCRIPT_HANDLE HANDLE_ACTOR, ACTOR, HACTOR, PED, HPED, HANDLE_PED;
typedef SCRIPT_HANDLE HANDLE_CAR, CAR, HCAR, VEHICLE, HVEHICLE, HANDLE_VEHICLE;
typedef SCRIPT_HANDLE HANDLE_OBJECT, OBJECT, HOBJECT;
typedef SCRIPT_HANDLE HSTREAM;
 
#pragma pack(push,1)
#ifdef __cplusplus
class CRunningScript
{
protected:
#else
struct CRunningScript
{
#endif
 	CRunningScript* Next;		// 0x00 next script in queue
 	CRunningScript* Previous;	// 0x04 previous script in queue
 	char Name[8];				// 0x08 name of script, given by 03A4 opcode
 	void* BaseIP;				// 0x10 pointer to begin of script in memory
 	BYTE* CurrentIP;			// 0x14 current instruction pointer
 	BYTE* Stack[8];				// 0x18 return stack for 0050, 0051
 	WORD SP;					// 0x38 current item in stack
    BYTE _pad3A[2];				// 0x3A padding
 	SCRIPT_VAR LocalVar[32];	// 0x3C script's local variables
 	DWORD Timers[2];			// 0xBC script's timers
 	bool bIsActive;				// 0xC4 is script active
 	bool bCondResult;			// 0xC5 condition result
 	bool bUseMissionCleanup;	// 0xC6 clean mission
 	bool bIsExternal;			// 0xC7 is thread external (from script.img)
 	bool bTextBlockOverride;	// 0xC8
	BYTE bExternalType;			// 0xC9
    BYTE _padCA[2];				// 0xCA padding
 	DWORD WakeTime;				// 0xCC time, when script starts again after 0001 opcode
 	eLogicalOperation LogicalOp;// 0xD0 opcode 00D6 parameter
 	bool NotFlag;				// 0xD2 opcode & 0x8000 != 0
 	bool bWastedBustedCheck;	// 0xD3 wasted_or_busted check flag
 	bool bWastedOrBusted;		// 0xD4 is player wasted or busted
    char _padD5[3];				// 0xD5 padding
 	void* SceneSkipIP;			// 0xD8 scene skip label ptr
 	bool bIsMission;			// 0xDC is this script mission
    WORD ScmFunction;			// 0xDD CLEO's previous scmFunction id
    bool bIsCustom;				// 0xDF is this CLEO script

#ifdef __cplusplus
public:
    CRunningScript();

    bool IsActive() const;
    bool IsExternal() const;
    bool IsMission() const;
    bool IsCustom() const; // is this CLEO Script?
    const char* GetName() const;
    BYTE* GetBasePointer() const;
    BYTE* GetBytePointer() const;
    void SetIp(void* ip);
    void SetBaseIp(void* ip);
    CRunningScript* GetNext() const;
    CRunningScript* GetPrev() const;
    void SetIsExternal(bool b);
    void SetActive(bool b);
    void SetNext(CRunningScript* v);
    void SetPrev(CRunningScript* v);
    SCRIPT_VAR* GetVarPtr();
    SCRIPT_VAR* GetVarPtr(int i);
    int* GetIntVarPtr(int i);
    int GetIntVar(int i) const;
    void SetIntVar(int i, int v);
    void SetFloatVar(int i, float v);
    char GetByteVar(int i) const;
    bool GetConditionResult() const;
	bool GetNotFlag() const;
	void SetNotFlag(bool state);

    char ReadDataType();
    short ReadDataVarIndex();
    short ReadDataArrayOffset();
    short ReadDataArrayIndex();
    short ReadDataArraySize();
    short ReadDataArrayFlags();
 
    void IncPtr(int n = 1);
    int ReadDataByte();
    short ReadDataWord();
    int ReadDataInt();

    void PushStack(BYTE* ptr);
    BYTE* PopStack();

    WORD GetScmFunction() const;
    void SetScmFunction(WORD id);

    #endif // __cplusplus
};
#pragma pack(pop)
static_assert(sizeof(CRunningScript) == 0xE0, "Invalid size of CRunningScript!");

// alias for legacy use
#ifdef __cplusplus
    typedef class CRunningScript CScriptThread;
#else
    typedef struct CRunningScript CScriptThread;
#endif

enum OpcodeResult : char
{
    OR_CONTINUE = 0,
    OR_INTERRUPT = 1
};

typedef OpcodeResult (CALLBACK* _pOpcodeHandler)(CRunningScript*);
typedef void(*FuncScriptDeleteDelegateT) (CRunningScript*);

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

DWORD WINAPI CLEO_GetVersion();
eCLEO_Version WINAPI CLEO_GetScriptVersion(const CRunningScript* thread);
eGameVersion WINAPI CLEO_GetGameVersion();

BOOL WINAPI CLEO_RegisterOpcode(WORD opcode, _pOpcodeHandler callback);
void WINAPI CLEO_RegisterCallback(eCallbackId id, void* func);

DWORD WINAPI CLEO_GetIntOpcodeParam(CRunningScript* thread);
float WINAPI CLEO_GetFloatOpcodeParam(CRunningScript* thread);

void WINAPI CLEO_SetIntOpcodeParam(CRunningScript* thread, DWORD value);
void WINAPI CLEO_SetFloatOpcodeParam(CRunningScript* thread, float value);

LPSTR WINAPI CLEO_ReadStringOpcodeParam(CRunningScript* thread, LPSTR buf, int size);
LPSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CRunningScript* thread, LPSTR buf, int size); // exactly same as CLEO_ReadStringOpcodeParam
void WINAPI CLEO_WriteStringOpcodeParam(CRunningScript* thread, LPCSTR str);

void WINAPI CLEO_SetThreadCondResult(CRunningScript* thread, BOOL result);

void WINAPI CLEO_SkipOpcodeParams(CRunningScript* thread, int count);

void WINAPI CLEO_ThreadJumpAtLabelPtr(CRunningScript* thread, int labelPtr);

eDataType WINAPI CLEO_GetOperandType(const CRunningScript* thread); // peep data type

extern SCRIPT_VAR *opcodeParams;

extern SCRIPT_VAR *missionLocals;

//intermediate data is stored in opcodeParams array
void WINAPI CLEO_RetrieveOpcodeParams(CRunningScript *thread, int count);
void WINAPI CLEO_RecordOpcodeParams(CRunningScript *thread, int count);

SCRIPT_VAR * WINAPI CLEO_GetPointerToScriptVariable(CRunningScript *thread);

DWORD WINAPI CLEO_GetScriptTextureById(CRunningScript* thread, int id); // ret RwTexture *

HSTREAM WINAPI CLEO_GetInternalAudioStream(CRunningScript* thread, DWORD stream); // arg CAudioStream *

CRunningScript* WINAPI CLEO_CreateCustomScript(CRunningScript* fromThread, const char *script_name, int label);

CRunningScript* WINAPI CLEO_GetLastCreatedCustomScript();

void WINAPI CLEO_AddScriptDeleteDelegate(FuncScriptDeleteDelegateT func);

void WINAPI CLEO_RemoveScriptDeleteDelegate(FuncScriptDeleteDelegateT func);

// convert to absolute file path
void WINAPI CLEO_ResolvePath(CRunningScript* thread, char* inOutPath, DWORD pathMaxLen);

#ifdef __cplusplus
}
#endif	//__cplusplus

} // CLEO namespace
