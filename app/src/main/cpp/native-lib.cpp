#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "NEFFmpeg.h"
#include <android/native_window_jni.h>

#include <unistd.h>

extern "C" {
#include "include/libavutil/avutil.h"
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
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

    JavaCallHelper *javaCallHelper = new JavaCallHelper(_vm, env, thiz);

    nefFmpeg = new NEFFmpeg(javaCallHelper, const_cast<char *>(dataSource));
    nefFmpeg->prepare();
    env->ReleaseStringUTFChars(data_source, dataSource);
}extern "C"
JNIEXPORT void JNICALL
Java_com_chengjs_ndkdemo_NEPlayer_startNative(JNIEnv *env, jobject thiz) {
    if (nefFmpeg) {
        nefFmpeg->start();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_chengjs_ndkdemo_MyPlayer_native_1start(JNIEnv *env, jobject thiz, jstring url,
                                                jobject surface) {
    const char *path = env->GetStringUTFChars(url, 0);
    avformat_network_init();
    AVFormatContext *avFormatContext = avformat_alloc_context();
    AVDictionary *dictionary = NULL;
    av_dict_set(&dictionary, "timeout", "3000000", 0);
    int ret = avformat_open_input(&avFormatContext, path, 0, &dictionary);
    if (ret) {
        return;
    }
    avformat_find_stream_info(avFormatContext, NULL);
    int video_stream_idx = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_idx = i;
            break;
        }
    }
    AVCodecParameters *codecpar = avFormatContext->streams[video_stream_idx]->codecpar;
    AVCodec *avCodec = avcodec_find_decoder(codecpar->codec_id);
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext, codecpar);
    avcodec_open2(avCodecContext, avCodec, NULL);
    AVPacket *packet = av_packet_alloc();
    ANativeWindow *aNativeWindow = ANativeWindow_fromSurface(env, surface);
    //    视频缓冲区
    ANativeWindow_Buffer outBuffer;
    //创建新的窗口用于视频显示
//    ANativeWindow
    int frameCount = 0;
    ANativeWindow_setBuffersGeometry(aNativeWindow, avCodecContext->width,
                                     avCodecContext->height,
                                     WINDOW_FORMAT_RGBA_8888);
    SwsContext *sws_ctx = sws_getContext(
            avCodecContext->width, avCodecContext->height, avCodecContext->pix_fmt,
            avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA,
            SWS_BILINEAR, 0, 0, 0);
    while (av_read_frame(avFormatContext, packet) == 0) {
        avcodec_send_packet(avCodecContext, packet);
        AVFrame *avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            break;
        }
        uint8_t *dst_data[4];
        int dst_linesize[4];
        av_image_alloc(dst_data, dst_linesize, avCodecContext->width, avCodecContext->height,
                       AV_PIX_FMT_RGBA, 1);

        if (video_stream_idx == packet->stream_index) {
            if (ret == 0) {
                ANativeWindow_lock(aNativeWindow, &outBuffer, NULL);

                sws_scale(sws_ctx,  (avFrame->data),avFrame->linesize,0,avFrame->height,dst_data,dst_linesize);
                uint8_t *dst= (uint8_t *) outBuffer.bits;
//            拿到一行有多少个字节 RGBA
                int destStride=outBuffer.stride*4;
                uint8_t *src_data = dst_data[0];
                int src_linesize = dst_linesize[0];
                uint8_t *firstWindown = static_cast<uint8_t *>(outBuffer.bits);
                for (int i = 0; i < outBuffer.height; ++i) {
                    memcpy(firstWindown + i * destStride, src_data + i * src_linesize, destStride);
                }
                ANativeWindow_unlockAndPost(aNativeWindow);
                usleep(1000 * 16);
                av_frame_free(&avFrame);
            }
        }
    }

    ANativeWindow_release(aNativeWindow);
    avcodec_close(avCodecContext);
    avformat_free_context(avFormatContext);
    env->ReleaseStringUTFChars(url, path);
}