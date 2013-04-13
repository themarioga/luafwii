#include "functions.h"

u32 pressod;

extern int luaopen_Sound(lua_State *l);
extern int luaopen_System(lua_State *l);
extern int luaopen_Screen(lua_State *l);
extern int luaopen_Controls(lua_State *l);
extern int luaopen_Timer(lua_State *l);

int main(int argc, char *argv[]) {
	InitSDL();
	l = luaL_newstate();
	luaL_openlibs(l);
	luaopen_Screen(l);
	luaopen_System(l);
	luaopen_Sound(l);
	luaopen_Controls(l);
	luaopen_Timer(l);
	int s = luaL_loadfile(l, "script.lua");
	while(1) {
		if (s == 0) {
			s = lua_pcall(l, 0, 0, 0);
		}
		if (s) {
			if (lua_tostring(l, -1)) {
				SDL_FillRect(screens, 0, SDL_MapRGB(screens->format, 0,0,0));
				fontPrintf(20, 20, "Error: %s", lua_tostring(l, -1));
				fontPrintf(20, 35, "Pulsa Home para salir.");
				SDL_Flip(screens);
				lua_pop(l, 1);
			}
			WPAD_ScanPads();
			pressod = WPAD_ButtonsDown(0);
			if(pressod & WPAD_BUTTON_HOME) {
				lua_close(l);
				exit(0);
			}
		}
	}
	lua_close(l);
	return 0;
}
