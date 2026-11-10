#include "lj_lifeinit.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

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

static const luaL_Reg controls_lib[] = {
    {"update", controls_update},
    {"held", controls_held},
    {"pressed", controls_pressed},
    {"released", controls_released},
    {NULL, NULL}
};

LUALIB_API int luaL_opencontrols(lua_State *L) {
    luaL_register(L, "controls", controls_lib);

    lua_pushinteger(L, SDLK_LEFT);   lua_setglobal(L, "SDLK_LEFT");
    lua_pushinteger(L, SDLK_RIGHT);  lua_setglobal(L, "SDLK_RIGHT");
    lua_pushinteger(L, SDLK_UP);     lua_setglobal(L, "SDLK_UP");
    lua_pushinteger(L, SDLK_DOWN);   lua_setglobal(L, "SDLK_DOWN");
    lua_pushinteger(L, SDLK_SPACE);  lua_setglobal(L, "SDLK_SPACE");
    lua_pushinteger(L, SDLK_RETURN); lua_setglobal(L, "SDLK_RETURN");
    lua_pushinteger(L, SDLK_ESCAPE); lua_setglobal(L, "SDLK_ESCAPE");

    return 1;
}