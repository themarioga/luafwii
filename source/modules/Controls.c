#include "../functions.h"

u32 pad;
u32 pre;
u32 rel;
ir_t ir;
vec3w_t acc;
expansion_t expan;
gforce_t gfor;
orient_t ori;
int irx, iry, irax, iray, irsx, irsy, irangle;
int accx, accy, accz;
int gforx, gfory;
int oriroll, oripitch, oriyaw;

int mag, ang;
int posx, posy, centerx, centery, minx, miny, maxx, maxy;
int accelx, accely, accelz;
int gforcex, gforcey;
int orientroll, orientpitch, orientyaw;
keyboard_event ke;

typedef struct _translation {
const char *ch;
int kb;
} translation;

translation KS[] = {
	{"q", KS_q},
	{"w", KS_w},
	{"e", KS_e},
	{"r", KS_r},
	{"t", KS_t},
	{"y", KS_y},
	{"u", KS_u},
	{"i", KS_i},
	{"o", KS_o},
	{"p", KS_p},
	{"[", KS_bracketleft},
	{"]", KS_bracketright},
	{"a", KS_a},
	{"s", KS_s},
	{"d", KS_d},
	{"f", KS_f},
	{"g", KS_g},
	{"h", KS_h},
	{"j", KS_j},
	{"k", KS_k},
	{"l", KS_l},
	{"{", KS_braceleft},
	{"z", KS_z},
	{"x", KS_x},
	{"c", KS_c},
	{"v", KS_v},
	{"b", KS_b},
	{"n", KS_n},
	{"m", KS_m},
	{",", KS_comma},
	{";", KS_semicolon},
	{".", KS_period},
	{":", KS_colon},
	{"-", KS_slash},
	{"_", KS_underscore},
	{"1", KS_1},
	{"2", KS_2},
	{"3", KS_3},
	{"4", KS_4},
	{"5", KS_5},
	{"6", KS_6},
	{"7", KS_7},
	{"8", KS_8},
	{"9", KS_9},
	{"0", KS_0},
	{"}", KS_braceright},
  {"", 0}
};

const char *keycatch(u8 keycode) {
int i;
	for (i = 0; i<47;i++) {
		if (keycode == KS[i].kb) {
			return KS[i].ch;
		} else {
			return "";
		}
	}
	return "";
}

static int Controls_kbdread(lua_State *l) {
if(lua_gettop(l) != 0) return luaL_error(l, "Wrong number of elements.");
s32 res = KEYBOARD_GetEvent(&ke);
	if (res && (ke.type == KEYBOARD_PRESSED)) { 
		lua_pushstring(l, keycatch(ke.keycode));
	} else {
		lua_pushstring(l, "");
	}
	return 1; 
}
static int Controls_kbdisconnected(lua_State *l) {
if(lua_gettop(l) != 0) return luaL_error(l, "Wrong number of elements.");
s32 res = KEYBOARD_GetEvent(&ke);
	if (res && (ke.type == KEYBOARD_CONNECTED)) { 
		lua_pushnumber(l, 1);
	} else {
		lua_pushnumber(l, 0);
	}
	return 1; 
}

static int Controls_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_ScanPads();
	pad = WPAD_ButtonsDown(wpad);
	rel = WPAD_ButtonsUp(wpad);
	pre = WPAD_ButtonsHeld(wpad);
	return 1;
}
static int Nunchuck_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_Expansion(wpad, &expan);
	return 1;
}
static int IR_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_IR(wpad, &ir);
	return 1;
}
static int Gforce_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_GForce(wpad, &gfor);
	return 1;
}
static int Acceleration_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_Accel(wpad, &acc);
	return 1;
}
static int Orientation_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_Orientation(wpad, &ori);
	return 1;
}
static int Controls_nunJoyAng(lua_State *l) {
	ang = expan.nunchuk.js.ang;
	lua_pushnumber(l, irsx);
	return 1;
}
static int Controls_nunJoyMag(lua_State *l) {
	mag = expan.nunchuk.js.mag;
	lua_pushnumber(l, mag);
	return 1;
}
#define CNJDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
	TYPE##DATA = expan.nunchuk.js.TYPE.DATA; \
	lua_pushnumber(l, TYPE##DATA); \
	return 1; \
}
CNJDataConv(nunJoyPosx, pos, x);
CNJDataConv(nunJoyPosy, pos, y);
CNJDataConv(nunJoyCenterx, center, x);
CNJDataConv(nunJoyCentery, center, y);
CNJDataConv(nunJoyMinx, min, x);
CNJDataConv(nunJoyMiny, min, y);
CNJDataConv(nunJoyMaxx, max, x);
CNJDataConv(nunJoyMaxy, max, y);

#define CNDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
	TYPE##DATA = expan.nunchuk.TYPE.DATA; \
	lua_pushnumber(l, TYPE##DATA); \
	return 1; \
}
CNDataConv(nunAccelx, accel, x);
CNDataConv(nunAccely, accel, y);
CNDataConv(nunAccelz, accel, z);
CNDataConv(nunGforcex, gforce, x);
CNDataConv(nunGforcey, gforce, y);
CNDataConv(nunOrientRoll, orient, pitch);
CNDataConv(nunOrientPitch, orient, roll);
CNDataConv(nunOrientYaw, orient, yaw);

#define CDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
	TYPE##DATA = TYPE.DATA; \
	lua_pushnumber(l, TYPE##DATA); \
	return 1; \
}
CDataConv(IRx, ir, x);
CDataConv(IRy, ir, y);
CDataConv(IRax, ir, ax);
CDataConv(IRay, ir, ay);
CDataConv(IRsx, ir, sx);
CDataConv(IRsy, ir, sy);
CDataConv(IRangle, ir, angle);
CDataConv(Accelx, acc, x);
CDataConv(Accely, acc, y);
CDataConv(Accelz, acc, z);
CDataConv(Gforcex, gfor, x);
CDataConv(Gforcey, gfor, y);
CDataConv(OrientRoll, ori, pitch);
CDataConv(OrientPitch, ori, roll);
CDataConv(OrientYaw, ori, yaw);

#define ControlsConv(NAMEFUN, NAMEBUT, TYPE) \
	static int Controls_##NAMEFUN (lua_State *l) { \
		if (TYPE & WPAD_##NAMEBUT) { \
			lua_pushnumber(l, 1); \
		} \
	return 1; \
} 

ControlsConv(padNunC, NUNCHUK_BUTTON_C, pad);
ControlsConv(preNunC, NUNCHUK_BUTTON_C, pre);
ControlsConv(relNunC, NUNCHUK_BUTTON_C, rel);
ControlsConv(padNunZ, NUNCHUK_BUTTON_Z, pad);
ControlsConv(preNunZ, NUNCHUK_BUTTON_Z, pre);
ControlsConv(relNunZ, NUNCHUK_BUTTON_Z, rel);
ControlsConv(padA, BUTTON_A, pad);
ControlsConv(preA, BUTTON_A, pre);
ControlsConv(relA, BUTTON_A, rel);
ControlsConv(padB, BUTTON_B, pad);
ControlsConv(preB, BUTTON_B, pre);
ControlsConv(relB, BUTTON_B, rel);
ControlsConv(pad1, BUTTON_1, pad);
ControlsConv(pre1, BUTTON_1, pre);
ControlsConv(rel1, BUTTON_1, rel);
ControlsConv(pad2, BUTTON_2, pad);
ControlsConv(pre2, BUTTON_2, pre);
ControlsConv(rel2, BUTTON_2, rel);
ControlsConv(padplus, BUTTON_PLUS, pad);
ControlsConv(preplus, BUTTON_PLUS, pre);
ControlsConv(relplus, BUTTON_PLUS, rel);
ControlsConv(padminus, BUTTON_MINUS, pad);
ControlsConv(preminus, BUTTON_MINUS, pre);
ControlsConv(relminus, BUTTON_MINUS, rel);
ControlsConv(padup, BUTTON_UP, pad);
ControlsConv(preup, BUTTON_UP, pre);
ControlsConv(relup, BUTTON_UP, rel);
ControlsConv(paddown, BUTTON_DOWN, pad);
ControlsConv(predown, BUTTON_DOWN, pre);
ControlsConv(reldown, BUTTON_DOWN, rel);
ControlsConv(padleft, BUTTON_LEFT, pad);
ControlsConv(preleft, BUTTON_LEFT, pre);
ControlsConv(relleft, BUTTON_LEFT, rel);
ControlsConv(padright, BUTTON_RIGHT, pad);
ControlsConv(preright, BUTTON_RIGHT, pre);
ControlsConv(relright, BUTTON_RIGHT, rel);
ControlsConv(padhome, BUTTON_HOME, pad);
ControlsConv(prehome, BUTTON_HOME, pre);
ControlsConv(relhome, BUTTON_HOME, rel);

static const luaL_reg Controls[] = {
	{"read", Controls_read},
	{"readNunchuck", Nunchuck_read},
	{"readGforce", Gforce_read},
	{"readAcceleration", Acceleration_read},
	{"readOrientation", Orientation_read},
	{"readIR", IR_read},
	{"IRx", Controls_IRx},
	{"IRy", Controls_IRy},
	{"IRrawx", Controls_IRax},
	{"IRrawy", Controls_IRay},
	{"IRsmoothx", Controls_IRsx},
	{"IRsmoothy", Controls_IRsy},
	{"IRangle", Controls_IRangle},
	{"accelx", Controls_Accelx},
	{"accely", Controls_Accely},
	{"accelz", Controls_Accelz},
	{"Gforcex", Controls_Gforcex},
	{"Gforcey", Controls_Gforcey},
	{"orientRoll", Controls_OrientRoll},
	{"orientPitch", Controls_OrientPitch},
	{"orientYaw", Controls_OrientYaw},
	{"nunAccelx", Controls_nunAccelx},
	{"nunAccely", Controls_nunAccely},
	{"nunAccelz", Controls_nunAccelz},
	{"nunGforcex", Controls_nunGforcex},
	{"nunGforcey", Controls_nunGforcey},
	{"nunOrientRoll", Controls_nunOrientRoll},
	{"nunorientPitch", Controls_nunOrientPitch},
	{"nunorientYaw", Controls_nunOrientYaw},
	{"nunJoyPosx", Controls_nunJoyPosx},
	{"nunJoyPosy", Controls_nunJoyPosy},
	{"nunJoyCenterx", Controls_nunJoyCenterx},
	{"nunJoyCentery", Controls_nunJoyCentery},
	{"nunJoyMinx", Controls_nunJoyMinx},
	{"nunJoyMiny", Controls_nunJoyMiny},
	{"nunJoyMaxx", Controls_nunJoyMaxx},
	{"nunJoyMaxy", Controls_nunJoyMaxy},
	{"nunJoyAng", Controls_nunJoyAng},
	{"nunJoyMag", Controls_nunJoyMag},
	{"pressA", Controls_padA },
	{"heldA", Controls_preA },
	{"releaseA", Controls_relA },
	{"pressB", Controls_padB },
	{"heldB", Controls_preB },
	{"releaseB", Controls_relB },
	{"pressUp", Controls_padup },
	{"heldUp", Controls_preup },
	{"releaseUp", Controls_relup },
	{"pressRight", Controls_padright },
	{"heldRight", Controls_preright },
	{"releaseRight", Controls_relright },
	{"pressDown", Controls_paddown },
	{"heldDown", Controls_predown },
	{"releaseDown", Controls_reldown },
	{"pressLeft", Controls_padleft },
	{"heldLeft", Controls_preleft },
	{"releaseLeft", Controls_relleft },
	{"press1", Controls_pad1 },
	{"held1", Controls_pre1 },
	{"release1", Controls_rel1 },
	{"press2", Controls_pad2 },
	{"held2", Controls_pre2 },
	{"release2", Controls_rel2 },
	{"pressPlus", Controls_padplus },
	{"heldPlus", Controls_preplus },
	{"releasePlus", Controls_relplus },
	{"pressMinus", Controls_padminus },
	{"heldMinus", Controls_preminus },
	{"releaseMinus", Controls_relminus },
	{"pressHome", Controls_padhome },
	{"heldHome", Controls_prehome },
	{"releaseHome", Controls_relhome },
	{"pressNunC", Controls_padNunC },
	{"heldNunC", Controls_preNunC },
	{"releaseNunC", Controls_relNunC },
	{"pressNunZ", Controls_padNunZ },
	{"heldNunZ", Controls_preNunZ },
	{"releaseNunZ", Controls_relNunZ },
	{"kbdRead", Controls_kbdread },
	{"kbdIsConnected", Controls_kbdisconnected },
  {NULL, NULL}
};

int luaopen_Controls(lua_State *l) {
	   luaL_register(l, "Controls", Controls);
       return 1;
}
