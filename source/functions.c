#include "functions.h"

SDL_Color errorcolor = {255,255,255};
SDL_Surface *screens = NULL;
TTF_Font *errorfont;
SDL_Surface *error_text_surface;

void keyPress_cb(char sym) {
	if (sym > 31 ) putchar(sym);
	if (sym == 13) putchar('\n');
	if (sym == 0x1b) putchar('\0');
}

void InitSDL() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	fatInitDefault();
	TTF_Init();
	WPAD_Init();
	WPAD_SetVRes(0, 640, 480);
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	ASND_Init();
	MP3Player_Init();
	MODPlay_Init(&play);
	KEYBOARD_Init(keyPress_cb);
    atexit(SDL_Quit);
    SDL_ShowCursor(SDL_DISABLE);
    screens = SDL_SetVideoMode(640, 480, 16, SDL_DOUBLEBUF);
}
void free_surf(SDL_Surface* surf){
	if(surf) SDL_FreeSurface(surf);
}
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}
void fontWrite(int x, int y, const char* text) {
	if (errorfont == NULL) {
		errorfont=TTF_OpenFont("error.ttf", 16);
	}
	if (text != NULL && errorfont != NULL) {
		error_text_surface=TTF_RenderText_Solid(errorfont, text, errorcolor);
		apply_surface(x, y, error_text_surface, screens, NULL);
		free_surf(error_text_surface);
	} else {
		exit(0);
	}
}
void fontPrintf(int x, int y, const char *format, ...) {
    va_list opt;
    char buff[2048];
    int bufsz;
    va_start(opt, format);
    bufsz = vsnprintf(buff, sizeof(buff), format, opt);
    va_end(opt);
	fontWrite(x, y, buff);
}
   PCMWAVFmt pFmt;
int wavPlayer(const char* file) {	
	pFmt.start=-1;
	pFmt.end=-1;
    void *buf = LoadPCMWav(file, &pFmt);
	if (buf) {
    ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_STEREO_16BIT, pFmt.sample_rate,0, buf, pFmt.sample_channel * pFmt.sample_count * pFmt.sample_byte, 255, 255, NULL);
    } else {
      return 0;
    }
    return 1;
}


