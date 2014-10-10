
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda
//  Game Engine
//
//  Android
//
//  --- GameEngineLib.java ---
//
//////////////////////////////////////////////////////////////////

package com.GameEngine.Light;

import android.content.res.AssetManager;

public class GameEngineLib
{
   public static void LoadSharedLibraries()
   {
      System.loadLibrary("Light");
   }
      
   public static native void Initialize(int width, int height);
   public static native void UpdateFrame();
    
   public static native void CreateAssetManager(AssetManager assetManager);

   public static native void InputTouchDown(int index, float x, float y);
   public static native void InputTouchMove(int index, float x, float y);
   public static native void InputTouchUp(int index, float x, float y);
   public static native void InputButtonDown(int button);
   public static native void InputButtonUp(int button);
   public static native void UpdateAccelerometerStatus(float x, float y, float z);
}
