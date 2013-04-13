#include "../functions.h"
#include <time.h>

// if the timer is running:
// measuredTime is the value of the last call to getCurrentMilliseconds
// offset is the value of startTime
//
// if the timer is stopped:
// measuredTime is 0
// offset is the value time() returns on stopped timers

typedef struct {
	clock_t measuredTime;
	clock_t offset;
} Timer;

toPush(Timer, Timer*)

static clock_t getCurrentMilliseconds() {
	return clock() / (clock_t) CLOCKS_PER_SEC / 1000;
}

static int lua_timerNew(lua_State *l) {
	int argc = lua_gettop(l); 
	if (argc != 0 && argc != 1) return luaL_error(l, "Argument error: Timer.new([startTime]) zero or one argument."); 
	
	Timer** luaTimer = pushTimer(l);
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	*luaTimer = timer;
	timer->measuredTime = getCurrentMilliseconds();
	timer->offset = argc == 1 ? luaL_checkint(l, 1) : 0;
	return 1;
}

static int lua_timerStart(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc > 1) return luaL_error(l, "Argument error: Timer:start() zero argument.");

	Timer* timer = *toTimer(l, 1);
	if (timer->measuredTime) {
		// timer is running
		clock_t currentTime = getCurrentMilliseconds();
		lua_pushnumber(l, currentTime - timer->measuredTime + timer->offset);
	} else {
		// timer is stopped
		timer->measuredTime = getCurrentMilliseconds();
		lua_pushnumber(l, timer->offset);
	}
	return 1;
}
static int lua_timerTime(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc > 1) return luaL_error(l, "Argument error: Timer:time() zero argument.");

	Timer* timer = *toTimer(l, 1);
	if (timer->measuredTime) {
		// timer is running
		clock_t currentTime = getCurrentMilliseconds();
		lua_pushnumber(l, currentTime - timer->measuredTime + timer->offset);
	} else {
		// timer is stopped
		lua_pushnumber(l, timer->offset);
	}
	return 1;
}

static int lua_timerStop(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc > 1) return luaL_error(l, "Argument error: Timer:stop() zero argument.");

	Timer* timer = *toTimer(l, 1);
	if (timer->measuredTime) {
		// timer is running
		clock_t currentTime = getCurrentMilliseconds();
		timer->offset = currentTime - timer->measuredTime + timer->offset;
		timer->measuredTime = 0;
	}
	lua_pushnumber(l, timer->offset);
	return 1;
}


static int lua_timerReset(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc > 2) return luaL_error(l, "Argument error: Timer:reset() zero or one argument.");

	Timer* timer = *toTimer(l, 1);
	if (timer->measuredTime) {
		// timer is running
		clock_t currentTime = getCurrentMilliseconds();
		lua_pushnumber(l, currentTime - timer->measuredTime + timer->offset);
	} else {
		// timer is stopped
		lua_pushnumber(l, timer->offset);
	}
	timer->offset = argc == 2 ? luaL_checkint(l, 2) : 0;
	timer->measuredTime = 0;
	return 1;
}

static int lua_timerFree(lua_State *l) {
	free(*toTimer(l, 1));
	return 0;
}

static const luaL_Reg Timers[] = {
	{"new", lua_timerNew},
	{"start", lua_timerStart},
	{"time", lua_timerTime},
	{"stop", lua_timerStop},
	{"reset", lua_timerReset},
	{"free", lua_timerFree},
	{NULL,NULL}
};

int luaregister_Timers (lua_State * l) {
	luaL_newlib(l, Timers);
	return 1;
}
int luaopen_Timer(lua_State *l) {
	luaL_requiref(l, "Timer", luaregister_Timers, 1);
    return 1;
}

