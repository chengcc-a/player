//
// Created by Dell on 2020/6/30.



//
#include "NEFFmpeg.h"


NEFFmpeg::NEFFmpeg(JavaCallHelper *javaCallHelper, char *dataSource) {
    this->javaCallHelper = javaCallHelper;
    this->dataSource = new char[strlen(dataSource) + 1];
    strcpy(this->dataSource, dataSource);
}

NEFFmpeg::~NEFFmpeg() {
    DELETE(dataSource);
    DELETE(javaCallHelper);
}

void *task_prepare(void *arg) {
    NEFFmpeg *ffmpeg = static_cast<NEFFmpeg *>(arg);

    ffmpeg->_prepare();

    /*必须要传return 0  不然会出问题？？*/
    return 0;
}

void *task_start(void *arg) {
    NEFFmpeg *ffmpeg = static_cast<NEFFmpeg *>(arg);

    ffmpeg->_start();

    /*必须要传return 0  不然会出问题？？*/
    return 0;
}

void NEFFmpeg::_prepare() {
    formatContext = avformat_alloc_context();
    AVDictionary *dictionary = NULL;
    av_dict_set(&dictionary, "timeout", "10000000", 0);

    int ret = avformat_open_input(&formatContext, dataSource, 0, &dictionary);
    av_dict_free(&dictionary);

    if (ret) {
        LOGE("打开媒体失败", av_err2str(ret));
        if (javaCallHelper) {
//    javaCallHelper->on
        }
    } else {}
    ret = avformat_find_stream_info(formatContext, NULL);
    if (ret < 0) {
        return;
    }
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        AVStream *stream = formatContext->streams[i];
        AVCodecParameters *codecParameters = stream->codecpar;
        AVCodec *avCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (!avCodec) {
            return;
        }
        AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
        ret = avcodec_parameters_to_context(avCodecContext, codecParameters);
        if (ret < 0) {
            return;
        }
        ret = avcodec_open2(avCodecContext, avCodec, NULL);
        if (ret) {

            return;
        }

        if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoChannel = new VideoChannel(i, avCodecContext);
        } else if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioChannel = new AudioChannel(i, avCodecContext);
        }

        if (!audioChannel && !videoChannel) {
            return;
        }
        if (javaCallHelper) {
            javaCallHelper->onPrepare(THREAD_CHILD);
        }
    }

}

void NEFFmpeg::prepare() {
    pthread_create(&pid_prepare, 0, task_prepare, this);
}

void NEFFmpeg::start() {
    isPlaying = true;
    videoChannel->start();
    pthread_create(&pid_start, 0, task_start, this);
}

void NEFFmpeg::_start() {
    while (isPlaying) {
        AVPacket *avPacket = av_packet_alloc();
        int ret = av_read_frame(formatContext, avPacket);
        if (!ret) {
            if (videoChannel && avPacket->stream_index == videoChannel->id) {
                videoChannel->packets.push(avPacket);
            } else if (audioChannel && avPacket->stream_index == audioChannel->id) {}
        } else if (ret == AVERROR_EOF) {}
        else {
            LOGE("读取数据包出错");
            break;
        }
    }
    isPlaying = 0;
}



