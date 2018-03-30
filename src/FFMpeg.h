#ifndef _FFMPEG_H_
#define _FFMPEG_H_

// reference from 
// http://blog.chinaunix.net/uid-15063109-id-4482932.html

#include <functional>	// for std::function
#include <queue>
#include <pthread.h>

extern "C" {
	// disable warnings about badly formed documentation from ffmpeg, which don't need at all
#pragma warning(disable:4635)
	// disable warning about conversion int64 to int32
#pragma warning(disable:4244)
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "swscale.lib")
}

/* Fallback support for older libavcodec versions */
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(54, 59, 100)
#define AV_CODEC_ID_H264 CODEC_ID_H264
#endif

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(56, 34, 2)
#define AV_CODEC_FLAG_LOOP_FILTER CODEC_FLAG_LOOP_FILTER
#define AV_CODEC_CAP_TRUNCATED CODEC_CAP_TRUNCATED
#define AV_CODEC_FLAG_TRUNCATED CODEC_FLAG_TRUNCATED
#endif

#if LIBAVUTIL_VERSION_MAJOR < 52
#define AV_PIX_FMT_YUV420P PIX_FMT_YUV420P
#endif

class Frame;
class Listener
{
private:

};

class DecodeListener : public Listener
{
public:
	virtual void onDecoded(void* frame) = 0;
};

class EncodeListener : public Listener
{
public:
	virtual void onEncoded() = 0;
};

class FFMpeg
{
public:
	FFMpeg();
	virtual ~FFMpeg();
	
	virtual int intialize();
	virtual int finalize();

	//virtual int decode(uint8_t* input, int nLen, bool bWaitIFrame = false) = 0;

	void setOnDecodedCallbackFunction(std::function<void(void*)> func);
	void setOnEncodedCallbackFunction(std::function<void()> func);

	void setListener(Listener* listener) { m_plistener = listener; }

	void setDstWidth(int width) { dstWidth = width; }
	void setDstHeight(int height) { dstHeight = height; }
	void setRescaleSize(int width, int height) { dstWidth = width; dstHeight = height; }
	int getWidth() { return dstWidth; }
	int getHeight() { return dstHeight; }

	void setSWSType(int type) { sws_flags = type; }
	int getSWSType() { return sws_flags; }

protected:
	int save_frame_as_jpeg(AVFrame *pframe);
	int save_frame_as_png(AVFrame *pframe);
	int save_frame_as_ppm(AVFrame *pframe);
	int save_frame_as_yuv420p(AVFrame *pFrame);
	int save_frame_as_yuv422p(AVFrame *pFrame);

	AVFrame* frame_rgb_yuv420p(AVFrame* pFrame);
	AVFrame* frame_yuv420p_rgb(AVFrame* pFrame);
protected:
	bool m_bInit;
	AVCodecContext *pCodecCtx;
	AVFormatContext *pFormatCtx;
	struct SwsContext *img_convert_ctx; 
	AVCodecID	codec_id;
	AVFrame *pFrame;
	AVCodec * pCodec;
	AVStream* pStream;

	Listener* m_plistener;
	std::function<void(void*)> onDecoded;
	std::function<void()> onEncoded;

	int srcWidth, dstWidth;
	int srcHeight, dstHeight;

	int frame_count;

	std::queue<uint8_t*> inqueue;
	pthread_mutex_t inqueue_mutex;
	std::queue<Frame *> outqueue;
	pthread_mutex_t outqueue_mutex;

private:
	int sws_flags;
};
#endif // _FFMPEG_H_