#ifndef _H264_READ_CAMERA_ENCODER_H_
#define _H264_READ_CAMERA_ENCODER_H_

#include "FFmpeg.h"

class H264ReadScreenEncoder : public FFMpeg
{
public:
	H264ReadScreenEncoder();
	virtual ~H264ReadScreenEncoder();
	
	virtual int intialize();
	virtual int finalize();

	static void *run(void *param);
//	int openDecoder(int width, int height, CDecodeCB* pCB);
//	int encode(uint8_t* input, int nLen, bool bWaitIFrame = false);

private:
	pthread_t thread_id;
	int thread_exit;
	int videoindex;
	int fps;

	int WriteFrame(AVFrame* frame);
	int ReadFrame_from_Screenshot();

protected:
	AVFormatContext* pScreenFormatCtx;
	AVCodecContext* pScreenCodecCtx;
};
#endif // _H264_READ_CAMERA_ENCODER_H_