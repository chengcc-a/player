//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_AUDIOCHANNEL_H
#define NDKDEMO_AUDIOCHANNEL_H


#include "BaseChannel.h"

class AudioChannel : public BaseChannel {
public:
    AudioChannel(int id);

     ~AudioChannel();

    void start();
    void stop();
};


#endif //NDKDEMO_AUDIOCHANNEL_H
