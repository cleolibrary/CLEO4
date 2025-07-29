#include "stdafx.h"
#include "ScmFunction.h"
#include "CCustomOpcodeSystem.h"
#include "CScriptEngine.h"

namespace CLEO
{
    ScmFunction* ScmFunction::store[Store_Size] = {0};
    size_t ScmFunction::allocationPlace         = 0;

    ScmFunction* ScmFunction::Get(unsigned short idx)
    {
        if (idx >= Store_Size) return nullptr;

        return store[idx];
    }

    void ScmFunction::Clear()
    {
        for each (ScmFunction* scmFunc in store)
        {
            if (scmFunc != nullptr) delete scmFunc;
        }
        ScmFunction::allocationPlace = 0;
    }

    void* ScmFunction::operator new(size_t size)
    {
        size_t start_search = allocationPlace;
        while (store[allocationPlace] != nullptr) // find first unused position in store
        {
            if (++allocationPlace >= Store_Size) allocationPlace = 0; // end of store reached
            if (allocationPlace == start_search)
            {
                SHOW_ERROR("CLEO function storage stack overfllow!");
                throw std::bad_alloc(); // the store is filled up
            }
        }
        ScmFunction* obj       = reinterpret_cast<ScmFunction*>(::operator new(size));
        store[allocationPlace] = obj;
        return obj;
    }

    void ScmFunction::operator delete(void* mem)
    {
        store[reinterpret_cast<ScmFunction*>(mem)->thisScmFunctionId] = nullptr;
        ::operator delete(mem);
    }

    ScmFunction::ScmFunction(CLEO::CRunningScript* thread)
        : prevScmFunctionId(reinterpret_cast<CCustomScript*>(thread)->GetScmFunction())
    {
        auto cs = reinterpret_cast<CCustomScript*>(thread);

        // create snapshot of current scope
        savedBaseIP   = cs->BaseIP;
        savedCodeSize = cs->IsCustom() ? cs->GetCodeSize() : -1;

        std::copy(std::begin(cs->Stack), std::end(cs->Stack), std::begin(savedStack));
        savedSP = cs->SP;

        auto scope = cs->IsMission() ? missionLocals : cs->LocalVar;
        std::copy(scope, scope + _countof(CRunningScript::LocalVar), savedTls);

        savedCondResult = cs->bCondResult;
        savedLogicalOp  = cs->LogicalOp;
        savedNotFlag    = cs->NotFlag;

        savedScriptFileDir  = cs->GetScriptFileDir();
        savedScriptFileName = cs->GetScriptFileName();

        // init new scope
        std::fill(std::begin(cs->Stack), std::end(cs->Stack), nullptr);
        cs->SP          = 0;
        cs->bCondResult = false;
        cs->LogicalOp   = eLogicalOperation::NONE;
        cs->NotFlag     = false;

        cs->SetScmFunction(thisScmFunctionId = (unsigned short)allocationPlace);
    }

    size_t ScmFunction::GetCallStackSize() const
    {
        if (prevScmFunctionId == 0) return 0;
        auto parent = Get(prevScmFunctionId);
        return parent ? parent->GetCallStackSize() + 1 : 1;
    }

    void ScmFunction::Return(CRunningScript* thread)
    {
        auto cs = reinterpret_cast<CCustomScript*>(thread);

        cs->BaseIP = savedBaseIP;
        if (cs->IsCustom()) cs->SetCodeSize(savedCodeSize);

        // restore parent scope's gosub call stack
        std::copy(std::begin(savedStack), std::end(savedStack), std::begin(cs->Stack));
        cs->SP = savedSP;

        // restore parent scope's local variables
        std::copy(savedTls, savedTls + 32, cs->IsMission() ? missionLocals : cs->LocalVar);

        // process conditional result of just ended function in parent scope
        bool condResult = cs->bCondResult;
        if (savedNotFlag) condResult = !condResult;

        if (savedLogicalOp >= eLogicalOperation::ANDS_1 && savedLogicalOp < eLogicalOperation::AND_END)
        {
            cs->bCondResult = savedCondResult && condResult;
            cs->LogicalOp   = --savedLogicalOp;
        }
        else if (savedLogicalOp >= eLogicalOperation::ORS_1 && savedLogicalOp < eLogicalOperation::OR_END)
        {
            cs->bCondResult = savedCondResult || condResult;
            cs->LogicalOp   = --savedLogicalOp;
        }
        else // eLogicalOperation::NONE
        {
            cs->bCondResult = condResult;
            cs->LogicalOp   = savedLogicalOp;
        }

        cs->SetScriptFileDir(savedScriptFileDir.c_str());
        cs->SetScriptFileName(savedScriptFileName.c_str());

        cs->SetIp(retnAddress);
        cs->SetScmFunction(prevScmFunctionId);
    }
}; // namespace CLEO
