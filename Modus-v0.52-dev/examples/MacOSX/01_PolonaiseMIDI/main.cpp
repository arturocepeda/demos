
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda Pérez
//  September 2012
//
//  --- main.cpp ---
//
////////////////////////////////////////////////////////////////////////

#include "modus.h"

//
//  NOTE: it is necessary to add the __MACOSX_CORE__ preprocessor definition to the project in order
//  to let RtMidi know which library we would like to use, as well as link with the CoreMidi,
//  CoreAudio and CoreFoundation frameworks
//
#include "MIDI/mxsoundgenmidi.h"
#include "externals/RtMidi/RtMidi.h"

#include <iostream>
#include "./../input.h"

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

using namespace std;

void TimerTick(const MSTimePosition& mTimePosition, void* pData);

int main(int argc, char* argv[])
{
    // instrument
    MSRange mPianoRange = {21, 108};
    MCInstrument* mPiano = new MCInstrument(1, mPianoRange, mPianoRange.getSize());

    // score
    MCScore* mScore = new MCScore();
    mScore->loadScriptFromFile("./../../common/scripts/score.piano.chopin.txt");

    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application";

    // list of MIDI out devices
    RtMidiOut* mDevice;
    unsigned int iNumDevices;
    unsigned int iSelectedDevice;
    unsigned int i;

    mDevice = new RtMidiOut(RtMidi::MACOSX_CORE, "Modus Sample");
    iNumDevices = mDevice->getPortCount();

    if(iNumDevices == 0)
    {
        cout << "\n\n  ERROR: no MIDI out devices found\n\n";
        return 0;
    }

    cout << "\n\n  Select a MIDI out device:\n";

    for(i = 0; i < iNumDevices; i++)
        cout << "\n  " << i + 1 << ") " << mDevice->getPortName(i);

    cout << "\n\n";

    do
    {
        cout << "  -> ";
        cin >> iSelectedDevice;

    } while(iSelectedDevice < 1 || iSelectedDevice > iNumDevices);

    iSelectedDevice--;

    // sound generator
    mDevice->openPort(iSelectedDevice);
    MCSoundGenMIDI* mSoundGen = new MCSoundGenMIDI(mPiano->getNumberOfChannels(), mDevice, 1, 1);

    // piano settings
    mPiano->setScore(mScore);
    mPiano->setSoundGen(mSoundGen);

    // timer
    MCTimer* mTimer = new MCTimer();
    mTimer->setCallbackTick(TimerTick, mPiano);
    mTimer->start();

    cout << "\n  Playing! Press any key to quit...";
    fflush(stdout);
    
    init_keyboard();

    while(!kbhit())
    {
#ifdef REDUCE_CPU_USAGE
        if(!mTimer->update())
            usleep(1000);
#else
        mTimer->update();
#endif
    }
    
    close_keyboard();

    delete mPiano;
    delete mScore;
    delete mSoundGen;
    delete mTimer;

    mDevice->closePort();
    delete mDevice;
    
    cout << "\n\n";

    return 0;
}

void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}
