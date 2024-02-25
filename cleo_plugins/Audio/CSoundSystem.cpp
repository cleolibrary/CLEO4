#include "CSoundSystem.h"
#include "CAudioStream.h"
#include "C3dAudioStream.h"
#include "CLEO_Utils.h"
#include "CAEAudioHardware.h"
#include "CCamera.h"

namespace CLEO
{
    bool CSoundSystem::useFloatAudio = false;
    BASS_3DVECTOR CSoundSystem::pos(0.0, 0.0, 0.0);
    BASS_3DVECTOR CSoundSystem::vel(0.0, 0.0, 0.0);
    BASS_3DVECTOR CSoundSystem::front(0.0, -1.0, 0.0);
    BASS_3DVECTOR CSoundSystem::top(0.0, 0.0, 1.0);
    float CSoundSystem::masterSpeed = 1.0f;
    float CSoundSystem::masterVolume = 1.0f;

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

    CSoundSystem::~CSoundSystem()
    {
        TRACE("Finalizing SoundSystem...");
        Clear();

        if (initialized)
        {
            TRACE("Freeing BASS library");
            BASS_Free();
            initialized = false;
        }
        TRACE("SoundSystem finalized");
    }

    bool CSoundSystem::Init()
    {
        if (initialized) return true; // already done

        int default_device, total_devices, enabled_devices;
        EnumerateBassDevices(total_devices, enabled_devices, default_device);

        BASS_DEVICEINFO info = { nullptr, nullptr, 0 };
        if (forceDevice != -1 && BASS_GetDeviceInfo(forceDevice, &info) &&
            info.flags & BASS_DEVICE_ENABLED)
            default_device = forceDevice;

        TRACE("On system found %d devices, %d enabled devices, assuming device to use: %d (%s)",
            total_devices, enabled_devices, default_device, BASS_GetDeviceInfo(default_device, &info) ?
            info.name : "Unknown device");

        if (BASS_Init(default_device, 44100, BASS_DEVICE_3D, RsGlobal.ps->window, nullptr) &&
            BASS_Set3DFactors(1.0f, 3.0f, 80.0f) &&
            BASS_Set3DPosition(&pos, &vel, &front, &top))
        {
            TRACE("SoundSystem initialized");

            // Can we use floating-point (HQ) audio streams?
            DWORD floatable = BASS_StreamCreate(44100, 1, BASS_SAMPLE_FLOAT, NULL, NULL); // floating-point channel support? 0 = no, else yes
            if (floatable)
            {
                TRACE("Floating-point audio supported!");
                useFloatAudio = true;
                BASS_StreamFree(floatable);
            }
            else TRACE("Floating-point audio not supported!");

            if (BASS_GetInfo(&SoundDevice))
            {
                if (SoundDevice.flags & DSCAPS_EMULDRIVER)
                    TRACE("Audio drivers not installed - using DirectSound emulation");
                if (!SoundDevice.eax)
                    TRACE("Audio hardware acceleration disabled (no EAX)");
            }

            initialized = true;
            BASS_Apply3D();
            return true;
        }

        LOG_WARNING(0, "Could not initialize BASS sound system. Error code: %d", BASS_ErrorGetCode());
        return false;
    }

    bool CSoundSystem::Initialized()
    {
        return initialized;
    }

    CAudioStream* CSoundSystem::CreateStream(const char *filename, bool in3d)
    {
        CAudioStream* result = in3d ? new C3DAudioStream(filename) : new CAudioStream(filename);
        if (!result->IsOk())
        {
            delete result;
            return nullptr;
        }

        streams.insert(result);
        return result;
    }

    void CSoundSystem::DestroyStream(CAudioStream *stream)
    {
        if (streams.erase(stream))
            delete stream;
        else
            TRACE("Unloading of stream that is not in list of loaded streams");
    }

    bool CSoundSystem::HasStream(CAudioStream* stream)
    {
        return streams.find(stream) != streams.end();
    }

    void CSoundSystem::Clear()
    {
        for (auto stream : streams)
        {
            delete stream;
        };
        streams.clear();
    }

    void CSoundSystem::Resume()
    {
        paused = false;
        for (auto stream : streams)
        {
            if(stream->GetState() == CAudioStream::Playing) stream->Resume();
        }
    }

    void CSoundSystem::Pause()
    {
        paused = true;
        for (auto stream : streams)
        {
            stream->Pause(false);
        };
    }

    void CSoundSystem::Process()
    {
        if (CTimer::m_UserPause || CTimer::m_CodePause) // covers menu pausing, no disc in drive pausing, etc.
        {
            if (!paused) Pause();
        }
        else // not in menu
        {
            if (paused) Resume();

            // get game globals
            masterSpeed = CTimer::ms_fTimeScale;
            masterVolume = AEAudioHardware.m_fEffectMasterScalingFactor * 0.5f; // fit to game's sfx volume

            // camera movements
            CMatrixLink * pMatrix = nullptr;
            CVector * pVec = nullptr;
            if (TheCamera.m_matrix)
            {
                pMatrix = TheCamera.m_matrix;
                pVec = &pMatrix->pos;
            }
            else pVec = &TheCamera.m_placement.m_vPosn;

            BASS_3DVECTOR prevPos = pos;
            pos = BASS_3DVECTOR(pVec->y, pVec->z, pVec->x);

            // calculate velocity
            vel = prevPos;
            vel.x -= pos.x;
            vel.y -= pos.y;
            vel.z -= pos.z;
            auto timeDelta = 0.001f * (CTimer::m_snTimeInMillisecondsNonClipped - CTimer::m_snPreviousTimeInMillisecondsNonClipped);
            vel.x *= timeDelta;
            vel.y *= timeDelta;
            vel.z *= timeDelta;

            // setup the ears
            if (!TheCamera.m_bJust_Switched && !TheCamera.m_bCameraJustRestored) // avoid camera change/jump cut velocity glitches
            {
                BASS_Set3DPosition(
                    &pos,
                    &vel,
                    pMatrix ? &BASS_3DVECTOR(pMatrix->at.y, pMatrix->at.z, pMatrix->at.x) : nullptr,
                    pMatrix ? &BASS_3DVECTOR(pMatrix->up.y, pMatrix->up.z, pMatrix->up.x) : nullptr
                );
            }

            // process streams
            for(auto stream : streams) stream->Process();

            // apply above changes
            BASS_Apply3D();
        }
    }
}
