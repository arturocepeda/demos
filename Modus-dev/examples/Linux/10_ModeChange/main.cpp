
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

#include "./../audio/audio.openal.h"
#include "OpenAL/mxsoundgenopenal.h"
#include "OpenAL/mxopenalsourcemanager.h"

#include <iostream>
#include "./../input.h"

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 32

#define LOOP 24

using namespace std;

struct SGlobal
{
    MCInstrument* mTrumpet;
    MCScore* mScore;
};

bool bThreadEnd = false;

void* MusicTimerThread(void* lp);
void TimerTick(const MSTimePosition& mTimePosition, void* pData);

int main(int argc, char* argv[])
{
    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application\n\n";

    // global data
    SGlobal sGlobal;

    // instrument
    MSRange mTrumpetRange = {52, 84};
    sGlobal.mTrumpet = new MCInstrument(1, mTrumpetRange, 1);

    // sound generator
    CAudio::init();
    MCOpenALSourceManager* mManager = new MCOpenALSourceManager(OPENAL_SOURCES);
    MCSoundGenOpenAL* mSoundGen = new MCSoundGenOpenAL(sGlobal.mTrumpet->getNumberOfChannels(), false, 1, mManager);
    mSoundGen->loadSamplePack("./../../common/instruments/Trumpet.msp");

    // score
    sGlobal.mScore = new MCScore();
    sGlobal.mScore->loadScriptFromFile("./../../common/scripts/score.trumpet.sample.txt");

    // instrument settings
    sGlobal.mTrumpet->setScore(sGlobal.mScore);
    sGlobal.mTrumpet->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer(132, 4);
    mTimer.setCallbackTick(TimerTick, &sGlobal);
    mTimer.start();

    // create music timer thread
    pthread_t hMusicTimerThread;
    pthread_create(&hMusicTimerThread, NULL, MusicTimerThread, &mTimer);

    cout << "  Playing! Options:\n";
    cout << "\n    M -> change mode";
    cout << "\n    ESC -> quit\n\n";

    int iKey = 0;
    bool bMode = false;
    unsigned int i;
    
    init_keyboard();

    while(iKey != 27)
    {
        iKey = toupper(getch());

        if(iKey == 'M')
        {
            for(i = 0; i < sGlobal.mScore->getNumberOfEntries(); i++)
                sGlobal.mScore->getEntry(i)->Note.Mode = (bMode)? 1: 0;

            bMode = !bMode;
        }

        usleep(1000);
    }
    
    close_keyboard();

    // wait until the music timer thread finishes
    bThreadEnd = true;
    pthread_join(hMusicTimerThread, NULL);

    delete sGlobal.mTrumpet;
    delete sGlobal.mScore;
    delete mSoundGen;
    delete mManager;

    CAudio::release();

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
#ifdef REDUCE_CPU_USAGE
        if(!mTimer->update())
            usleep(1000);
#else
        mTimer->update();
#endif
    }
}

//
//  Timer tick callback
//
void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    SGlobal* sGlobal = (SGlobal*)pData;
    
    // loop management
    if(mTimePosition.Measure % LOOP == 1 && mTimePosition.Measure > LOOP && 
       mTimePosition.Beat == 1 && mTimePosition.Tick == 0)
    {    
        sGlobal->mScore->displace(LOOP);
    }

    // update instrument
    sGlobal->mTrumpet->update(mTimePosition);
}
