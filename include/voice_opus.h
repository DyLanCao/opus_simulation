#ifndef __VOICE_OPUS_H__
#define __VOICE_OPUS_H__

#include "opus_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint32_t	heapSize;
	uint8_t		channelCnt;
	uint8_t		complexity;
	uint8_t		packetLossPercentage;
	uint8_t		sizePerSample;
	uint16_t	appType;	
	uint16_t	bandWidth;
	uint32_t	bitRate;
	uint32_t	sampleRate;
	uint32_t	signalType;
	uint32_t	periodPerFrame;
	uint8_t		isUseVbr			:1;
	uint8_t		isConstraintUseVbr	:1;
	uint8_t		isUseInBandFec		:1;
	uint8_t		isUseDtx			:1;
	uint8_t		reserve				:4;
		
} VOICE_OPUS_CONFIG_T;

#if APP_SMARTVOICE_LOOPBACK_TEST
#define VOICE_OPUS_HEAP_SIZE 			(45*1024)
#else
#define VOICE_OPUS_HEAP_SIZE 			(25*1024)
#endif

#define VOICE_OPUS_CHANNEL_COUNT 		(1)
#define VOICE_OPUS_COMPLEXITY 			(0)
#define VOICE_OPUS_PACKET_LOSS_PERC 	(0)
#define VOICE_SIZE_PER_SAMPLE			(2)	// 16 bits, 1 channel
#define VOICE_OPUS_APP 					(OPUS_APPLICATION_VOIP)
#define VOICE_OPUS_BANDWIDTH 			(OPUS_BANDWIDTH_WIDEBAND)
#define VOICE_OPUS_BITRATE 				(32000)
#define VOICE_OPUS_SAMPLE_RATE			(16000)
#define VOICE_SIGNAL_TYPE				(OPUS_SIGNAL_VOICE)

#if defined(LC_MMSE_NOISE_SUPPRESS) || defined(COHERENT_DENOISE)
#define VOICE_FRAME_PERIOD				(OPUS_FRAMESIZE_80_MS)
#else
#define VOICE_FRAME_PERIOD				(OPUS_FRAMESIZE_20_MS)
#endif

#define VOICE_OPUS_USE_VBR 				(0)
#define VOICE_OPUS_CONSTRAINT_USE_VBR 	(0)
#define VOICE_OPUS_USE_INBANDFEC 		(0)
#define VOICE_OPUS_USE_DTX 				(0)

s
#if defined(LC_MMSE_NOISE_SUPPRESS) || defined(COHERENT_DENOISE)
#define VOICE_FRAME_PERIOD_IN_MS		(80)
s
#else
#define VOICE_FRAME_PERIOD_IN_MS		(20)
#endif

#define OPUS_COMPARESS_RATION           (VOICE_OPUS_SAMPLE_RATE*VOICE_SIZE_PER_SAMPLE*8/VOICE_OPUS_BITRATE)
#define VOICE_OPUS_ENCODED_DATA_SIZE_PER_FRAME	(VOICE_OPUS_BITRATE*VOICE_FRAME_PERIOD_IN_MS/1000/8)	
#define VOICE_OPUS_PCM_DATA_SIZE_PER_FRAME	(VOICE_OPUS_ENCODED_DATA_SIZE_PER_FRAME*OPUS_COMPARESS_RATION)


int voice_opus_init(VOICE_OPUS_CONFIG_T* pConfig, uint8_t* ptrHeap);
int voice_opus_init_encoder(uint16_t sampleRate);
int voice_opus_init_decoder(uint16_t sampleRate);
int voice_opus_deinit(void);
uint32_t voice_opus_encode(uint8_t *bitstream, uint8_t *speech, uint32_t sampleCount, uint8_t isReset);
uint32_t voice_opus_decode(uint8_t *speech, uint32_t speechLen, uint8_t *bitstream, uint32_t sampleCount, uint8_t isReset);

#ifdef __cplusplus	
}
#endif

#endif	// __VOICE_OPUS_H__
