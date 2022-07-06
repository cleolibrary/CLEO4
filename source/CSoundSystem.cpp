#include "stdafx.h"
#include "CSoundSystem.h"
#include "bass.h"
#include "CDebug.h"
#include "cleo.h"
#include <windows.h>

namespace CLEO
{
    HWND(__cdecl * CreateMainWindow)(HINSTANCE hinst);
    LRESULT(__stdcall * imp_DefWindowProc)(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

    HWND OnCreateMainWindow(HINSTANCE hinst)
    {
        if (HIWORD(BASS_GetVersion()) != BASSVERSION) Error("An incorrect version of bass.dll has been loaded");
        TRACE("Creating main window...");
        HWND wnd = CreateMainWindow(hinst);
        if (!GetInstance().SoundSystem.Init(wnd)) TRACE("CSoundSystem::Init() failed. Error code: %d", BASS_ErrorGetCode());
        return wnd;
    }

    CPlaceable *camera;
    RwCamera	**	pRwCamera;
    bool		*	userPaused;
    bool		*	codePaused;

    LRESULT __stdcall HOOK_DefWindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (GetInstance().SoundSystem.Initialized())
        {
            // pause streams if the window loses focus, or if SA found any other reason to pause
            if (*codePaused) GetInstance().SoundSystem.PauseStreams();
            else
            {
                switch (msg)
                {
                case WM_ACTIVATE:
					if (wparam == 0)
					{
						GetInstance().SoundSystem.PauseStreams();
					}
					else if (wparam == 1)
					{
						GetInstance().SoundSystem.ResumeStreams();
					}
                    break;
                case WM_KILLFOCUS:
                    GetInstance().SoundSystem.PauseStreams();
                    break;
                }
            }
        }
        return imp_DefWindowProc(wnd, msg, wparam, lparam);
    }

    void CSoundSystem::Inject(CCodeInjector& inj)
    {
        TRACE("Injecting SoundSystem...");
        CGameVersionManager& gvm = GetInstance().VersionManager;
        CreateMainWindow = gvm.TranslateMemoryAddress(MA_CREATE_MAIN_WINDOW_FUNCTION);
        inj.ReplaceFunction(OnCreateMainWindow, gvm.TranslateMemoryAddress(MA_CALL_CREATE_MAIN_WINDOW));
        camera = gvm.TranslateMemoryAddress(MA_CAMERA);
        userPaused = gvm.TranslateMemoryAddress(MA_USER_PAUSE);
        codePaused = gvm.TranslateMemoryAddress(MA_CODE_PAUSE);
        pRwCamera = gvm.TranslateMemoryAddress(MA_RW_CAMERA_PP);
        auto addr = gvm.TranslateMemoryAddress(MA_DEF_WINDOW_PROC_PTR);
        static const auto pWindowProcHook = &HOOK_DefWindowProc;
        DWORD ptr;
        inj.MemoryRead(addr, ptr);
        inj.MemoryRead(ptr, imp_DefWindowProc);
        inj.MemoryWrite(addr, (DWORD)&pWindowProcHook);
    }

    void EnumerateBassDevices(int& total, int& enabled, int& default_device)
    {
        BASS_DEVICEINFO info;
        for (default_device = -1, enabled = 0, total = 0; BASS_GetDeviceInfo(total, &info); ++total)
        {
            if (info.flags & BASS_DEVICE_ENABLED) ++enabled;
            if (info.flags & BASS_DEVICE_DEFAULT) default_device = total;
            TRACE("Found sound device %d%s: %s", total, default_device == total ?
                " (default)" : "", info.name);
        }
    }

    BASS_3DVECTOR pos(0, 0, 0), vel(0, 0, 0), front(0, -1.0, 0), top(0, 0, 1.0);

    bool CSoundSystem::Init(HWND hwnd)
    {
        int default_device, total_devices, enabled_devices;
        BASS_DEVICEINFO info = { nullptr, nullptr, 0 };
        EnumerateBassDevices(total_devices, enabled_devices, default_device);
        if (forceDevice != -1 && BASS_GetDeviceInfo(forceDevice, &info) &&
            info.flags & BASS_DEVICE_ENABLED)
            default_device = forceDevice;

        TRACE("On system found %d devices, %d enabled devices, assuming device to use: %d (%s)",
            total_devices, enabled_devices, default_device, BASS_GetDeviceInfo(default_device, &info) ?
            info.name : "Unknown device");

        if (BASS_Init(default_device, 44100, BASS_DEVICE_3D, hwnd, nullptr) &&
            BASS_Set3DFactors(1.0f, 0.3f, 1.0f) &&
            BASS_Set3DPosition(&pos, &vel, &front, &top))
        {
            TRACE("SoundSystem initialized");

            // Can we use floating-point (HQ) audio streams?
            DWORD floatable; // floating-point channel support? 0 = no, else yes
            if (floatable = BASS_StreamCreate(44100, 1, BASS_SAMPLE_FLOAT, NULL, NULL))
            {
                TRACE("Floating-point audio supported!");
                BASS_StreamFree(floatable);
            }
            else TRACE("Floating-point audio not supported!");

            // 
            if (BASS_GetInfo(&SoundDevice))
            {
                if (SoundDevice.flags & DSCAPS_EMULDRIVER)
                    TRACE("Audio drivers not installed - using DirectSound emulation");
                if (!SoundDevice.eax)
                    TRACE("Audio hardware acceleration disabled (no EAX)");
            }

            initialized = true;
            this->hwnd = hwnd;
            BASS_Apply3D();
            return true;
        }
        Warning("Could not initialize BASS sound system");
        return false;
    }

    CAudioStream *CSoundSystem::LoadStream(const char *filename, bool in3d)
    {
        CAudioStream *result = in3d ? new C3DAudioStream(filename) : new CAudioStream(filename);
        if (result->OK)
        {
            streams.insert(result);
            return result;
        }
        delete result;
        return nullptr;
    }

    void CSoundSystem::UnloadStream(CAudioStream *stream)
    {
        if (streams.erase(stream))
            delete stream;
        else
            TRACE("Unloading of stream that is not in list of loaded streams");
    }

    void CSoundSystem::UnloadAllStreams()
    {
        std::for_each(streams.begin(), streams.end(), [](CAudioStream *stream)
        {
            delete stream;
        });
        streams.clear();
    }

    void CSoundSystem::ResumeStreams()
    {
        paused = false;
        std::for_each(streams.begin(), streams.end(), [](CAudioStream *stream) {
            if (stream->state == CAudioStream::playing) stream->Resume();
        });
    }

    void CSoundSystem::PauseStreams()
    {
        paused = true;
        std::for_each(streams.begin(), streams.end(), [](CAudioStream *stream) {
            if (stream->state == CAudioStream::playing) stream->Pause(false);
        });
    }

    void CSoundSystem::Update()
    {
        //// steam has a relocated var, so get it manually for now
        //CGameVersionManager& gvm = GetInstance().VersionManager;
        //bool bMenuActive = gvm.GetGameVersion() != GV_STEAM ? MenuManager->IsActive() : *((bool*)0xC3315C);

        if (*userPaused || *codePaused)		// covers menu pausing, no disc in drive pausing, etc.
        {
            if (!paused) PauseStreams();
        }
        else
        {
            if (paused) ResumeStreams();

            // not in menu
            // process camera movements

            CMatrixLink * pMatrix = nullptr;
            CVector * pVec = nullptr;
            if (camera->m_matrix)
            {
                pMatrix = camera->m_matrix;
                pVec = &pMatrix->pos;
            }
            else pVec = &camera->m_placement.m_vPosn;

            BASS_Set3DPosition(
                &BASS_3DVECTOR(pVec->y, pVec->z, pVec->x),
                nullptr,
                pMatrix ? &BASS_3DVECTOR(pMatrix->at.y, pMatrix->at.z, pMatrix->at.x) : nullptr,
                pMatrix ? &BASS_3DVECTOR(pMatrix->up.y, pMatrix->up.z, pMatrix->up.x) : nullptr
            );

            // process all streams
            std::for_each(streams.begin(), streams.end(), [](CAudioStream *stream) {
                stream->Process();
            });
            // apply above changes
            BASS_Apply3D();
        }
    }

    CAudioStream::CAudioStream()
        : streamInternal(0), state(no), OK(false)
    {
    }

    CAudioStream::CAudioStream(const char *src) : state(no), OK(false)
    {
        unsigned flags = BASS_SAMPLE_SOFTWARE;
        if (GetInstance().SoundSystem.bUseFPAudio)
            flags |= BASS_SAMPLE_FLOAT;
        if (!(streamInternal = BASS_StreamCreateFile(FALSE, src, 0, 0, flags)) &&
            !(streamInternal = BASS_StreamCreateURL(src, 0, flags, 0, nullptr)))
        {
            TRACE("Loading audiostream %s failed. Error code: %d", src, BASS_ErrorGetCode());
        }
        else OK = true;
    }

    CAudioStream::~CAudioStream()
    {
        if (streamInternal) BASS_StreamFree(streamInternal);
    }

    C3DAudioStream::C3DAudioStream(const char *src) : CAudioStream(), link(nullptr)
    {
        unsigned flags = BASS_SAMPLE_3D | BASS_SAMPLE_MONO | BASS_SAMPLE_SOFTWARE;
        if (GetInstance().SoundSystem.bUseFPAudio)
            flags |= BASS_SAMPLE_FLOAT;
        if (!(streamInternal = BASS_StreamCreateFile(FALSE, src, 0, 0, flags)) &&
            !(streamInternal = BASS_StreamCreateURL(src, 0, flags, nullptr, nullptr)))
        {
            TRACE("Loading 3d-audiostream %s failed. Error code: %d", src, BASS_ErrorGetCode());
        }
        else
        {
            BASS_ChannelSet3DAttributes(streamInternal, 0, -1.0, -1.0, -1, -1, -1.0);
            OK = true;
        }
    }

    C3DAudioStream::~C3DAudioStream()
    {
        if (streamInternal) BASS_StreamFree(streamInternal);
    }

    void CAudioStream::Play()
    {
        BASS_ChannelPlay(streamInternal, TRUE);
        state = playing;
    }

    void CAudioStream::Pause(bool change_state)
    {
        BASS_ChannelPause(streamInternal);
        if (change_state) state = paused;
    }

    void CAudioStream::Stop()
    {
        BASS_ChannelPause(streamInternal);
        BASS_ChannelSetPosition(streamInternal, 0, BASS_POS_BYTE);
        state = paused;
    }

    void CAudioStream::Resume()
    {
        BASS_ChannelPlay(streamInternal, FALSE);
        state = playing;
    }

    DWORD CAudioStream::GetLength()
    {
        return (unsigned)BASS_ChannelBytes2Seconds(streamInternal,
            BASS_ChannelGetLength(streamInternal, BASS_POS_BYTE));
    }

    DWORD CAudioStream::GetState()
    {
        if (state == stopped) return -1;		// dont do this in case we changed state by pausing
        switch (BASS_ChannelIsActive(streamInternal))
        {
        case BASS_ACTIVE_STOPPED:
        default:
            return -1;
        case BASS_ACTIVE_PLAYING:
        case BASS_ACTIVE_STALLED:
            return 1;
        case BASS_ACTIVE_PAUSED:
            return 2;
        };
    }

    float CAudioStream::GetVolume()
    {
        float result;
        if (!BASS_ChannelGetAttribute(streamInternal, BASS_ATTRIB_VOL, &result))
            return -1.0f;
        return result;
    }

    void CAudioStream::SetVolume(float val)
    {
        BASS_ChannelSetAttribute(streamInternal, BASS_ATTRIB_VOL, val);
    }

    void CAudioStream::Loop(bool enable)
    {
        BASS_ChannelFlags(streamInternal, enable ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
    }

	HSTREAM CAudioStream::GetInternal()
	{
		return streamInternal;
	}

    void CAudioStream::Process()
    {
        // no actions required			// liez!

        switch (BASS_ChannelIsActive(streamInternal))
        {
        case BASS_ACTIVE_PAUSED:
            state = paused;
            break;
        case BASS_ACTIVE_PLAYING:
        case BASS_ACTIVE_STALLED:
            state = playing;
            break;
        case BASS_ACTIVE_STOPPED:
            state = stopped;
            break;
        }
    }

    void CAudioStream::Set3dPosition(const CVector& pos)
    {
        TRACE("Unimplemented CAudioStream::Set3dPosition()");
    }

    void CAudioStream::Link(CPlaceable *placable)
    {
        TRACE("Unimplemented CAudioStream::Link()");
    }

    void C3DAudioStream::Set3dPosition(const CVector& pos)
    {
        position.x = pos.y;
        position.y = pos.z;
        position.z = pos.x;
        link = nullptr;
        BASS_ChannelSet3DPosition(streamInternal, &position, nullptr, nullptr);
    }

    void C3DAudioStream::Link(CPlaceable *placable)
    {
        link = placable;
        //Set3dPosition(placable->GetPos());
    }

    void C3DAudioStream::Process()
    {
        // update playing position of the linked object
        switch (BASS_ChannelIsActive(streamInternal))
        {
        case BASS_ACTIVE_PAUSED:
            state = paused;
            break;
        case BASS_ACTIVE_PLAYING:
        case BASS_ACTIVE_STALLED:
            state = playing;
            break;
        case BASS_ACTIVE_STOPPED:
            state = stopped;
            break;
        }
        if (state == playing)
        {
            if (link)
            {
                CVector * pVec = link->m_matrix ? &link->m_matrix->pos : &link->m_placement.m_vPosn;
                BASS_ChannelSet3DPosition(streamInternal, &BASS_3DVECTOR(pVec->y, pVec->z, pVec->x), nullptr, nullptr);
            }
            else
            {
                BASS_ChannelSet3DPosition(streamInternal, &position, nullptr, nullptr);
                //BASS_ChannelGet3DPosition(streamInternal, &position, nullptr, nullptr);
            }
        }
    }
}
