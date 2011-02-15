#include "../functions.h"
#include "sound.h"

toPush(Wav, PCMWAVFmt)
toPush(Mod, MODPlay)
toPush(Mp3, mp3modStruct)

static int lua_oldSoundInit(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	ASND_End();
	AESND_Init();
	return 1;
}

//WAV SOUNDS
static int lua_soundWavLoad(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	PCMWAVFmt pFmt = wavLoad(file);
	PCMWAVFmt *buffer = pushWav(l);
	*buffer = pFmt;
	free(pFmt.sample_buf);
	return 1;
}
static int lua_soundWavPlay(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	PCMWAVFmt pFmt = *toWav(l, 1);
	if (pFmt.sample_buf) {
		ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_STEREO_16BIT, pFmt.sample_rate,0, pFmt.sample_buf, pFmt.sample_channel * pFmt.sample_count * pFmt.sample_byte, 255, 255, NULL);
    } else {
      return luaL_error(l, "wrong number of arguments");
    }
	return 1;
}
/*static int lua_soundWavFree(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	//free(*toWav(l, 1)->sample_buf);
	return 1;
}*/

//OGG SOUNDS
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

//MP3 SOUNDS
static int lua_soundMp3Load(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	mp3modStruct* buffer = pushMp3(l);
	mp3modStruct mp3 = mp3modLoad(file);
	*buffer = mp3;
	return 1;
}
static int lua_soundMp3Play(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	mp3modStruct mp3 = *toMp3(l, 1);
	int repeat = luaL_checkint(l, 2);
	MP3Player_PlayBuffer(mp3.buffer, mp3.lSize, NULL);
	if (repeat == 1 && (!MP3Player_IsPlaying())) {
		MP3Player_PlayBuffer(mp3.buffer, mp3.lSize, NULL);
	}
	return 1;
}
/*static int lua_soundMp3Free(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	//free(*toMp3(l, 1));
	return 1;
}*/
static int lua_soundMp3Stop(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	MP3Player_Stop();
	return 1;
}
static int lua_soundMp3Volume(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	MP3Player_Volume(luaL_checkint(l, 1));
	return 1;
}
static int lua_soundMp3isPlaying(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	lua_pushnumber(l, MP3Player_IsPlaying());
	return 1;
}
	
//MOD SOUNDS
static int lua_soundModLoad(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	MODPlay mod_track;
	MODPlay* buffer = pushMod(l);
	mp3modStruct mod = mp3modLoad(file);
	MODPlay_SetMOD (&mod_track, mod.buffer);
	free(mod.buffer);
	MODPlay_SetVolume( &mod_track, 32,32);
	*buffer = mod_track;
	return 1;
}
static int lua_soundModPlay(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	MODPlay mod_track = *toMod(l, 1);
	//int repeat = luaL_checkint(l, 2);
	MODPlay_Start(&mod_track);
	return 1;
}
static int lua_soundModStop(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	MODPlay mod_track = *toMod(l, 1);
	MODPlay_Stop(&mod_track);
	return 1;
}
static int lua_soundModVolume(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	MODPlay mod_track = *toMod(l, 1);
	u32 tim = luaL_checkint(l, 2);
	MODPlay_SetVolume(&mod_track,tim,tim);
	return 1;
}
static int lua_soundModPauseResume(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	MODPlay mod_track = *toMod(l, 1);
	int pause = luaL_checkint(l, 2);
	if (pause == 1) {
		MODPlay_Pause(&mod_track,true);
	} else {
		MODPlay_Pause(&mod_track,false);
	}
	return 1;
}
static const struct luaL_reg Sound[] = {
  {"oldSoundInit", lua_oldSoundInit},
  {"wavLoad",lua_soundWavLoad},
  {"mp3Load",lua_soundMp3Load},
  //{"oggLoad",lua_soundOggLoad},
  {"modLoad",lua_soundModLoad},
  {"wavPlay",lua_soundWavPlay},
  {"mp3Play",lua_soundMp3Play},
  {"oggPlay",lua_soundOggPlay},
  {"modPlay",lua_soundModPlay},
  //{"wavStop",lua_soundWavStop},
  {"mp3Stop",lua_soundMp3Stop},
  {"oggStop",lua_soundOggStop},
  {"modStop",lua_soundModStop},
  //{"wavVolume",lua_soundMp3Volume},
  {"mp3Volume",lua_soundMp3Volume},
  {"oggVolume",lua_soundOggVolume},
  {"modVolume",lua_soundModVolume},
  {"mp3isPlaying",lua_soundMp3isPlaying},
  {"oggGetTime",lua_soundOggGetTime},
  {"oggSetTime",lua_soundOggSetTime},
  {"oggPauseResume",lua_soundOggPauseResume},
  {"modPauseResume",lua_soundModPauseResume},
  {NULL, NULL}
};
int luaopen_Sound(lua_State *l) {
	luaL_register(l, "Sound", Sound);
	return 1;
}
