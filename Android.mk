LIBGESTURES_PATH := $(call my-dir)
LOCAL_PATH := $(LIBGESTURES_PATH)

include $(LOCAL_PATH)/src/Android.mk

ifeq ($(BUILD_LIBGESTURES_SAMPLES), 1)
include $(LIBGESTURES_PATH)/samples/Android.mk
endif
