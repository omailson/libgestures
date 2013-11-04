LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME := GesturesSample

LOCAL_PROGUARD_FLAG_FILES := proguard.flags

# If this is an unbundled build (to install seprately) then include
# the libraries in the APK, otherwise just put them in /system/lib and
# leave them out of the APK
ifneq (,$(TARGET_BUILD_APPS))
  LOCAL_JNI_SHARED_LIBRARIES := libgesturessample
else
  LOCAL_REQUIRED_LIBRARIES := libgesturessample
endif

include $(BUILD_PACKAGE)

include $(LOCAL_PATH)/jni/Android.mk
