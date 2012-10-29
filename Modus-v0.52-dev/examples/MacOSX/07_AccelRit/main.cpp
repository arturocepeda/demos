
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
#include "./../input.h"

#define LOOP        4
#define TEMPO_A     80.0f
#define TEMPO_B     150.0f

// define to avoid 100% CPU usage
#define REDUCE_CPU_USAGE

// try with a higher value in case it doesn't sound properly
#define FMOD_DSP_BUFFER_SIZE    512

using namespace std;

struct SGlobal
{
    MCInstrument* mDrums;
    MCScore* mScore;
    MCTimer* mTimer;
    MCSongStructure* mStructure;
};

void TimerTick(const MSTimePosition& mTimePosition, void* pData);

int main(int argc, char* argv[])
{
    // header
    cout << "\n  Modus " << MODUS_VERSION;
    cout << "\n  C++ Music Library";
    cout << "\n  Sample Application\n";

    // global data
    SGlobal sGlobal;

    // instrument
    MSRange mDrumsRange = {35, 59};
    sGlobal.mDrums = new MCInstrument(1, mDrumsRange, 16);

    // score
    sGlobal.mScore = new MCScore();
    sGlobal.mScore->loadScriptFromFile("./../../common/scripts/score.drums.sample.txt");

    // sound generator
    CAudio::init(FMOD_DSP_BUFFER_SIZE);
    MCSoundGenFMOD* mSoundGen = new MCSoundGenFMOD(sGlobal.mDrums->getNumberOfChannels(), false, 
                                                   CAudio::getSoundSystem());
    mSoundGen->loadSamplePack("./../../common/instruments/drums.msp");

    // drums settings
    sGlobal.mDrums->setScore(sGlobal.mScore);
    sGlobal.mDrums->setSoundGen(mSoundGen);

    // song structure object
    sGlobal.mStructure = new MCSongStructure();

    MSSongSection mFirstSection;
    mFirstSection.FirstMeasure = 1;
    mFirstSection.LastMeasure = LOOP;
    mFirstSection.Tempo = TEMPO_A;
    mFirstSection.TargetTempo = TEMPO_B;
    mFirstSection.BeatsPerMeasure = 4;

    sGlobal.mStructure->addEntry(mFirstSection);

    // timer
    sGlobal.mTimer = new MCTimer();
    sGlobal.mTimer->setSongStructure(sGlobal.mStructure);
    sGlobal.mTimer->setCallbackTick(TimerTick, &sGlobal);
    sGlobal.mTimer->start();

    cout << "\n  Playing! Press any key to quit\n";
    cout << "\n  Accelerando...";
    fflush(stdout);

    init_keyboard();
    
    while(!kbhit())
    {
        if(sGlobal.mTimer->update())
            CAudio::update();
#ifdef REDUCE_CPU_USAGE
        else
            usleep(1000);
#endif
    }
    
    close_keyboard();

    delete sGlobal.mDrums;
    delete sGlobal.mScore;
    delete mSoundGen;
    delete sGlobal.mTimer;
    delete sGlobal.mStructure;

    CAudio::release();
    
    cout << "\n\n";

    return 0;
}

void TimerTick(const MSTimePosition& mTimePosition, void* pData)
{
    static bool bAccelRit = false;

    SGlobal* sGlobal = (SGlobal*)pData;

    // loop management
    if(mTimePosition.Measure % LOOP == 0 && mTimePosition.Beat == 4 && mTimePosition.Tick == M_LAST_TICK)
    {
        sGlobal->mScore->displace(LOOP);

        MSSongSection mSection;
        mSection.FirstMeasure = mTimePosition.Measure + 1;
        mSection.LastMeasure = mTimePosition.Measure + LOOP;
        mSection.BeatsPerMeasure = 4;
        
        // accelerando
        if(bAccelRit)
        {
            mSection.Tempo = TEMPO_A;
            mSection.TargetTempo = TEMPO_B;

            cout << "\n  Accelerando...";
            fflush(stdout);
        }

        // ritardando
        else
        {
            mSection.Tempo = TEMPO_B;
            mSection.TargetTempo = TEMPO_A;

            cout << "\n  Ritardando...";
            fflush(stdout);
        }

        sGlobal->mStructure->addEntry(mSection);
        bAccelRit = !bAccelRit;
    }

    // update
    sGlobal->mDrums->update(mTimePosition);
}
