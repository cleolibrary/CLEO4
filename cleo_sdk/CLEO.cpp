#include "CLEO.h"

namespace CLEO
{

#ifdef __cplusplus
CRunningScript::CRunningScript()
{
	strcpy(Name, "noname");
	BaseIP = 0;
	Previous = 0;
	Next = 0;
	CurrentIP = 0;
	memset(Stack, 0, sizeof(Stack));
	SP = 0;
	WakeTime = 0;
	bIsActive = 0;
	bCondResult = 0;
	bUseMissionCleanup = 0;
	bIsExternal = 0;
	bTextBlockOverride = 0;
	bExternalType = -1;
	memset(LocalVar, 0, sizeof(LocalVar));
	LogicalOp = eLogicalOperation::NONE;
	NotFlag = 0;
	bWastedBustedCheck = 1;
	bWastedOrBusted = 0;
	SceneSkipIP = 0;
	bIsMission = 0;
	ScmFunction = 0;
	bIsCustom = 0;
}

bool CRunningScript::IsActive() const { return bIsActive; }

bool CRunningScript::IsExternal() const { return bIsExternal; }

bool CRunningScript::IsMission() const { return bIsMission; }

bool CRunningScript::IsCustom() const { return bIsCustom; }

const char* CRunningScript::GetName() const { return Name; }

BYTE* CRunningScript::GetBasePointer() const { return (BYTE*)BaseIP; }

BYTE* CRunningScript::GetBytePointer() const { return CurrentIP; }

void CRunningScript::SetIp(void* ip) { CurrentIP = (BYTE*)ip; }

void CRunningScript::SetBaseIp(void* ip) { BaseIP = ip; }

CRunningScript* CRunningScript::GetNext() const { return Next; }

CRunningScript* CRunningScript::GetPrev() const { return Previous; }

void CRunningScript::SetIsExternal(bool b) { bIsExternal = b; }

void CRunningScript::SetActive(bool b) { bIsActive = b; }

void CRunningScript::SetNext(CRunningScript* v) { Next = v; }

void CRunningScript::SetPrev(CRunningScript* v) { Previous = v; }

SCRIPT_VAR* CRunningScript::GetVarPtr() { return LocalVar; }

SCRIPT_VAR* CRunningScript::GetVarPtr(int i) { return &LocalVar[i]; }

int* CRunningScript::GetIntVarPtr(int i) { return (int*)&LocalVar[i].dwParam; }

int CRunningScript::GetIntVar(int i) const { return LocalVar[i].dwParam; }

void CRunningScript::SetIntVar(int i, int v) { LocalVar[i].dwParam = v; }

void CRunningScript::SetFloatVar(int i, float v) { LocalVar[i].fParam = v; }

char CRunningScript::GetByteVar(int i) const { return LocalVar[i].bParam; }

bool CRunningScript::GetConditionResult() const { return bCondResult != 0; }

bool CRunningScript::CRunningScript::GetNotFlag() const { return NotFlag; }

void CRunningScript::CRunningScript::SetNotFlag(bool state) { NotFlag = state; }

char CRunningScript::ReadDataType() { return ReadDataByte(); }

short CRunningScript::ReadDataVarIndex() { return ReadDataWord(); }

short CRunningScript::ReadDataArrayOffset() { return ReadDataWord(); }

short CRunningScript::ReadDataArrayIndex() { return ReadDataWord(); }

short CRunningScript::ReadDataArraySize() { return ReadDataByte(); }

short CRunningScript::ReadDataArrayFlags() { return ReadDataByte(); }

void CRunningScript::IncPtr(int n) { CurrentIP += n; }

int CRunningScript::ReadDataByte()
{
	char b = *CurrentIP;
	++CurrentIP;
	return b;
}

short CRunningScript::ReadDataWord()
{
	short v = *(short*)CurrentIP;
	CurrentIP += 2;
	return v;
}

int CRunningScript::ReadDataInt()
{
	int i = *(int*)CurrentIP;
	CurrentIP += 4;
	return i;
}

void CRunningScript::PushStack(BYTE* ptr) { Stack[SP++] = ptr; }

BYTE* CRunningScript::PopStack() { return Stack[--SP]; }

WORD CRunningScript::GetScmFunction() const { return ScmFunction; }

void CRunningScript::SetScmFunction(WORD id) { ScmFunction = id; }

#endif // __cplusplus

} // CLEO namespace
