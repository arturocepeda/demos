package com.GameEngine.Overview;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GameEngineView extends GLSurfaceView
{
	private static String TAG = "Overview";
	private static final boolean DEBUG = false;

	public GameEngineView(Context context)
	{
		super(context);
		init(false, 0, 0);
	}

	public GameEngineView(Context context, boolean translucent, int depth, int stencil)
	{
		super(context);
		init(translucent, depth, stencil);
	}

	private void init(boolean translucent, int depth, int stencil)
	{
		if (translucent)
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);

		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser(translucent 
			? new ConfigChooser(8, 8, 8, 8, depth, stencil) 
			:	new ConfigChooser(5, 6, 5, 0, depth, stencil));
		setRenderer(new Renderer());
	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
		{
			egl.eglDestroyContext(display, context);
		}
	}

	private static void checkEglError(String prompt, EGL10 egl)
	{
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
			Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
	{
		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
		{
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		private static int EGL_OPENGL_ES2_BIT = 4;
		private static int[] s_configAttribs2 =
		{
			EGL10.EGL_RED_SIZE, 4,
			EGL10.EGL_GREEN_SIZE, 4,
			EGL10.EGL_BLUE_SIZE, 4,
			EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL10.EGL_NONE
		};

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
		{
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);
			int numConfigs = num_config[0];
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			for	(EGLConfig config : configs)
			{
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);
				if (d < mDepthSize || s < mStencilSize)
					continue;

				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);
				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
					return config;
			}

			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
		{
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
				return mValue[0];

			return defaultValue;
		}

		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}

	private static class Renderer implements GLSurfaceView.Renderer
	{
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			GameEngineLib.Initialize(width, height);
		}

		public void onDrawFrame(GL10 gl)
		{
			GameEngineLib.UpdateFrame();
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {}
	}
}