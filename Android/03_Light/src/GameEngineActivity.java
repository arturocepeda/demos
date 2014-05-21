package com.GameEngine.Light;

import android.util.Log;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.view.View.OnTouchListener;
import android.content.res.AssetManager;
import java.io.File;

public class GameEngineActivity extends Activity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		CreateMainView();
		CreateAssetManager();
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

  @Override
	protected void onPause()
	{
		super.onPause();
    view.onPause();
  }

  @Override
	protected void onResume()
	{
    super.onResume();
    view.onResume();
  }

	@Override
	public boolean dispatchTouchEvent(MotionEvent event)
	{
		switch (event.getAction())
		{
			case MotionEvent.ACTION_DOWN:
            for(int i = 0; i < event.getPointerCount(); i++)
				   GameEngineLib.InputTouchDown(event.getPointerId(i), event.getX(i), event.getY(i));
				break;
			case MotionEvent.ACTION_MOVE:
            for(int i = 0; i < event.getPointerCount(); i++)
				   GameEngineLib.InputTouchMove(event.getPointerId(i), event.getX(i), event.getY(i));
				break;
			case MotionEvent.ACTION_UP:
            for(int i = 0; i < event.getPointerCount(); i++)
				   GameEngineLib.InputTouchUp(event.getPointerId(i), event.getX(i), event.getY(i));
				break;
		}

		return super.dispatchTouchEvent(event);
	}

	@Override
	public boolean onKeyDown(final int keyCode, KeyEvent event)
	{
		if (!keyPressed[keyCode])
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
		if (keyPressed[keyCode])
		{
			keyPressed[keyCode] = false;
			GameEngineLib.InputButtonUp(keyCode);
		}

		return super.onKeyUp(keyCode, event);
	}

	@Override
	public boolean dispatchGenericMotionEvent(MotionEvent event)
	{
		return super.dispatchGenericMotionEvent(event);
	}

	public GameEngineActivity()
	{
		GameEngineLib.LoadSharedLibraries();		
	}	
}