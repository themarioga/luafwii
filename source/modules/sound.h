#include <mp3player.h>
#include <oggplayer.h>
#include <wavplayer.h>
#include <gcmodplay.h>
#include <asndlib.h>
#include <aesndlib.h>

typedef struct mp3modStruct {
	char *buffer;
	long lSize;
	int ret;
} mp3modStruct;
typedef struct wavStruct {
	void *sample_buf;
	PCMWAVFmt pFmt;
} wavStruct;

wavStruct wavLoad(const char* file) {
	wavStruct ws;
	ws.pFmt.start=-1;
	ws.pFmt.end=-1;
    ws.sample_buf = LoadPCMWav(file, &ws.pFmt);
	return ws;
}
mp3modStruct mp3modLoad(const char* file) {
	mp3modStruct mms;
	FILE *fr;
	fr= fopen(file, "rb");
	if (!fr) {
		mms.ret = -1;
		return mms;
	}
	fseek(fr , 0 , SEEK_END);
	mms.lSize = ftell(fr);
	rewind(fr);
	mms.buffer = (char*) malloc (sizeof(char)*mms.lSize);
	fread(mms.buffer,1,mms.lSize,fr);
	if (!mms.buffer) {
		fclose(fr);
		mms.ret = -2;
		return mms;
	}
	fclose(fr);
	return mms;
}

