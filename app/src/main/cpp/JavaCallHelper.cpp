//
// Created by Dell on 2020/6/30.
//

#include "JavaCallHelper.h"
#include "macro.h"

JavaCallHelper::JavaCallHelper(JavaVM *javaVm_, JNIEnv *jniEnv_, jobject instance_) {
    this->javaVm = javaVm_;
    this->jniEnv = jniEnv_;
    this->instance = jniEnv->NewGlobalRef(instance_);
    jclass clazz = jniEnv->GetObjectClass(instance_);
    jmd_prepared = jniEnv->GetMethodID(clazz, "onPrepared", "()V");
}

JavaCallHelper::~JavaCallHelper() {
    javaVm = NULL;
    jniEnv->DeleteGlobalRef(instance);
    instance=NULL;
}

void JavaCallHelper::onPrepare(int threadMode) {
    if (threadMode == THREAD_MAIN) {
        jniEnv->CallVoidMethod(instance, jmd_prepared);
    } else {
        JNIEnv *env_child;
        javaVm->AttachCurrentThread(&env_child, NULL);
        env_child->CallVoidMethod(instance, jmd_prepared);
        javaVm->DetachCurrentThread();
    }
}