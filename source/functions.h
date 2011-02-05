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
#include <mp3player.h>
#include <oggplayer.h>
#include <wavplayer.h>
#include <gcmodplay.h>
#include <asndlib.h>
#include <fat.h>
#include <wiiuse/wpad.h> 
#include <wiikeyboard/keyboard.h>
// SDL includes
#include <SDL/sdl.h>
#include <SDL/sdl_image.h>
#include <SDL/sdl_draw.h>
#include <SDL/sdl_ttf.h>

#define UserdataStubs(HANDLE, DATATYPE) \
DATATYPE *to##HANDLE (lua_State *L, int index) { \
  DATATYPE* handle  = (DATATYPE*)lua_touserdata(L, index); \
  return handle; \
} \
DATATYPE* push##HANDLE(lua_State *L) { \
	DATATYPE * newvalue = (DATATYPE*)lua_newuserdata(L, sizeof(DATATYPE)); \
	return newvalue; \
}\
int HANDLE##_register(lua_State *L) { \
	luaL_newmetatable(L, #HANDLE); \
	lua_pushliteral(L, "__index"); \
	lua_pushvalue(L, -2); \
	lua_rawset(L, -3); \
	lua_pushstring(L, #HANDLE); \
	lua_gettable(L, LUA_GLOBALSINDEX); \
	luaL_getmetatable(L, #HANDLE); \
	lua_setmetatable(L, -2); \
	return 1; \
}

SDL_Surface *screens;
lua_State *l;
MODPlay play;
TTF_Font *errorfont;
SDL_Surface *error_text_surface;
SDL_Color errorcolor;

void InitSDL();
void free_surf(SDL_Surface* surf);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
void udelay(int us);
void fontPrintf(int x, int y, const char *format, ...);
int wavPlayer(const char* file);
