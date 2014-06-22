###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := pugixml
LOCAL_SRC_FILES := ../../lib/Release/libpugixml.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := stb
LOCAL_SRC_FILES := ../../lib/Release/libstb.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := GameEngine
LOCAL_SRC_FILES := ../../lib/Release/libGameEngineAndroid.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################