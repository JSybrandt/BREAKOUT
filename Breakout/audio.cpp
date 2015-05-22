// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.cpp v1.2

#include "audio.h"

//=============================================================================
// default constructor
//=============================================================================
Audio::Audio()
{
    xactEngine = NULL;
    waveBank = NULL;
    soundBank = NULL;
    cueI = 0;
    mapWaveBank = NULL;         // Call UnmapViewOfFile() to release file
    soundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        coInitialized = true;
    else
        coInitialized = false;
}

//=============================================================================
// destructor
//=============================================================================
Audio::~Audio()
{
    // Shutdown XACT
    if( xactEngine )
    {
        xactEngine->ShutDown(); // shut down XACT engine and free resources
        xactEngine->Release();
    }

    if ( soundBankData )
        delete[] soundBankData;
    soundBankData = NULL;

    // After xactEngine->ShutDown() returns, release memory mapped files
    if( mapWaveBank )
        UnmapViewOfFile( mapWaveBank );
    mapWaveBank = NULL;

    if( coInitialized )        // if CoInitializeEx succeeded
        CoUninitialize();
}

//=============================================================================
// initialize
// This function does the following:
//      1. Initialize XACT by calling xactEngine->Initialize 
//      2. Create the XACT wave bank(s) you want to use
//      3. Create the XACT sound bank(s) you want to use
//      4. Store indices to the XACT cue(s) your game uses
//=============================================================================
HRESULT Audio::initialize()
{
    HRESULT result = E_FAIL;
    HANDLE hFile;
    DWORD fileSize;
    DWORD bytesRead;
    HANDLE hMapFile;

    result = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED(result) )
        result = XACT3CreateEngine( 0, &xactEngine );

    if( FAILED( result ) || xactEngine == NULL )
        return E_FAIL;

    result = XACT3CreateEngine( 0, &xactEngine );
    if( FAILED( result ) || xactEngine == NULL )
        return E_FAIL;

    // Load the global settings file and pass it into XACTInitialize
    VOID* pGlobalSettingsData = NULL;
    DWORD dwGlobalSettingsFileSize = 0;
    bool bSuccess = false;

	/*
	
		DOES THIS ONE CHANGE TOO?
	
	*/
    hFile = CreateFileA( XGS_FILE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile )
    {
        dwGlobalSettingsFileSize = GetFileSize( hFile, NULL );
        if( dwGlobalSettingsFileSize != INVALID_FILE_SIZE )
        {
            pGlobalSettingsData = CoTaskMemAlloc( dwGlobalSettingsFileSize );
            if( pGlobalSettingsData )
            {
                if( 0 != ReadFile( hFile, pGlobalSettingsData, dwGlobalSettingsFileSize, &bytesRead, NULL ) )
                {
                    bSuccess = true;
                }
            }
        }
        CloseHandle( hFile );
    }
    if( !bSuccess )
    {
        if( pGlobalSettingsData )
            CoTaskMemFree( pGlobalSettingsData );
        pGlobalSettingsData = NULL;
        dwGlobalSettingsFileSize = 0;
    }


    // Initialize & create the XACT runtime 
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.pGlobalSettingsBuffer = pGlobalSettingsData;
    xactParams.globalSettingsBufferSize = dwGlobalSettingsFileSize;
    // delete the buffer when not needed
    xactParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
    // Set the look agead time to default
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    // Initialize the XACT engine
    result = xactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // Create an "in memory" XACT wave bank file using memory mapped file IO
    result = E_FAIL; // default to failure code, replaced on success
    hFile = CreateFileA( WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, fileSize, NULL );
            if( hMapFile )
            {
                mapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( mapWaveBank )
                    result = xactEngine->CreateInMemoryWaveBank( mapWaveBank, fileSize, 0, 0, &waveBank );

                CloseHandle( hMapFile );    // mapWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile );    // mapWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    // Read and register the sound bank file with XACT.
    result = E_FAIL;    // default to failure code, replaced on success
    hFile = CreateFileA( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            soundBankData = new BYTE[fileSize];    // reserve memory for sound bank
            if( soundBankData )
            {
                if( 0 != ReadFile( hFile, soundBankData, fileSize, &bytesRead, NULL ) )
                    result = xactEngine->CreateSoundBank( soundBankData, fileSize, 0, 0, &soundBank );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );






	//3d sound
	// setup the XACT3D
	XACT3DInitialize(xactEngine,xact3dInstance);
 
	// check how many output channels are supported
	WAVEFORMATEXTENSIBLE format;
	xactEngine->GetFinalMixFormat(&format);
 
	// fill the DSP
	ZeroMemory(&dspSettings,sizeof(dspSettings));
	// different code's seem to suggest 1 or 2 channels for the emitter
	// i'm going for 1
	dspSettings.SrcChannelCount = CHANNEL_COUNT;
	dspSettings.DstChannelCount = format.Format.nChannels;  // as supported  
	dspSettings.pMatrixCoefficients = new FLOAT32[dspSettings.SrcChannelCount * dspSettings.DstChannelCount];
	ZeroMemory(dspSettings.pMatrixCoefficients ,sizeof(FLOAT32)*dspSettings.SrcChannelCount * dspSettings.DstChannelCount);



	updateCamera(Vector3(0,0,0),Vector3(1,0,0),Vector3(0,1,0),Vector3(0,0,0));



    return S_OK;

}

//=============================================================================
// perform periodic sound engine tasks
//=============================================================================
void Audio::run()
{
    if (xactEngine == NULL)
        return;  
	for(int i = 0; i < NUM_EMITTERS; i++)
	{
		if(datum[i].inUse && datum[i].played){

						// the falloff curve:
			// at range 0=>100%, MIN_SOUND_DIST=>100%, MAX_SOUND_DIST=>0%
			X3DAUDIO_DISTANCE_CURVE_POINT soundFalloffPt[] = { 0.0f, 1.0f, MIN_SOUND_DIST, 1.0f, MAX_SOUND_DIST, 0.0f  };
			X3DAUDIO_DISTANCE_CURVE       soundFalloff          = { &soundFalloffPt[0], 3 };
			// set the falloff curve
			datum[i]._emitter.pVolumeCurve =(X3DAUDIO_DISTANCE_CURVE*)&soundFalloff;
			datum[i]._emitter.CurveDistanceScaler = 1.0; // this is a multiplier on all distances
 
			XACT3DCalculate( xact3dInstance, &listener, &datum[i]._emitter, &dspSettings );
			XACT3DApply( &dspSettings, datum[i]._cue);
		}
	}
	xactEngine->DoWork();
}

//=============================================================================
// play sound specified by cue from sound bank
// if cue does not exist no error occurs, there is simply no sound played
//=============================================================================
void Audio::playCue(const char name[])
{
    if (soundBank == NULL)
        return;
    cueI = soundBank->GetCueIndex( name );       // get cue index from sound bank
    soundBank->Play( cueI, 0, 0, NULL );//store pointer to played audio

}

void Audio::playCue(AudioData* d)
{
	 if (soundBank == NULL)
        return;

	cueI = soundBank->GetCueIndex( d->_name );       // get cue index from sound bank
    soundBank->Play( cueI, 0, 0, &d->_cue );//store pointer to played audio
	d->played=true;
	
	// computer the effects on the sound and apply it
	XACT3DCalculate( xact3dInstance, &listener, &d->_emitter, &dspSettings );
	XACT3DApply( &dspSettings, d->_cue);
	
}

//=============================================================================
// stop a playing sound specified by cue from sound bank
// if cue does not exist no error occurs
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (soundBank == NULL)
        return;
    cueI = soundBank->GetCueIndex( cue );        // get cue index from sound bank
    soundBank->Stop( cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}

//use for 3d sounds
void Audio::stopCue(AudioData* data)
{
	 if (soundBank == NULL)
        return;
	data->_cue->Stop(XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
	data->played=false;
}

//=============================================================================
// Pause sound specified by category.
// If the category does not exist no error occurs.
//=============================================================================
void Audio::pauseCategory(const char category[])
{
    if (soundBank == NULL)
        return;
    XACTCATEGORY iCategory = xactEngine->GetCategory(category);
    xactEngine->Pause(iCategory,true);
}

//=============================================================================
// Resume playback of paused sound specified by category.
// If cue does not exist no error occurs.
//=============================================================================
void Audio::resumeCategory(const char category[])
{
    if (soundBank == NULL)
        return;
    XACTCATEGORY iCategory = xactEngine->GetCategory(category);
    xactEngine->Pause(iCategory,false);
}

void Audio::updateCamera(Vector3 pos, Vector3 dir, Vector3 up, Vector3 vel)
{
	// Set the listener information
	ZeroMemory(&listener,sizeof(listener));
	// set the XYZ of the camera
	listener.Position = pos;
	// set the direction the camera looks in
	// this must be a unit vector
	listener.OrientFront = dir;
	// set the up-direction of the camera
	// again, this must be a unit vector and orthogonal to camLook
	listener.OrientTop = up;
	// assuming no doppler effect 
	listener.Velocity = vel;
}

void AudioData::update(Vector3 pos)
{
	// Set the emitter information
	ZeroMemory(&_emitter,sizeof(_emitter));
 
	// only worrying about its position, just giving fixed other info
	_emitter.OrientFront = D3DXVECTOR3(0,0,1);
	_emitter.OrientTop = D3DXVECTOR3(0,1,0);
	_emitter.Position = pos;
	_emitter.Velocity = D3DXVECTOR3(0,0,0);
	// emitter ChannelCount and DSP Setting's SrcChannelCount must match
	_emitter.ChannelCount = CHANNEL_COUNT;
}


AudioData* Audio::buildData(const char name[])
{
	for(int i = 0 ; i < NUM_EMITTERS; i++)
	{
		if(!datum[i].inUse)
		{
			datum[i].inUse=true;
			datum[i].update(Vector3(0,0,0));//set init vals get cue
			datum[i]._name = const_cast<char*>(name);
			return &datum[i];
		}
	}
	return nullptr;
}

void Audio::updateCue(AudioData**data, const char name[])
{
	if((*data)->played)
		stopCue(*data);
	(*data)->inUse = (*data)->played = false;
	(*data) = buildData(name);
}
