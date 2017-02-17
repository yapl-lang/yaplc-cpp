SRC_PATH := $(LOCAL_PATH)/src
SRC_PATH := $(abspath $(SRC_PATH))

LOCAL_SRC_FILES_RAW := $(shell find $(SRC_PATH) -name '*.c') $(shell find $(SRC_PATH) -name '*.cpp')
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES_RAW)
LOCAL_SRC_FILES := $(filter-out $(SRC_PATH)/regex/slre/unit_test.c, $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_LDLIBS := -lstdc++

LOCAL_MODULE := yaplc

APP_CPP_COMPILER := g++
APP_CPPFLAGS += \
	-std=c++11 \
	-frtti \
	-fexceptions \
	-D'INPUT_FILE="$(shell realpath $(LOCAL_PATH)/test)"'

APP_STL := gnustl_static

# For android
APP_ABI := armeabi-v7a
APP_PLATFORM := android-14

