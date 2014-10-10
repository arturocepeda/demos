LOCAL_PATH := $(call my-dir)

BUILD_CONFIG := Debug

include $(LOCAL_PATH)/../../GameEngine.mk
include $(CLEAR_VARS)

LOCAL_MODULE            := Overview
LOCAL_C_INCLUDES        := $(LOCAL_PATH) $(LOCAL_PATH)/../../../../../GameEngine $(LOCAL_PATH)/../../../source/01_Overview
LOCAL_SRC_FILES         := Android.cpp AssetManager.cpp ../../../source/01_Overview/StateSample.cpp
LOCAL_STATIC_LIBRARIES  := GameEngine stb pugixml
LOCAL_LDLIBS            := -llog -landroid -lGLESv2 -lOpenSLES
LOCAL_CFLAGS            := -std=c++11 -DLIB
LOCAL_ARM_MODE          := arm

include $(BUILD_SHARED_LIBRARY)