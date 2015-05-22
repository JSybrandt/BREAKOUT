// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.h v1.2

#ifndef _AUDIO_H                // Prevent multiple definitions if this 
#define _AUDIO_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Audio;

#include "d3dUtil.h"
#include <Xact3.h>
#include <Xact3d3.h>
#include <X3daudio.h>
#include "constants.h"

namespace audioNS{
	const int NUM_EMITTERS = 10;
	const int CHANNEL_COUNT = 1;
	const float MIN_SOUND_DIST = 10;
	const float MAX_SOUND_DIST = 30;
};

struct AudioData{
	AudioData(){inUse=played = false;}
	bool inUse;
	bool played;

	void update(Vector3 pos);

	//don't touch
	X3DAUDIO_EMITTER _emitter;
	IXACT3Cue* _cue;
	char* _name;
};

using namespace audioNS;

class Audio
{
    // properties
  private:
    IXACT3Engine *xactEngine;   // pointer to XACT sound engine
    IXACT3WaveBank *waveBank;   // pointer to XACT wave bank
    IXACT3SoundBank *soundBank; // pointer to XACT sound bank
    IXACT3Wave *ppWave;         // pointer to XACT wave object
    XACTINDEX cueI;             // XACT sound index
    void *mapWaveBank;          // call UnmapViewOfFile() to release file
    void *soundBankData;
    bool coInitialized;         // set true if coInitialize is successful

	//3d sound
	// you need one set of these:
	X3DAUDIO_HANDLE xact3dInstance;
	X3DAUDIO_DSP_SETTINGS dspSettings;
	X3DAUDIO_LISTENER listener;
 
	AudioData datum[NUM_EMITTERS];

  public:
    // Constructor
    Audio();

    // Destructor
    virtual ~Audio();

    // member functions

    // Initialize Audio
    HRESULT initialize();

    // Perform periodic sound engine tasks.
    void run();

    // Play sound specified by cue from sound bank.
    // If cue does not exist no error occurs, there is simply no sound played.
    void playCue(const char name[]);

	//use for 3d sounds
	void playCue(AudioData* data);

	//use for 3d sounds
	void stopCue(AudioData* data);

    // Stop a playing sound specified by cue from sound bank.
    // If cue does not exist no error occurs.
    void stopCue(const char name[]);

    // Pause sound specified by category from sound bank.
    // If category does not exist no error occurs.
    void pauseCategory(const char category[]);

    // Resume playback of paused sound specified by category from sound bank.
    // If category does not exist no error occurs.
    void resumeCategory(const char category[]);

	void updateCamera(Vector3 pos, Vector3 dir, Vector3 up, Vector3 vel);

	AudioData* buildData(const char name[]);
	void updateCue(AudioData **data, const char name[]);
};

#endif
