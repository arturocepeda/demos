
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

#include "./../audio/audio.openal.h"
#include "OpenAL/mxsoundgenopenal.h"
#include "OpenAL/mxopenalsourcemanager.h"

#include <iostream>

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 16

using namespace std;

bool bThreadEnd = false;

void* MusicTimerThread(void* lp);
void TimerTick(const MSTimePosition& mTimePosition, void* pData);

int main(int argc, char* argv[])
{
    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application\n\n";

    // instrument
    MSRange mDoubleBassRange = {28, 60};
    MCInstrument* mDoubleBass = new MCInstrument(1, mDoubleBassRange, 1);

    // sound generator
    CAudio::init();
    MCOpenALSourceManager* mManager = new MCOpenALSourceManager(OPENAL_SOURCES);
    MCSoundGenOpenAL* mSoundGen = new MCSoundGenOpenAL(mDoubleBass->getNumberOfChannels(), false, 1, mManager);
    mSoundGen->loadSamplePack("./../../common/instruments/doublebass.msp");

    // double bass settings
    mDoubleBass->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer;
    mTimer.setCallbackTick(TimerTick, mDoubleBass);
    mTimer.start();

    // create music timer thread
    pthread_t hMusicTimerThread;
    pthread_create(&hMusicTimerThread, NULL, MusicTimerThread, &mTimer);

    // bending parameters
    MSNote mNote;
    int iBendingCents;
    unsigned int iBendingTicks;

    while(1)
    {
        do
        {
            cout << "  Bending cents [-1200, 1200]. Enter 0 to quit: ";
            cin >> iBendingCents;

        } while(iBendingCents < -1200 || iBendingCents > 1200);

        // quit
        if(iBendingCents == 0)
            break;

        do
        {
            cout << "  Bending duration in ticks [1, 96]: ";
            cin >> iBendingTicks;

        } while(iBendingTicks < 1 || iBendingTicks > 96);

        cout << "  Playing note and bending...\n\n";

        mNote.Pitch = 40;
        mNote.Intensity = 127;

        mDoubleBass->playNote(mNote);
        mDoubleBass->bend(0, iBendingCents, iBendingTicks);
    }

    bThreadEnd = true;

    // wait until the music timer thread finishes
    bThreadEnd = true;
    pthread_join(hMusicTimerThread, NULL);    

    delete mDoubleBass;
    delete mSoundGen;
    delete mManager;

    CAudio::release();
    
    cout << "\n";

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
    MCInstrument* mDoubleBass = (MCInstrument*)pData;
    mDoubleBass->update(mTimePosition);
}
