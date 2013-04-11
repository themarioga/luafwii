#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "wavplayer.h"

#define IS_BIG_ENDIAN       1

#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))

#define GET3BYTES(p)    ((p)[0] | ((p)[1] << 8) | ((p)[2] << 16))

#define SWAP16(x)\
    ((u16)((((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8)))

#define SWAP32(x)\
    ((u32)((((x) & 0x000000FF) << 24) | (((x) & 0x0000FF00) << 8) | \
     (((x) & 0x00FF0000) >> 8) | (((x) & 0xFF000000) >> 24)))

typedef unsigned int        u32;  
typedef unsigned short int  u16; 

#if IS_BIG_ENDIAN
    #define SWAP_ED16(x)    SWAP16(x)
    #define SWAP_ED32(x)    SWAP32(x)
    #define SAMP_ENDIAN     (u16(2))
#else
    #define SWAP_ED16(x)    (x)
    #define SWAP_ED32(x)    (x)
    #define SAMP_ENDIAN     (u16(1))
#endif

void ReverseEndian(void* buffer, int byteWidth, int frames)
{
    switch (byteWidth)
    {
    case 2 :
        {
            u16* pusBuf  = (u16*)buffer;
            for (int i = 0; i < frames; i++) 
            {
                pusBuf[i] = SWAP16(pusBuf[i]);
            }
            break;
        }
    case 3 :
        {
            char* a = (char*)buffer;
            char c; 
            while(--frames >= 0)
            {
                c = a[0];
                a[0] = a[2];
                a[2] = c;
                a += 3;
            }
            break;
        }
    case 4 :
        {
            u32* puiBuf  = (u32*)buffer;
            for (int i = 0; i < frames; i++) 
            {
                puiBuf[i] = SWAP32(puiBuf[i]);
            }
            break;
        }
    default:
        break;
    }
}

int ReadFile( FILE* fd, void *buf, int n, int offset = 0)
{
    if (offset != 0) fseek(fd, offset, SEEK_CUR);
    int readlen = (int)fread(buf, 1, n, fd); 
    return readlen;
}

int CloseFile( FILE* fd)
{
    return fclose(fd); 
}

#define VERIFY_CHUNKID(fd, id, len) \
{   \
    char chunkID[(len)+1];  \
    ReadFile((fd), chunkID, (len));   chunkID[(len)] = char(0); \
    if ( strncmp(&chunkID[0], (id), (len)) )    \
    {   \
        return -1;  \
    }   \
}   
#define LOAD_2BYTES(fd, x)  \
{   \
    ReadFile((fd), &(x), 2);   \
    (x) = SWAP_ED16(x);   \
}
#define LOAD_4BYTES(fd, x)  \
{   \
    ReadFile((fd), &(x), 4);   \
    (x) = SWAP_ED32(x);   \
}

static int _load_pcmwavfmt(FILE* fd, PCMWAVFmt* pFmt)
{
    u32    chunkSize, samplesPerSec, bytesPerSec;
    u16  waveFmtType, channel, blockSize, bitsPerSample;

    VERIFY_CHUNKID(fd, "RIFF", 4);
    LOAD_4BYTES(fd, chunkSize);
    VERIFY_CHUNKID(fd, "WAVEfmt ", 8);
    LOAD_4BYTES(fd, chunkSize);
    LOAD_2BYTES(fd, waveFmtType);
    LOAD_2BYTES(fd, channel);
    LOAD_4BYTES(fd, samplesPerSec);
    LOAD_4BYTES(fd, bytesPerSec);
    LOAD_2BYTES(fd, blockSize);
    LOAD_2BYTES(fd, bitsPerSample);
    VERIFY_CHUNKID(fd, "data", 4);
    LOAD_4BYTES(fd, chunkSize);

    pFmt->sample_rate   = samplesPerSec;
    pFmt->sample_byte   = blockSize/channel;
    pFmt->sample_bit    = bitsPerSample;
    pFmt->sample_count  = chunkSize/blockSize;
    pFmt->sample_channel= channel;
    pFmt->sample_format = waveFmtType;
    return 0;
}

void* LoadPCMWav(const char* file, PCMWAVFmt* pFmt) {
    FILE* fd;
	void* pWav = NULL;
    if (!(fd = fopen(file, "rb"))) {
		pFmt->ret = -1;
        return NULL;
    }
    if (_load_pcmwavfmt(fd,pFmt)) {
        CloseFile(fd);
		pFmt->ret = -2;
        return NULL;
    }
    //check the wavefile length
    int iLen;
    if ((fseek(fd, 0, SEEK_END)!=0) || ((iLen = ftell(fd))<0) || (fseek(fd, 44L, SEEK_SET)!=0) ) {
        CloseFile(fd);
		pFmt->ret = -3;
        return NULL;
    }
    iLen -= 44;

    int wavlen = pFmt->sample_byte*pFmt->sample_count*pFmt->sample_channel;
    if (iLen != wavlen)
    {
        CloseFile(fd);
		pFmt->ret = -4;
        return NULL;
    }
    u32 stFrame = (pFmt->start != PCMClock(-1)) ? PCMClock2Frame(pFmt->start, pFmt->sample_rate) : 0;
    u32 enFrame = (pFmt->end   != PCMClock(-1)) ? PCMClock2Frame(pFmt->end  , pFmt->sample_rate) : pFmt->sample_count;
    enFrame = MIN(enFrame, pFmt->sample_count);
    stFrame = MIN(stFrame, enFrame);
    u32 npad = enFrame - stFrame; 
    if ( npad <= 0 )
    {
        CloseFile(fd);
		pFmt->ret = -5;
        return NULL;
    }
    int vecSize = pFmt->sample_byte*pFmt->sample_channel;
    int offset = stFrame * vecSize;
    pWav = (void*)memalign(128, npad*vecSize);

    u32 readlen = ReadFile(fd, pWav, npad*vecSize, offset);
    if (readlen != npad*vecSize)
    {
        CloseFile(fd);
        free(pWav);
		pFmt->ret = -6;
        return NULL;
    }
    if (pFmt->sample_format != SAMP_ENDIAN)
    {
        ReverseEndian(pWav, pFmt->sample_byte, pFmt->sample_count*pFmt->sample_channel);
    } 
    pFmt->sample_format = SAMP_ENDIAN;
    pFmt->sample_count  = npad;
	pFmt->ret = 0;
    return pWav;
}

