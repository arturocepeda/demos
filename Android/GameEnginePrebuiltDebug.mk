###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := pugixml
LOCAL_SRC_FILES := ../../lib/Debug/libpugixml.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := stb
LOCAL_SRC_FILES := ../../lib/Debug/libstb.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := GameEngine
LOCAL_SRC_FILES := ../../lib/Debug/libGameEngineAndroid.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################