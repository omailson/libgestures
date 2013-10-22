#include <stdint.h>
#include <jni.h>
#include <android/input.h>

#include "jniapi.h"
#include "logger.h"

#define LOG_TAG "GesturesSample-JNI"

static JavaVM *jvm = 0;
static jclass motionEventClass;

JNIEnv* getEnv()
{
    LOG_INFO("getEnv()");

    JNIEnv *env = 0;
    jint rs = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (rs == JNI_EDETACHED) {
        LOG_INFO("EDETACHED");
        if (jvm->AttachCurrentThread(&env, NULL) < 0) {
            LOG_INFO("Failed to get the environment using AttachCurrentThread()");
        }
    } else if (rs == JNI_EVERSION) {
        LOG_INFO("Failed to get the environment. EVERSION error");
    }

    return env;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*)
{
    LOG_INFO("JNI_OnLoad");

    jvm = vm;
    JNIEnv *env = getEnv();

    jclass clazz = env->FindClass("android/view/MotionEvent");
    motionEventClass = reinterpret_cast<jclass>(env->NewGlobalRef(clazz));

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnStart(JNIEnv* jenv, jobject)
{
    LOG_INFO("nativeOnStart");
    return;
}

JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnTouch(JNIEnv *env, jobject, jobject motionEvent)
{
    static jfieldID field = env->GetFieldID(motionEventClass, "mNativePtr", "I");
    jint nativeEvent = env->GetIntField(motionEvent, field);

    const AInputEvent* ev = (AInputEvent*)nativeEvent;
    if (!ev)
        return;

    if (AInputEvent_getType(ev) != AINPUT_EVENT_TYPE_MOTION)
        return;

    for (unsigned int i = 0; i < AMotionEvent_getPointerCount(ev); ++i) {
        LOG_INFO("id: %d; x: %f; y: %f", AMotionEvent_getPointerId(ev, i), AMotionEvent_getX(ev, i), AMotionEvent_getY(ev, i));
    }
}
