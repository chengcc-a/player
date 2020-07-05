//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_NEFFMPEG_H
#define NDKDEMO_NEFFMPEG_H


#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"
#include <cstring>
#include <pthread.h>
#include "macro.h"
extern "C" {
#include <libavformat/avformat.h>
}
class NEFFmpeg {
public:

    NEFFmpeg(JavaCallHelper *javaCallHelper, char *dataSource);

    ~NEFFmpeg();


    void prepare();

    void _prepare();


    void start();

    void _start();

private:
    JavaCallHelper *javaCallHelper = NULL;
    AudioChannel *audioChannel = NULL;
    VideoChannel *videoChannel = NULL;
    char *dataSource = NULL;
    pthread_t pid_prepare = NULL;
    pthread_t pid_start = NULL;
    bool isPlaying;
    AVFormatContext *formatContext = NULL;


};


#endif //NDKDEMO_NEFFMPEG_H
