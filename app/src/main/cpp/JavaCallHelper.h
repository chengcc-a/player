//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_JAVACALLHELPER_H
#define NDKDEMO_JAVACALLHELPER_H


#include <jni.h>

class JavaCallHelper {
public:
    JavaCallHelper(JavaVM *javaVm,JNIEnv *jniEnv,jobject instance);
    ~JavaCallHelper();
    void onPrepare(int threadMode);

private:
    JavaVM* javaVm=NULL;
    JNIEnv*jniEnv=NULL;
    jobject  instance=NULL;
    jmethodID jmd_prepared;
};


#endif //NDKDEMO_JAVACALLHELPER_H
