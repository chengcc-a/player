//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_VIDEOCHANNEL_H
#define NDKDEMO_VIDEOCHANNEL_H


#include "BaseChannel.h"

class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id);

     ~VideoChannel();
    void start();
    void stop();
};


#endif //NDKDEMO_VIDEOCHANNEL_H
