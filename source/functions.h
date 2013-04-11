//Lua
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
// normal includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <math.h>
#include <dirent.h>
#include <filebrowser.h>
#include <asndlib.h>
#include <aesndlib.h>
#include <mp3player.h>
#include <oggplayer.h>
#include <wavplayer.h>
#include <gcmodplay.h>
#include <fat.h>
#include <wiiuse/wpad.h> 
#include <wiikeyboard/keyboard.h>
// SDL includes
#include <SDL/sdl.h>
#include <SDL/sdl_image.h>
#include <SDL/sdl_draw.h>
#include <SDL/sdl_ttf.h>

#define toPush(HANDLE, DATATYPE) \
DATATYPE *to##HANDLE (lua_State *l, int index) { \
  DATATYPE* handle  = (DATATYPE*)lua_touserdata(l, index); \
  return handle; \
} \
DATATYPE* push##HANDLE(lua_State *l) { \
	DATATYPE * newvalue = (DATATYPE*)lua_newuserdata(l, sizeof(DATATYPE)); \
	return newvalue; \
}
/*
 * ** compatibility with Lua 5.2
 * */
#if (LUA_VERSION_NUM == 502)
#undef luaL_register
#define luaL_register(L,n,f) \
                { if ((n) == NULL) luaL_setfuncs(L,f,0); else luaL_newlib(L,f); }

#endif

SDL_Surface *screens;
lua_State *l;
TTF_Font *errorfont;
SDL_Surface *error_text_surface;
SDL_Color errorcolor;
typedef struct fileStruct {
	char *buffer;
	long size;
	int ret;
} fileStruct;

void InitSDL();
void free_surf(SDL_Surface* surf);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
void udelay(int us);
void fontPrintf(int x, int y, const char *format, ...);
fileStruct fileLoad(const char* file);
