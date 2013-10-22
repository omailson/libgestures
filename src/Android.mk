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
    events/ainputeventhelper.cpp

LOCAL_SHARED_LIBRARIES := \
    libstlport

LOCAL_C_INCLUDES := \
    ndk/sources/cxx-stl/stlport/stlport/ \
    hydralibs/webkit-nix/Source \
    hydralibs/webkit-nix/include \
    hydralibs/webkit-nix/include/WebKit2 \
    hydralibs/webkit-nix/include/Platform \
    hydralibs/webkit-nix/include/JavaScriptCore

include $(BUILD_SHARED_LIBRARY)
