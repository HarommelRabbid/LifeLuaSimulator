#include "lj_lifeinit.h"

static int os_delay(lua_State *L){
    float secs = (float)luaL_checknumber(L, 1);
    SDL_Delay((uint32_t)secs * 1000);
    return 0;
}

static int os_message(lua_State *L){
    const char *text = luaL_checkstring(L, 1);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message from Lua", text, window);
    return 0;
}

static const luaL_Reg os_lib[] = {
    {"delay", os_delay},
    {"message", os_message},
    {NULL, NULL}
};

LUALIB_API int luaL_extendos(lua_State *L) {
    luaL_register(L, "os", os_lib);
    return 1;
}