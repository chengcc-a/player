//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_VIDEOCHANNEL_H
#define NDKDEMO_VIDEOCHANNEL_H


#include "BaseChannel.h"

class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id, AVCodecContext *avCodecContext);

     ~VideoChannel();
    void start();
    void stop();

    void video_decode();

    void video_play();

private:
    pthread_t  pid_video_decode;
    pthread_t  pid_video_play;
};


#endif //NDKDEMO_VIDEOCHANNEL_H
