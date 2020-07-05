//
// Created by Dell on 2020/6/30.
//

#ifndef NDKDEMO_BASECHANNEL_H
#define NDKDEMO_BASECHANNEL_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
};

#include "safe_queue.h"


class BaseChannel {
public:
    BaseChannel(int id) : id(id) {
        packets.setReleaseCallback(releaseAVPacket);
        frames.setReleaseCallback(releaseAVFrame);
    }

    virtual ~BaseChannel() {
        packets.clear();
        frames.clear();
    }

    /**
     * 释放 AVPacket
     * @param packet
     */
    static void releaseAVPacket(AVPacket **packet) {
        if (packet) {
            av_packet_free(packet);
            *packet = 0;
        }
    }

    /**
     * 释放 AVFrame
     * @param frame
     */
    static void releaseAVFrame(AVFrame **frame) {
        if (frame) {
            av_frame_free(frame);
            *frame = 0;
        }
    }

    //纯虚函数（抽象方法）
    virtual void start() = 0;

    virtual void stop() = 0;


    SafeQueue<AVPacket *> packets;
    SafeQueue<AVFrame *> frames;
    int id;

};
#endif //NDKDEMO_BASECHANNEL_H
