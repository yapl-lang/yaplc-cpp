LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC_PATH := $(LOCAL_PATH)/../src
SRC_PATH := $(realpath $(SRC_PATH))

LOCAL_SRC_FILES_RAW := $(shell find $(SRC_PATH) -name '*.c') $(shell find $(SRC_PATH) -name '*.cpp')
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES_RAW:$(SRC_PATH)/%=../src/%)
LOCAL_SRC_FILES := $(filter-out ../src/regex/slre/unit_test.c, $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src
LOCAL_LDLIBS := -lstdc++

LOCAL_MODULE := yaplc

include $(BUILD_EXECUTABLE)
