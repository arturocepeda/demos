
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda Pérez
//  July 2012
//
////////////////////////////////////////////////////////////////////////

#include "modus.h"

#include "./../audio/audio.openal.h"
#include "OpenAL/mxsoundgenopenal.h"
#include "OpenAL/mxopenalsourcemanager.h"

#include <iostream>
#include <conio.h>

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 32

#ifdef _MSC_VER
#pragma comment(lib, ".\\..\\..\\..\\lib\\Win32\\modus.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OpenAL\\lib.win32\\OpenAL32.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libogg_static.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libvorbis_static.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libvorbisfile_static.lib")
#pragma comment(lib, "winmm.lib")
#endif

using namespace std;

MCMIDIReceiver* mMIDIReceiver;

void TimerTick(const MSTimePosition& mTimePosition, void* pData);
void CALLBACK MidiInProc(HMIDIIN mHandle, UINT iMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

int main(int argc, char* argv[])
{
    // instrument
    MSRange mPianoRange = {21, 108};
    MCInstrument* mPiano = new MCInstrument(1, mPianoRange, mPianoRange.getSize());

    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application";

    // list of MIDI in devices
    unsigned int iNumDevices;
    unsigned int iSelectedDevice;
    unsigned int i;

    MIDIINCAPS mCaps;
    HMIDIIN hDevice;

    iNumDevices = midiInGetNumDevs();

    if(iNumDevices == 0)
    {
        cout << "\n\n  ERROR: no MIDI in devices found\n\n";
        getchar();

        return 0;
    }

    cout << "\n\n  Select a MIDI in device:\n";

    for(i = 0; i < iNumDevices; i++)
    {
        midiInGetDevCaps(i, &mCaps, sizeof(MIDIINCAPS));
        cout << "\n  " << i + 1 << ") " << mCaps.szPname;
    }

    cout << "\n\n";

    do
    {
        cout << "  -> ";
        cin >> iSelectedDevice;

    } while(iSelectedDevice < 1 || iSelectedDevice > iNumDevices);

    iSelectedDevice--;

    // MIDI listener
    midiInOpen(&hDevice, iSelectedDevice, (DWORD)MidiInProc, 0, CALLBACK_FUNCTION);
    midiInStart(hDevice);
    
    // Modus MIDI receiver
    mMIDIReceiver = new MCMIDIReceiver(mPiano);
    mMIDIReceiver->listenToAllMIDIChannels();
    mMIDIReceiver->attachAllPitchesToDifferentChannels();

    // sound generator
    CAudio::init();
    MCOpenALSourceManager* mALManager = new MCOpenALSourceManager(OPENAL_SOURCES);
    MCSoundGenOpenAL* mSoundGen = new MCSoundGenOpenAL(mPiano->getNumberOfChannels(), false, 1, mALManager);
    mSoundGen->loadSamplePack(".\\..\\..\\common\\instruments\\piano.msp");
    mPiano->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer;
    mTimer.setCallbackTick(TimerTick, mPiano);
    mTimer.start();

    cout << "\n  Listening, you can play now! Press any key to quit...";

    while(!_kbhit())
    {
#ifdef REDUCE_CPU_USAGE
        if(!mTimer.update())
            Sleep(1);
#else
        mTimer.update();
#endif
    }

    delete mPiano;
    delete mSoundGen;
    delete mALManager;
    delete mMIDIReceiver;

    midiInStop(hDevice);
    midiInClose(hDevice);
    CAudio::release();

    return 0;
}

void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}

void CALLBACK MidiInProc(HMIDIIN mHandle, UINT iMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    if(iMsg == MIM_DATA)
        mMIDIReceiver->message32inv(dwParam1);
}
