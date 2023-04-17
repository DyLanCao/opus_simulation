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


#define FRAME_SIZE 320
#define SAMPLE_RATE 16000
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_VOIP

#define MAX_FRAME_SIZE 6*320
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

#define FRAME_OPUS 80
uint32_t data_num = 0;
char file_data[200] = "check_result.txt";
char txt_data[200];

#if 1
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

        FILE *data_file = fopen(fileIn,"r");
        FILE *fout = fopen(fileOut,"w");
        if(data_file == NULL || fout == NULL)
        {
                fprintf(stderr, "failed to open pcm\n");
                return -1;
        }

        FILE *txt_file  = fopen(file_data,"w");
        if(txt_file == NULL)
        {
                fprintf(stderr, "failed to open txt \n");
                return -1;
        }


        opus_int16 in[FRAME_SIZE*CHANNELS],out[MAX_FRAME_SIZE*CHANNELS];
        unsigned char cbits[MAX_PICKET_SIZE];
        int nbBytes;


        voice_opus_init_encoder(SAMPLE_RATE);
        voice_opus_init_decoder(SAMPLE_RATE);
        int frame_size,cnt = 0;
        unsigned char pcm_bytes[MAX_FRAME_SIZE*CHANNELS*2];
        while(1)
        {
                unsigned char buf1[2];
                int n = fread(buf1,sizeof(unsigned char), 1,data_file);
                if ((n > 0) && (buf1[0] == 0xff))
                {
                        unsigned char buf2[2];
                        //printf("read buf1 is:0x%x \n\r",buf1[0]);

                        int n_data = fread(buf2,sizeof(unsigned char), 1,data_file);
                        if((n_data > 0) && (buf2[0] == 0xff))
                        {
                                //printf("read buf2 is:0x%x \n\r",buf2[0]);
                                                                                
                                //dump8(buf1,2);
                                n = fread(buf2,sizeof(unsigned char), 2,data_file);

                                //dump8(buf2,2);
                                if(buf2[0] > 0)
                                {
                                        sprintf(txt_data,"err code is:%d file name is:%s\n\r",buf2[0],fileIn);
                                        fwrite(txt_data,strlen(txt_data),1,txt_file);
                                }
                                printf("read buf2 is:0x%x 0x%x \n\r",buf2[0],buf2[1]);
                                //if (n > 0 && (buf2[0] == 0x00) &&(buf2[1] == 0x28))
                                if (n > 0)
                                {
                                        unsigned char buf3[1920];
                                        cnt++;
                                        data_num = fread(buf3,sizeof(unsigned char), FRAME_OPUS,data_file);

		                        frame_size = voice_opus_decode(buf3,FRAME_OPUS,out,MAX_FRAME_SIZE,0);

                                        printf("ss read buf3 decode size:%d seq nums is:%d  \n\r",frame_size,cnt);
                                        fwrite((short*)out,sizeof(short),frame_size*CHANNELS,fout);
                                        //fwrite(out,sizeof(short),frame_size*CHANNELS,fout);


                                }



                        }	
                }

                if(n <= 0)
                {
                        printf("file read end amount is:%d \n\r",cnt);
                        break;
                }

        }
        
        finish = clock();
        printf("all speed time:%f \n",(double)(finish - start) / CLOCKS_PER_SEC);

        // opus_encoder_destory(encoder);
        // opus_decoder_destory(decoder);

        fclose(data_file);
        //fclose(fin);
        fclose(fout);

        return 0;
}
#else


#define MAX_PACKET (1500)
#define SAMPLES (16000*30)
#define SSAMPLES (SAMPLES/3)
#define MAX_FRAME_SAMP (5760)
#define PI (3.141592653589793238462643f)
#define RAND_SAMPLE(a) (a[fast_rand() % sizeof(a)/sizeof(a[0])])


void main(void)
{
   int samp_count = 0;
   opus_int16 *inbuf;
   unsigned char packet[MAX_PACKET+257];
   int len;
   opus_int16 *outbuf;
   int out_samples;

        OpusEncoder *enc;
        OpusDecoder *dec;

        enc = opus_encoder_create(SAMPLE_RATE,CHANNELS,APPLICATION,&err);
        if(err < 0)
        {
                fprintf(stderr,"failed to create an encoder",opus_strerror(err));
                return EXIT_FAILURE;
        }

        err = opus_encoder_ctl(enc,OPUS_SET_BITRATE(BITRATE));
        if(err < 0)
        {
                fprintf(stderr,"failed to set an encoder",opus_strerror(err));
                return EXIT_FAILURE;
        }

        dec = opus_decoder_create(SAMPLE_RATE,CHANNELS,&err);

   /* Generate input data */
   inbuf = (opus_int16*)malloc(sizeof(*inbuf)*SSAMPLES);
   generate_music(inbuf, SSAMPLES/2);

   /* Allocate memory for output data */
   outbuf = (opus_int16*)malloc(sizeof(*outbuf)*MAX_FRAME_SAMP*3);

   /* Encode data, then decode for sanity check */
   do {
      len = opus_encode(enc, &inbuf[samp_count*channels], frame_size, packet, MAX_PACKET);
      if(len<0 || len>MAX_PACKET) {
         fprintf(stderr,"%s\n",debug_info);
         fprintf(stderr,"opus_encode() returned %d\n",len);
         test_failed();
      }

      out_samples = opus_decode(dec, packet, len, outbuf, MAX_FRAME_SAMP, 0);
      if(out_samples!=frame_size) {
         fprintf(stderr,"%s\n",debug_info);
         fprintf(stderr,"opus_decode() returned %d\n",out_samples);
         test_failed();
      }

      samp_count += frame_size;
   } while (samp_count < ((SSAMPLES/2)-MAX_FRAME_SAMP));

   /* Clean up */
   free(inbuf);
   free(outbuf);
}

#endif
