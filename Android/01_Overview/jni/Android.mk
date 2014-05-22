LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/../../GameEnginePrebuiltDebug.mk
include $(CLEAR_VARS)

LOCAL_MODULE            := Overview
LOCAL_C_INCLUDES        := $(LOCAL_PATH) $(LOCAL_PATH)/../../../../GameEngine $(LOCAL_PATH)/../../../../GameEngine/Core/Android
LOCAL_SRC_FILES         := Android.cpp AssetManager.cpp SceneSample.cpp
LOCAL_STATIC_LIBRARIES  := GameEngine stb
LOCAL_LDLIBS			   := -llog -landroid -lGLESv2 -lOpenSLES
LOCAL_CFLAGS			   := -std=c++11 -DLIB
LOCAL_ARM_MODE			   := arm

include $(BUILD_SHARED_LIBRARY)