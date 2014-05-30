
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Android
//
//  --- Android.cpp ---
//
//////////////////////////////////////////////////////////////////

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <memory>
#include <typeinfo>

#include "config.h"
#include "Rendering/OpenGL/GERenderingES20.h"
#include "Audio/OpenSL/GEAudioOpenSL.h"
#include "Core/GEDevice.h"
#include "Scenes/GEScene.h"
#include "Core/GETimer.h"

#include "SceneSample.h"

GERendering* cRender;
GEAudio* cAudio;
GEScene* cScenes[NUM_SCENES];

GETimer cTimer;
double dTime;

int iCurrentScene;
int iFingerID[MAX_FINGERS];
GEVector2 vFingerPosition[MAX_FINGERS];

extern "C"
{
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button);
	JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button);
   JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z);
};

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height)
{
   // screen size
   GEDevice::ScreenWidth = width;
   GEDevice::ScreenHeight = height;

   __android_log_print(ANDROID_LOG_VERBOSE, "GameEngine", "Width: %d - Height: %d", width, height);

   // IDs for touch management
   for(int i = 0; i < MAX_FINGERS; i++)
      iFingerID[i] = -1;
   
    // initialize rendering system
   cRender = new GERenderingES20();
   cRender->setBackgroundColor(GEColor(0.5f, 0.5f, 1.0f));
   
   // initialize audio system
   cAudio = new GEAudioOpenSL();
   cAudio->init();
   
   // create scenes
   cScenes[0] = new GESceneSample(cRender, cAudio, (void*)0);
   // ...
   // ...
   
   // select the first scene   
   iCurrentScene = 0;
   cScenes[0]->init();
   
   // start the timer
   cTimer.start();
   dTime = 0.0;
}

void selectScene(unsigned int Scene)
{
   cScenes[iCurrentScene]->release();
   iCurrentScene = Scene;
   cScenes[iCurrentScene]->init();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj)
{
   // delta time
   double dCurrentTime = cTimer.getTime();
   float fDeltaTime = (dCurrentTime - dTime) * 0.001f;
   dTime = dCurrentTime;

   // update
   cScenes[iCurrentScene]->update(fDeltaTime);
    
   // scene change request
   if(cScenes[iCurrentScene]->getNextScene() >= 0)
      selectScene(cScenes[iCurrentScene]->getNextScene());
      
   // render
   cRender->renderBegin();
   cScenes[iCurrentScene]->render();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == -1)
      {
         iFingerID[i] = index;
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cScenes[iCurrentScene]->inputTouchBegin(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         GEVector2 vPreviousPosition(vFingerPosition[i]);
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cScenes[iCurrentScene]->inputTouchMove(i, vPreviousPosition, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         iFingerID[i] = -1;
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cScenes[iCurrentScene]->inputTouchEnd(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button)
{
	
}

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button)
{
	
}

const float AccelFactor = 0.01f;

JNIEXPORT void JNICALL Java_com_GameEngine_Overview_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z)
{
   if(cScenes[iCurrentScene])
      cScenes[iCurrentScene]->updateAccelerometerStatus(GEVector3(x * -AccelFactor, y * -AccelFactor, z * AccelFactor));
}
