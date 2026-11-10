#include "lj_lifeinit.h"

void lua_pushcolor(lua_State *L, unsigned int r, unsigned int g, unsigned int b, unsigned int a){
    Color *color = (Color *)lua_newuserdata(L, sizeof(Color));
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;

    luaL_getmetatable(L, "color");
    lua_setmetatable(L, -2);
}

Color *lua_tocolor(lua_State *L, int n){
    int type = lua_type(L, n);

    switch(type){
        case LUA_TNUMBER: {
            unsigned int rawcolor = luaL_checkinteger(L, n);
            Color *color = (Color *)lua_newuserdata(L, sizeof(Color));
            color->r = (rawcolor) & 0xFF;
            color->g = (rawcolor >> 8) & 0xFF;
            color->b = (rawcolor >> 16) & 0xFF;
            color->a = (rawcolor >> 24) & 0xFF;
            return color;
        }
        case LUA_TUSERDATA: {
            Color *color = (Color *)luaL_checkudata(L, n, "color");
            return color;
        }
        default: break;
    }

    luaL_typerror(L, n, "color or number");
    return NULL;
}

static int color_tocolor(lua_State *L){
    unsigned int rawcolor = luaL_checkinteger(L, 1);
    Color *color = (Color *)lua_newuserdata(L, sizeof(Color));
    color->r = (rawcolor) & 0xFF;
    color->g = (rawcolor >> 8) & 0xFF;
    color->b = (rawcolor >> 16) & 0xFF;
    color->a = (rawcolor >> 24) & 0xFF;
    return 1;
}

static int color_new(lua_State *L){
    unsigned int r = luaL_checkint(L, 1);
    unsigned int g = luaL_checkint(L, 2);
    unsigned int b = luaL_checkint(L, 3);
    unsigned int a = luaL_optint(L, 4, 255);

    lua_pushcolor(L, r, g, b, a);
    return 1;
}

static int color_r(lua_State *L){
    Color *color = lua_tocolor(L, 1);
    if(!lua_isnone(L, 1)){
        unsigned int r = luaL_checkint(L, 1);
        lua_pushcolor(L, r, color->g, color->b, color->a);
    }else lua_pushinteger(L, color->r);
    return 1;
}

static int color_g(lua_State *L){
    Color *color = lua_tocolor(L, 1);
    if(!lua_isnone(L, 1)){
        unsigned int g = luaL_checkint(L, 1);
        lua_pushcolor(L, color->r, g, color->b, color->a);
    }else lua_pushinteger(L, color->g);
    return 1;
}

static int color_b(lua_State *L){
    Color *color = lua_tocolor(L, 1);
    if(!lua_isnone(L, 1)){
        unsigned int b = luaL_checkint(L, 1);
        lua_pushcolor(L, color->r, color->g, b, color->a);
    }else lua_pushinteger(L, color->b);
    return 1;
}

static int color_a(lua_State *L){
    Color *color = lua_tocolor(L, 1);
    if(!lua_isnone(L, 1)){
        unsigned int a = luaL_checkint(L, 1);
        lua_pushcolor(L, color->r, color->g, color->b, a);
    }else lua_pushinteger(L, color->g);
    return 1;
}

static const luaL_Reg color_lib[] = {
    {"new", color_new},
    {"r", color_r},
    {"g", color_g},
    {"b", color_b},
    {"a", color_a},
    {NULL, NULL}
};

static const luaL_Reg color_meta[] = {
    {"r", color_r},
    {"g", color_g},
    {"b", color_b},
    {"a", color_a},
    {NULL, NULL}
};

LUALIB_API int luaL_opencolor(lua_State *L){
    luaL_newmetatable(L, "color");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    
    luaL_register(L, NULL, color_meta);

    lua_register(L, "tocolor", color_tocolor);
	luaL_register(L, "color", color_lib);
    return 1;
}