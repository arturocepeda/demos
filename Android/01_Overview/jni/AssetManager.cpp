
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Android
//
//  --- AssetManager.cpp ---
//
//////////////////////////////////////////////////////////////////

#include <android/asset_manager_jni.h>
#include "Core/GEDevice.h"

static AAssetManager* NativeAssetManager = 0;

extern "C"
{
   JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_CreateAssetManager(JNIEnv* env, jclass clazz, jobject assetManager)
   {
      NativeAssetManager = AAssetManager_fromJava(env, assetManager);
   }
};

unsigned int GEDevice::getFileLength(const char* Filename)
{
    AAsset* aAsset = AAssetManager_open(NativeAssetManager, Filename, AASSET_MODE_UNKNOWN);
    
    if(aAsset)
    {
        int iReadBytes = AAsset_getLength(aAsset);
        AAsset_close(aAsset);
        return iReadBytes;
    }

    return 0;
}

unsigned int GEDevice::readFile(const char* Filename, unsigned char* ReadBuffer, unsigned int BufferSize)
{
    AAsset* aAsset = AAssetManager_open(NativeAssetManager, Filename, AASSET_MODE_UNKNOWN);
    
    if(aAsset)
    {
        int iReadBytes = AAsset_read(aAsset, ReadBuffer, BufferSize);
        AAsset_close(aAsset);
        return iReadBytes;
    }

    return 0;
}
