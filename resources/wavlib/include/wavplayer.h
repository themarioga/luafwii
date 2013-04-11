#ifndef _PCMWAVFILE_H
#define _PCMWAVFILE_H

typedef unsigned long long	PCMClock;

typedef struct _PCMWAVFmt {
    unsigned int    sample_rate;    /* Sample frequency                 */
    unsigned short  sample_byte;    /* number of bytes per sample       */
    unsigned short  sample_bit;     /* number of valid bits per sample  */
    unsigned int    sample_count;   /* number of samples                */
    unsigned short  sample_channel; /* number of channels per sample    */
    unsigned short  sample_format;  /* little-endian(1), big-endian(2)  */
    PCMClock start, end;            /* start/end time in 100ns          */
	int ret;                        /* return variable for debug        */
}PCMWAVFmt;

#define PCMClock2Frame(clock, samprate) \
    ((unsigned int)(PCMClock(samprate)*(clock)/PCMClock(10000000)))

#define Frame2PCMClock(frame, samprate) \
        (PCMClock(frame)*PCMClock(10000000)/PCMClock(samprate))

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

void* LoadPCMWav(const char* file, PCMWAVFmt* pFmt);

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
#endif // _PCMWAVFILE_H
