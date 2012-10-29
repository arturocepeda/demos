
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda Pérez
//  September 2012
//
////////////////////////////////////////////////////////////////////////

#include "modus.h"

#include "./../audio/audio.fmod.h"
#include "FMOD/mxsoundgenfmod.h"

#include <iostream>
#include <pthread.h>
#include "./../input.h"

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a higher value in case it doesn't sound properly
#define FMOD_DSP_BUFFER_SIZE    512

using namespace std;

bool bThreadEnd = false;

void* MusicTimerThread(void* lp);
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
    mScore->loadScriptFromFile("./../../common/scripts/score.piano.chopin.txt");

    // sound generator
    CAudio::init(FMOD_DSP_BUFFER_SIZE);
    MCSoundGenFMOD* mSoundGen = new MCSoundGenFMOD(mPiano->getNumberOfChannels(), false, CAudio::getSoundSystem());
    mSoundGen->loadSamplePack("./../../common/instruments/Piano.msp");

    // piano settings
    mPiano->setScore(mScore);
    mPiano->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer;
    mTimer.setCallbackTick(TimerTick, mPiano);
    mTimer.start();

    cout << "\n\n  Playing! Press any key to quit...\n\n";
    fflush(stdout);

    // message
    char sMessage[] = {"  The main thread is performing this simple task while the music thread\n"
                       "  plays the music!\n\n"};
    int iCurrentChar = 0;

    // create music timer thread
    pthread_t hMusicTimerThread;
    pthread_create(&hMusicTimerThread, NULL, MusicTimerThread, &mTimer);
    
    init_keyboard();

    while(!kbhit())
    {
        cout << sMessage[iCurrentChar];
        fflush(stdout);
        iCurrentChar++;

        if(iCurrentChar == strlen(sMessage))
            iCurrentChar = 0;

        usleep(100000);
    }
    
    close_keyboard();

    // wait until the music timer thread finishes
    bThreadEnd = true;
    pthread_join(hMusicTimerThread, NULL);

    delete mPiano;
    delete mScore;
    delete mSoundGen;

    CAudio::release();
    
    cout << "\n\n";

    return 0;
}

//
//  Thread function for the music timer
//
void* MusicTimerThread(void* lp)
{
    MCTimer* mTimer = (MCTimer*)lp;

    while(!bThreadEnd)
    {
        if(mTimer->update())
            CAudio::update();
#ifdef REDUCE_CPU_USAGE
        else
            usleep(1000);
#endif
    }
}

//
//  Timer tick callback
//
void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    MCInstrument* mPiano = (MCInstrument*)pData;
    mPiano->update(mTimePosition);
}
