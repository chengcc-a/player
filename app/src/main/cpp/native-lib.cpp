#include <jni.h>
#include <string>
extern "C"{
#include "include/libavutil/avutil.h"
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_chengjs_ndkdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
    return env->NewStringUTF(av_version_info());
}
