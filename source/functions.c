#include "functions.h"

SDL_Color errorcolor = {255,255,255};
SDL_Surface *screens = NULL;
TTF_Font *errorfont;
SDL_Surface *error_text_surface;

void InitSDL() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	fatInitDefault();
	TTF_Init();
	WPAD_Init();
	WPAD_SetVRes(0, 640, 480);
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	ASND_Init();
	MP3Player_Init();
	//KEYBOARD_Init(keyPress_cb);
    atexit(SDL_Quit);
    SDL_ShowCursor(SDL_DISABLE);
    screens = SDL_SetVideoMode(640, 480, 16, SDL_DOUBLEBUF);
	errorfont=TTF_OpenFont("error.ttf", 16);
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
    va_start(opt, format);
    vsnprintf(buff, sizeof(buff), format, opt);
    va_end(opt);
	fontWrite(x, y, buff);
}
fileStruct fileLoad(const char* file) {
	fileStruct fs;
	FILE *fr;
	fr= fopen(file, "rb");
	if (!fr) {
		fs.ret = -1;
		return fs;
	}
	fseek(fr , 0 , SEEK_END);
	fs.size = ftell(fr);
	rewind(fr);
	fs.buffer = (char*) malloc (sizeof(char)*fs.size);
	fread(fs.buffer,1,fs.size,fr);
	if (!fs.buffer) {
		fclose(fr);
		fs.ret = -2;
		return fs;
	}
	fclose(fr);
	return fs;
}
