
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

#include "./../audio/audio.fmod.h"
#include "FMOD/mxsoundgenfmod.h"

#include <iostream>

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
    cout << "\n  Sample Application\n\n";

    // instrument
    MSRange mTenorSaxRange = {44, 75};
    MCInstrument* mTenorSax = new MCInstrument(1, mTenorSaxRange, 1);

    // sound generator
    CAudio::init(FMOD_DSP_BUFFER_SIZE);
    MCSoundGenFMOD* mSoundGen = new MCSoundGenFMOD(mTenorSax->getNumberOfChannels(), false, CAudio::getSoundSystem());
    mSoundGen->loadSamplePack("./../../common/instruments/tenorsax.msp");

    // sax settings
    mTenorSax->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer(120, 4);
    mTimer.setCallbackTick(TimerTick, mTenorSax);
    mTimer.start();

    // create music timer thread
    pthread_t hMusicTimerThread;
    pthread_create(&hMusicTimerThread, NULL, MusicTimerThread, &mTimer);

    // intensity change parameters
    MSNote mNote;
    unsigned int iIntensityA;
    unsigned int iIntensityB;
    unsigned int iChangeTicks;

    while(1)
    {
        do
        {
            cout << "  Intensity [1, 127]. Enter 0 to quit: ";
            cin >> iIntensityA;

        } while(iIntensityA > 127);

        // quit
        if(iIntensityA == 0)
            break;

        do
        {
            cout << "  Final intensity [1, 127]: ";
            cin >> iIntensityB;

        } while(iIntensityB < 1 || iIntensityB > 127);

        do
        {
            cout << "  Intensity change duration in ticks [1, 96]: ";
            cin >> iChangeTicks;

        } while(iChangeTicks < 1 || iChangeTicks > 96);

        cout << "  Playing note...\n\n";

        memset(&mNote, 0, sizeof(MSNote));
        mNote.Pitch = 48;
        mNote.Intensity = iIntensityA;

        mTenorSax->playNote(mNote);
        mTenorSax->changeIntensity(0, iIntensityB, iChangeTicks);
    }

    // wait until the music timer thread finishes
    bThreadEnd = true;
    pthread_join(hMusicTimerThread, NULL);

    delete mTenorSax;
    delete mSoundGen;

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
    MCInstrument* mTenorSax = (MCInstrument*)pData;
    mTenorSax->update(mTimePosition);
}
