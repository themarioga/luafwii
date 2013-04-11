#include "../functions.h"

toPush(Image, SDL_Surface*)
toPush(Font, TTF_Font*)

//screens
static int lua_screenCreateSurface(lua_State *l) {
	if (lua_gettop(l) != 0 && lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	SDL_Surface* image;
	if (lua_gettop(l) == 0) { 
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, 0, 0, 0, 0);
	} else {
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, luaL_checkint(l, 1), luaL_checkint(l, 2), 32, 0, 0, 0, 0);
	}
	SDL_Surface** buffer = pushImage(l);
	*buffer = image;
	return 1;
}
static int lua_screenBlitSurface(lua_State *l) {
	if (lua_gettop(l) != 3) return luaL_error(l, "wrong number of arguments");
	apply_surface(luaL_checkint(l, 1), luaL_checkint(l, 2), *toImage(l, 3), screens, NULL);
	return 1;
}
static int lua_screenFreeSurface(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	free_surf(*toImage(l, 1));
	return 1;
}
static int lua_screenClear(lua_State *l) {
	if (lua_gettop(l) != 1 && lua_gettop(l) == 2) return luaL_error(l, "wrong number of arguments");
	int color = luaL_checkint(l, 1);
	if (lua_gettop(l) == 1) {
		SDL_FillRect(screens, 0, color);
	} else if (lua_gettop(l) == 2) {
		SDL_FillRect(*toImage(l, 2), 0, color);
	}
	return 1;
}
static int lua_screenFlip(lua_State *l) {
	if (lua_gettop(l) != 0) return luaL_error(l, "wrong number of arguments");
	SDL_Flip(screens);
	return 1;
}
static int lua_screenDelay(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	SDL_Delay(luaL_checkint(l, 1)*1000);
	return 1;
}
static int lua_screenPrintf(lua_State *l) {
	if (lua_gettop(l) != 3) return luaL_error(l, "wrong number of arguments");
	fontPrintf(luaL_checkint(l, 1), luaL_checkint(l, 2), luaL_checkstring(l, 3));
	return 1;
}
static int lua_screenPrintfSetColor(lua_State *l) {
	if (lua_gettop(l) != 3) return luaL_error(l, "wrong number of arguments");
	errorcolor.r = luaL_checkint(l, 1);
	errorcolor.g = luaL_checkint(l, 2);
	errorcolor.b = luaL_checkint(l, 3);
	return 1;
}
static int lua_screenColor(lua_State *l) {
	int arg = lua_gettop(l);
	if (arg != 3 && arg != 4) return luaL_error(l, "wrong number of arguments");
	int red = luaL_checkint(l, 1);
	int g = luaL_checkint(l, 2);
	int b = luaL_checkint(l, 3);
	int color;
	if (arg == 3) color = SDL_MapRGB(screens->format, red, g, b);
	if (arg == 4) { int a = luaL_checkint(l, 4); color = SDL_MapRGBA(screens->format, red, g, b, a); }
	lua_pushnumber(l, color);
	return 1;
}
//Line
static int lua_screenPixel(lua_State *l) {
	int arg = lua_gettop(l);
	if (arg != 3 && arg != 4) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int color = luaL_checkint(l, 3);
	if (arg == 3) {
		Draw_Pixel(screens, x, y, color); 
	} else {
		Draw_Pixel(*toImage(l, 4), x, y, color); 
	}
	return 1;
}
//Box
static int lua_screenfillBox(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 5 && arg != 6) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int color = luaL_checkint(l, 5);
	if (arg == 5) {
		Draw_FillRect(screens, x, y, w, h, color);
	} else {
		Draw_FillRect(*toImage(l, 6), x, y, w, h, color);
	}
	return 1;
}
static int lua_screenBox(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 5 && arg != 6) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int color = luaL_checkint(l, 5);
	if (arg == 5) {
		Draw_Rect(screens, x, y, w, h, color);
	} else {
		Draw_Rect(*toImage(l, 6), x, y, w, h, color);
	}
	return 1;
}
//Circle
static int lua_screenfillCircle(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 4 && arg != 5) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int rad = luaL_checkint(l, 3);
	int color = luaL_checkint(l, 4);
	if (arg == 4) {
		Draw_FillCircle(screens, x, y, rad, color);
	} else {
		Draw_FillCircle(*toImage(l, 5), x, y, rad, color);
	}
	return 1;
}
static int lua_screenCircle(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 4 && arg != 5) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int rad = luaL_checkint(l, 3);
	int color = luaL_checkint(l, 4);
	if (arg == 4) {
		Draw_Circle(screens, x, y, rad, color);
	} else {
		Draw_Circle(*toImage(l, 5), x, y, rad, color);
	}
	return 1;
}
//Ellipse
static int lua_screenfillEllipse(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 5 && arg != 6) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int color = luaL_checkint(l, 5);
	if (arg == 5) {
		Draw_FillEllipse(screens, x, y, w, h, color);
	} else {
		Draw_FillEllipse(*toImage(l, 6), x, y, w, h, color);
	}
	return 1;
}
static int lua_screenEllipse(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 5 && arg != 6) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int color = luaL_checkint(l, 5);
	if (arg == 5) {
		Draw_Ellipse(screens, x, y, w, h, color);
	} else {
		Draw_Ellipse(*toImage(l, 6), x, y, w, h, color);
	}
	return 1;
}
//Round
static int lua_screenfillRound(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 6 && arg != 7) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int ron = luaL_checkint(l, 5);
	int color = luaL_checkint(l, 6);
	if (arg == 6) {
		Draw_FillRound(screens, x, y, w, h, ron, color);
	} else {
		Draw_FillRound(*toImage(l, 7), x, y, w, h, ron, color);
	}
	return 1;
}
static int lua_screenRound(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 6 && arg != 7) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	int w = luaL_checkint(l, 3);
	int h = luaL_checkint(l, 4);
	int ron = luaL_checkint(l, 5);
	int color = luaL_checkint(l, 6);
	if (arg == 6) {
		Draw_Round(screens, x, y, w, h, ron, color); 
	} else {
		Draw_Round(*toImage(l, 7), x, y, w, h, ron, color); 
	}
	return 1;
}
//Line
static int lua_screenLine(lua_State *l) {
	int arg = lua_gettop(l); 
    if (arg != 5 && arg != 6) return luaL_error(l, "wrong number of arguments");
	int x1 = luaL_checkint(l, 1);
	int y1 = luaL_checkint(l, 2);
	int x2 = luaL_checkint(l, 3);
	int y2 = luaL_checkint(l, 4);
	int color = luaL_checkint(l, 5);
	if (arg == 5) {
		Draw_Line(screens, x1, y1, x2, y2, color);
	} else {
		Draw_Line(*toImage(l, 7), x1, y1, x2, y2, color);
	}
	return 1;
}
//Font
static int lua_screenFontLoad(lua_State *l) {
	if (lua_gettop(l) != 2) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	int size = luaL_checkint(l, 2);
	TTF_Font *font=TTF_OpenFont(file, size);
	if (font == NULL) {
		return luaL_error(l, "lua cannot load font");
	}
	TTF_Font** buffer = pushFont(l);
	*buffer = font;
	return 1;
}
static int lua_screenFontClose(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	TTF_Font *font = *toFont(l, 1);
	if (!font) return luaL_error(l, "no font");
	TTF_CloseFont(font);
	free(*toFont(l, 1));
	return 1;
}
static int lua_screenFontWrite(lua_State *l) {
	if (lua_gettop(l) != 4 && lua_gettop(l) != 5 && lua_gettop(l) != 7 && lua_gettop(l) != 8) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	const char *texto = luaL_checkstring(l, 3);
	TTF_Font *font = *toFont(l, 4);
	SDL_Surface *text_surface;
	SDL_Color font_color;
	if (lua_gettop(l) == 4) {
		text_surface=TTF_RenderText_Solid(font, texto, errorcolor);
		apply_surface(x, y, text_surface, screens, NULL);
		free_surf(text_surface);
	} else if (lua_gettop(l) == 5) {
		text_surface=TTF_RenderText_Solid(font, texto, errorcolor);
		apply_surface(x, y, text_surface, *toImage(l, 5), NULL);
		free_surf(text_surface);
	} else if (lua_gettop(l) == 7) {
		font_color.r = luaL_checkint(l, 5);
		font_color.g = luaL_checkint(l, 6);
		font_color.b = luaL_checkint(l, 7);
		text_surface=TTF_RenderText_Solid(font, texto, font_color);
		apply_surface(x, y, text_surface, screens, NULL);
		free_surf(text_surface);
	} else if (lua_gettop(l) == 8) {
		font_color.r = luaL_checkint(l, 5);
		font_color.g = luaL_checkint(l, 6);
		font_color.b = luaL_checkint(l, 7);
		text_surface=TTF_RenderText_Solid(font, texto, font_color);
		apply_surface(x, y, text_surface, *toImage(l, 8), NULL);
		free_surf(text_surface);
	}
	return 1;
}

static int lua_imageLoad(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	const char *file = luaL_checkstring(l, 1);
	SDL_Surface* image = IMG_Load(file);
	if (!image) return luaL_error(l, IMG_GetError());
	SDL_Surface** buffer = pushImage(l);
	*buffer = image;
	return 1;
}
static int lua_imageBlit(lua_State *l) {
	if (lua_gettop(l) != 3 && lua_gettop(l) != 4 && lua_gettop(l) != 7 && lua_gettop(l) != 8) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	SDL_Surface *image = *toImage(l, 3);
	if (lua_gettop(l) == 3) {
		apply_surface(x,y, image, screens, NULL);
	} else if (lua_gettop(l) == 4) {
		apply_surface(x,y, image, *toImage(l, 4), NULL);
	} else if (lua_gettop(l) == 7) {
		SDL_Rect clip;
		clip.x = luaL_checkint(l, 4);
		clip.y = luaL_checkint(l, 5);
		clip.w = luaL_checkint(l, 6);
		clip.h = luaL_checkint(l, 7);
		apply_surface(x,y, image, screens, &clip);
	} else if (lua_gettop(l) == 8) {
		SDL_Rect clip;
		clip.x = luaL_checkint(l, 4);
		clip.y = luaL_checkint(l, 5);
		clip.w = luaL_checkint(l, 6);
		clip.h = luaL_checkint(l, 7);
		apply_surface(x,y, image, *toImage(l, 8), &clip);
	}
	return 1;
}
static int lua_imageFree(lua_State *l) {
	if (lua_gettop(l) != 1) return luaL_error(l, "wrong number of arguments");
	free_surf(*toImage(l, 1));
	return 1;
}
static int lua_imageLoadinOne(lua_State *l) {
	if (lua_gettop(l) != 3 && lua_gettop(l) != 4 && lua_gettop(l) != 7 && lua_gettop(l) != 8) return luaL_error(l, "wrong number of arguments");
	int x = luaL_checkint(l, 1);
	int y = luaL_checkint(l, 2);
	const char *file = luaL_checkstring(l, 3);
	SDL_Surface *image = IMG_Load(file);
	if (!image) return luaL_error(l, "could not load image");
	if (lua_gettop(l) == 3) {
		apply_surface(x,y, image, screens, NULL);
	} else if (lua_gettop(l) == 4) {
		apply_surface(x,y, image, *toImage(l, 4), NULL);
	} else if (lua_gettop(l) == 7) {
		SDL_Rect clip;
		clip.x = luaL_checkint(l, 4);
		clip.y = luaL_checkint(l, 5);
		clip.w = luaL_checkint(l, 6);
		clip.h = luaL_checkint(l, 7);
		apply_surface(x,y, image, screens, &clip);
	} else if (lua_gettop(l) == 8) {
		SDL_Rect clip;
		clip.x = luaL_checkint(l, 4);
		clip.y = luaL_checkint(l, 5);
		clip.w = luaL_checkint(l, 6);
		clip.h = luaL_checkint(l, 7);
		apply_surface(x,y, image, *toImage(l, 8), &clip);
	}
	free_surf(image);
	return 1;
}

static const struct luaL_Reg Screen[] = {
  {"surfaceCreate",lua_screenCreateSurface}, //Nuevo
  {"surfaceBlit",lua_screenBlitSurface}, //Nuevo
  {"surfaceFree",lua_screenFreeSurface}, //Nuevo
  {"imageLoadinOne",lua_imageLoadinOne}, 
  {"imageLoad",lua_imageLoad}, 
  {"imageBlit",lua_imageBlit},
  {"imageFree",lua_imageFree}, 
  {"fontLoad",lua_screenFontLoad}, 
  {"fontClose",lua_screenFontClose},
  {"fontWrite",lua_screenFontWrite}, 
  {"print",lua_screenPrintf},
  {"printSetColor",lua_screenPrintfSetColor}, 
  {"color",lua_screenColor},
  {"flip",lua_screenFlip},
  {"clear",lua_screenClear},
  {"delay",lua_screenDelay},
  {"drawPixel",lua_screenPixel},
  {"drawfillBox",lua_screenfillBox},
  {"drawBox",lua_screenBox},
  {"drawfillCircle",lua_screenfillCircle},
  {"drawCircle",lua_screenCircle},
  {"drawfillEllipse",lua_screenfillEllipse},
  {"drawEllipse",lua_screenEllipse},
  {"drawfillRound",lua_screenfillRound},
  {"drawRound",lua_screenRound},
  {"drawLine",lua_screenLine},
  {NULL, NULL}
};

int luaopen_Screen(lua_State *l) {
	luaL_register(l, "Screen", Screen);
    return 1;
}

