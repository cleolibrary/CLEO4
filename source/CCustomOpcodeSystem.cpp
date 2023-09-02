#include "stdafx.h"
#include "cleo.h"
#include "CLegacy.h"
#include "CGameVersionManager.h"
#include "CCustomOpcodeSystem.h"
#include "CTextManager.h"
#include "CModelInfo.h"

namespace CLEO {
	DWORD FUNC_fopen;
	DWORD FUNC_fclose;
	DWORD FUNC_fwrite;
	DWORD FUNC_fread;
	DWORD FUNC_fgetc;
	DWORD FUNC_fgets;
	DWORD FUNC_fputs;
	DWORD FUNC_fseek;
	DWORD FUNC_fprintf;
	DWORD FUNC_ftell;
	DWORD FUNC_fflush;
	DWORD FUNC_feof;
	DWORD FUNC_ferror;

	OpcodeResult __stdcall opcode_0A8C(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A8D(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A8E(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A8F(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A90(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A91(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A92(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A93(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A94(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A95(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A96(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A97(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A98(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A99(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9A(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9B(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9C(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9D(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9E(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0A9F(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA0(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA1(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA2(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA3(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA4(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA5(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA6(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA7(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA8(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AA9(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAA(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAB(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAC(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAD(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAE(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AAF(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB0(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB1(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB2(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB3(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB4(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB5(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB6(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB7(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB8(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AB9(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABA(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABB(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABC(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABD(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABE(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ABF(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC0(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC1(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC2(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC3(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC4(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC5(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC6(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC7(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC8(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AC9(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACA(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACB(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACC(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACD(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACE(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ACF(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD0(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD1(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD2(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD3(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD4(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD5(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD6(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD7(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD8(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AD9(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADA(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADB(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADC(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADD(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADE(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0ADF(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE0(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE1(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE2(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE3(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE4(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE5(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE6(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE7(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE8(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AE9(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AEA(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AEB(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AEC(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AED(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AEE(CRunningScript *thread);
	OpcodeResult __stdcall opcode_0AEF(CRunningScript *thread);

	CustomOpcodeHandler customOpcodeHandlers[100] =
	{
		opcode_0A8C, opcode_0A8D, opcode_0A8E, opcode_0A8F, opcode_0A90,
		opcode_0A91, opcode_0A92, opcode_0A93, opcode_0A94, opcode_0A95,
		opcode_0A96, opcode_0A97, opcode_0A98, opcode_0A99, opcode_0A9A,
		opcode_0A9B, opcode_0A9C, opcode_0A9D, opcode_0A9E, opcode_0A9F,
		opcode_0AA0, opcode_0AA1, opcode_0AA2, opcode_0AA3, opcode_0AA4,
		opcode_0AA5, opcode_0AA6, opcode_0AA7, opcode_0AA8, opcode_0AA9,
		opcode_0AAA, opcode_0AAB, opcode_0AAC, opcode_0AAD, opcode_0AAE,
		opcode_0AAF, opcode_0AB0, opcode_0AB1, opcode_0AB2, opcode_0AB3,
		opcode_0AB4, opcode_0AB5, opcode_0AB6, opcode_0AB7, opcode_0AB8,
		opcode_0AB9, opcode_0ABA, opcode_0ABB, opcode_0ABC, opcode_0ABD,
		opcode_0ABE, opcode_0ABF, opcode_0AC0, opcode_0AC1, opcode_0AC2,
		opcode_0AC3, opcode_0AC4, opcode_0AC5, opcode_0AC6, opcode_0AC7,
		opcode_0AC8, opcode_0AC9, opcode_0ACA, opcode_0ACB, opcode_0ACC,
		opcode_0ACD, opcode_0ACE, opcode_0ACF, opcode_0AD0, opcode_0AD1,
		opcode_0AD2, opcode_0AD3, opcode_0AD4, opcode_0AD5, opcode_0AD6,
		opcode_0AD7, opcode_0AD8, opcode_0AD9, opcode_0ADA, opcode_0ADB,
		opcode_0ADC, opcode_0ADD, opcode_0ADE, opcode_0ADF, opcode_0AE0,
		opcode_0AE1, opcode_0AE2, opcode_0AE3, opcode_0AE4, opcode_0AE5,
		opcode_0AE6, opcode_0AE7, opcode_0AE8, opcode_0AE9, opcode_0AEA,
		opcode_0AEB, opcode_0AEC, opcode_0AED, opcode_0AEE, opcode_0AEF,
	};

	typedef OpcodeResult(__thiscall *_OpcodeHandler)(CRunningScript *thread, unsigned short opcode);

	typedef void(*FuncScriptDeleteDelegateT) (CRunningScript *script);
	struct ScriptDeleteDelegate {
		std::vector<FuncScriptDeleteDelegateT> funcs;
		template<class FuncScriptDeleteDelegateT> void operator+=(FuncScriptDeleteDelegateT mFunc) { funcs.push_back(mFunc); }
		template<class FuncScriptDeleteDelegateT> void operator-=(FuncScriptDeleteDelegateT mFunc) { funcs.erase(std::remove(funcs.begin(), funcs.end(), mFunc), funcs.end()); }
		void operator()(CRunningScript *script) { for (auto& f : funcs) f(script); }
	};
	ScriptDeleteDelegate scriptDeleteDelegate;
	void RunScriptDeleteDelegate(CRunningScript *script) { scriptDeleteDelegate(script); }

	_OpcodeHandler *oldOpcodeHandlerTable;
	_OpcodeHandler newOpcodeHandlerTable[329];
	CustomOpcodeHandler extraOpcodeHandlers[100][300];

	CBuildingPool		**buildingPool = nullptr;			// add for future CLEO releases
	CVehiclePool			**vehiclePool = nullptr;
	CObjectPool			**objectPool = nullptr;
	CPedPool				**pedPool = nullptr;

	inline CPedPool& GetPedPool() { return **pedPool; }
	inline CVehiclePool& GetVehiclePool() { return **vehiclePool; }
	inline CObjectPool& GetObjectPool() { return **objectPool; }

	void(__thiscall * ProcessScript)(CRunningScript*);

	const char * (__cdecl * GetUserDirectory)();
	void(__cdecl * ChangeToUserDir)();
	void(__cdecl * ChangeToProgramDir)(const char *);

	float(__cdecl * FindGroundZ)(float x, float y);
	CMarker		* RadarBlips;

	CHandling	* Handling;

	CPlayerPed * (__cdecl * GetPlayerPed)(DWORD);
	CBaseModelInfo **Models;

	void(__cdecl * SpawnCar)(DWORD);

	WORD last_opcode = 0;
	WORD last_custom_opcode = 0;
	char last_thread[8] = "none";
	CRunningScript * last_script;
	ptrdiff_t last_off = -1;

	// opcode handler for opcodes, defined by user with cleo api
	OpcodeResult __fastcall extraOpcodeHandler(CRunningScript *thread, int dummy, unsigned short opcode)
	{
		last_custom_opcode = opcode;
		last_script = thread;
		return extraOpcodeHandlers[opcode % 100][opcode / 100 - 28](thread);
	}

	// opcode handler for custom opcodes
	OpcodeResult __fastcall customOpcodeHandler(CRunningScript *thread, int dummy, unsigned short opcode)
	{
		last_custom_opcode = opcode;
		last_script = thread;
		return customOpcodeHandlers[opcode - 0x0A8C](thread);
	}

	char ScriptExecutionLoop()
	{
		CCustomScript *thread;
		OpcodeResult res;

		_asm mov thread, esi

		last_script = thread;

		try
		{
			do
			{
				ptrdiff_t off = thread->IsCustom() ? thread->GetBytePointer() - thread->GetBasePointer() : thread->GetBytePointer() - scmBlock;
				WORD opcode = thread->ReadDataWord();
				last_opcode = opcode;
				last_off = off;
				memcpy(last_thread, thread->GetName(), 8);
				thread->SetNotFlag((opcode & 0x8000) != 0);
				opcode &= 0x7FFF;
				res = newOpcodeHandlerTable[opcode / 100](thread, opcode);
			} while (res == OR_CONTINUE);
		}
		catch (const char * e)
		{
			char str[128];
			sprintf(str, "%s encountered while parsing opcode '%04X' in script '%s'", e, last_opcode, last_thread);
			Error(str);
		}
		return 0;
	}

	void CCustomOpcodeSystem::Inject(CCodeInjector& inj)
	{
		TRACE("Injecting CustomOpcodeSystem...");
		CGameVersionManager& gvm = GetInstance().VersionManager;
		oldOpcodeHandlerTable = gvm.TranslateMemoryAddress(MA_OPCODE_HANDLER);

		// add handler for custom opcodes
		oldOpcodeHandlerTable[27] = reinterpret_cast<_OpcodeHandler>(customOpcodeHandler);

		// replace old OpcodeHandlerTable with the new one
		//inj.MemoryWrite(gvm.TranslateMemoryAddress(MA_OPCODE_HANDLER_REF), reinterpret_cast<_OpcodeHandler>(&newOpcodeHandlerTable[0]));
		MemWrite(gvm.TranslateMemoryAddress(MA_OPCODE_HANDLER_REF), reinterpret_cast<_OpcodeHandler>(&newOpcodeHandlerTable[0]));

		// copy old table to the new                                                             
		//std::copy(oldOpcodeHandlerTable, oldOpcodeHandlerTable + 28, newOpcodeHandlerTable);
		MemCopy<_OpcodeHandler>(newOpcodeHandlerTable, oldOpcodeHandlerTable, (&oldOpcodeHandlerTable[28] - oldOpcodeHandlerTable) * 4);
		//MemCopy(newOpcodeHandlerTable, oldOpcodeHandlerTable, oldOpcodeHandlerTable - (oldOpcodeHandlerTable + 28));

		// fill the rest with default handler
		std::fill(newOpcodeHandlerTable + 28, newOpcodeHandlerTable + 329, reinterpret_cast<_OpcodeHandler>(extraOpcodeHandler));

		FUNC_fopen = gvm.TranslateMemoryAddress(MA_FOPEN_FUNCTION);
		FUNC_fclose = gvm.TranslateMemoryAddress(MA_FCLOSE_FUNCTION);
		FUNC_fread = gvm.TranslateMemoryAddress(MA_FREAD_FUNCTION);
		FUNC_fwrite = gvm.TranslateMemoryAddress(MA_FWRITE_FUNCTION);
		FUNC_fgetc = gvm.TranslateMemoryAddress(MA_FGETC_FUNCTION);
		FUNC_fgets = gvm.TranslateMemoryAddress(MA_FGETS_FUNCTION);
		FUNC_fputs = gvm.TranslateMemoryAddress(MA_FPUTS_FUNCTION);
		FUNC_fseek = gvm.TranslateMemoryAddress(MA_FSEEK_FUNCTION);
		FUNC_fprintf = gvm.TranslateMemoryAddress(MA_FPRINTF_FUNCTION);
		FUNC_ftell = gvm.TranslateMemoryAddress(MA_FTELL_FUNCTION);
		FUNC_fflush = gvm.TranslateMemoryAddress(MA_FFLUSH_FUNCTION);
		FUNC_feof = gvm.TranslateMemoryAddress(MA_FEOF_FUNCTION);
		FUNC_ferror = gvm.TranslateMemoryAddress(MA_FERROR_FUNCTION);

		pedPool = gvm.TranslateMemoryAddress(MA_PED_POOL);
		vehiclePool = gvm.TranslateMemoryAddress(MA_VEHICLE_POOL);
		objectPool = gvm.TranslateMemoryAddress(MA_OBJECT_POOL);
		GetUserDirectory = gvm.TranslateMemoryAddress(MA_GET_USER_DIR_FUNCTION);
		ChangeToUserDir = gvm.TranslateMemoryAddress(MA_CHANGE_TO_USER_DIR_FUNCTION);
		ChangeToProgramDir = gvm.TranslateMemoryAddress(MA_CHANGE_TO_PROGRAM_DIR_FUNCTION);
		FindGroundZ = gvm.TranslateMemoryAddress(MA_FIND_GROUND_Z_FUNCTION);
		GetPlayerPed = gvm.TranslateMemoryAddress(MA_GET_PLAYER_PED_FUNCTION);
		Handling = gvm.TranslateMemoryAddress(MA_HANDLING);
		Models = gvm.TranslateMemoryAddress(MA_MODELS);
		SpawnCar = gvm.TranslateMemoryAddress(MA_SPAWN_CAR_FUNCTION);

		// TODO: consider version-agnostic code
		if (gvm.GetGameVersion() == GV_US10) {
			// make it compatible with fastman92's limit adjuster (only required for 1.0 US)
			RadarBlips = injector::ReadMemory<CMarker*>(0x583A05 + 2, true);
		}
		else {
			RadarBlips = gvm.TranslateMemoryAddress(MA_RADAR_BLIPS);
		}

		/*if(gvm.GetGameVersion() == GV_US10)
		{
		inj.Nop(0x469FB0, 0x469FFB - 0x469FB0);
		inj.ReplaceFunction(ScriptExecutionLoop, 0x469FF6);
		inj.Nop(0x469FF2, 0x469FFB - 0x469FF2);
		inj.ReplaceFunction(ScriptExecutionLoop, 0x469FF6);
		}*/
	}

	inline CRunningScript& operator>>(CRunningScript& thread, DWORD& uval)
	{
		GetScriptParams(&thread, 1);
		uval = opcodeParams[0].dwParam;
		return thread;
	}

	inline CRunningScript& operator<<(CRunningScript& thread, DWORD uval)
	{
		opcodeParams[0].dwParam = uval;
		SetScriptParams(&thread, 1);
		return thread;
	}

	inline CRunningScript& operator>>(CRunningScript& thread, int& nval)
	{
		GetScriptParams(&thread, 1);
		nval = opcodeParams[0].nParam;
		return thread;
	}

	inline CRunningScript& operator<<(CRunningScript& thread, int nval)
	{
		opcodeParams[0].nParam = nval;
		SetScriptParams(&thread, 1);
		return thread;
	}

	inline CRunningScript& operator>>(CRunningScript& thread, float& fval)
	{
		GetScriptParams(&thread, 1);
		fval = opcodeParams[0].fParam;
		return thread;
	}

	inline CRunningScript& operator<<(CRunningScript& thread, float fval)
	{
		opcodeParams[0].fParam = fval;
		SetScriptParams(&thread, 1);
		return thread;
	}

	inline CRunningScript& operator>>(CRunningScript& thread, CVector& vec)
	{
		GetScriptParams(&thread, 3);
		vec.x = opcodeParams[0].fParam;
		vec.y = opcodeParams[1].fParam;
		vec.z = opcodeParams[2].fParam;
		return thread;
	}

	inline CRunningScript& operator<<(CRunningScript& thread, const CVector& vec)
	{
		opcodeParams[0].fParam = vec.x;
		opcodeParams[1].fParam = vec.y;
		opcodeParams[2].fParam = vec.z;
		SetScriptParams(&thread, 3);
		return thread;
	}

	template<typename T>
	inline CRunningScript& operator>>(CRunningScript& thread, T *& pval)
	{
		GetScriptParams(&thread, 1);
		pval = reinterpret_cast<T *>(opcodeParams[0].pParam);
		return thread;
	}

	template<typename T>
	inline CRunningScript& operator<<(CRunningScript& thread, T *pval)
	{
		opcodeParams[0].pParam = (void *)(pval);
		SetScriptParams(&thread, 1);
		return thread;
	}

	inline CRunningScript& operator>>(CRunningScript& thread, memory_pointer& pval)
	{
		GetScriptParams(&thread, 1);
		pval = opcodeParams[0].pParam;
		return thread;
	}

	template<typename T>
	inline CRunningScript& operator<<(CRunningScript& thread, memory_pointer pval)
	{
		opcodeParams[0].pParam = pval;
		SetScriptParams(&thread, 1);
		return thread;
	}

	// read string parameter according to convention on strings
	char *readString(CRunningScript *thread, char* buf = nullptr, BYTE size = 0)
	{
		if (size == 0) size = MAX_STR_LEN;

		auto paramType = *thread->GetBytePointer();
		if (!paramType) return nullptr;

		if (paramType >= DT_DWORD && paramType <= DT_LVAR_ARRAY) // process parameter as a pointer to string
		{
			GetScriptParams(thread, 1);

			if (buf != nullptr)
			{
				strncpy(buf, opcodeParams[0].pcParam, size - 1);
				buf[size - 1] = '\0';
			}

			return opcodeParams[0].pcParam; // original string pointer
		}
		else // process as scm string
		{
			// no user output buffer provided
			if (buf == nullptr)
			{
				static char result[MAX_STR_LEN];
				buf = result;
				size = sizeof(result);
			}

			std::fill(buf, buf + size, '\0');

			if (paramType == DT_VARLEN_STRING)
			{
				// prococess here as GetScriptStringParam can not obtain strings with lenght greater than 128
				thread->IncPtr(1); // already read paramType

				BYTE length = *thread->GetBytePointer(); // as unsigned!
				thread->IncPtr(1); // length

				if (length > 0)
				{
					auto count = min(size, length);
					memcpy(buf, thread->GetBytePointer(), count);

					thread->IncPtr(length); // read text
				}
			}
			else
			{
				GetScriptStringParam(thread, buf, size);
			}

			return buf;
		}
	}

	// perform 'sprintf'-operation for parameters, passed through SCM
	int format(CRunningScript *thread, char *str, size_t len, const char *format)
	{
		unsigned int written = 0;
		const char *iter = format;
		char bufa[256], fmtbufa[64], *fmta;

		while (*iter)
		{
			while (*iter && *iter != '%')
			{
				if (written++ >= len)
					return -1;
				*str++ = *iter++;
			}
			if (*iter == '%')
			{
				if (iter[1] == '%')
				{
					if (written++ >= len)
						return -1;
					*str++ = '%'; /* "%%"->'%' */
					iter += 2;
					continue;
				}

				//get flags and width specifier
				fmta = fmtbufa;
				*fmta++ = *iter++;
				while (*iter == '0' ||
					   *iter == '+' ||
					   *iter == '-' ||
					   *iter == ' ' ||
					   *iter == '*' ||
					   *iter == '#')
				{
					if (*iter == '*')
					{
						char *buffiter = bufa;
						//get width
						GetScriptParams(thread, 1);
						_itoa(opcodeParams[0].dwParam, buffiter, 10);
						while (*buffiter)
							*fmta++ = *buffiter++;
					}
					else
						*fmta++ = *iter;
					iter++;
				}

				//get immidiate width value
				while (isdigit(*iter))
					*fmta++ = *iter++;

				//get precision
				if (*iter == '.')
				{
					*fmta++ = *iter++;
					if (*iter == '*')
					{
						char *buffiter = bufa;
						GetScriptParams(thread, 1);
						_itoa(opcodeParams[0].dwParam, buffiter, 10);
						while (*buffiter)
							*fmta++ = *buffiter++;
					}
					else
						while (isdigit(*iter))
							*fmta++ = *iter++;
				}
				//get size
				if (*iter == 'h' || *iter == 'l')
					*fmta++ = *iter++;

				switch (*iter)
				{
				case 's':
				{
					static const char none[] = "(null)";
					const char *astr = readString(thread);
					const char *striter = astr ? astr : none;
					while (*striter)
					{
						if (written++ >= len)
							return -1;
						*str++ = *striter++;
					}
					iter++;
					break;
				}

				case 'c':
					if (written++ >= len)
						return -1;
					GetScriptParams(thread, 1);
					*str++ = (char)opcodeParams[0].nParam;
					iter++;
					break;

				default:
				{
					/* For non wc types, use system sprintf and append to wide char output */
					/* FIXME: for unrecognised types, should ignore % when printing */
					char *bufaiter = bufa;
					if (*iter == 'p' || *iter == 'P')
					{
						GetScriptParams(thread, 1);
						sprintf(bufaiter, "%08X", opcodeParams[0].dwParam);
					}
					else
					{
						*fmta++ = *iter;
						*fmta = '\0';
						if (*iter == 'a' || *iter == 'A' ||
							*iter == 'e' || *iter == 'E' ||
							*iter == 'f' || *iter == 'F' ||
							*iter == 'g' || *iter == 'G')
						{
							GetScriptParams(thread, 1);
							sprintf(bufaiter, fmtbufa, opcodeParams[0].fParam);
						}
						else
						{
							GetScriptParams(thread, 1);
							sprintf(bufaiter, fmtbufa, opcodeParams[0].pParam);
						}
					}
					while (*bufaiter)
					{
						if (written++ >= len)
							return -1;
						*str++ = *bufaiter++;
					}
					iter++;
					break;
				}
				}
			}
		}
		if (written >= len)
			return -1;
		*str++ = 0;
		return (int)written;
	}

	// Legacy modes for CLEO 3
	FILE * legacy_fopen(const char * szPath, const char * szMode)
	{
		FILE * hFile;
		_asm
		{
			push szMode
			push szPath
			call FUNC_fopen
			add esp, 8
			mov hFile, eax
		}
		return hFile;
	}
	void legacy_fclose(FILE * hFile)
	{
		_asm
		{
			push hFile
			call FUNC_fclose
			add esp, 4
		}
	}
	size_t legacy_fread(void * buf, size_t len, size_t count, FILE * stream)
	{
		_asm
		{
			push stream
			push count
			push len
			push buf
			call FUNC_fread
			add esp, 0x10
		}
	}
	size_t legacy_fwrite(const void * buf, size_t len, size_t count, FILE * stream)
	{
		_asm
		{
			push stream
			push count
			push len
			push buf
			call FUNC_fwrite
			add esp, 0x10
		}
	}
	char legacy_fgetc(FILE * stream)
	{
		_asm
		{
			push stream
			call FUNC_fgetc
			add esp, 0x4
		}
	}
	char * legacy_fgets(char *pStr, int num, FILE * stream)
	{
		_asm
		{
			push stream
			push num
			push pStr
			call FUNC_fgets
			add esp, 0xC
		}
	}
	int legacy_fputs(const char *pStr, FILE * stream)
	{
		_asm
		{
			push stream
			push pStr
			call FUNC_fputs
			add esp, 0x8
		}
	}
	int legacy_fseek(FILE * stream, long int offs, int original)
	{
		_asm
		{
			push stream
			push offs
			push original
			call FUNC_fseek
			add esp, 0xC
		}
	}
	int legacy_ftell(FILE * stream)
	{
		_asm
		{
			push stream
			call FUNC_ftell
			add esp, 0x4
		}
	}
	int __declspec(naked) fprintf(FILE * stream, const char * format, ...)
	{
		_asm jmp FUNC_fprintf
	}
	int legacy_fflush(FILE * stream)
	{
		_asm
		{
			push stream
			call FUNC_fflush
			add esp, 0x4
		}
	}
	int legacy_feof(FILE * stream)
	{
		_asm
		{
			push stream
			call FUNC_feof
			add esp, 0x4
		}
	}
	int legacy_ferror(FILE * stream)
	{
		_asm
		{
			push stream
			call FUNC_ferror
			add esp, 0x4
		}
	}

	bool is_legacy_handle(DWORD dwHandle) { return (dwHandle & 0x1) == 0; }
	FILE * convert_handle_to_file(DWORD dwHandle) { return dwHandle ? reinterpret_cast<FILE*>(is_legacy_handle(dwHandle) ? dwHandle : dwHandle & ~(0x1)) : nullptr; }

	inline DWORD open_file(const char * szPath, const char * szMode, bool bLegacy)
	{
		FILE * hFile = bLegacy ? legacy_fopen(szPath, szMode) : fopen(szPath, szMode);
		if (hFile) return bLegacy ? (DWORD)hFile : (DWORD)hFile | 0x1;
		return NULL;
	}
	inline void close_file(DWORD dwHandle)
	{
		if (is_legacy_handle(dwHandle)) legacy_fclose(convert_handle_to_file(dwHandle));
		else fclose(convert_handle_to_file(dwHandle));
	}
	inline DWORD file_get_size(DWORD file_handle)
	{
		FILE * hFile = convert_handle_to_file(file_handle);
		if (hFile)
		{
			auto savedPos = ftell(hFile);
			fseek(hFile, 0, SEEK_END);
			DWORD dwSize = static_cast<DWORD>(ftell(hFile));
			fseek(hFile, savedPos, SEEK_SET);
			return dwSize;
		}
		return 0;
	}
	inline DWORD read_file(void *buf, DWORD size, DWORD count, DWORD hFile)
	{
		return is_legacy_handle(hFile) ? legacy_fread(buf, size, 1, convert_handle_to_file(hFile)) : fread(buf, size, 1, convert_handle_to_file(hFile));
	}
	inline DWORD write_file(const void *buf, DWORD size, DWORD count, DWORD hFile)
	{
		return is_legacy_handle(hFile) ? legacy_fwrite(buf, size, 1, convert_handle_to_file(hFile)) : fwrite(buf, size, 1, convert_handle_to_file(hFile));
	}
	inline void flush_file(DWORD dwHandle)
	{
		if (is_legacy_handle(dwHandle)) legacy_fflush(convert_handle_to_file(dwHandle));
		else fflush(convert_handle_to_file(dwHandle));
	}

	/*inline void __impl_RetrieveScriptParam(SCRIPT_VAR*) { }
	template<typename ..Params> inline void __impl_RetrieveScriptParam(SCRIPT_VAR *, unsigned&, Params&...);
	template<typename Params> inline void __impl_RetrieveScriptParam(SCRIPT_VAR *, int&, Params&...);
	template<typename Params> inline void __impl_RetrieveScriptParam(SCRIPT_VAR *, float&, Params&...);
	template<typename ThisParam, typename Params> inline void __impl_RetrieveScriptParam(SCRIPT_VAR *, ThisParam *&, Params&...);

	template<typename Params>
	inline void __impl_RetrieveScriptParam(SCRIPT_VAR *var, unsigned& thisParam, Params&... restParams)
	{
	thisParam = var->dwParam;
	__impl_RetrieveScriptParam(var + 1, restParams...);
	}

	template<typename... Params>
	inline void __impl_RetrieveScriptParam(SCRIPT_VAR *var, int& thisParam, Params&... restParams)
	{
	thisParam = var->nParam;
	__impl_RetrieveScriptParam(var + 1, restParams...);
	}

	template<typename... Params>
	inline void __impl_RetrieveScriptParam(SCRIPT_VAR *var, float& thisParam, Params&... restParams)
	{
	thisParam = var->fParam;
	__impl_RetrieveScriptParam(var + 1, restParams...);
	}

	template<typename ThisParam, typename... Params>
	inline void __impl_RetrieveScriptParam(SCRIPT_VAR *var, ThisParam *& thisParam, Params&... restParams)
	{
	thisParam = reinterpret_cast<ThisParam *>(var->pParam);
	__impl_RetrieveScriptParam(var + 1, restParams...);
	}


	template<typename... Params>
	inline void RetrieveScriptParams(CCustomScript *thread, Params&... params)
	{
	GetScriptParams(thread, sizeof...(params));
	__impl_RetrieveScriptParam(opcodeParams, params...);
	}*/

	inline void ThreadJump(CRunningScript *thread, int off)
	{
		thread->SetIp(off < 0 ? thread->GetBasePointer() - off : scmBlock + off);
	}

	inline void SkipUnusedParameters(CRunningScript *thread)
	{
		while (*thread->GetBytePointer()) GetScriptParams(thread, 1);	// skip parameters
		thread->ReadDataByte();
	}

	struct ScmFunction
	{
		unsigned short prevScmFunctionId, thisScmFunctionId;
		BYTE *retnAddress;
		SCRIPT_VAR savedTls[32];
		std::list<std::string> stringParams; // texts with this scope lifetime
		bool savedCondResult;
		eLogicalOperation savedLogicalOp;
		bool savedNotFlag;
		static const size_t store_size = 0x400;
		static ScmFunction *Store[store_size];
		static size_t allocationPlace;			// contains an index of last allocated object

		void *operator new(size_t size)
		{
			size_t start_search = allocationPlace;
			while (Store[allocationPlace])	// find first unused position in store
			{
				if (++allocationPlace >= store_size) allocationPlace = 0;		// end of store reached
				if (allocationPlace == start_search) throw std::bad_alloc();	// the store is filled up
			}
			ScmFunction *obj = reinterpret_cast<ScmFunction *>(::operator new(size));
			Store[allocationPlace] = obj;
			return obj;
		}

		void operator delete(void *mem)
		{
			Store[reinterpret_cast<ScmFunction *>(mem)->thisScmFunctionId] = nullptr;
			::operator delete(mem);
		}

		ScmFunction(CRunningScript *thread) :
			prevScmFunctionId(reinterpret_cast<CCustomScript*>(thread)->GetScmFunction())
		{
			auto cs = reinterpret_cast<CCustomScript*>(thread);

			// create snapshot of current scope
			auto scope = cs->IsMission() ? missionLocals : cs->LocalVar;
			std::copy(scope, scope + 32, savedTls);
			savedCondResult = cs->bCondResult;
			savedLogicalOp = cs->LogicalOp;
			savedNotFlag = cs->NotFlag;

			cs->bCondResult = false;
			cs->LogicalOp = eLogicalOperation::NONE;
			cs->NotFlag = false;

			cs->SetScmFunction(thisScmFunctionId = allocationPlace);
		}

		void Return(CRunningScript *thread)
		{
			// restore parent scope's local variables
			auto cs = reinterpret_cast<CCustomScript*>(thread);
			std::copy(savedTls, savedTls + 32, cs->IsMission() ? missionLocals : cs->LocalVar);

			// process conditional result of just ended function in parent scope
			bool condResult = cs->bCondResult;
			if (savedNotFlag) condResult = !condResult;

			if (savedLogicalOp >= eLogicalOperation::AND_2 && savedLogicalOp < eLogicalOperation::AND_END)
			{
				cs->bCondResult = savedCondResult && condResult;
				cs->LogicalOp = --savedLogicalOp;
			}
			else if(savedLogicalOp >= eLogicalOperation::OR_2 && savedLogicalOp < eLogicalOperation::OR_END)
			{
				cs->bCondResult = savedCondResult || condResult;
				cs->LogicalOp = --savedLogicalOp;
			}
			else // eLogicalOperation::NONE
			{
				cs->bCondResult = condResult;
				cs->LogicalOp = savedLogicalOp;
			}

			cs->SetIp(retnAddress);
			cs->SetScmFunction(prevScmFunctionId);
		}
	};

	ScmFunction *ScmFunction::Store[store_size] = { /* default initializer - nullptr */ };
	size_t ScmFunction::allocationPlace = 0;

	void ResetScmFunctionStore()
	{
		for each(ScmFunction *scmFunc in ScmFunction::Store)
		{
			if (scmFunc) delete scmFunc;
		}
		ScmFunction::allocationPlace = 0;
	}

	/************************************************************************/
	/*						Opcode definitions								*/
	/************************************************************************/

	//0A8C=4,write_memory %1d% size %2d% value %3d% virtual_protect %4d%
	OpcodeResult __stdcall opcode_0A8C(CRunningScript *thread)
	{
		GetScriptParams(thread, 4);
		void *Address = opcodeParams[0].pParam;
		DWORD size = opcodeParams[1].dwParam;
		DWORD value = opcodeParams[2].dwParam;
		bool vp = opcodeParams[3].bParam;
		switch (size)
		{
		default:
			GetInstance().CodeInjector.MemoryWrite<BYTE>(Address, value, vp, size);
			break;
		case 2:
			GetInstance().CodeInjector.MemoryWrite<WORD>(Address, value, vp);
			break;
		case 4:
			GetInstance().CodeInjector.MemoryWrite<DWORD>(Address, value, vp);
			break;
		}
		return OR_CONTINUE;
	}

	//0A8D=4,%4d% = read_memory %1d% size %2d% virtual_protect %3d%
	OpcodeResult __stdcall opcode_0A8D(CRunningScript *thread)
	{
		GetScriptParams(thread, 3);
		//DWORD value;
		void *Address = opcodeParams[0].pParam;
		DWORD size = opcodeParams[1].dwParam;
		bool vp = opcodeParams[2].bParam;

		opcodeParams[0].dwParam = 0;

		switch (size)
		{
		case 1:
			GetInstance().CodeInjector.MemoryRead(Address, (BYTE)opcodeParams[0].ucParam, vp);
			break;
		case 2:
			GetInstance().CodeInjector.MemoryRead(Address, (WORD)opcodeParams[0].usParam, vp);
			break;
		case 4:
			GetInstance().CodeInjector.MemoryRead(Address, (DWORD)opcodeParams[0].dwParam, vp);
			break;
		default:
			TRACE("[0A8D] Unallowed size %u", size);
		}

		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0A8E=3,%3d% = %1d% + %2d% ; int
	OpcodeResult __stdcall opcode_0A8E(CRunningScript *thread)
	{
		GetScriptParams(thread, 2);
		opcodeParams[0].nParam += opcodeParams[1].nParam;
		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0A8F=3,%3d% = %1d% - %2d% ; int
	OpcodeResult __stdcall opcode_0A8F(CRunningScript *thread)
	{
		GetScriptParams(thread, 2);
		opcodeParams[0].nParam -= opcodeParams[1].nParam;
		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0A90=3,%3d% = %1d% * %2d% ; int
	OpcodeResult __stdcall opcode_0A90(CRunningScript *thread)
	{
		GetScriptParams(thread, 2);
		opcodeParams[0].nParam *= opcodeParams[1].nParam;
		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0A91=3,%3d% = %1d% / %2d% ; int
	OpcodeResult __stdcall opcode_0A91(CRunningScript *thread)
	{
		GetScriptParams(thread, 2);
		opcodeParams[0].nParam /= opcodeParams[1].nParam;
		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0A92=-1,create_custom_thread %1d%
	OpcodeResult __stdcall opcode_0A92(CRunningScript *thread)
	{
		const char *script_name = readString(thread);
		TRACE("[0A92] Starting new custom script %s from thread named %s", script_name, thread->GetName());
		char cwd[MAX_PATH];
		_getcwd(cwd, sizeof(cwd));
		_chdir(cleo_dir);
		auto cs = new CCustomScript(script_name);
		SetScriptCondResult(thread, cs && cs->IsOK());
		if (cs && cs->IsOK())
		{
			GetInstance().ScriptEngine.AddCustomScript(cs);
			TransmitScriptParams(thread, cs);
		}
		else
		{
			if (cs) delete cs;
			SkipUnusedParameters(thread);
			TRACE("[0A92] Failed to load script '%s' from script '%s'.", script_name, thread->GetName());
		}
		_chdir(cwd);
		return OR_CONTINUE;
	}

	//0A93=0,end_custom_thread
	OpcodeResult __stdcall opcode_0A93(CRunningScript *thread)
	{
		CCustomScript *cs = reinterpret_cast<CCustomScript *>(thread);
		if (thread->IsMission() || !cs->IsCustom())
		{
			TRACE("[0A93] Incorrect usage of opcode in script '%s'", thread->GetName());
			return OR_CONTINUE;
		}
		GetInstance().ScriptEngine.RemoveCustomScript(cs);
		return OR_INTERRUPT;
	}

	//0A94=-1,create_custom_mission %1d%
	OpcodeResult __stdcall opcode_0A94(CRunningScript *thread)
	{
		char script_name[MAX_PATH];
		readString(thread, script_name);
		strcat(script_name, ".cm");		// add custom mission extension
		TRACE("[0A94] Starting new custom mission %s from thread named %s", script_name, thread->GetName());
		char cwd[MAX_PATH];
		_getcwd(cwd, sizeof(cwd));
		_chdir(cleo_dir);
		auto cs = new CCustomScript(script_name, true);
		SetScriptCondResult(thread, cs && cs->IsOK());
		if (cs && cs->IsOK())
		{
			auto csscript = reinterpret_cast<CCustomScript*>(thread);
			if (csscript->IsCustom())
				cs->SetCompatibility(csscript->GetCompatibility());
			GetInstance().ScriptEngine.AddCustomScript(cs);
			TransmitScriptParams(thread, (CRunningScript*)((BYTE*)missionLocals - 0x3C));
		}
		else
		{
			if (cs) delete cs;
			SkipUnusedParameters(thread);
			TRACE("[0A94] Failed to load mission '%s' from script '%s'.", script_name, thread->GetName());
		}
		_chdir(cwd);
		return OR_CONTINUE;
	}

	//0A95=0,enable_thread_saving
	OpcodeResult __stdcall opcode_0A95(CRunningScript *thread)
	{
		reinterpret_cast<CCustomScript *>(thread)->enable_saving();
		return OR_CONTINUE;
	}

	//0A96=2,%2d% = actor %1d% struct
	OpcodeResult __stdcall opcode_0A96(CRunningScript *thread)
	{
		DWORD handle;
		*thread >> handle;
		*thread << GetPedPool().GetAtRef(handle);
		return OR_CONTINUE;
	}

	//0A97=2,%2d% = car %1d% struct
	OpcodeResult __stdcall opcode_0A97(CRunningScript *thread)
	{
		DWORD handle;
		*thread >> handle;
		*thread << GetVehiclePool().GetAtRef(handle);
		return OR_CONTINUE;
	}

	//0A98=2,%2d% = object %1d% struct
	OpcodeResult __stdcall opcode_0A98(CRunningScript *thread)
	{
		DWORD handle;
		*thread >> handle;
		*thread << GetObjectPool().GetAtRef(handle);
		return OR_CONTINUE;
	}

	//0A99=1,chdir %1b:userdir/rootdir%
	OpcodeResult __stdcall opcode_0A99(CRunningScript *thread)
	{
		auto paramType = *thread->GetBytePointer();
		if (paramType >= 1 && paramType <= 8)
		{
			// integer param
			DWORD param;
			*thread >> param;
			//_chdir(param ? GetUserDirectory() : "");
			if (param) ChangeToUserDir();
			else ChangeToProgramDir("");
		}
		else
		{
			// string param
			char buf[MAX_PATH];
			std::fill(buf, buf + sizeof(buf), '\0');
			GetScriptStringParam(thread, buf, (BYTE)sizeof(buf));
			_chdir(buf);
		}
		return OR_CONTINUE;
	}

	//0A9A=3,%3d% = openfile %1d% mode %2d% // IF and SET
	OpcodeResult __stdcall opcode_0A9A(CRunningScript *thread)
	{
		const char *fname = readString(thread);
		auto paramType = *thread->GetBytePointer();
		char mode[0x10];

		// either CLEO 3 or CLEO 4 made a big mistake! (they differ in one major unapparent preference)
		// lets try to resolve this with a legacy mode
		auto cs = (CCustomScript*)thread;
		bool bLegacyMode = cs->IsCustom() && cs->GetCompatibility() < CLEO_VER_4_3;

		if (paramType >= 1 && paramType <= 8)
		{
			// integer param (for backward compatibility with CLEO 3)
			union
			{
				DWORD uParam;
				char strParam[4];
			} param;
			*thread >> param.uParam;
			strcpy(mode, param.strParam);
		}
		else
		{
			// string param
			GetScriptStringParam(thread, mode, sizeof(mode));
		}

		if (auto hfile = open_file(fname, mode, bLegacyMode))
		{
			GetInstance().OpcodeSystem.m_hFiles.insert(hfile);

			*thread << hfile;
			SetScriptCondResult(thread, true);
		}
		else
		{
			*thread << NULL;
			SetScriptCondResult(thread, false);
		}

		char szBlah[MAX_PATH];
		_getcwd(szBlah, MAX_PATH);

		return OR_CONTINUE;
	}

	//0A9B=1,closefile %1d%
	OpcodeResult __stdcall opcode_0A9B(CRunningScript *thread)
	{
		DWORD hFile;
		*thread >> hFile;
		if (convert_handle_to_file(hFile))
		{
			close_file(hFile);
			GetInstance().OpcodeSystem.m_hFiles.erase(hFile);
		}
		return OR_CONTINUE;
	}

	//0A9C=2,%2d% = file %1d% size
	OpcodeResult __stdcall opcode_0A9C(CRunningScript *thread)
	{
		DWORD hFile;
		*thread >> hFile;
		if (convert_handle_to_file(hFile)) *thread << file_get_size(hFile);
		return OR_CONTINUE;
	}

	//0A9D=3,readfile %1d% size %2d% to %3d%
	OpcodeResult __stdcall opcode_0A9D(CRunningScript *thread)
	{
		DWORD hFile;
		DWORD size;
		*thread >> hFile >> size;

		SCRIPT_VAR* buf = GetScriptParamPointer(thread);
		buf->dwParam = 0; // https://github.com/cleolibrary/CLEO4/issues/91

		if (convert_handle_to_file(hFile)) read_file(buf, size, 1, hFile);
		return OR_CONTINUE;
	}

	//0A9E=3,writefile %1d% size %2d% from %3d%
	OpcodeResult __stdcall opcode_0A9E(CRunningScript *thread)
	{
		DWORD hFile;
		DWORD size;
		const void *buf;
		*thread >> hFile >> size;
		buf = GetScriptParamPointer(thread);
		if (convert_handle_to_file(hFile))
		{
			write_file(buf, size, 1, hFile);
			flush_file(hFile);
		}
		return OR_CONTINUE;
	}

	//0A9F=1,%1d% = current_thread_pointer
	OpcodeResult __stdcall opcode_0A9F(CRunningScript *thread)
	{
		*thread << thread;
		return OR_CONTINUE;
	}

	//0AA0=1,gosub_if_false %1p%
	OpcodeResult __stdcall opcode_0AA0(CRunningScript *thread)
	{
		int off;
		*thread >> off;
		if (thread->GetConditionResult()) return OR_CONTINUE;
		thread->PushStack(thread->GetBytePointer());
		ThreadJump(thread, off);
		return OR_CONTINUE;
	}

	//0AA1=0,return_if_false
	OpcodeResult __stdcall opcode_0AA1(CRunningScript *thread)
	{
		if (thread->GetConditionResult()) return OR_CONTINUE;
		thread->SetIp(thread->PopStack());
		return OR_CONTINUE;
	}

	//0AA2=2,%2h% = load_library %1d% // IF and SET
	OpcodeResult __stdcall opcode_0AA2(CRunningScript *thread)
	{
		auto libHandle = LoadLibrary(readString(thread));
		*thread << libHandle;
		SetScriptCondResult(thread, libHandle != nullptr);
		if (libHandle) GetInstance().OpcodeSystem.m_hNativeLibs.insert(libHandle);

		return OR_CONTINUE;
	}

	//0AA3=1,free_library %1h%
	OpcodeResult __stdcall opcode_0AA3(CRunningScript *thread)
	{
		HMODULE libHandle;
		*thread >> libHandle;
		FreeLibrary(libHandle);
		GetInstance().OpcodeSystem.m_hNativeLibs.erase(libHandle);
		return OR_CONTINUE;
	}

	//0AA4=3,%3d% = get_proc_address %1d% library %2d% // IF and SET
	OpcodeResult __stdcall opcode_0AA4(CRunningScript *thread)
	{
		char *funcName = readString(thread);
		HMODULE libHandle;
		*thread >> libHandle;
		void *funcAddr = (void *)GetProcAddress(libHandle, funcName);
		*thread << funcAddr;
		SetScriptCondResult(thread, funcAddr != nullptr);
		return OR_CONTINUE;
	}

	//0AA5=-1,call %1d% num_params %2h% pop %3h%
	OpcodeResult __stdcall opcode_0AA5(CRunningScript *thread)
	{
		static char textParams[5][MAX_STR_LEN]; unsigned currTextParam = 0;
		static SCRIPT_VAR arguments[50] = { 0 };
		void(*func)();
		DWORD numParams;
		DWORD stackAlign;
		*thread >> func >> numParams >> stackAlign;
		if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
		stackAlign *= 4;
		SCRIPT_VAR	*arguments_end = arguments + numParams;

		// retrieve parameters
		for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
		{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				*thread >> arg->dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				(*arg).pcParam = readString(thread, textParams[currTextParam++], MAX_STR_LEN);
			}
		}

		// call function
		_asm
		{
			lea ecx, arguments
			loop_0AA5 :
			cmp ecx, arguments_end
				jae loop_end_0AA5
				push[ecx]
				add ecx, 0x4
				jmp loop_0AA5
				loop_end_0AA5 :
			call func
				add esp, stackAlign
		}

		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AA6=-1,call_method %1d% struct %2d% num_params %3h% pop %4h%
	OpcodeResult __stdcall opcode_0AA6(CRunningScript *thread)
	{
		static char textParams[5][MAX_STR_LEN]; unsigned currTextParam = 0;
		static SCRIPT_VAR arguments[50] = { 0 };
		void(*func)();
		void *struc;
		DWORD numParams;
		DWORD stackAlign;
		*thread >> func >> struc >> numParams >> stackAlign;
		if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
		stackAlign *= 4;
		SCRIPT_VAR *arguments_end = arguments + numParams;

		// retrieve parameters
		for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
		{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				*thread >> arg->dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				arg->pcParam = readString(thread, textParams[currTextParam++], MAX_STR_LEN);
			}
		}

		_asm
		{
			lea ecx, arguments
			loop_0AA6 :
			cmp ecx, arguments_end
				jae loop_end_0AA6
				push[ecx]
				add ecx, 0x4
				jmp loop_0AA6
				loop_end_0AA6 :
			mov ecx, struc
				call func
				add esp, stackAlign
		}

		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AA7=-1,call_function %1d% num_params %2h% pop %3h%
	OpcodeResult __stdcall opcode_0AA7(CRunningScript *thread)
	{
		static char textParams[5][MAX_STR_LEN];
		static SCRIPT_VAR arguments[50] = { 0 };
		DWORD currTextParam = 0;
		void(*func)();
		DWORD numParams;
		DWORD stackAlign;
		*thread >> func >> numParams >> stackAlign;
		if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
		stackAlign *= 4;
		SCRIPT_VAR	*	arguments_end = arguments + numParams;
		// retrieve parameters
		for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
		{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				*thread >> arg->dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				arg->pcParam = readString(thread, textParams[currTextParam++], MAX_STR_LEN);
				break;
			}
		}

		DWORD result;

		_asm
		{
			lea ecx, arguments
			loop_0AA7 :
			cmp ecx, arguments_end
				jae loop_end_0AA7
				push[ecx]
				add ecx, 0x4
				jmp loop_0AA7
				loop_end_0AA7 :
			call func
				mov result, eax
				add esp, stackAlign
		}

		*thread << result;
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AA8=-1,call_function_method %1d% struct %2d% num_params %3h% pop %4h%
	OpcodeResult __stdcall opcode_0AA8(CRunningScript *thread)
	{
		static char textParams[5][MAX_STR_LEN];
		static SCRIPT_VAR arguments[50] = { 0 };
		DWORD currTextParam = 0;
		void(*func)();
		void *struc;
		DWORD numParams;
		DWORD stackAlign;
		*thread >> func >> struc >> numParams >> stackAlign;
		if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
		stackAlign *= 4;
		SCRIPT_VAR	*arguments_end = arguments + numParams;

		// retrieve parameters
		for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
		{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				*thread >> arg->dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				arg->pcParam = readString(thread, textParams[currTextParam++], MAX_STR_LEN);
			}
		}

		DWORD result;

		_asm
		{
			lea ecx, arguments
			loop_0AA8 :
			cmp ecx, arguments_end
				jae loop_end_0AA8
				push[ecx]
				add ecx, 0x4
				jmp loop_0AA8
				loop_end_0AA8 :
			mov ecx, struc
				call func
				mov result, eax
				add esp, stackAlign
		}

		*thread << result;
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AA9=0,  is_game_version_original
	OpcodeResult __stdcall opcode_0AA9(CRunningScript *thread)
	{
		auto gv = GetInstance().VersionManager.GetGameVersion();
		auto cs = (CCustomScript*)thread;
		SetScriptCondResult(thread, gv == GV_US10 || (cs->IsCustom() && cs->GetCompatibility() <= CLEO_VER_4_MIN && gv == GV_EU10));
		return OR_CONTINUE;
	}

	//0AAA=2,  %2d% = thread %1d% pointer  // IF and SET
	OpcodeResult __stdcall opcode_0AAA(CRunningScript *thread)
	{
		char *threadName = readString(thread);
		threadName[7] = '\0';
		CRunningScript *cs = GetInstance().ScriptEngine.FindCustomScriptNamed(threadName);
		if (!cs) cs = GetInstance().ScriptEngine.FindScriptNamed(threadName);
		*thread << cs;
		SetScriptCondResult(thread, cs != nullptr);
		return OR_CONTINUE;
	}

	//0AAB=1,  file_exists %1d%
	OpcodeResult __stdcall opcode_0AAB(CRunningScript *thread)
	{
		DWORD fAttr = GetFileAttributes(readString(thread));
		SetScriptCondResult(thread, (fAttr != INVALID_FILE_ATTRIBUTES) && !(fAttr & FILE_ATTRIBUTE_DIRECTORY));
		return OR_CONTINUE;
	}

	//0AAC=2,  %2d% = load_audiostream %1d%  // IF and SET
	OpcodeResult __stdcall opcode_0AAC(CRunningScript *thread)
	{
		auto stream = GetInstance().SoundSystem.LoadStream(readString(thread));
		*thread << stream;
		SetScriptCondResult(thread, stream != nullptr);
		return OR_CONTINUE;
	}

	//0AAD=2,set_audiostream %1d% perform_action %2d%
	OpcodeResult __stdcall opcode_0AAD(CRunningScript *thread)
	{
		CAudioStream *stream;
		int action;
		*thread >> stream >> action;
		if (stream)
		{
			switch (action)
			{
			case 0: stream->Stop();   break;
			case 1: stream->Play();   break;
			case 2: stream->Pause();  break;
			case 3: stream->Resume(); break;
			default:
				TRACE("[0AAD] Unknown audiostream's action: %d", action);
			}
		}
		return OR_CONTINUE;
	}

	//0AAE=1,release_audiostream %1d%
	OpcodeResult __stdcall opcode_0AAE(CRunningScript *thread)
	{
		CAudioStream *stream;
		*thread >> stream;
		if (stream) GetInstance().SoundSystem.UnloadStream(stream);
		return OR_CONTINUE;
	}

	//0AAF=2,%2d% = get_audiostream_length %1d%
	OpcodeResult __stdcall opcode_0AAF(CRunningScript *thread)
	{
		CAudioStream *stream;
		*thread >> stream;
		*thread << (stream ? stream->GetLength() : -1);
		return OR_CONTINUE;
	}

	//0AB0=1,  key_pressed %1d%
	OpcodeResult __stdcall opcode_0AB0(CRunningScript *thread)
	{
		DWORD key;
		*thread >> key;
		SHORT state = GetKeyState(key);
		SetScriptCondResult(thread, (GetKeyState(key) & 0x8000) != 0);
		return OR_CONTINUE;
	}

	//0AB1=-1,call_scm_func %1p%
	OpcodeResult __stdcall opcode_0AB1(CRunningScript *thread)
	{
		int		label;
		*thread >> label;

		DWORD nParams = 0;
		if(*thread->GetBytePointer()) *thread >> nParams;

		ScmFunction* scmFunc = new ScmFunction(thread);
		
		static SCRIPT_VAR arguments[32];
		SCRIPT_VAR* locals = thread->IsMission() ? missionLocals : thread->GetVarPtr();
		SCRIPT_VAR* localsEnd = locals + 32;
		SCRIPT_VAR* storedLocals = scmFunc->savedTls;

		// collect arguments
		for (DWORD i = 0; i < min(nParams, 32); i++)
		{
			SCRIPT_VAR* arg = arguments + i;
				
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				*thread >> arg->dwParam;
				break;

			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread);
				if (arg->pParam >= locals && arg->pParam < localsEnd) // correct scoped variable's pointer
				{
					arg->dwParam -= (DWORD)locals;
					arg->dwParam += (DWORD)storedLocals;
				}
				break;

			case DT_STRING:
			case DT_TEXTLABEL:
			case DT_VARLEN_STRING:
				scmFunc->stringParams.emplace_back(readString(thread)); // those texts exists in script code, but without terminator character. Copy is necessary
				arg->pcParam = (char*)scmFunc->stringParams.back().c_str();
				break;
			}
		}

		// skip unused args
		if (nParams > 32) 
			GetScriptParams(thread, nParams - 32);

		// all areguments read
		scmFunc->retnAddress = thread->GetBytePointer();

		// pass arguments as new scope local variables
		memcpy(locals, arguments, nParams * sizeof(SCRIPT_VAR));

		// initialize rest of new scope local variables
		auto cs = reinterpret_cast<CCustomScript*>(thread);
		if (cs->IsCustom() && cs->GetCompatibility() >= CLEO_VER_4_MIN) // CLEO 3 did not initialised local variables
		{
			for (DWORD i = nParams; i < 32; i++)
			{
				cs->SetIntVar(i, 0); // fill with zeros
			}
		}

		// jump to label
		ThreadJump(thread, label);
		return OR_CONTINUE;
	}

	//0AB2=-1,ret
	OpcodeResult __stdcall opcode_0AB2(CRunningScript *thread)
	{
		ScmFunction *scmFunc = ScmFunction::Store[reinterpret_cast<CCustomScript*>(thread)->GetScmFunction()];
		DWORD nRetParams;
		*thread >> nRetParams;
		if (nRetParams) GetScriptParams(thread, nRetParams);
		scmFunc->Return(thread);
		if (nRetParams) SetScriptParams(thread, nRetParams);
		SkipUnusedParameters(thread);
		delete scmFunc;
		return OR_CONTINUE;
	}

	//0AB3=2,var %1d% = %2d%
	OpcodeResult __stdcall opcode_0AB3(CRunningScript *thread)
	{
		DWORD	varId,
			value;
		*thread >> varId >> value;
		GetInstance().ScriptEngine.CleoVariables[varId].dwParam = value;
		return OR_CONTINUE;
	}

	//0AB4=2,%2d% = var %1d%
	OpcodeResult __stdcall opcode_0AB4(CRunningScript *thread)
	{
		DWORD varId;
		*thread >> varId;
		*thread << GetInstance().ScriptEngine.CleoVariables[varId].dwParam;
		return OR_CONTINUE;
	}

	//0AB5=3,store_actor %1d% closest_vehicle_to %2d% closest_ped_to %3d%
	OpcodeResult __stdcall opcode_0AB5(CRunningScript *thread)
	{
		DWORD actor;
		*thread >> actor;
		auto pPlayerPed = GetPedPool().GetAtRef(actor);
		CPedIntelligence * pedintel;
		if (pPlayerPed && (pedintel = pPlayerPed->m_pIntelligence))
		{
			CVehicle * pVehicle = nullptr;
			for (int i = 0; i < NUM_SCAN_ENTITIES; i++)
			{
				pVehicle = (CVehicle*)pedintel->m_vehicleScanner.m_apEntities[i];
				if (pVehicle && pVehicle->m_nCreatedBy != 2 && !pVehicle->m_nVehicleFlags.bFadeOut)
					break;
				pVehicle = nullptr;
			}

			CPed * pPed = nullptr;
			for (int i = 0; i < NUM_SCAN_ENTITIES; i++)
			{
				pPed = (CPed*)pedintel->m_pedScanner.m_apEntities[i];
				if (pPed && pPed != pPlayerPed && (pPed->m_nCreatedBy & 0xFF) == 1 && !pPed->m_nPedFlags.bFadeOut)
					break;
				pPed = nullptr;
			}

			*thread << (pVehicle ? GetVehiclePool().GetRef(pVehicle) : -1) << (pPed ? GetPedPool().GetRef(pPed) : -1);
		}
		else *thread << -1 << -1;
		return OR_CONTINUE;
	}

	//0AB6=3,store_target_marker_coords_to %1d% %2d% %3d% // IF and SET
	OpcodeResult __stdcall opcode_0AB6(CRunningScript *thread)
	{
		// steam offset is different, so get it manually for now
		CGameVersionManager& gvm = GetInstance().VersionManager;
		DWORD hMarker = gvm.GetGameVersion() != GV_STEAM ? MenuManager->m_nTargetBlipIndex : *((DWORD*)0xC3312C);
		CMarker *pMarker;
		if (hMarker && (pMarker = &RadarBlips[LOWORD(hMarker)]) && /*pMarker->m_nPoolIndex == HIWORD(hMarker) && */pMarker->m_nBlipDisplay)
		{
			CVector coords(pMarker->m_vecPos);
			coords.z = FindGroundZ(coords.x, coords.y);
			*thread << coords;
			SetScriptCondResult(thread, true);
		}
		else
		{
			GetScriptParams(thread, 3);
			SetScriptCondResult(thread, false);
		}

		return OR_CONTINUE;
	}

	//0AB7=2,get_vehicle %1d% number_of_gears_to %2d%
	OpcodeResult __stdcall opcode_0AB7(CRunningScript *thread)
	{
		DWORD hVehicle;
		*thread >> hVehicle;
		*thread << GetVehiclePool().GetAtRef(hVehicle)->m_pHandlingData->m_transmissionData.m_nNumberOfGears;
		return OR_CONTINUE;
	}

	//0AB8=2,get_vehicle %1d% current_gear_to %2d%
	OpcodeResult __stdcall opcode_0AB8(CRunningScript *thread)
	{
		DWORD hVehicle;
		*thread >> hVehicle;
		*thread << GetVehiclePool().GetAtRef(hVehicle)->m_nCurrentGear;
		return OR_CONTINUE;
	}

	//0AB9=2,get_audiostream %1d% state_to %2d%
	OpcodeResult __stdcall opcode_0AB9(CRunningScript *thread)
	{
		CAudioStream *stream;
		*thread >> stream;
		*thread << (stream ? stream->GetState() : -1);
		return OR_CONTINUE;
	}

	//0ABA=1,end_custom_thread_named %1d%
	OpcodeResult __stdcall opcode_0ABA(CRunningScript *thread)
	{
		char *threadName = readString(thread);
		auto deleted_thread = GetInstance().ScriptEngine.FindCustomScriptNamed(threadName);
		if (deleted_thread)
		{
			GetInstance().ScriptEngine.RemoveCustomScript(deleted_thread);
		}
		return deleted_thread == thread ? OR_INTERRUPT : OR_CONTINUE;
	}

	//0ABB=2,%2d% = audiostream %1d% volume
	OpcodeResult __stdcall opcode_0ABB(CRunningScript *thread)
	{
		CAudioStream *stream;
		*thread >> stream;
		*thread << (stream ? stream->GetVolume() : 0.0f);
		return OR_CONTINUE;
	}

	//0ABC=2,set_audiostream %1d% volume %2d%
	OpcodeResult __stdcall opcode_0ABC(CRunningScript *thread)
	{
		CAudioStream *stream;
		float volume;
		*thread >> stream >> volume;
		if (stream) stream->SetVolume(volume);
		return OR_CONTINUE;
	}

	//0ABD=1,  vehicle %1d% siren_on
	OpcodeResult __stdcall opcode_0ABD(CRunningScript *thread)
	{
		DWORD hVehicle;
		*thread >> hVehicle;
		SetScriptCondResult(thread, GetVehiclePool().GetAtRef(hVehicle)->m_nVehicleFlags.bSirenOrAlarm);
		return OR_CONTINUE;
	}

	//0ABE=1,  vehicle %1d% engine_on
	OpcodeResult __stdcall opcode_0ABE(CRunningScript *thread)
	{
		DWORD hVehicle;
		*thread >> hVehicle;
		SetScriptCondResult(thread, GetVehiclePool().GetAtRef(hVehicle)->m_nVehicleFlags.bEngineOn);
		return OR_CONTINUE;
	}

	//0ABF=2,set_vehicle %1d% engine_state_to %2d%
	OpcodeResult __stdcall opcode_0ABF(CRunningScript *thread)
	{
		DWORD	hVehicle,
			state;
		*thread >> hVehicle >> state;
		auto veh = GetVehiclePool().GetAtRef(hVehicle);
		veh->m_nVehicleFlags.bEngineOn = state != false;
		return OR_CONTINUE;
	}

	//0AC0=2,loop_audiostream %1d% flag %2d%
	OpcodeResult __stdcall opcode_0AC0(CRunningScript *thread)
	{
		CAudioStream *stream;
		DWORD loop;
		*thread >> stream >> loop;
		if (stream) stream->Loop(loop != false);
		return OR_CONTINUE;
	}

	//0AC1=2,%2d% = load_audiostream_with_3d_support %1d% //IF and SET
	OpcodeResult __stdcall opcode_0AC1(CRunningScript *thread)
	{
		auto stream = GetInstance().SoundSystem.LoadStream(readString(thread), true);
		*thread << stream;
		SetScriptCondResult(thread, stream != nullptr);
		return OR_CONTINUE;
	}

	//0AC2=4,set_3d_audiostream %1d% position %2d% %3d% %4d%
	OpcodeResult __stdcall opcode_0AC2(CRunningScript *thread)
	{
		CAudioStream *stream;
		CVector pos;
		*thread >> stream >> pos;
		if (stream) stream->Set3dPosition(pos);
		return OR_CONTINUE;
	}

	//0AC3=2,link_3d_audiostream %1d% to_object %2d%
	OpcodeResult __stdcall opcode_0AC3(CRunningScript *thread)
	{
		CAudioStream *stream;
		DWORD handle;
		*thread >> stream >> handle;
		if (stream) stream->Link(GetObjectPool().GetAtRef(handle));
		return OR_CONTINUE;
	}

	//0AC4=2,link_3d_audiostream %1d% to_actor %2d%
	OpcodeResult __stdcall opcode_0AC4(CRunningScript *thread)
	{
		CAudioStream *stream;
		DWORD handle;
		*thread >> stream >> handle;
		if (stream) stream->Link(GetPedPool().GetAtRef(handle));
		return OR_CONTINUE;
	}

	//0AC5=2,link_3d_audiostream %1d% to_vehicle %2d%
	OpcodeResult __stdcall opcode_0AC5(CRunningScript *thread)
	{
		CAudioStream *stream;
		DWORD handle;
		*thread >> stream >> handle;
		if (stream) stream->Link(GetVehiclePool().GetAtRef(handle));
		return OR_CONTINUE;
	}

	//0AC6=2,%2d% = label %1p% offset
	OpcodeResult __stdcall opcode_0AC6(CRunningScript *thread)
	{
		int label;
		*thread >> label;
		*thread << (label < 0 ? thread->GetBasePointer() - label : scmBlock + label);
		return OR_CONTINUE;
	}

	//0AC7=2,%2d% = var %1d% offset
	OpcodeResult __stdcall opcode_0AC7(CRunningScript *thread)
	{
		*thread << GetScriptParamPointer(thread);
		return OR_CONTINUE;
	}

	//0AC8=2,%2d% = allocate_memory_size %1d%
	OpcodeResult __stdcall opcode_0AC8(CRunningScript *thread)
	{
		DWORD size;
		*thread >> size;
		void *mem = malloc(size);
		if (mem) GetInstance().OpcodeSystem.m_pAllocations.insert(mem);
		*thread << mem;
		SetScriptCondResult(thread, mem != nullptr);
		return OR_CONTINUE;
	}

	//0AC9=1,free_allocated_memory %1d%
	OpcodeResult __stdcall opcode_0AC9(CRunningScript *thread)
	{
		void *mem;
		*thread >> mem;
		auto & allocs = GetInstance().OpcodeSystem.m_pAllocations;
		if (allocs.find(mem) != allocs.end())
		{
			free(mem);
			allocs.erase(mem);
		}
		return OR_CONTINUE;
	}

	//0ACA=1,show_text_box %1d%
	OpcodeResult __stdcall opcode_0ACA(CRunningScript *thread)
	{
		PrintHelp(readString(thread));
		return OR_CONTINUE;
	}

	//0ACB=3,show_styled_text %1d% time %2d% style %3d%
	OpcodeResult __stdcall opcode_0ACB(CRunningScript *thread)
	{
		const char *text = readString(thread);
		DWORD	time,
			style;
		*thread >> time >> style;
		PrintBig(text, time, style);
		return OR_CONTINUE;
	}

	//0ACC=2,show_text_lowpriority %1d% time %2d%
	OpcodeResult __stdcall opcode_0ACC(CRunningScript *thread)
	{
		const char *text = readString(thread);
		DWORD time;
		*thread >> time;
		Print(text, time);
		return OR_CONTINUE;
	}

	//0ACD=2,show_text_highpriority %1d% time %2d%
	OpcodeResult __stdcall opcode_0ACD(CRunningScript *thread)
	{
		const char *text = readString(thread);
		DWORD time;
		*thread >> time;
		PrintNow(text, time);
		return OR_CONTINUE;
	}

	//0ACE=-1,show_formatted_text_box %1d%
	OpcodeResult __stdcall opcode_0ACE(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN];
		char text[MAX_STR_LEN];
		readString(thread, fmt, sizeof(fmt));
		format(thread, text, sizeof(text), fmt);
		PrintHelp(text);
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0ACF=-1,show_formatted_styled_text %1d% time %2d% style %3d%
	OpcodeResult __stdcall opcode_0ACF(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN]; char text[MAX_STR_LEN];
		DWORD time, style;
		readString(thread, fmt, sizeof(fmt));
		*thread >> time >> style;
		format(thread, text, sizeof(text), fmt);
		PrintBig(text, time, style);
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AD0=-1,show_formatted_text_lowpriority %1d% time %2d%
	OpcodeResult __stdcall opcode_0AD0(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN]; char text[MAX_STR_LEN];
		DWORD time;
		readString(thread, fmt, sizeof(fmt));
		*thread >> time;
		format(thread, text, sizeof(text), fmt);
		Print(text, time);
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AD1=-1,show_formatted_text_highpriority %1d% time %2d%
	OpcodeResult __stdcall opcode_0AD1(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN]; char text[MAX_STR_LEN];
		DWORD time;
		readString(thread, fmt, sizeof(fmt));
		*thread >> time;
		format(thread, text, sizeof(text), fmt);
		PrintNow(text, time);
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AD2=2,  %2d% = player %1d% targeted_actor //IF and SET
	OpcodeResult __stdcall opcode_0AD2(CRunningScript *thread)
	{
		DWORD playerId;
		*thread >> playerId;
		auto pPlayerPed = GetPlayerPed(playerId);
		auto pTargetEntity = GetWeaponTarget(pPlayerPed);
		if (!pTargetEntity) pTargetEntity = (CEntity*)pPlayerPed->m_pPlayerTargettedPed;
		if (pTargetEntity && pTargetEntity->m_nType == ENTITY_TYPE_PED)
		{
			*thread << GetPedPool().GetRef(reinterpret_cast<CPed*>(pTargetEntity));
			SetScriptCondResult(thread, true);
		}
		else
		{
			*thread << -1;
			SetScriptCondResult(thread, false);
		}
		return OR_CONTINUE;
	}

	//0AD3=-1,string %1d% format %2d% ...
	OpcodeResult __stdcall opcode_0AD3(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN], *dst;

		if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) *thread >> dst;
		else dst = &GetScriptParamPointer(thread)->cParam;

		readString(thread, fmt, sizeof(fmt));
		format(thread, dst, -1, fmt);
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0AD4=-1,%3d% = scan_string %1d% format %2d%  //IF and SET
	OpcodeResult __stdcall opcode_0AD4(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN], *format, *src;
		src = readString(thread);
		format = readString(thread, fmt, sizeof(fmt));

		size_t cExParams = 0;
		int *result = (int *)GetScriptParamPointer(thread);
		SCRIPT_VAR *ExParams[35];

		// read extra params
		for (int i = 0; i < 35; i++)
		{
			if (*thread->GetBytePointer())
			{
				ExParams[i] = GetScriptParamPointer(thread);
				cExParams++;
			}
			else ExParams[i] = nullptr;
		}
		thread->IncPtr();
		*result = sscanf(src, format,
						 /* extra parameters (will be aligned automatically, but the limit of 35 elements maximum exists) */
						 ExParams[0], ExParams[1], ExParams[2], ExParams[3], ExParams[4], ExParams[5],
						 ExParams[6], ExParams[7], ExParams[8], ExParams[9], ExParams[10], ExParams[11],
						 ExParams[12], ExParams[13], ExParams[14], ExParams[15], ExParams[16], ExParams[17],
						 ExParams[18], ExParams[19], ExParams[20], ExParams[21], ExParams[22], ExParams[23],
						 ExParams[24], ExParams[25], ExParams[26], ExParams[27], ExParams[28], ExParams[29],
						 ExParams[30], ExParams[31], ExParams[32], ExParams[33], ExParams[34]);

		SetScriptCondResult(thread, cExParams == *result);
		return OR_CONTINUE;
	}

	//0AD5=3,file %1d% seek %2d% from_origin %3d% //IF and SET
	OpcodeResult __stdcall opcode_0AD5(CRunningScript *thread)
	{
		DWORD hFile;
		int seek, origin;
		*thread >> hFile >> seek >> origin;
		if (convert_handle_to_file(hFile)) SetScriptCondResult(thread, fseek(convert_handle_to_file(hFile), seek, origin) == 0);
		else SetScriptCondResult(thread, false);
		return OR_CONTINUE;
	}

	//0AD6=1,end_of_file %1d% reached
	OpcodeResult __stdcall opcode_0AD6(CRunningScript *thread)
	{
		DWORD hFile;
		*thread >> hFile;
		if (FILE *file = convert_handle_to_file(hFile))
			SetScriptCondResult(thread, ferror(file) || feof(file) != 0);
		else
			SetScriptCondResult(thread, true);
		return OR_CONTINUE;
	}

	//0AD7=3,read_string_from_file %1d% to %2d% size %3d% //IF and SET
	OpcodeResult __stdcall opcode_0AD7(CRunningScript *thread)
	{
		DWORD hFile;
		char *buf;
		DWORD size;
		*thread >> hFile;
		if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) *thread >> buf;
		else buf = (char *)GetScriptParamPointer(thread);
		*thread >> size;
		if (convert_handle_to_file(hFile)) SetScriptCondResult(thread, fgets(buf, size, convert_handle_to_file(hFile)) == buf);
		else SetScriptCondResult(thread, false);
		return OR_CONTINUE;
	}

	//0AD8=2,write_string_to_file %1d% from %2d% //IF and SET
	OpcodeResult __stdcall opcode_0AD8(CRunningScript *thread)
	{
		DWORD hFile;
		*thread >> hFile;
		if (FILE * file = convert_handle_to_file(hFile))
		{
			SetScriptCondResult(thread, fputs(readString(thread), file) > 0);
			fflush(file);
		}
		else {
			SetScriptCondResult(thread, false);
		}
		return OR_CONTINUE;
	}

	//0AD9=-1,write_formated_text %2d% to_file %1d%
	OpcodeResult __stdcall opcode_0AD9(CRunningScript *thread)
	{
		char fmt[MAX_STR_LEN]; char text[MAX_STR_LEN];
		DWORD hFile;
		*thread >> hFile;
		readString(thread, fmt, sizeof(fmt));
		format(thread, text, sizeof(text), fmt);
		if (FILE * file = convert_handle_to_file(hFile))
		{
			fputs(text, file);
			fflush(file);
		}
		SkipUnusedParameters(thread);
		return OR_CONTINUE;
	}

	//0ADA=-1,%3d% = scan_file %1d% format %2d% //IF and SET
	OpcodeResult __stdcall opcode_0ADA(CRunningScript *thread)
	{
		DWORD hFile;
		*thread >> hFile;
		char *fmt = readString(thread);
		int *result = (int *)GetScriptParamPointer(thread);


		size_t cExParams = 0;
		SCRIPT_VAR *ExParams[35];
		// read extra params
		while (*thread->GetBytePointer()) ExParams[cExParams++] = GetScriptParamPointer(thread);
		thread->IncPtr();

		if (FILE *file = convert_handle_to_file(hFile))
		{
			*result = fscanf(file, fmt,
							 /* extra parameters (will be aligned automatically, but the limit of 35 elements maximum exists) */
							 ExParams[0], ExParams[1], ExParams[2], ExParams[3], ExParams[4], ExParams[5],
							 ExParams[6], ExParams[7], ExParams[8], ExParams[9], ExParams[10], ExParams[11],
							 ExParams[12], ExParams[13], ExParams[14], ExParams[15], ExParams[16], ExParams[17],
							 ExParams[18], ExParams[19], ExParams[20], ExParams[21], ExParams[22], ExParams[23],
							 ExParams[24], ExParams[25], ExParams[26], ExParams[27], ExParams[28], ExParams[29],
							 ExParams[30], ExParams[31], ExParams[32], ExParams[33], ExParams[34]);
		}
		SetScriptCondResult(thread, cExParams == *result);
		return OR_CONTINUE;
	}

	//0ADB=2,%2d% = car_model %1o% name
	OpcodeResult __stdcall opcode_0ADB(CRunningScript *thread)
	{
		DWORD mi;
		char *buf;
		*thread >> mi;

		CVehicleModelInfo* model;
		// if 1.0 US, prefer GetModelInfo function — makes it compatible with fastman92's limit adjuster
		if (CLEO::GetInstance().VersionManager.GetGameVersion() == CLEO::GV_US10) {
			model = plugin::CallAndReturn<CVehicleModelInfo *, 0x403DA0, int>(mi);
		}
		else {
			model = reinterpret_cast<CVehicleModelInfo*>(Models[mi]);
		}
		if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) *thread >> buf;
		else buf = (char *)GetScriptParamPointer(thread);
		memcpy(buf, model->m_szGameName, 8);
		return OR_CONTINUE;
	}

	//0ADC=1, test_cheat %1d%
	OpcodeResult __stdcall opcode_0ADC(CRunningScript *thread)
	{
		SetScriptCondResult(thread, TestCheat(readString(thread)));
		return OR_CONTINUE;
	}

	//0ADD=1,spawn_car_with_model %1o% at_player_location 
	OpcodeResult __stdcall opcode_0ADD(CRunningScript *thread)
	{
		DWORD mi;
		*thread >> mi;

		CVehicleModelInfo* model;
		// if 1.0 US, prefer GetModelInfo function — makes it compatible with fastman92's limit adjuster
		if (CLEO::GetInstance().VersionManager.GetGameVersion() == CLEO::GV_US10) {
			model = plugin::CallAndReturn<CVehicleModelInfo *, 0x403DA0, int>(mi);
		}
		else {
			model = reinterpret_cast<CVehicleModelInfo*>(Models[mi]);
		}
		if (model->m_nVehicleType != VEHICLE_TYPE_TRAIN && model->m_nVehicleType != VEHICLE_TYPE_UNKNOWN) SpawnCar(mi);
		return OR_CONTINUE;
	}

	//0ADE=2,%2d% = text_by_GXT_entry %1d%
	OpcodeResult __stdcall opcode_0ADE(CRunningScript *thread)
	{
		const char *gxt = readString(thread);
		if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8)
			*thread << GetInstance().TextManager.Get(gxt);
		else
			strcpy((char *)GetScriptParamPointer(thread), GetInstance().TextManager.Get(gxt));
		return OR_CONTINUE;
	}

	//0ADF=2,add_dynamic_GXT_entry %1d% text %2d%
	OpcodeResult __stdcall opcode_0ADF(CRunningScript *thread)
	{
		char gxtLabel[8]; // 7 + terminator character
		readString(thread, gxtLabel, sizeof(gxtLabel));

		char *text = readString(thread);

		GetInstance().TextManager.AddFxt(gxtLabel, text);
		return OR_CONTINUE;
	}

	//0AE0=1,remove_dynamic_GXT_entry %1d%
	OpcodeResult __stdcall opcode_0AE0(CRunningScript *thread)
	{
		GetInstance().TextManager.RemoveFxt(readString(thread));
		return OR_CONTINUE;
	}

	//0AE1=7,%7d% = find_actor_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% pass_deads %6h% //IF and SET
	OpcodeResult __stdcall opcode_0AE1(CRunningScript *thread)
	{
		CVector center;
		float radius;
		DWORD next, pass_deads;
		static DWORD stat_last_found = 0;
		auto& pool = GetPedPool();
		*thread >> center >> radius >> next >> pass_deads;

		DWORD& last_found = reinterpret_cast<CCustomScript *>(thread)->IsCustom() ?
			reinterpret_cast<CCustomScript *>(thread)->GetLastSearchPed() :
			stat_last_found;

		if (!next) last_found = 0;

		for (int index = last_found; index < pool.m_nSize; ++index)
		{
			if (auto obj = pool.GetAt(index))
			{
				if (pass_deads != -1 && (obj->IsPlayer() || (pass_deads && !IsAvailable(obj))/* || obj->GetOwner() == 2*/ || obj->m_nPedFlags.bFadeOut))
					continue;

				if (radius >= 1000.0f || (VectorSqrMagnitude(obj->GetPosition() - center) <= radius * radius))
				{
					last_found = index + 1;	// on next opcode call start search from next index
											//if(last_found >= (unsigned)pool.GetSize()) last_found = 0;
											//obj->PedCreatedBy = 2; // add reference to found actor

					*thread << pool.GetRef(obj);
					SetScriptCondResult(thread, true);
					return OR_CONTINUE;
				}
			}
		}

		*thread << -1;
		last_found = 0;
		SetScriptCondResult(thread, false);
		return OR_CONTINUE;
	}

	//0AE2=7,%7d% = find_vehicle_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% pass_wrecked %6h% //IF and SET
	OpcodeResult __stdcall opcode_0AE2(CRunningScript *thread)
	{
		CVector center;
		float radius;
		DWORD next, pass_wrecked;
		static DWORD stat_last_found = 0;

		auto& pool = GetVehiclePool();
		*thread >> center >> radius >> next >> pass_wrecked;

		DWORD& last_found = reinterpret_cast<CCustomScript*>(thread)->IsCustom() ?
			reinterpret_cast<CCustomScript *>(thread)->GetLastSearchVehicle() :
			stat_last_found;

		if (!next) last_found = 0;

		for (int index = last_found; index < pool.m_nSize; ++index)
		{
			if (auto obj = pool.GetAt(index))
			{
				if ((pass_wrecked && IsWrecked(obj)) || (/*obj->GetOwner() == 2 ||*/ obj->m_nVehicleFlags.bFadeOut))
					continue;

				if (radius >= 1000.0f || (VectorSqrMagnitude(obj->GetPosition() - center) <= radius * radius))
				{
					last_found = index + 1;	// on next opcode call start search from next index
											//if(last_found >= (unsigned)pool.GetSize()) last_found = 0;
											// obj.referenceType = 2; // add reference to found actor
					*thread << pool.GetRef(obj);
					SetScriptCondResult(thread, true);
					return OR_CONTINUE;
				}
			}
		}

		*thread << -1;
		last_found = 0;
		SetScriptCondResult(thread, false);
		return OR_CONTINUE;
	}

	//0AE3=6,%6d% = find_object_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% //IF and SET
	OpcodeResult __stdcall opcode_0AE3(CRunningScript *thread)
	{
		CVector center;
		float radius;
		DWORD next;
		static DWORD stat_last_found = 0;
		auto& pool = GetObjectPool();
		*thread >> center >> radius >> next;

		auto cs = reinterpret_cast<CCustomScript *>(thread);
		DWORD& last_found = cs->IsCustom() ? cs->GetLastSearchObject() : stat_last_found;

		if (!next) last_found = 0;

		for (int index = last_found; index < pool.m_nSize; ++index)
		{
			if (auto obj = pool.GetAt(index))
			{
				if (obj->m_nObjectFlags.bFadingIn) continue; // this is actually .bFadingOut (yet?)

				if (radius >= 1000.0f || (VectorSqrMagnitude(obj->GetPosition() - center) <= radius * radius))
				{
					last_found = index + 1;	// on next opcode call start search from next index
											//if(last_found >= (unsigned)pool.GetSize()) last_found = 0;
											// obj.referenceType = 2; // add reference to found actor
					*thread << pool.GetRef(obj);
					SetScriptCondResult(thread, true);
					return OR_CONTINUE;
				}
			}
		}

		last_found = 0;
		*thread << -1;
		SetScriptCondResult(thread, false);
		return OR_CONTINUE;
	}

	//0AE4=1,  directory_exist %1d%
	OpcodeResult __stdcall opcode_0AE4(CRunningScript *thread)
	{
		auto fAttr = GetFileAttributes(readString(thread));
		SetScriptCondResult(thread, (fAttr != INVALID_FILE_ATTRIBUTES) && (fAttr & FILE_ATTRIBUTE_DIRECTORY));
		return OR_CONTINUE;
	}

	//0AE5=1,create_directory %1d% //IF and SET
	OpcodeResult __stdcall opcode_0AE5(CRunningScript *thread)
	{
		bool condResult = CreateDirectory(readString(thread), NULL) != 0;
		SetScriptCondResult(thread, condResult);
		return OR_CONTINUE;
	}

	//0AE6=3,%2d% = find_first_file %1d% get_filename_to %3d% //IF and SET
	OpcodeResult __stdcall opcode_0AE6(CRunningScript *thread)
	{
		WIN32_FIND_DATA ffd;
		memset(&ffd, 0, sizeof(ffd));

		HANDLE handle = FindFirstFile(readString(thread), &ffd);
		*thread << handle;
		GetInstance().OpcodeSystem.m_hFileSearches.insert(handle);
		if (handle != INVALID_HANDLE_VALUE)
		{
			auto type = *thread->GetBytePointer();
			char* str;
			switch (type)
			{
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_STRING_ARRAY:
			case DT_LVAR_STRING_ARRAY:
				str = (char*)GetScriptParamPointer(thread);
				memcpy(str, ffd.cFileName, 16);
				str[15] = '\0';
				break;
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
			case DT_VAR_TEXTLABEL_ARRAY:
			case DT_LVAR_TEXTLABEL_ARRAY:
				str = (char*)GetScriptParamPointer(thread);
				memcpy(str, ffd.cFileName, 8);
				str[7] = '\0';
				break;
			default:
				*thread >> str;
				strcpy(str, ffd.cFileName);
			}
			SetScriptCondResult(thread, true);
		}
		else
		{
			readString(thread);
			SetScriptCondResult(thread, false);
		}
		return OR_CONTINUE;
	}

	//0AE7=2,%2d% = find_next_file %1d% //IF and SET
	OpcodeResult __stdcall opcode_0AE7(CRunningScript *thread)
	{
		WIN32_FIND_DATA ffd;
		memset(&ffd, 0, sizeof(ffd));

		HANDLE handle;
		*thread >> handle;
		if (FindNextFile(handle, &ffd))
		{
			auto type = *thread->GetBytePointer();
			char* str;
			switch (type)
			{
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_STRING_ARRAY:
			case DT_LVAR_STRING_ARRAY:
				str = (char*)GetScriptParamPointer(thread);
				memcpy(str, ffd.cFileName, 16);
				str[15] = '\0';
				break;
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
			case DT_VAR_TEXTLABEL_ARRAY:
			case DT_LVAR_TEXTLABEL_ARRAY:
				str = (char*)GetScriptParamPointer(thread);
				memcpy(str, ffd.cFileName, 8);
				str[7] = '\0';
				break;
			default:
				*thread >> str;
				strcpy(str, ffd.cFileName);
			}
			SetScriptCondResult(thread, true);
		}
		else
		{
			readString(thread);
			SetScriptCondResult(thread, false);
		}
		return OR_CONTINUE;
	}

	//0AE8=1,find_close %1d%
	OpcodeResult __stdcall opcode_0AE8(CRunningScript *thread)
	{
		HANDLE handle;
		*thread >> handle;
		FindClose(handle);
		GetInstance().OpcodeSystem.m_hFileSearches.erase(handle);
		return OR_CONTINUE;
	}

	//0AE9=0,pop_float
	OpcodeResult __stdcall opcode_0AE9(CRunningScript *thread)
	{
		float result;
		_asm fstp result
		opcodeParams[0].fParam = result;
		SetScriptParams(thread, 1);
		return OR_CONTINUE;
	}

	//0AEA=2,%2d% = actor_struct %1d% handle
	OpcodeResult __stdcall opcode_0AEA(CRunningScript *thread)
	{
		CPed *struc;
		*thread >> struc;
		*thread << GetPedPool().GetRef(struc);
		return OR_CONTINUE;
	}

	//0AEB=2,%2d% = car_struct %1d% handle
	OpcodeResult __stdcall opcode_0AEB(CRunningScript *thread)
	{
		CVehicle *struc;
		*thread >> struc;
		*thread << GetVehiclePool().GetRef(struc);
		return OR_CONTINUE;
	}

	//0AEC=2,%2d% = object_struct %1d% handle
	OpcodeResult __stdcall opcode_0AEC(CRunningScript *thread)
	{
		CObject *struc;
		*thread >> struc;
		*thread << GetObjectPool().GetRef(struc);
		return OR_CONTINUE;
	}

	//0AED=3,%3d% = float %1d% to_string_format %2d%
	OpcodeResult __stdcall opcode_0AED(CRunningScript *thread)
	{
		// this opcode is useless now
		float val;
		char *format, *result;
		*thread >> val;
		format = readString(thread);
		if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8)
			*thread >> result;
		else
			result = &GetScriptParamPointer(thread)->cParam;
		sprintf(result, format, val);
		return OR_CONTINUE;
	}

	//0AEE=3,%3d% = %1d% exp %2d% //all floats
	OpcodeResult __stdcall opcode_0AEE(CRunningScript *thread)
	{
		float base, arg;
		*thread >> base >> arg;
		*thread << (float)pow(base, arg);
		return OR_CONTINUE;
	}

	//0AEF=3,%3d% = log %1d% base %2d% //all floats
	OpcodeResult __stdcall opcode_0AEF(CRunningScript *thread)
	{
		float base, arg;
		*thread >> arg >> base;
		*thread << (float)(log(arg) / log(base));
		return OR_CONTINUE;
	}
}



/********************************************************************/

// API
extern "C"
{
	using namespace CLEO;

	// Define external symbols with MSVC decorating schemes
	BOOL WINAPI CLEO_RegisterOpcode(WORD opcode, CustomOpcodeHandler callback);
	DWORD WINAPI CLEO_GetIntOpcodeParam(CRunningScript* thread);
	float WINAPI CLEO_GetFloatOpcodeParam(CRunningScript* thread);
	void WINAPI CLEO_SetIntOpcodeParam(CRunningScript* thread, DWORD value);
	void WINAPI CLEO_SetFloatOpcodeParam(CRunningScript* thread, float value);
	LPSTR WINAPI CLEO_ReadStringOpcodeParam(CRunningScript* thread, char *buf, int size);
	LPSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CRunningScript* thread, char *buf, int size);
	void WINAPI CLEO_WriteStringOpcodeParam(CRunningScript* thread, LPCSTR str);
	void WINAPI CLEO_SetThreadCondResult(CRunningScript* thread, BOOL result);
	void WINAPI CLEO_SkipOpcodeParams(CRunningScript* thread, int count);
	void WINAPI CLEO_ThreadJumpAtLabelPtr(CRunningScript* thread, int labelPtr);
	int WINAPI CLEO_GetOperandType(CRunningScript* thread);
	void WINAPI CLEO_RetrieveOpcodeParams(CRunningScript *thread, int count);
	void WINAPI CLEO_RecordOpcodeParams(CRunningScript *thread, int count);
	SCRIPT_VAR * WINAPI CLEO_GetPointerToScriptVariable(CRunningScript* thread);
	RwTexture * WINAPI CLEO_GetScriptTextureById(CRunningScript* thread, int id);
	HSTREAM WINAPI CLEO_GetInternalAudioStream(CRunningScript* thread, CAudioStream *stream);
	CRunningScript* WINAPI CLEO_CreateCustomScript(CRunningScript* fromThread, const char *fileName, int label);

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4550)
#endif

	BOOL WINAPI CLEO_RegisterOpcode(WORD opcode, CustomOpcodeHandler callback)
	{
		if ((opcode > 0x7FFF) || (opcode < 0x0AF0))
			return FALSE;

		CustomOpcodeHandler& dst = extraOpcodeHandlers[opcode % 100][opcode / 100 - 28];

		if (*dst)
		{
			Error("Warning! CLEO couldn't register opcode handler.");
			return FALSE;
		}
		dst = callback;
		return TRUE;
	}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

	DWORD WINAPI CLEO_GetIntOpcodeParam(CRunningScript* thread)
	{
		DWORD result;
		*thread >> result;
		return result;
	}

	float WINAPI CLEO_GetFloatOpcodeParam(CRunningScript* thread)
	{
		float result;
		*thread >> result;
		return result;
	}

	void WINAPI CLEO_SetIntOpcodeParam(CRunningScript* thread, DWORD value)
	{
		*thread << value;
	}

	void WINAPI CLEO_SetFloatOpcodeParam(CRunningScript* thread, float value)
	{
		*thread << value;
	}

	LPSTR WINAPI CLEO_ReadStringOpcodeParam(CRunningScript* thread, char *buf, int size)
	{
		static char internal_buf[MAX_STR_LEN];
		if (!buf) { buf = internal_buf; size = MAX_STR_LEN; }
		if (!size) size = MAX_STR_LEN;
		std::fill(buf, buf + size, '\0');
		GetScriptStringParam(thread, buf, size);
		return buf;
	}

	LPSTR WINAPI CLEO_ReadStringPointerOpcodeParam(CRunningScript* thread, char *buf, int size)
	{
		static char internal_buf[MAX_STR_LEN];
		if (!buf) { buf = internal_buf; size = MAX_STR_LEN; }
		if (!size) size = MAX_STR_LEN;
		std::fill(buf, buf + size, '\0');
		return readString(thread, buf, size);
	}

	void WINAPI CLEO_WriteStringOpcodeParam(CRunningScript* thread, LPCSTR str)
	{
		auto dst = (char *)GetScriptParamPointer(thread);
		memcpy(dst, str, 16);
		dst[15] = '\0';
	}

	void WINAPI CLEO_SetThreadCondResult(CRunningScript* thread, BOOL result)
	{
		SetScriptCondResult(thread, result != FALSE);
	}

	void WINAPI CLEO_SkipOpcodeParams(CRunningScript* thread, int count)
	{
		int len;
		for (int i = 0; i < count; i++)
		{
			switch (thread->ReadDataType())
			{
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				thread->IncPtr(2);
				break;
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				thread->IncPtr(6);
				break;
			case DT_BYTE:
				thread->IncPtr();
				break;
			case DT_WORD:
				thread->IncPtr(2);
				break;
			case DT_DWORD:
			case DT_FLOAT:
				thread->IncPtr(4);
				break;
			case DT_VARLEN_STRING:
				len = thread->ReadDataByte();
				thread->IncPtr(len);
				break;

			case DT_TEXTLABEL:
				thread->IncPtr(8);
				break;
			case DT_STRING:
				thread->IncPtr(16);
				break;
			}
		}
	}

	void WINAPI CLEO_ThreadJumpAtLabelPtr(CRunningScript* thread, int labelPtr)
	{
		ThreadJump(thread, labelPtr);
	}

	int WINAPI CLEO_GetOperandType(CRunningScript* thread)
	{
		return *thread->GetBytePointer();
	}

	void WINAPI CLEO_RetrieveOpcodeParams(CRunningScript *thread, int count)
	{
		GetScriptParams(thread, count);
	}

	void WINAPI CLEO_RecordOpcodeParams(CRunningScript *thread, int count)
	{
		SetScriptParams(thread, count);
	}

	SCRIPT_VAR * WINAPI CLEO_GetPointerToScriptVariable(CRunningScript* thread)
	{
		return GetScriptParamPointer(thread);
	}

	RwTexture * WINAPI CLEO_GetScriptTextureById(CRunningScript* thread, int id)
	{
		CCustomScript* customScript = reinterpret_cast<CCustomScript*>(thread);
		// We need to store-restore to update the texture list, not optimized, but this will not be used every frame anyway
		customScript->StoreScriptTextures();
		RwTexture *texture = customScript->GetScriptTextureById(id - 1);
		customScript->RestoreScriptTextures();
		return texture;
	}

	HSTREAM WINAPI CLEO_GetInternalAudioStream(CRunningScript* thread, CAudioStream *stream)
	{
		return stream->GetInternal();
	}

	CRunningScript* WINAPI CLEO_CreateCustomScript(CRunningScript* fromThread, const char *script_name, int label)
	{
		if (label != 0) // create from label
		{
			TRACE("Starting new custom script from thread named %s label %i", script_name, label);
		}
		else
		{
			TRACE("Starting new custom script %s", script_name);
		}
		char cwd[MAX_PATH];
		_getcwd(cwd, sizeof(cwd));
		_chdir(cleo_dir);
		// if "label == 0" then "script_name" need to be the file name
		auto cs = new CCustomScript(script_name, false, reinterpret_cast<CCustomScript*>(fromThread), label);
		if (fromThread) SetScriptCondResult(fromThread, cs && cs->IsOK());
		if (cs && cs->IsOK())
		{
			GetInstance().ScriptEngine.AddCustomScript(cs);
			if (fromThread) TransmitScriptParams(fromThread, cs);
		}
		else
		{
			if (cs) delete cs;
			if (fromThread) SkipUnusedParameters(fromThread);
			TRACE("Failed to load script '%s'.", script_name);
		}
		_chdir(cwd);
		return cs;
	}

	CRunningScript* WINAPI CLEO_GetLastCreatedCustomScript()
	{
		return lastScriptCreated;
	}

	void WINAPI CLEO_AddScriptDeleteDelegate(FuncScriptDeleteDelegateT func)
	{
		scriptDeleteDelegate += func;
	}

	void WINAPI CLEO_RemoveScriptDeleteDelegate(FuncScriptDeleteDelegateT func)
	{
		scriptDeleteDelegate -= func;
	}

}