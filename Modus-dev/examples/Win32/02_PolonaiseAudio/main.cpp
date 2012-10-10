
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

#include "./../audio/audio.fmod.h"
#include "FMOD/mxsoundgenfmod.h"

#include <iostream>
#include <conio.h>

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a higher value in case it doesn't sound properly
#define FMOD_DSP_BUFFER_SIZE    512

#ifdef _MSC_VER
#pragma comment(lib, ".\\..\\..\\..\\lib\\Win32\\modus.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\FMOD\\lib.win32\\fmodex_vc.lib")
#endif

using namespace std;

void TimerTick(const MSTimePosition& mTimePosition, void* pData);

int main(int argc, char* argv[])
{
    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application";

    // instrument
    MSRange mPianoRange = {21, 108};
    MCInstrument* mPiano = new MCInstrument(1, mPianoRange, mPianoRange.getSize());

    // score
    MCScore* mScore = new MCScore();
    mScore->loadScriptFromFile(".\\..\\..\\common\\scripts\\score.piano.chopin.txt");

    // sound generator
    CAudio::init(FMOD_DSP_BUFFER_SIZE);
    MCSoundGenFMOD* mSoundGen = new MCSoundGenFMOD(mPiano->getNumberOfChannels(), false, CAudio::getSoundSystem());
    mSoundGen->loadSamplePack(".\\..\\..\\common\\instruments\\piano.msp");

    // piano settings
    mPiano->setScore(mScore);
    mPiano->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer(120, 4);
    mTimer.setCallbackTick(TimerTick, mPiano);
    mTimer.start();

    cout << "\n\n  Playing! Press any key to quit...";

    while(!_kbhit())
    {
        if(mTimer.update())
            CAudio::update();
#ifdef REDUCE_CPU_USAGE
        else
            Sleep(1);
#endif
    }

    delete mPiano;
    delete mScore;
    delete mSoundGen;

    CAudio::release();

    return 0;
}

void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}
