#include "lj_lifeinit.h"

static bool current_keys[SDL_SCANCODE_COUNT] = {false};
static bool previous_keys[SDL_SCANCODE_COUNT] = {false};

static int controls_update(lua_State *L) {
    SDL_memcpy(previous_keys, current_keys, sizeof(current_keys));

    SDL_Event event;
    while (SDL_PollEvent(&event));

    int numkeys = 0;
    const bool *state = SDL_GetKeyboardState(&numkeys);

    if (numkeys > SDL_SCANCODE_COUNT) numkeys = SDL_SCANCODE_COUNT;

    for (int i = 0; i < numkeys; i++) current_keys[i] = state[i];

    return 0;
}

static SDL_Scancode get_scancode_from_arg(lua_State *L, int arg) {
    SDL_Keycode keycode = (SDL_Keycode)luaL_checkinteger(L, arg);
    return SDL_GetScancodeFromKey(keycode, NULL);
}

static int controls_held(lua_State *L) {
    SDL_Scancode scancode = get_scancode_from_arg(L, 1);
    bool is_held = false;

    if (scancode > SDL_SCANCODE_UNKNOWN && scancode < SDL_SCANCODE_COUNT) {
        is_held = current_keys[scancode];
    }

    lua_pushboolean(L, is_held);
    return 1;
}

static int controls_pressed(lua_State *L) {
    SDL_Scancode scancode = get_scancode_from_arg(L, 1);
    bool is_pressed = false;

    if (scancode > SDL_SCANCODE_UNKNOWN && scancode < SDL_SCANCODE_COUNT) {
        is_pressed = current_keys[scancode] && !previous_keys[scancode];
    }

    lua_pushboolean(L, is_pressed);
    return 1;
}

static int controls_released(lua_State *L) {
    SDL_Scancode scancode = get_scancode_from_arg(L, 1);
    bool is_released = false;

    if (scancode > SDL_SCANCODE_UNKNOWN && scancode < SDL_SCANCODE_COUNT) {
        is_released = !current_keys[scancode] && previous_keys[scancode];
    }

    lua_pushboolean(L, is_released);
    return 1;
}

static int controls_fronttouch(lua_State *L){
    lua_newtable(L);

    float x, y;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);

    if(buttons & SDL_BUTTON_LMASK){
        lua_newtable(L);
        lua_pushnumber(L, x); lua_setfield(L, -2, "x");
        lua_pushnumber(L, y); lua_setfield(L, -2, "y");

        lua_rawseti(L, -2, 1);
    }
    return 1;
}

static int controls_reartouch(lua_State *L){
    lua_newtable(L);

    float x, y;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);

    if(buttons & SDL_BUTTON_RMASK){
        lua_newtable(L);
        lua_pushnumber(L, x); lua_setfield(L, -2, "x");
        lua_pushnumber(L, y); lua_setfield(L, -2, "y");

        lua_rawseti(L, -2, 1);
    }
    return 1;
}

static const luaL_Reg controls_lib[] = {
    {"update", controls_update},
    {"held", controls_held},
    {"pressed", controls_pressed},
    {"released", controls_released},
    {"fronttouch", controls_fronttouch},
    {"reartouch", controls_reartouch},
    {NULL, NULL}
};

LUALIB_API int luaL_opencontrols(lua_State *L) {
    luaL_register(L, "controls", controls_lib);

    luaL_pushglobalint_as(L, SDLK_LEFT, "SCE_CTRL_LEFT");
    luaL_pushglobalint_as(L, SDLK_RIGHT, "SCE_CTRL_RIGHT");
    luaL_pushglobalint_as(L, SDLK_UP, "SCE_CTRL_UP");
    luaL_pushglobalint_as(L, SDLK_DOWN, "SCE_CTRL_DOWN");
    luaL_pushglobalint_as(L, SDLK_BACKSPACE, "SCE_CTRL_CIRCLE");
    luaL_pushglobalint_as(L, SDLK_RETURN, "SCE_CTRL_CROSS");
    luaL_pushglobalint_as(L, SDLK_ESCAPE, "SCE_CTRL_START");
    luaL_pushglobalint_as(L, SDLK_W, "SCE_CTRL_TRIANGLE");
    luaL_pushglobalint_as(L, SDLK_S, "SCE_CTRL_SQUARE");

    return 1;
}