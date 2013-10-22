#ifndef JNIAPI_H
#define JNIAPI_H

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnStart(JNIEnv* jenv, jobject);
    JNIEXPORT void JNICALL Java_org_indt_gesturessample_MainActivity_nativeOnTouch(JNIEnv*, jobject, jobject);
};

#endif // JNIAPI_H
