LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libgesturessample
LOCAL_CFLAGS    := -Wall -std=gnu++11
LOCAL_SRC_FILES := jniapi.cpp
LOCAL_LDFLAGS    := -llog -landroid

LOCAL_SHARED_LIBRARIES := \
    libgestures

LOCAL_C_INCLUDES := \
    hydralibs/libgestures/src


include $(BUILD_SHARED_LIBRARY)
