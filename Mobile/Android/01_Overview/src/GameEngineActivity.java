
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda
//  Game Engine
//
//  Android
//
//  --- GameEngineActivity.java ---
//
//////////////////////////////////////////////////////////////////

package com.GameEngine.Overview;

import android.util.Log;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.view.View.OnTouchListener;
import android.content.res.AssetManager;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import java.io.File;

public class GameEngineActivity extends Activity implements SensorEventListener
{
   private static final boolean UseAccelerometer = true;

   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      CreateMainView();
      CreateAssetManager();

      if(UseAccelerometer)
         InitializeAccelerometer();
   }

   private void CreateMainView()
   {
      view = new GameEngineView(getApplication());
      setContentView(view);
   }

   private void CreateAssetManager()
   {
      assetManager = getAssets();			
      GameEngineLib.CreateAssetManager(assetManager);
   }

   static AssetManager assetManager;
   GameEngineView view;
   
   private void InitializeAccelerometer()
   {
      mSensorManager = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
      mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
      mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_FASTEST);
   }
   
   private SensorManager mSensorManager;
   private Sensor mAccelerometer;

   @Override
   protected void onPause()
   {
      super.onPause();

      if(UseAccelerometer)
         mSensorManager.unregisterListener(this);

      GameEngineLib.Pause();
   }

   @Override
   protected void onResume()
   {
      super.onResume();

      if(UseAccelerometer)
         mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_FASTEST);

      GameEngineLib.Resume();
   }

   @Override
   public boolean onKeyDown(final int keyCode, KeyEvent event)
   {
      if(!keyPressed[keyCode])
      {
         keyPressed[keyCode] = true;
         GameEngineLib.InputButtonDown(keyCode);
      }

      return super.onKeyDown(keyCode, event);
   }

   private boolean keyPressed[] = new boolean[256];

   @Override
   public boolean onKeyUp(final int keyCode, KeyEvent event)
   {
      if(keyPressed[keyCode])
      {
         keyPressed[keyCode] = false;
         GameEngineLib.InputButtonUp(keyCode);
      }

      return super.onKeyUp(keyCode, event);
   }

   @Override
   public void onAccuracyChanged(Sensor sensor, int accuracy) 
   {
   }

   @Override
   public final void onSensorChanged(SensorEvent event)
   {
      GameEngineLib.UpdateAccelerometerStatus(event.values[0], event.values[1], event.values[2]);
   }

   public GameEngineActivity()
   {
      GameEngineLib.LoadSharedLibraries();		
   }	
}
