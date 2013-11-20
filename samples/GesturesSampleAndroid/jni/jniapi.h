#ifndef JNIAPI_H
#define JNIAPI_H

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnStart(JNIEnv* jenv, jobject, jobject activity);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnStop(JNIEnv* jenv, jobject);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnTouch(JNIEnv*, jobject, jobject);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeUpdateTimestamp(JNIEnv*, jobject, jlong timestamp);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeSetSwipeArea(JNIEnv*, jobject, jint, jint, jint, jint);
};

#endif // JNIAPI_H
