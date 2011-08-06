#include "../functions.h"

typedef struct wavStruct {
	void *sample_buf;
	PCMWAVFmt pFmt;
	s32 voice;
} wavStruct;

wavStruct wavLoad(const char* file) {
	wavStruct ws;
	ws.pFmt.start=-1;
	ws.pFmt.end=-1;
    ws.sample_buf = LoadPCMWav(file, &ws.pFmt);
	return ws;
}

toPush(Wav, wavStruct)

//WAV SOUNDS
static int lua_soundWavLoad(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	wavStruct pFmt = wavLoad(file);
	if (!pFmt.sample_buf) {
		return luaL_error(l, "could not load. Error: %i", pFmt.pFmt.ret);
    }
	wavStruct *buffer = pushWav(l);
	*buffer = pFmt;
	return 1;
}
static int lua_soundWavPlay(lua_State *l) {
if (lua_gettop(l) != 1 && lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	wavStruct pFmt = *toWav(l, 1);
	if (pFmt.sample_buf) {
		pFmt.voice = ASND_GetFirstUnusedVoice();
		if (lua_gettop(l) == 1) {
			ASND_SetVoice(pFmt.voice, VOICE_STEREO_16BIT, pFmt.pFmt.sample_rate,0, pFmt.sample_buf, pFmt.pFmt.sample_channel * pFmt.pFmt.sample_count * pFmt.pFmt.sample_byte, 255, 255, NULL);
		} else if (lua_gettop(l) == 2) {
			s32 bits = luaL_checkint(l, 2);
			ASND_SetVoice(pFmt.voice, bits, pFmt.pFmt.sample_rate,0, pFmt.sample_buf, pFmt.pFmt.sample_channel * pFmt.pFmt.sample_count * pFmt.pFmt.sample_byte, 255, 255, NULL);
		}
	} else {
		return luaL_error(l, "could not play");
	}
	return 1;
}
static int lua_soundWavFree(lua_State *l) {
if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	free(toWav(l, 1));
	return 1;
}
static int lua_soundWavLoadandPlay(lua_State *l) {
if (lua_gettop(l) != 1 && lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	wavStruct pFmt;
	pFmt.pFmt.start=-1;
    pFmt.pFmt.end=-1;
    pFmt.sample_buf = LoadPCMWav(file, &pFmt.pFmt);
    if (pFmt.sample_buf) {
		pFmt.voice = ASND_GetFirstUnusedVoice();
		if (lua_gettop(l) == 1) {
			ASND_SetVoice(pFmt.voice, VOICE_STEREO_16BIT, pFmt.pFmt.sample_rate,0, pFmt.sample_buf, pFmt.pFmt.sample_channel * pFmt.pFmt.sample_count * pFmt.pFmt.sample_byte, 255, 255, NULL);
		} else if (lua_gettop(l) == 2) {
			s32 bits = luaL_checkint(l, 2);
			ASND_SetVoice(pFmt.voice, bits, pFmt.pFmt.sample_rate,0, pFmt.sample_buf, pFmt.pFmt.sample_channel * pFmt.pFmt.sample_count * pFmt.pFmt.sample_byte, 255, 255, NULL);
		}
	} else {
		return luaL_error(l, "could not load. Error: %i", pFmt.pFmt.ret);
	}
	free(pFmt.sample_buf);
	return 1;
}
static int lua_soundWavStop(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	wavStruct pFmt = *toWav(l, 1);
	ASND_StopVoice(pFmt.voice);
	return 1;
}
static int lua_soundWavPauseResume(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	wavStruct pFmt = *toWav(l, 1);
	s32 pause = luaL_checkint(l, 2);
	ASND_PauseVoice(pFmt.voice, pause);
	return 1;
}
static int lua_soundWavVolume(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	wavStruct pFmt = *toWav(l, 1);
	s32 volume = luaL_checkint(l, 2);
	ASND_ChangeVolumeVoice(pFmt.voice, volume, volume);
	return 1;
}
//OGG SOUNDS
static int lua_soundOggLoadandPlay(lua_State *l) {
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

toPush(Mp3, fileStruct)
//MP3 SOUNDS
static int lua_soundMp3Load(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	fileStruct mp3 = fileLoad(file);
	if (!mp3.buffer) {
		return luaL_error(l, "could not load");
	}
	fileStruct* buffer = pushMp3(l);
	*buffer = mp3;
	return 1;
}
static int lua_soundMp3Play(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	fileStruct mp3 = *toMp3(l, 1);
	if (!mp3.buffer) {
		return luaL_error(l, "could not load");
	}
	int repeat = luaL_checkint(l, 2);
	MP3Player_PlayBuffer(mp3.buffer, mp3.size, NULL);
	if (repeat == 1 && (!MP3Player_IsPlaying())) {
		MP3Player_PlayBuffer(mp3.buffer, mp3.size, NULL);
	}
	return 1;
}
static int lua_soundMp3Free(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	free(toMp3(l, 1));
	return 1;
}
static int lua_soundMp3LoadandPlay(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	fileStruct mp3 = fileLoad(file);
	if (!mp3.buffer) {
		return luaL_error(l, "could not load");
	}
	int repeat = luaL_checkint(l, 2);
	MP3Player_PlayBuffer(mp3.buffer, mp3.size, NULL);
	if (repeat == 1 && (!MP3Player_IsPlaying())) {
		MP3Player_PlayBuffer(mp3.buffer, mp3.size, NULL);
	}
	return 1;
}
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

/** //MOD SOUNDS
//toPush(Mod, MODPlay)
static int lua_soundModLoad(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	MODPlay mod_track;
	MODPlay* buffer = pushMod(l);
	fileStruct mod = fileLoad(file);
	if (!mod.buffer) {
		return luaL_error(l, "could not load");
	}
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
static int lua_soundModLoadandPlay(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	MODPlay mod_track;
	fileStruct mod = fileLoad(file);
	if (!mod.buffer) {
		return luaL_error(l, "could not load");
	}
	MODPlay_SetMOD (&mod_track, mod.buffer);
	free(mod.buffer);
	MODPlay_SetVolume( &mod_track, 32,32);
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
}*/
static const struct luaL_reg Sound[] = {
  {"wavLoad",lua_soundWavLoad}, //Nuevo
  {"mp3Load",lua_soundMp3Load}, //Nuevo
  //{"oggLoad",lua_soundOggLoad},
  //{"modLoad",lua_soundModLoad},
  {"wavPlay",lua_soundWavPlay}, //Nuevo
  {"mp3Play",lua_soundMp3Play}, //Nuevo
  //{"oggPlay",lua_soundOggPlay},
  //{"modPlay",lua_soundModPlay},
  {"wavFree",lua_soundWavFree}, //Nuevo
  {"mp3Free",lua_soundMp3Free}, //Nuevo
  //{"oggFree",lua_soundOggFree},
  //{"modFree",lua_soundModFree},
  {"wavLoadandPlay",lua_soundWavLoadandPlay}, //Nuevo
  {"mp3LoadandPlay",lua_soundMp3LoadandPlay}, //Nuevo
  {"oggLoadandPlay",lua_soundOggLoadandPlay}, //Nuevo
  //{"modLoadandPlay",lua_soundModLoadandPlay},
  {"wavStop",lua_soundWavStop}, //Nuevo
  {"mp3Stop",lua_soundMp3Stop},
  {"oggStop",lua_soundOggStop},
  //{"modStop",lua_soundModStop},
  {"wavVolume",lua_soundWavVolume}, //Nuevo
  {"mp3Volume",lua_soundMp3Volume},
  {"oggVolume",lua_soundOggVolume},
  //{"modVolume",lua_soundModVolume},
  {"wavPauseResume",lua_soundWavPauseResume}, //Nuevo
  //{"mp3PauseResume",lua_soundMp3PauseResume},
  {"oggPauseResume",lua_soundOggPauseResume},
  //{"modPauseResume",lua_soundModPauseResume},
  {"mp3isPlaying",lua_soundMp3isPlaying},
  {"oggGetTime",lua_soundOggGetTime},
  {"oggSetTime",lua_soundOggSetTime},
  {NULL, NULL}
};
int luaopen_Sound(lua_State *l) {
	luaL_register(l, "Sound", Sound);
	return 1;
}
