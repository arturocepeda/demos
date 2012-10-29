
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
//#define REDUCE_CPU_USAGE

// try with a lower value in case it doesn't work properly
#define OPENAL_SOURCES 16

#define LOOP 24

#ifdef _MSC_VER
#pragma comment(lib, ".\\..\\..\\..\\lib\\Win32\\modus.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OpenAL\\lib.win32\\OpenAL32.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libogg_static.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libvorbis_static.lib")
#pragma comment(lib, ".\\..\\..\\..\\soundgen\\externals\\OggVorbis\\lib.win32\\libvorbisfile_static.lib")
#endif

using namespace std;

struct SGlobal
{
    MCInstrument* mTrumpet;
    MCScore* mScore;
};

bool bThreadEnd = false;

DWORD WINAPI MusicTimerThread(LPVOID lp);
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
    mSoundGen->loadSamplePack(".\\..\\..\\common\\instruments\\trumpet.msp");

    // score
    sGlobal.mScore = new MCScore();
    sGlobal.mScore->loadScriptFromFile(".\\..\\..\\common\\scripts\\score.trumpet.sample.txt");

    // instrument settings
    sGlobal.mTrumpet->setScore(sGlobal.mScore);
    sGlobal.mTrumpet->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer(132, 4);
    mTimer.setCallbackTick(TimerTick, &sGlobal);
    mTimer.start();

    // create music timer thread
    HANDLE hMusicTimerThread = CreateThread(NULL, 0, MusicTimerThread, &mTimer, 0, NULL);

    cout << "\n  Playing! Options:\n";
    cout << "\n    M -> change mode";
    cout << "\n    ESC -> quit\n\n";

    int iKey = 0;
    bool bMode = false;
    unsigned int i;

    while(iKey != 27)
    {
        iKey = toupper(_getch());

        if(iKey == 'M')
        {
            for(i = 0; i < sGlobal.mScore->getNumberOfEntries(); i++)
                sGlobal.mScore->getEntry(i)->Note.Mode = (bMode)? 1: 0;

            bMode = !bMode;
        }

        Sleep(1);
    }

    // wait until the music timer thread finishes
    bThreadEnd = true;
    WaitForSingleObject(hMusicTimerThread, INFINITE);
    CloseHandle(hMusicTimerThread);

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
DWORD WINAPI MusicTimerThread(LPVOID lp)
{
    MCTimer* mTimer = (MCTimer*)lp;

    while(!bThreadEnd)
    {
#ifdef REDUCE_CPU_USAGE
        if(!mTimer->update())
            Sleep(1);
#else
        mTimer->update();
#endif
    }

    return 0;
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
