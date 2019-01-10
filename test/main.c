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

#define MAXSTRLEN 1024

#define SAMPLE_LENGTH 128


typedef unsigned char uint8_t;

#define true 1;
#define false 0;

static uint8_t opusEncodeIsReset;
static uint8_t opus_encode_cache_buff[100];


static void opus_init(void)
{
	/*
    uint8_t* opus_voice_heap_ptr;

    mem_alloc(&opus_voice_heap_ptr,   
    VOICE_OPUS_HEAP_SIZE);
    
    voice_opus_init(&opusConfig, opus_voice_heap_ptr);
	*/

    opusEncodeIsReset = true;
}

static void opus_deinit(void)
{
    //voice_opus_deinit();

    opusEncodeIsReset = false;
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
	//  status = front.FrontInit(16000, 16, 6, 3, 3);
	//  test_func_web();
	//WebRtcNsx_denoise_init(16000,1);

	FILE *inFp  = fopen(fileIn,"r");
	FILE *outFp = fopen(fileOut,"w");
	if(inFp == NULL || outFp == NULL)
	{
		fprintf(stderr, "failed to open pcm\n");
		return -1;
	}
	int tempSize = 320;
	short *in  = (short*)calloc(tempSize, sizeof(short));
	short *out = (short*)calloc(tempSize, sizeof(short));
	int pcmLen = tempSize;

	short tmp_in[SAMPLE_LENGTH],tmp_out[SAMPLE_LENGTH];

	//static short in[160], out[160];
    unsigned int encodeOutputsize,decodeOutputsize;

	opus_init();

	start = clock();

	while(pcmLen > 0)
	{
		pcmLen = fread(in, sizeof(short), tempSize, inFp);

		//WebRtcNsx_Alg_Process(tmp_in,tmp_out);
		//memcpy(out,in,pcmLen*sizeof(short));
		encodeOutputsize = voice_opus_encode((uint8_t*)in, opus_encode_cache_buff, tempSize*sizeof(short), opusEncodeIsReset);
    	
		decodeOutputsize = voice_opus_decode(opus_encode_cache_buff,encodeOutputsize,out,tempSize*sizeof(short),opusEncodeIsReset);

		opusEncodeIsReset = false;

		printf(".. count:%d decodeOutputsize:%d encodeOutputsize:%d \n\t",count,decodeOutputsize,encodeOutputsize);

		//
		fwrite(out, sizeof(short), tempSize, outFp);
		count++;

	}

	//   test_func_web();
	//test_aaa(1000);

	finish = clock();
	printf("count:%d speed time:%f \n",count,(double)(finish - start) / CLOCKS_PER_SEC);

	/*
	   fclose(inFp);
	   fclose(outFp);
	   free(in);
	   free(out);
	   */

	return 0;
}
