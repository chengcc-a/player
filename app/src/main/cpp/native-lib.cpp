#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "NEFFmpeg.h"

extern "C" {
#include "include/libavutil/avutil.h"
}

NEFFmpeg *nefFmpeg;
extern "C" JNIEXPORT jstring JNICALL
Java_com_chengjs_ndkdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
    return env->NewStringUTF(av_version_info());
}
JavaVM *_vm;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    _vm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_chengjs_ndkdemo_NEPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, 0);

    JavaCallHelper *javaCallHelper = new JavaCallHelper(_vm,env, thiz);

     nefFmpeg = new NEFFmpeg(javaCallHelper, const_cast<char *>(dataSource));
    nefFmpeg->prepare();
    env->ReleaseStringUTFChars(data_source, dataSource);
}extern "C"
JNIEXPORT void JNICALL
Java_com_chengjs_ndkdemo_NEPlayer_startNative(JNIEnv *env, jobject thiz) {
    if(nefFmpeg){
        nefFmpeg->start();
    }
}