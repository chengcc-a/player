//
// Created by Dell on 2020/6/30.
//

#include "VideoChannel.h"

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext) : BaseChannel(id,avCodecContext) {}

VideoChannel::~VideoChannel() {

}

void *task_video_decode(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->video_decode();
    return 0;
}

void *task_video_paly(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->video_play();
    return 0;
}

void VideoChannel::start() {
    isPlaying = 1;
    packets.setWork(1);
    pthread_create(&pid_video_decode, 0, task_video_decode, this);
    pthread_create(&pid_video_play, 0, task_video_paly, this);
}

void VideoChannel::stop() {

}

void VideoChannel::video_decode() {
    AVPacket *packet = NULL;
    while (isPlaying) {
        int ret = packets.pop(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }

        avcodec_send_packet(avCodecContext,packet);
    }
    releaseAVPacket(&packet);
}

void VideoChannel::video_play() {

}
