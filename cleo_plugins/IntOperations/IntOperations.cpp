#include "plugin.h"
#include "CLEO.h"

using namespace plugin;

class IntOperations
{
public:
    IntOperations()
    {
        //check cleo version
        if (CLEO_GetVersion() >= CLEO_VERSION)
        {
            //register opcodes
            CLEO_RegisterOpcode(0x0B10, Script_IntOp_AND);
            CLEO_RegisterOpcode(0x0B11, Script_IntOp_OR);
            CLEO_RegisterOpcode(0x0B12, Script_IntOp_XOR);
            CLEO_RegisterOpcode(0x0B13, Script_IntOp_NOT);
            CLEO_RegisterOpcode(0x0B14, Script_IntOp_MOD);
            CLEO_RegisterOpcode(0x0B15, Script_IntOp_SHR);
            CLEO_RegisterOpcode(0x0B16, Script_IntOp_SHL);
            CLEO_RegisterOpcode(0x0B17, Scr_IntOp_AND);
            CLEO_RegisterOpcode(0x0B18, Scr_IntOp_OR);
            CLEO_RegisterOpcode(0x0B19, Scr_IntOp_XOR);
            CLEO_RegisterOpcode(0x0B1A, Scr_IntOp_NOT);
            CLEO_RegisterOpcode(0x0B1B, Scr_IntOp_MOD);
            CLEO_RegisterOpcode(0x0B1C, Scr_IntOp_SHR);
            CLEO_RegisterOpcode(0x0B1D, Scr_IntOp_SHL);
        }
        else
            MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IntOpearations.cleo", MB_ICONERROR);
    }

    static OpcodeResult WINAPI Script_IntOp_AND(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B10=3,%3d% = %1d% AND %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a & b);

        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Script_IntOp_OR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B11=3,%3d% = %1d% OR %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a | b);

        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Script_IntOp_XOR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B12=3,%3d% = %1d% XOR %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a ^ b);

        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Script_IntOp_NOT(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B13=2,%2d% = NOT %1d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, ~a);

        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Script_IntOp_MOD(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B14=3,%3d% = %1d% MOD %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a % b);

        return OR_CONTINUE;
    }

    static  OpcodeResult WINAPI Script_IntOp_SHR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B15=3,%3d% = %1d% SHR %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a >> b);

        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Script_IntOp_SHL(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B16=3,%3d% = %1d% SHL %2d%
        ****************************************************************/
    {
        int a = CLEO_GetIntOpcodeParam(thread);
        int b = CLEO_GetIntOpcodeParam(thread);

        CLEO_SetIntOpcodeParam(thread, a << b);

        return OR_CONTINUE;
    }

    /****************************************************************
    Now do them as real operators...
    *****************************************************************/

    static OpcodeResult WINAPI Scr_IntOp_AND(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B17=2,%1d% &= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam &= val;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_OR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B18=2,%1d% |= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam |= val;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_XOR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B19=2,%1d% ^= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam ^= val;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_NOT(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B1A=1,~%1d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        op->dwParam = ~op->dwParam;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_MOD(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B1B=2,%1d% %= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam %= val;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_SHR(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B1C=3,%1d% >>= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam >>= val;
        return OR_CONTINUE;
    }

    static OpcodeResult WINAPI Scr_IntOp_SHL(CScriptThread* thread)
        /****************************************************************
        Opcode Format
        0B1D=3,%1d% <<= %2d%
        ****************************************************************/
    {
        SCRIPT_VAR * op = CLEO_GetPointerToScriptVariable(thread);
        int val = CLEO_GetIntOpcodeParam(thread);
        op->dwParam <<= val;
        return OR_CONTINUE;
    }
} intOperations;
