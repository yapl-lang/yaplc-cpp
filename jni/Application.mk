APP_ABI := armeabi-v7a
APP_PLATFORM := android-14
APP_CPPFLAGS += \
	-std=c++11 \
	-frtti \
	-fexceptions \
	-D'INPUT_FILE="$(realpath $(call my-dir)/../test.ypl)"'
APP_STL := gnustl_static
