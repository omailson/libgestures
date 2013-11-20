LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libgestures
LOCAL_CFLAGS    := -Wall -std=gnu++11
LOCAL_LDFLAGS   := -llog -landroid
LOCAL_SRC_FILES := \
    gesture.cpp \
    gesturemanager.cpp \
    gesturerecognizer.cpp \
    gestures/pan.cpp \
    gestures/tap.cpp \
    gestures/pinch.cpp \
    gestures/doubletap.cpp \
    gestures/longpress.cpp \
    gestures/swipe.cpp \
    events/ainputeventhelper.cpp \
    events/gestureeventfilter.cpp

LOCAL_SHARED_LIBRARIES := \
    libstlport

LOCAL_C_INCLUDES := \
    ndk/sources/cxx-stl/stlport/stlport/

include $(BUILD_SHARED_LIBRARY)
