
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
#include "Rendering/OpenGL/GERenderSystemES20.h"
#include "Audio/OpenSL/GEAudioSystemOpenSL.h"
#include "Core/GEDevice.h"
#include "States/GEState.h"
#include "Core/GETimer.h"

#include "StateSample.h"

using namespace GE;
using namespace GE::Core;
using namespace GE::Rendering;
using namespace GE::Audio;
using namespace GE::States;

RenderSystem* cRender;
AudioSystem* cAudio;
State* cStates[NUM_STATES];

Timer cTimer;
double dTime;

int iCurrentState;
int iFingerID[MAX_FINGERS];
Vector2 vFingerPosition[MAX_FINGERS];

extern "C"
{
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button);
   JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z);
};

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height)
{
   // screen size
   Device::ScreenWidth = width;
   Device::ScreenHeight = height;

   // IDs for touch management
   for(int i = 0; i < MAX_FINGERS; i++)
      iFingerID[i] = -1;
   
    // initialize rendering system
   cRender = new RenderSystemES20();
   cRender->setBackgroundColor(Color(0.5f, 0.5f, 1.0f));
   
   // initialize audio system
   cAudio = new AudioSystemOpenSL();
   cAudio->init();
   
   // create states
   cStates[0] = new GEStateSample(cRender, cAudio, (void*)0);
   // ...
   // ...
   
   // select the first state   
   iCurrentState = 0;
   cStates[0]->init();

   // start the timer
   cTimer.start();
   dTime = 0.0;
}

void selectState(unsigned int State)
{
   cStates[iCurrentState]->release();
   iCurrentState = State;
   cStates[iCurrentState]->init();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj)
{
   // delta time
   double dCurrentTime = cTimer.getTime();
   float fDeltaTime = (dCurrentTime - dTime) * 0.001f;
   dTime = dCurrentTime;

   // update
   cStates[iCurrentState]->update(fDeltaTime);
    
   // state change request
   if(cStates[iCurrentState]->getNextState() >= 0)
      selectState(cStates[iCurrentState]->getNextState());
      
   // render
   cRender->renderBegin();
   cStates[iCurrentState]->render();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == -1)
      {
         iFingerID[i] = index;
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cStates[iCurrentState]->inputTouchBegin(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         Vector2 vPreviousPosition(vFingerPosition[i]);
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cStates[iCurrentState]->inputTouchMove(i, vPreviousPosition, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         iFingerID[i] = -1;
         vFingerPosition[i].X = x;
         vFingerPosition[i].Y = y;
         cStates[iCurrentState]->inputTouchEnd(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button)
{
   
}

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button)
{
   
}

const float AccelFactor = 0.01f;

JNIEXPORT void JNICALL Java_com_GameEngine_Light_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z)
{
   if(cStates[iCurrentState])
      cStates[iCurrentState]->updateAccelerometerStatus(Vector3(x * -AccelFactor, y * -AccelFactor, z * AccelFactor));
}
