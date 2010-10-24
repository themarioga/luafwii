#include "../functions.h"

#define mp3Volume MP3Player_Volume
#define mp3Stop MP3Player_Stop
//mp3
FILE *BGM;
long lSize;
char *buffer;
size_t result;
bool mp3isready = false;
bool isplaying = false;

int mp3IsPlaying() {
	MP3Player_IsPlaying();
	if (MP3Player_IsPlaying() == true) {
	return 1;
	} else {
	return 0;
	}
}
void mp3Loop() {
	if(mp3IsPlaying() == 0) {
	MP3Player_PlayBuffer(buffer, lSize, NULL);
	}
}

static int lua_soundWavPlay(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	int ret = wavPlayer(file);
	if (ret == 0) {
		return luaL_error(l, "wrong number of arguments");
	}
	return 1;
}

static int lua_soundOggPlay(lua_State *l) {
if (lua_gettop(l) != 3) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	int timepos = luaL_checkint(l, 2);
	int repeat = luaL_checkint(l, 3);
	if (oggStatus() != 1) {
	oggPlayFile(file, timepos, repeat);
	}
	return 1;
}
static int lua_soundOggStop(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	oggStop();
	return 1;
}
static int lua_soundOggPauseResume(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	int pause = luaL_checkint(l, 1);
	oggPause(pause);
	return 1;
}
static int lua_soundOggVolume(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	int vol = luaL_checkint(l, 1);
	oggSetVolume(vol);
	return 1;
}
static int lua_soundOggGetTime(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	int time = oggGetTime();
	lua_pushnumber(l, time);
	return 1;
}
static int lua_soundOggSetTime(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	s32 tim = luaL_checkint(l, 1);
	oggSetTime(tim);
	return 1;
}
static int lua_soundMp3Play(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	int repeat = luaL_checkint(l, 2);
	if (mp3IsPlaying() == 0) {
	BGM = fopen(file, "rb");
	fseek(BGM , 0 , SEEK_END);
	lSize = ftell(BGM);
	rewind(BGM);
	buffer = (char*) malloc (sizeof(char)*lSize);
	result = fread(buffer,1,lSize,BGM);
	fclose(BGM);
	MP3Player_PlayBuffer(buffer, lSize, NULL);
	}
	mp3isready = true;
	if (repeat == 1) {
		mp3Loop();
	} else {
		free(buffer);
	}
	return 1;
}
static int lua_soundMp3Stop(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	mp3Stop();
	return 1;
}
static int lua_soundMp3Volume(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	u32 tim = luaL_checkint(l, 1);
	mp3Volume(tim);
	return 1;
}
static int lua_soundMp3isPlaying(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	int ispla = mp3IsPlaying();
	lua_pushnumber(l, ispla);
	return 1;
}
static const struct luaL_reg Sound[] = {
  {"wavPlay",lua_soundWavPlay},
  {"mp3Play",lua_soundMp3Play},
  {"oggPlay",lua_soundOggPlay},
  {"mp3Stop",lua_soundMp3Stop},
  {"oggStop",lua_soundOggStop},
  {"mp3Volume",lua_soundMp3Volume},
  {"oggVolume",lua_soundOggVolume},
  {"mp3isPlaying",lua_soundMp3isPlaying},
  {"oggGetTime",lua_soundOggGetTime},
  {"oggSetTime",lua_soundOggSetTime},
  {"oggPause",lua_soundOggPauseResume},
  {NULL, NULL}
};
int luaopen_Sound(lua_State *l) {
	   luaL_register(l, "Sound", Sound);
       return 1;
}
