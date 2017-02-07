LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)

include $(LOCAL_PATH)/project/yapl.mk
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES:$(SRC_PATH)/%=src/%)

include $(BUILD_EXECUTABLE)
