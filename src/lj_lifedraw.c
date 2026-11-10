#include "lj_lifeinit.h"

static int draw_swapbuffers(lua_State *L) {
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) exit(0);
    }

    if(!lua_isnoneornil(L, 1)){
        Color *color = lua_tocolor(L, 1);
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    }else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    return 0;
}

/*static int draw_clear(lua_State *L) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    return 0;
}*/

static int draw_rect(lua_State *L) {
    SDL_FRect rect;

    rect.x = (float)luaL_checknumber(L, 1);
    rect.y = (float)luaL_checknumber(L, 2);
    rect.w = (float)luaL_checknumber(L, 3);
    rect.h = (float)luaL_checknumber(L, 4);
    Color *color = lua_tocolor(L, 5);
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}

static int draw_line(lua_State *L){
    float x1 = (float)luaL_checknumber(L, 1);
    float y1 = (float)luaL_checknumber(L, 2);
    float x2 = (float)luaL_checknumber(L, 3);
    float y2 = (float)luaL_checknumber(L, 4);
    Color *color = lua_tocolor(L, 5);
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

    SDL_RenderLine(renderer, x1, y1, x2, y2);
    return 0;
}

static int draw_text(lua_State *L){
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    const char *text = luaL_checkstring(L, 3);
    if(!lua_isnone(L, 5)){
        float size = (float)luaL_checknumber(L, 4);
        SDL_SetRenderScale(renderer, size+1.0, size+1.0);
    }else SDL_SetRenderScale(renderer, 2.0, 2.0);
    Color *color = lua_tocolor(L, 4);
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

    SDL_RenderDebugText(renderer, x, y, text);
    return 0;
}

static int draw_pixel(lua_State *L){
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    Color *color = lua_tocolor(L, 3);
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

    SDL_RenderPoint(renderer, x, y);
    return 0;
}

static const luaL_Reg draw_lib[] = {
    {"text", draw_text},
    {"rect", draw_rect},
    {"line", draw_line},
    {"pixel", draw_pixel},
    {"swapbuffers", draw_swapbuffers},
    {NULL, NULL}
};

LUALIB_API int luaL_opendraw(lua_State *L) {
    luaL_register(L, "draw", draw_lib);
    return 1;
}