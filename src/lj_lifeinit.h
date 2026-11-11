#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define LL_VERSION "26-11-10"
#define luaL_pushglobalint(L, value) lua_pushinteger(L, value); lua_setglobal(L, #value);
#define luaL_pushglobalint_as(L, value, var) lua_pushinteger(L, value); lua_setglobal(L, var);
#define luaL_pushglobalint_alsoas(L, value, var) luaL_pushglobalint(L, value); luaL_pushglobalint_as(L, value, var);

extern SDL_Window *window;
extern SDL_Renderer *renderer;

typedef struct {
    unsigned int r, g, b, a;
} Color;
void lua_pushcolor(lua_State *L, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
Color *lua_tocolor(lua_State *L, int n);

LUALIB_API int luaL_opendraw(lua_State *L);
LUALIB_API int luaL_extendos(lua_State *L);
LUALIB_API int luaL_opencolor(lua_State *L);
LUALIB_API int luaL_opencontrols(lua_State *L);
LUALIB_API int luaL_openimage(lua_State *L);