###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := pugixml
LOCAL_SRC_FILES := ../../lib/$(BUILD_CONFIG)/libpugixml.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := stb
LOCAL_SRC_FILES := ../../lib/$(BUILD_CONFIG)/libstb.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := GameEngine
LOCAL_SRC_FILES := ../../lib/$(BUILD_CONFIG)/libGameEngineAndroid.a
include $(PREBUILT_STATIC_LIBRARY)

###################################################################################################