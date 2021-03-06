/*******************************************************************************
*  Copyright (c) 1998 MFC Forum
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Module Name:
*
* Revision History:
*
* Date        Ver Name                    Description
* ----------  --- --------------------- -----------------------------------------
* 07-Jun-2016 0.1 Youngho Kim             Created
* ----------  --- --------------------- -----------------------------------------
*
* DESCRIPTION:
*
*  $Author:
*  $LastChangedBy:
*  $Date:
*  $Revision: 2949 $
*  $Id:
*  $HeadURL:
*******************************************************************************/
#ifndef _FFMPEG_ENCODER_H_
#define _FFMPEG_ENCODER_H_

#include "FFmpeg.h"

class FFMpegEncoder : public FFMpeg
{
public:
#if USE_LIVE555
	FFMpegEncoder(UsageEnvironment& env);
#else
	FFMpegEncoder();
#endif
	virtual ~FFMpegEncoder();
	
	virtual int intialize();
	virtual int finalize();

	char GetFrame(uint8_t** FrameBuffer, unsigned int *FrameSize);
	char ReleaseFrame();

private:
	pthread_t thread_id;
	int thread_exit;
	int videoindex;
	int fps;

	virtual int ReadFrame() = 0;
	virtual int WriteFrame(AVFrame* frame) = 0;

protected:
	static void *run(void *param);
	virtual int SetupCodec() = 0;

protected:
	AVFormatContext* pSourceFormatCtx;
	AVCodecContext* pSourceCodecCtx;
};
#endif // _FFMPEG_ENCODER_H_