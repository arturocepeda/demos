
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
#include <stdio.h>
#include <memory>
#include <typeinfo>
#include <vector>

#include <cpu-features.h>

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
std::vector<State*> cStates;
bool bInitialized = false;

Timer cTimer;
double dTime;

int iCurrentState;
int iFingerID[GE_MAX_FINGERS];
Vector2 vFingerPosition[GE_MAX_FINGERS];

Line* cPixelToScreenX;
Line* cPixelToScreenY;

extern "C"
{
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Pause(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Resume(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button);
   JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z);
};

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Initialize(JNIEnv* env, jobject obj, jint width, jint height)
{
   if(bInitialized)
      return;

   // screen size
   Device::ScreenWidth = width;
   Device::ScreenHeight = height;

   // device orientation
#ifdef GE_ORIENTATION_PORTRAIT
   Device::Orientation = DOPortrait;
#else
   Device::Orientation = DOLandscape;
#endif

   // IDs for touch management
   for(int i = 0; i < GE_MAX_FINGERS; i++)
      iFingerID[i] = -1;

   cPixelToScreenX = new Line(0.0f, -1.0f, Device::ScreenWidth, 1.0f);
   cPixelToScreenY = new Line(0.0f, Device::getAspectRatio(), Device::ScreenHeight, -Device::getAspectRatio());
   
    // initialize rendering system
   cRender = new RenderSystemES20();
   cRender->setBackgroundColor(Color(0.5f, 0.5f, 1.0f));
   
   // initialize audio system
   cAudio = new AudioSystemOpenSL();
   cAudio->init();
   
   // create states
   cStates.push_back(new GEStateSample(cRender, cAudio, (void*)0));
   // ...
   // ...
   
   // select the first state   
   iCurrentState = 0;
   cStates[0]->init();
   
   // start the timer
   cTimer.start();
   dTime = 0.0;

   // set the initialized flag
   bInitialized = true;
}

void selectState(unsigned int State)
{
   cStates[iCurrentState]->release();
   iCurrentState = State;
   cStates[iCurrentState]->init();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_UpdateFrame(JNIEnv* env, jobject obj)
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

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Pause(JNIEnv* env, jobject obj)
{
   cTimer.stop();
   cStates[iCurrentState]->pause();
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_Resume(JNIEnv* env, jobject obj)
{
   if(!bInitialized)
      return;

   cTimer.start();
   cStates[iCurrentState]->resume();
}

GE::Vector2 pixelToScreen(const GE::Vector2& vPixelPosition)
{
   return GE::Vector2((float)cPixelToScreenX->y(vPixelPosition.X), (float)cPixelToScreenY->y(vPixelPosition.Y));
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchDown(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < GE_MAX_FINGERS; i++)
   {
      if(iFingerID[i] == -1)
      {
         iFingerID[i] = index;
         vFingerPosition[i] = pixelToScreen(Vector2(x, y));
         cStates[iCurrentState]->inputTouchBegin(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchMove(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < GE_MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         Vector2 vPreviousPosition = vFingerPosition[i];
         vFingerPosition[i] = pixelToScreen(Vector2(x, y));
         cStates[iCurrentState]->inputTouchMove(i, vPreviousPosition, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputTouchUp(JNIEnv* env, jclass clazz, jint index, jfloat x, jfloat y)
{
   for(int i = 0; i < GE_MAX_FINGERS; i++)
   {
      if(iFingerID[i] == index)
      {
         iFingerID[i] = -1;
         vFingerPosition[i] = pixelToScreen(Vector2(x, y));
         cStates[iCurrentState]->inputTouchEnd(i, vFingerPosition[i]);
         break;
      }
   }
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputButtonDown(JNIEnv* env, jclass clazz, jint button)
{
   
}

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_InputButtonUp(JNIEnv* env, jclass clazz, jint button)
{
   
}

const float AccelFactor = 0.01f;

JNIEXPORT void JNICALL Java_com_GameEngine_Bugs_GameEngineLib_UpdateAccelerometerStatus(JNIEnv* env, jclass clazz, jfloat x, jfloat y, jfloat z)
{
   if(cStates.size() > iCurrentState && cStates[iCurrentState])
      cStates[iCurrentState]->updateAccelerometerStatus(Vector3(x * -AccelFactor, y * -AccelFactor, z * AccelFactor));
}

int Device::getNumberOfCPUCores()
{
   return android_getCpuCount();
}
