
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda PŽrez
//  September 2012
//
////////////////////////////////////////////////////////////////////////

#include "modus.h"

#include "externals/RtMidi/RtMidi.h"

#include "./../audio/audio.openal.h"
#include "OpenAL/mxsoundgenopenal.h"
#include "OpenAL/mxopenalsourcemanager.h"

#include <iostream>
#include <vector>
#include "./../input.h"

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 32

using namespace std;

MCMIDIReceiver* mMIDIReceiver;

void TimerTick(const MSTimePosition& mTimePosition, void* pData);
void MidiInProc(double dTime, std::vector<unsigned char>* vMessage, void* pData);

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
    std::string sDevice;
    unsigned int iNumDevices;
    unsigned int iSelectedDevice;
    unsigned int i;

    RtMidiIn* mDevice = new RtMidiIn(RtMidi::MACOSX_CORE, "Modus Sample");
    iNumDevices = mDevice->getPortCount();

    if(iNumDevices == 0)
    {
        cout << "\n\n  ERROR: no MIDI in devices found\n\n";
        return 0;
    }
    
    cout << "\n\n  Select a MIDI in device:\n";

    for(i = 0; i < iNumDevices; i++)
    {
        sDevice = mDevice->getPortName(i);
        cout << "\n  " << i + 1 << ") " << sDevice;
    }

    cout << "\n\n";

    do
    {
        cout << "  -> ";
        cin >> iSelectedDevice;

    } while(iSelectedDevice < 1 || iSelectedDevice > iNumDevices);

    iSelectedDevice--;

    // MIDI listener
    mDevice->openPort(iSelectedDevice);
    mDevice->setCallback(&MidiInProc);
    
    // Modus MIDI receiver
    mMIDIReceiver = new MCMIDIReceiver(mPiano);
    mMIDIReceiver->listenToAllMIDIChannels();
    mMIDIReceiver->attachAllPitchesToDifferentChannels();

    // sound generator
    CAudio::init();
    MCOpenALSourceManager* mALManager = new MCOpenALSourceManager(OPENAL_SOURCES);
    MCSoundGenOpenAL* mSoundGen = new MCSoundGenOpenAL(mPiano->getNumberOfChannels(), false, 1, mALManager);
    mSoundGen->loadSamplePack("./../../common/instruments/piano.msp");
    mPiano->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer;
    mTimer.setCallbackTick(TimerTick, mPiano);
    mTimer.start();

    cout << "\n  Listening, you can play now! Press any key to quit...";
    fflush(stdout);

    init_keyboard();
    
    while(!kbhit())
    {
#ifdef REDUCE_CPU_USAGE
        if(!mTimer.update())
            usleep(1000);
#else
        mTimer.update();
#endif
    }
    
    close_keyboard();

    delete mPiano;
    delete mSoundGen;
    delete mALManager;
    delete mMIDIReceiver;

    delete mDevice;
    CAudio::release();
    
    cout << "\n\n";

    return 0;
}

void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}

void MidiInProc(double dTime, std::vector<unsigned char>* vMessage, void* pData)
{
    mMIDIReceiver->message(*vMessage);
}
