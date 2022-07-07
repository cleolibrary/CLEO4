#pragma once
#include "stdafx.h"

#include <map>

#include "CCodeInjector.h"
#include <set>
#include "bass.h"

namespace CLEO
{
    class CAudioStream;
    class C3DAudioStream;

    class CSoundSystem : VInjectible
    {
        friend class CAudioStream;
        friend class C3DAudioStream;

        std::set<CAudioStream *> streams;
        BASS_INFO SoundDevice;
        bool initialized;
        int forceDevice;
        bool paused;
        bool bUseFPAudio;
        HWND hwnd;
        std::map<std::string, std::pair<unsigned int, std::vector<byte>>> cachedAudioFiles;

    public:
        virtual void Inject(CCodeInjector& inj);
        bool Init(HWND hwnd);
        inline bool Initialized() { return initialized; }

        CSoundSystem() : initialized(false), forceDevice(-1), paused(false), bUseFPAudio(false)
        {
            // TODO: give to user an ability to force a sound device to use (ini-file or cmd-line?)

        }

        ~CSoundSystem()
        {
            TRACE("Closing SoundSystem...");
            UnloadAllStreams();
            if (initialized)
            {
                TRACE("Freeing BASS library");
                BASS_Free();
                initialized = false;
            }
            TRACE("SoundSystem closed!");
        }

        CAudioStream * LoadStream(const char *filename, bool in3d = false);
        void PauseStreams();
        void ResumeStreams();
        void UnloadStream(CAudioStream *stream);
        void UnloadAllStreams();
        void Update();
    };

    class CAudioStream
    {
        friend class CSoundSystem;

        CAudioStream(const CAudioStream&);

    protected:
        HSTREAM streamInternal;
        enum eStreamState
        {
            no,
            playing,
            paused,
            stopped,
        } state;
        bool OK;
        std::string audioFile;

        CAudioStream();
        static unsigned calculateAudioStreamFlags();

    public:
        CAudioStream(const char *src);
        CAudioStream(const std::vector<BYTE>* fileBytes, const char* filename);
        virtual ~CAudioStream();

        // actions on streams
        void Play();
        void Pause(bool change_state = true);
        void Stop();
        void Resume();
        DWORD GetLength();
        DWORD GetState();
        float GetVolume();
        void SetVolume(float val);
        void Loop(bool enable);
		HSTREAM GetInternal();

        // overloadable actions
        virtual void Set3dPosition(const CVector& pos);
        virtual void Link(CPlaceable *placable = nullptr);
        virtual void Process();
    };

    class C3DAudioStream : public CAudioStream
    {
        friend class CSoundSystem;

        C3DAudioStream(const C3DAudioStream&);

    protected:
        CPlaceable	*	link;
        BASS_3DVECTOR	position;

        static unsigned calculate3dAudioStreamFlags();
        void set3dAttributes();
    public:
        C3DAudioStream(const char *src);
        C3DAudioStream(const std::vector<BYTE>* fileBytes, const char* filename);
        virtual ~C3DAudioStream();

        // overloaded actions
        virtual void Set3dPosition(const CVector& pos);
        virtual void Link(CPlaceable *placable = nullptr);
        virtual void Process();
    };
}
