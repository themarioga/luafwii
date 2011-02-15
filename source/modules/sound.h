#include <mp3player.h>
#include <oggplayer.h>
#include <wavplayer.h>
#include <gcmodplay.h>
#include <asndlib.h>
#include <aesndlib.h>

typedef struct mp3modStruct {
	char *buffer;
	long lSize;
} mp3modStruct;

PCMWAVFmt wavLoad(const char* file) {
	PCMWAVFmt pFmt;
	pFmt.start=-1;
	pFmt.end=-1;
    pFmt.sample_buf = LoadPCMWav(file, &pFmt);
	return pFmt;
}
mp3modStruct mp3modLoad(const char* file) {
	mp3modStruct mms;
	FILE *f;
	f = fopen(file, "rb");
	fseek(f , 0 , SEEK_END);
	mms.lSize = ftell(f);
	rewind(f);
	mms.buffer = (char*) malloc (sizeof(char)*mms.lSize);
	fread(mms.buffer,1,mms.lSize,f);
	fclose(f);
	return mms;
}

