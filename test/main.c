/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年12月03日 16时29分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//#include "webrtc_test.h"
//#include "webrtc_main.h"
//#include "nsx_main.h"
#include "opus.h"


#define FRAME_SIZE 960
#define SAMPLE_RATE 48000
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_AUDIO
#define BITRATE 64000

#define MAX_FRAME_SIZE 6*960
#define MAX_PICKET_SIZE 3*1276

uint32_t voice_opus_encode_test(OpusEncoder *st, uint8_t *bitstream, uint8_t *speech, uint32_t sampleCount, uint8_t isReset)
{

	uint32_t outputBytes = opus_encode(st, (opus_int16*)bitstream, sampleCount, speech, sampleCount*4);
	return outputBytes;
}

uint32_t voice_opus_decode_test(OpusDecoder *st,uint8_t *speech, uint32_t speechLen, uint8_t *bitstream, uint32_t sampleCount, uint8_t isReset)
{

	uint32_t outputPcmCount = opus_decode(st, speech, speechLen, (opus_int16*)bitstream, sampleCount, 0);
	return outputPcmCount;
}

int main(int argc, char *argv[])
{
        if(argc != 3)
        {
                fprintf(stderr, "Usage: <in> <out>\n");
                return -1;
        }
        char *fileIn  = argv[1];
        char *fileOut = argv[2];
        //ZeusFront front;
        int status;

        clock_t start, finish;
        double  duration;
        int count;
        int err;

        FILE *fin = fopen(fileIn,"r");
        FILE *fout = fopen(fileOut,"w");
        if(fin == NULL || fout == NULL)
        {
                fprintf(stderr, "failed to open pcm\n");
                return -1;
        }

        opus_int16 in[FRAME_SIZE*CHANNELS],out[MAX_FRAME_SIZE*CHANNELS];
        unsigned char cbits[MAX_PICKET_SIZE];
        int nbBytes;

        OpusEncoder *encoder;
        OpusDecoder *decoder;

        encoder = opus_encoder_create(SAMPLE_RATE,CHANNELS,APPLICATION,&err);
        if(err < 0)
        {
                fprintf(stderr,"failed to create an encoder",opus_strerror(err));
                return EXIT_FAILURE;
        }

        err = opus_encoder_ctl(encoder,OPUS_SET_BITRATE(BITRATE));
        if(err < 0)
        {
                fprintf(stderr,"failed to set an encoder",opus_strerror(err));
                return EXIT_FAILURE;
        }

        decoder = opus_decoder_create(SAMPLE_RATE,CHANNELS,&err);

        if(err < 0)
        {
                fprintf(stderr,"failed to create an decoder",opus_strerror(err));
                return EXIT_FAILURE;
        }

		voice_opus_init_encoder(SAMPLE_RATE);
		voice_opus_init_decoder(SAMPLE_RATE);

        while(1)
        {
                int i;
                unsigned char pcm_bytes[MAX_FRAME_SIZE*CHANNELS*2];
                int frame_size;

                int test = fread(pcm_bytes,sizeof(short)*CHANNELS,FRAME_SIZE,fin);
                if(test <= 0)
                {

                        printf("read test:%d  ms\n",test);
                        break;
                }

                for(i = 0; i<CHANNELS*FRAME_SIZE;i++)
                        in[i] = pcm_bytes[2*i + 1]<<8|pcm_bytes[2*i];

                start = clock();
                //nbBytes = voice_opus_encode_test(encoder,in,cbits,FRAME_SIZE,0);
				nbBytes = voice_opus_encode(in,cbits,FRAME_SIZE,0);
                if(nbBytes < 0)
                {
                        fprintf(stderr,"failed to encoder",opus_strerror(err));
                        return EXIT_FAILURE;
                }

				frame_size = voice_opus_decode(cbits,nbBytes,out,MAX_FRAME_SIZE,0);

                finish = clock();

                printf("count:%d speed time:%f ms\n",count,(double)(finish - start)*1000 / CLOCKS_PER_SEC);

                if(frame_size < 0)
                {
                        fprintf(stderr,"failed to  decoder",opus_strerror(err));
                        return EXIT_FAILURE;
                }

                for(i = 0; i < CHANNELS*frame_size; i++)
                {
                        pcm_bytes[2*i] = out[i]&0xff;
                        pcm_bytes[2*i + 1] = (out[i]>>8)&0xff;
                }
                count++;
                fwrite(pcm_bytes,sizeof(short),frame_size*CHANNELS,fout);
        }


        //opus_encoder_destory(encoder);
        //opus_decoder_destory(decoder);

        fclose(fin);
        fclose(fout);

        return 0;
}