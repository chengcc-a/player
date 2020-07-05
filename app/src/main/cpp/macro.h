//
// Created by Dell on 2020/6/30.
//
#include <android/log.h>
#ifndef NDKDEMO_MACRO_H
#define NDKDEMO_MACRO_H
#define DELETE(object) if(object){delete object;object=NULL;}
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"NEFFMPEG",__VA_ARGS__)

#define THREAD_MAIN 1
#define THREAD_CHILD 2
#endif //NDKDEMO_MACRO_H
