
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Windows Phone 8
//
//  --- AppMain.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/DX11/GERenderSystemDX11.h"
#include "Audio/XAudio2/GEAudioSystemXAudio2.h"
#include "States/GEState.h"
#include "Core/GETimer.h"

#include "config.h"

ref class AppMain sealed : public Windows::ApplicationModel::Core::IFrameworkView
{
public:
   AppMain();
   
   virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
   virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
   virtual void Load(Platform::String^ entryPoint);
   virtual void Run();
   virtual void Uninitialize();

protected:
   void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
   void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
   void OnResuming(Platform::Object^ sender, Platform::Object^ args);
   void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
   void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
   void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
   void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
   void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
   void OnAccelerometerReading(Windows::Devices::Sensors::Accelerometer^ accelerometer,
      Windows::Devices::Sensors::AccelerometerReadingChangedEventArgs^ args);

private:
   GE::Rendering::RenderSystemDX11* cRender;
   GE::Audio::AudioSystemXAudio2* cAudio;
   GE::States::State* cStates[NUM_STATES];
   GE::Core::Timer cTimer;

   bool bWindowClosed;
   bool bWindowVisible;

   int iCurrentState;

   int iFingerID[MAX_FINGERS];
   GE::Vector2 vFingerPosition[MAX_FINGERS];

   Windows::Devices::Sensors::Accelerometer^ wpAccelerometer;
};

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
   virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
