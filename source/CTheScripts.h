#pragma once

enum eDataType
{
    DT_END,
    DT_DWORD,
    DT_VAR,
    DT_LVAR,
    DT_BYTE,
    DT_WORD,
    DT_FLOAT,
    DT_VAR_ARRAY,
    DT_LVAR_ARRAY,
    DT_TEXTLABEL,
    DT_VAR_TEXTLABEL,
    DT_LVAR_TEXTLABEL,
    DT_VAR_TEXTLABEL_ARRAY,
    DT_LVAR_TEXTLABEL_ARRAY,
    DT_VARLEN_STRING,
    DT_STRING,
    DT_VAR_STRING,
    DT_LVAR_STRING,
    DT_VAR_STRING_ARRAY,
    DT_LVAR_STRING_ARRAY
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

union SCRIPT_VAR
{
    DWORD	dwParam;
    short	wParam;
    WORD	usParam;
    BYTE	ucParam;
    char	cParam;
    bool	bParam;
    int		nParam;
    float	fParam;
    void*	pParam;
    char*	pcParam;
};

class CRunningScript
{
protected:
    CRunningScript *Next;			// +0x0
    CRunningScript *Previous;				// +0x4
    char Name[8];						// +0x8
    void *BaseIP;						// +0x10
    BYTE *CurrentIP;					// +0x14
    BYTE *Stack[8];						// +0x18
    WORD SP;							// +0x38
    SCRIPT_VAR LocalVar[32];			// +0x3C
    DWORD Timers[2];					// +0xBC
    bool bIsActive;						// +0xC4
    bool bCondResult;					// +0xC5
    bool bUseMissionCleanup;			// +0xC6
    bool bIsExternal;					// +0xC7
    bool bTextBlockOverride;			// +0xC8
    BYTE bExternalType;					// +0xC9
    DWORD WakeTime;						// +0xCC
    eLogicalOperation LogicalOp;		// +0xD0
    bool NotFlag;						// +0xD2
    bool bWastedBustedCheck;			// +0xD3
    bool bWastedOrBusted;				// +0xD4
    void *SceneSkipIP;					// +0xD8
    bool bIsMission;					// +0xDC

public:
    inline	bool				IsActive() {
        return bIsActive;
    }
    inline	bool				IsExternal() {
        return bIsExternal;
    }
    inline	bool				IsMission() {
        return bIsMission;
    }
    inline	const char		*	GetName() {
        return Name;
    }
    inline	BYTE			*	GetBasePointer() {
        return (BYTE*)BaseIP;
    }
    inline	BYTE			*	GetBytePointer() {
        return CurrentIP;
    }
    inline	void				SetIp(void *ip) {
        CurrentIP = (BYTE*)ip;
    }
    inline	void				SetBaseIp(void *ip) {
        BaseIP = ip;
    }
    inline	CRunningScript	*	GetNext() {
        return Next;
    }
    inline	CRunningScript	*	GetPrev() {
        return Previous;
    }
    inline	void				SetIsExternal(bool b) {
        bIsExternal = b;
    }
    inline	void				SetActive(bool b) {
        bIsActive = b;
    }
    inline	void				SetNext(CRunningScript *v) {
        Next = v;
    }
    inline	void				SetPrev(CRunningScript *v) {
        Previous = v;
    }
    inline	SCRIPT_VAR		*	GetVarPtr() {
        return LocalVar;
    }
    inline	SCRIPT_VAR		*	GetVarPtr(int i) {
        return &LocalVar[i];
    }
    inline	int				*	GetIntVarPtr(int i) {
        return (int*)&LocalVar[i].dwParam;
    }
    inline	int					GetIntVar(int i) {
        return LocalVar[i].dwParam;
    }
    inline	void				SetIntVar(int i, int v) {
        LocalVar[i].dwParam = v;
    }
    inline	void				SetFloatVar(int i, float v) {
        LocalVar[i].fParam = v;
    }
    inline	char				GetByteVar(int i) {
        return LocalVar[i].bParam;
    }
    inline	bool				GetConditionResult() {
        return bCondResult != 0;
    }
    /*
    inline	int			GetLocalVarVal(int i) {
    return IsMission() ? CTheScripts::GetMissionLocal(i) : GetIntVar(i);
    }
    inline	int		*	GetLocalVarPtr(int i) {
    return IsMission() ? CTheScripts::GetMissionLocalPtr(i) : GetIntVarPtr(i);
    }*/

    inline	char			ReadDataType() {
        return ReadDataByte();
    }
    inline	short		ReadDataVarIndex() {
        return ReadDataWord();
    }
    inline	short		ReadDataArrayOffset() {
        return ReadDataWord();
    }
    inline	short		ReadDataArrayIndex() {
        return ReadDataWord();
    }
    inline	short		ReadDataArraySize() {
        return ReadDataByte();
    }
    inline	short		ReadDataArrayFlags() {
        return ReadDataByte();
    }
    /*
    inline	int			ReadDataGlobalIntVal()
    {
    short i = ReadDataVarIndex();
    return CTheScripts::GetGlobalVarVal(i);
    };

    inline	int			ReadDataLocalIntVal()
    {
    short i = ReadDataVarIndex();
    return IsMission() ? CTheScripts::GetMissionLocal(i) : GetIntVar(i);
    };*/
    /*
    inline	int			ReadDataGlobalArrayIntVal()
    {
    short	offset = ReadDataArrayOffset(),
    index = ReadDataArrayIndex(),
    size = ReadDataArraySize(),
    flags = ReadDataArrayFlags(),
    indexVal;

    if(flags >= 0x80) indexVal = GetLocalVarVal(index);
    else indexVal = CTheScripts::GetGlobalVarVal(index);

    return CTheScripts::GetGlobalVarVal(indexVal + offset);
    };

    inline	int			ReadDataLocalArrayIntVal()
    {
    short	offset = ReadDataArrayOffset(),
    index = ReadDataArrayIndex(),
    size = ReadDataArraySize(),
    flags = ReadDataArrayFlags(),
    indexVal;

    if(flags >= 0x80) indexVal = GetLocalVarVal(index);
    else indexVal = CTheScripts::GetGlobalVarVal(index);

    return GetLocalVarVal(indexVal + offset);
    };*/

    inline	void		IncPtr(int n = 1) {
        CurrentIP += n;
    };

    inline	int			ReadDataByte()
    {
        char b = *CurrentIP;
        ++CurrentIP;
        return b;
    };

    inline	short		ReadDataWord()
    {
        short v = *(short*)CurrentIP;
        CurrentIP += 2;
        return v;
    };

    inline	int			ReadDataInt()
    {
        int i = *(int*)CurrentIP;
        CurrentIP += 4;
        return i;
    };

    inline void			PushStack(BYTE *ptr)
    {
        Stack[SP++] = ptr;
    }

    inline BYTE	*		PopStack()
    {
        return Stack[--SP];
    }

    CRunningScript()
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
        bWastedOrBusted = 0;
        SceneSkipIP = 0;
        bIsMission = 0;
        bWastedBustedCheck = 1;
    }

    // absolute path to directory where script's source file is located
    const char* GetScriptFileDir() const;
    void SetScriptFileDir(const char* directory);

    // filename with type extension of script's source file
    const char* GetScriptFileName() const;
    void SetScriptFileName(const char* filename);
};
