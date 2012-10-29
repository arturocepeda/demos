
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

#include "main.h"

#include "./../audio/audio.openal.h"
#include "OpenAL/mxsoundgenopenal.h"
#include "OpenAL/mxopenalsourcemanager.h"

#include <iostream>
#include <conio.h>

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 16

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

int main(int argc, char* argv[])
{
    // instrument
    MSRange mPianoRange = {21, 108};
    MCInstrument* mPiano = new MCInstrument(1, mPianoRange, mPianoRange.getSize());

    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application";

    // global data
    SGlobal sGlobal;

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

    // score to write the notes
    sGlobal.mScore = new MCScore();

    // timer
    sGlobal.mTimer = new MCTimer();
    sGlobal.mTimer->setCallbackTick(TimerTick, mPiano);
    sGlobal.mTimer->start();

    // set piano callbacks
    mPiano->setCallbackPlay(PianoPlayNote, &sGlobal);
    mPiano->setCallbackRelease(PianoReleaseNote, &sGlobal);
    mPiano->setCallbackDamper(PianoDamper, &sGlobal);

    cout << "\n  Press any key to start recording. Press ESC to quit...";

    int iLastKey = 0;
    sGlobal.bRecordingMode = false;

    while(iLastKey != 27)
    {
        // key pressed
        if(_kbhit())
        {
            iLastKey = toupper(_getch());

            // mode change
            if(iLastKey != 27)
            {
                sGlobal.bRecordingMode = !sGlobal.bRecordingMode;
                sGlobal.mTimer->reset();
                sGlobal.mTimer->start();

                if(sGlobal.bRecordingMode)
                {
                    sGlobal.mScore->clear();
                    mPiano->setScore(NULL);
                    mPiano->releaseAll();
                    cout << "\n\n  Recording! Press any key to hear. Press ESC to quit...";
                }
                else
                {
                    mPiano->setScore(sGlobal.mScore);
                    cout << "\n\n  Playing! Press any key to record again. Press ESC to quit...";
                }
            }
        }

        // timer update
#ifdef REDUCE_CPU_USAGE
        if(!sGlobal.mTimer->update())
            Sleep(1);
#else
        sGlobal.mTimer->update();
#endif
    }

    delete sGlobal.mScore;
    delete sGlobal.mTimer;
    delete mPiano;
    delete mSoundGen;
    delete mALManager;
    delete mMIDIReceiver;

    midiInStop(hDevice);
    midiInClose(hDevice);
    CAudio::release();

    return 0;
}


//
//  timer tick callback
//
void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}


//
//  MIDI in callback
//
void CALLBACK MidiInProc(HMIDIIN mHandle, UINT iMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    if(iMsg == MIM_DATA)
        mMIDIReceiver->message32inv(dwParam1);
}


//
//  piano callbacks
//
void PianoPlayNote(unsigned int iInstrID, const MSNote& mNote, void* pData)
{
    SGlobal* sGlobal = (SGlobal*)pData;
    
    // write to the score
    if(sGlobal->bRecordingMode)
    {
        MSScoreEntry se;
        se.Note = mNote;
        se.TimePosition = sGlobal->mTimer->getTimePosition();
        sGlobal->mScore->addEntry(se);
    }
}

void PianoReleaseNote(unsigned int iInstrID, const MSNote& mNote, void* pData)
{
    SGlobal* sGlobal = (SGlobal*)pData;
    
    // write to the score
    if(sGlobal->bRecordingMode)
    {
        MSScoreEntry se;
        se.Note = mNote;
        se.Note.Intensity = 0;  // note off
        se.TimePosition = sGlobal->mTimer->getTimePosition();
        sGlobal->mScore->addEntry(se);
    }
}

void PianoDamper(unsigned int iInstrID, bool bDamperState, void* pData)
{
    SGlobal* sGlobal = (SGlobal*)pData;
    
    // write to the score
    if(sGlobal->bRecordingMode)
    {
        MSScoreEntry se;        
        se.Note.Pitch = M_CTRL_DAMPER;
        se.Note.Intensity = (unsigned char)bDamperState;
        se.TimePosition = sGlobal->mTimer->getTimePosition();
        sGlobal->mScore->addEntry(se);
    }
}
