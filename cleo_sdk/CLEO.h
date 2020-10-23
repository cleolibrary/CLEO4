/*
	CLEO 4.4 header file;
	Copyright (c) 2020 Alien, Deji, Junior_Djjr;
*/
#pragma once

#include <wtypes.h>

#define CLEO_VERSION 0x04040000
#define CLEO_VERSIONTEXT "4.4.0"

//result of CLEO_GetGameVersion()
#define GV_US10 0	//1.0 us
#define GV_US11 1	//1.01 us - not supported
#define GV_EU10 2	//1.0 eu
#define GV_EU11 3	//1.01 eu
#define GV_UNK -1	//any other

typedef union
{
	DWORD	dwParam;
	int		nParam;
	float	fParam;
	void *	pParam;
	char *	szParam;
} SCRIPT_VAR;

//operand types
#define	globalVar			2		//$
#define	localVar			3		//@
#define	globalArr			7		//$(,)
#define	localArr 			8		//@(,)
#define	imm8 				4		//char
#define	imm16 				5		//short
#define	imm32 				1		//long, unsigned long
#define	imm32f 				6		//float
#define	vstring 			0x0E	//""
#define	sstring 			9		//''
#define	globalVarVString 	0x10	//v$
#define	localVarVString 	0x11	//@v
#define	globalVarSString 	0x0A	//s$
#define	localVarSString 	0x0B	//@s


typedef int SCRIPT_HANDLE;
typedef SCRIPT_HANDLE HANDLE_ACTOR, ACTOR, HACTOR, PED, HPED, HANDLE_PED;
typedef SCRIPT_HANDLE HANDLE_CAR, CAR, HCAR, VEHICLE, HVEHICLE, HANDLE_VEHICLE;
typedef SCRIPT_HANDLE HANDLE_OBJECT, OBJECT, HOBJECT;
typedef SCRIPT_HANDLE HSTREAM;
 
typedef struct CScriptThread CScriptThread;

#pragma pack(push,1)
struct CScriptThread
{
	CScriptThread	*next;					//next script in queue
	CScriptThread	*prev;					//previous script in queue
	char			threadName[8];			//name of thread, given by 03A4 opcode
	BYTE			*baseIp;				//pointer to begin of script in memory
	BYTE			*ip;					//current index pointer
	BYTE			*stack[8];				//return stack for 0050, 0051
	WORD			sp;						//current item in stack
	WORD			_f3A;					//padding
	SCRIPT_VAR		tls[34];				//thread's local variables
	BYTE			isActive;				//is current thread active
	char			condResult;				//condition result (true or false)
	char			missionCleanupFlag;		//clean mission
	char			external;				//is thread external (from script.img)
	BYTE			_fC8;					//unknown
	BYTE			_fC9;					//unknown
	BYTE			_fCA;					//unknown
	BYTE			_fCB;					//unknown
	DWORD			wakeTime;				//time, when script starts again after 0001 opcode
	WORD			logicalOp;				//00D6 parameter
	BYTE			notFlag;				//opcode & 0x8000 != 0
	BYTE			wbCheckEnabled;			//wasted_or_busted check flag
	BYTE			wastedOrBusted;			//is player wasted or busted
	BYTE			_fD5;					//unknown
	WORD			_fD6;					//unknown
	DWORD			sceneSkip;				//scene skip label ptr
	BYTE			missionFlag;			//is mission thread
	BYTE			_fDD[3];				//padding
};
#pragma pack(pop)

#define OR_CONTINUE 0
#define OR_INTERRUPT 1

typedef int OpcodeResult;

typedef OpcodeResult (CALLBACK* _pOpcodeHandler)(CScriptThread*);
typedef void(*FuncScriptDeleteDelegateT) (CScriptThread*);

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

DWORD WINAPI CLEO_GetVersion();
int   WINAPI CLEO_GetGameVersion();

BOOL  WINAPI CLEO_RegisterOpcode(WORD opcode, _pOpcodeHandler callback);

DWORD WINAPI CLEO_GetIntOpcodeParam(CScriptThread* thread);
float WINAPI CLEO_GetFloatOpcodeParam(CScriptThread* thread);

void  WINAPI CLEO_SetIntOpcodeParam(CScriptThread* thread, DWORD value);
void  WINAPI CLEO_SetFloatOpcodeParam(CScriptThread* thread, float value);

LPSTR WINAPI CLEO_ReadStringOpcodeParam(CScriptThread* thread, LPSTR buf, int size);
LPSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CScriptThread* thread, LPSTR buf, int size);
void  WINAPI CLEO_WriteStringOpcodeParam(CScriptThread* thread, LPCSTR str);

void  WINAPI CLEO_SetThreadCondResult(CScriptThread* thread, BOOL result);

void  WINAPI CLEO_SkipOpcodeParams(CScriptThread* thread, int count);

void  WINAPI CLEO_ThreadJumpAtLabelPtr(CScriptThread* thread, int labelPtr);

int   WINAPI CLEO_GetOperandType(CScriptThread* thread);

extern SCRIPT_VAR *opcodeParams;

extern SCRIPT_VAR *missionLocals;

//intermediate data is stored in opcodeParams array
void WINAPI CLEO_RetrieveOpcodeParams(CScriptThread *thread, int count);
void WINAPI CLEO_RecordOpcodeParams(CScriptThread *thread, int count);

SCRIPT_VAR * WINAPI CLEO_GetPointerToScriptVariable(CScriptThread *thread);

DWORD WINAPI CLEO_GetScriptTextureById(CScriptThread* thread, int id); // ret RwTexture *

HSTREAM WINAPI CLEO_GetInternalAudioStream(CScriptThread* thread, DWORD stream); // arg CAudioStream *

CScriptThread* WINAPI CLEO_CreateCustomScript(CScriptThread* fromThread, const char *script_name, int label);

CScriptThread* WINAPI CLEO_GetLastCreatedCustomScript();

void WINAPI CLEO_AddScriptDeleteDelegate(FuncScriptDeleteDelegateT func);

void WINAPI CLEO_RemoveScriptDeleteDelegate(FuncScriptDeleteDelegateT func);

#ifdef __cplusplus
}
#endif	//__cplusplus
