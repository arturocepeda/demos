
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
#endif

using namespace std;

bool bThreadEnd = false;

DWORD WINAPI MusicTimerThread(LPVOID lp);
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
    mSoundGen->loadSamplePack(".\\..\\..\\common\\instruments\\doublebass.msp");

    // double bass settings
    mDoubleBass->setSoundGen(mSoundGen);

    // timer
    MCTimer mTimer;
    mTimer.setCallbackTick(TimerTick, mDoubleBass);
    mTimer.start();

    // create music timer thread
    HANDLE hMusicTimerThread = CreateThread(NULL, 0, MusicTimerThread, &mTimer, 0, NULL);
    SetThreadPriority(hMusicTimerThread, THREAD_PRIORITY_IDLE);

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
    WaitForSingleObject(hMusicTimerThread, INFINITE);
    CloseHandle(hMusicTimerThread);

    delete mDoubleBass;
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
    MCInstrument* mDoubleBass = (MCInstrument*)pData;
    mDoubleBass->update(mTimePosition);
}
