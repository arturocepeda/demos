
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Windows Phone 8
//
//  --- AppMain.cpp ---
//
//////////////////////////////////////////////////////////////////


#include "AppMain.h"
#include <ppltasks.h>

#include "Core/GEDevice.h"

#include "StateSample.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Sensors;

using namespace GE::Core;
using namespace GE::Rendering;
using namespace GE::Audio;

AppMain::AppMain()
   : cRender(nullptr)
   , cAudio(nullptr)
   , bWindowClosed(false)
   , bWindowVisible(true)
{
   for(int i = 0; i < MAX_FINGERS; i++)
      iFingerID[i] = -1;
}

void AppMain::Initialize(CoreApplicationView^ applicationView)
{
   applicationView->Activated +=
      ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &AppMain::OnActivated);
   CoreApplication::Suspending +=
      ref new EventHandler<SuspendingEventArgs^>(this, &AppMain::OnSuspending);
   CoreApplication::Resuming +=
      ref new EventHandler<Platform::Object^>(this, &AppMain::OnResuming);
}

int convertDipsToPixels(float dips)
{
   static const float dipsPerInch = 96.0f;
   return (int)floor(dips * Windows::Graphics::Display::DisplayProperties::LogicalDpi / dipsPerInch + 0.5f);
}

void AppMain::SetWindow(CoreWindow^ window)
{
   // register callbacks
   window->VisibilityChanged +=
      ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &AppMain::OnVisibilityChanged);
   window->Closed += 
      ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &AppMain::OnWindowClosed);
   window->PointerPressed +=
      ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &AppMain::OnPointerPressed);
   window->PointerMoved +=
      ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &AppMain::OnPointerMoved);
   window->PointerReleased +=
      ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &AppMain::OnPointerReleased);

   // orientation settings
   DisplayProperties::AutoRotationPreferences = Windows::Graphics::Display::DisplayOrientations::None
#ifdef PORTRAIT_UP
      | Windows::Graphics::Display::DisplayOrientations::Portrait
#endif
#ifdef LANDSCAPE_HOME_LEFT
      | Windows::Graphics::Display::DisplayOrientations::Landscape
#endif
#ifdef LANDSCAPE_HOME_RIGHT
      | Windows::Graphics::Display::DisplayOrientations::LandscapeFlipped
#endif
      ;

   // set screen size
   Device::ScreenWidth = convertDipsToPixels(window->Bounds.Width);
   Device::ScreenHeight = convertDipsToPixels(window->Bounds.Height);

   // create render system
   cRender = new RenderSystemDX11(CoreWindow::GetForCurrentThread());
   cRender->setBackgroundColor(GE::Color(0.5f, 0.5f, 1.0f));

   // create audio system
   cAudio = new AudioSystemXAudio2();
   cAudio->init();

   // create states
   cStates[0] = new GEStateSample(cRender, cAudio, (void*)0);
   // ...
   // ...

   // select the first state   
   iCurrentState = 0;
   cStates[0]->init();

   // initialize accelerometer
   try
   {
      wpAccelerometer = Accelerometer::GetDefault();
   }
   catch(Platform::Exception^ e)
   {
   }

   if(wpAccelerometer)
   {
      wpAccelerometer->ReadingChanged +=
         ref new TypedEventHandler<Accelerometer^, AccelerometerReadingChangedEventArgs^>(this, &AppMain::OnAccelerometerReading);
   }
}

void AppMain::Load(Platform::String^ entryPoint)
{
}

void AppMain::Run()
{
   cTimer.start();

   double dCurrentTime = cTimer.getTime();
   double dPreviousTime = dCurrentTime;
   double dDeltaTime = 0.0;
   double dTotalTime = 0.0;

   while(!bWindowClosed)
   {
      if(bWindowVisible)
      {
         CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

         dCurrentTime = cTimer.getTime();
         dDeltaTime = dCurrentTime - dPreviousTime;
         dTotalTime += dDeltaTime;
         dPreviousTime = dCurrentTime;
    
         // state change request
         if(cStates[iCurrentState]->getNextState() >= 0)
         {
            cStates[iCurrentState]->release();
            iCurrentState = cStates[iCurrentState]->getNextState();
            cStates[iCurrentState]->init();
         }

         // state update
         cStates[iCurrentState]->update((float)dDeltaTime * 0.001f);

         // render
         cRender->renderBegin();
         cStates[iCurrentState]->render();
         cRender->renderEnd();
      }
      else
      {
         CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
      }
   }
}

void AppMain::Uninitialize()
{
}

void AppMain::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
   bWindowVisible = args->Visible;
}

void AppMain::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
   bWindowClosed = true;
}

void AppMain::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == -1)
      {
         iFingerID[i] = (int)args->CurrentPoint->PointerId;
         vFingerPosition[i].X = args->CurrentPoint->Position.X;
         vFingerPosition[i].Y = args->CurrentPoint->Position.Y;
         cStates[iCurrentState]->inputTouchBegin(i, vFingerPosition[i]);
         break;
      }
   }
}

void AppMain::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == (int)args->CurrentPoint->PointerId)
      {
         GE::Vector2 vPreviousPosition(vFingerPosition[i]);
         vFingerPosition[i].X = args->CurrentPoint->Position.X;
         vFingerPosition[i].Y = args->CurrentPoint->Position.Y;
         cStates[iCurrentState]->inputTouchMove(i, vPreviousPosition, vFingerPosition[i]);
         break;
      }
   }
}

void AppMain::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
   for(int i = 0; i < MAX_FINGERS; i++)
   {
      if(iFingerID[i] == (int)args->CurrentPoint->PointerId)
      {
         iFingerID[i] = -1;
         vFingerPosition[i].X = args->CurrentPoint->Position.X;
         vFingerPosition[i].Y = args->CurrentPoint->Position.Y;
         cStates[iCurrentState]->inputTouchEnd(i, vFingerPosition[i]);
         break;
      }
   }
}

void AppMain::OnAccelerometerReading(Accelerometer^ accelerometer, AccelerometerReadingChangedEventArgs^ args)
{
   cStates[iCurrentState]->updateAccelerometerStatus(
      GE::Vector3((float)args->Reading->AccelerationX, (float)args->Reading->AccelerationY, (float)args->Reading->AccelerationZ));
}

void AppMain::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
   CoreWindow::GetForCurrentThread()->Activate();
}

void AppMain::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
   SuspendingDeferral^ rcDeferral = args->SuspendingOperation->GetDeferral();
   cRender->releaseResourcesForSuspending();

   Concurrency::create_task([this, rcDeferral]()
   {
      rcDeferral->Complete();
   });
}
 
void AppMain::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
   cRender->createWindowSizeDependentResources();
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
   return ref new AppMain();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
   auto dxApplicationSource = ref new Direct3DApplicationSource();
   CoreApplication::Run(dxApplicationSource);
   return 0;
}
