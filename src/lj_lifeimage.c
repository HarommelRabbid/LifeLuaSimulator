#include "lj_lifeinit.h"
#define SDL_STBIMG_SDL3
#define SDL_STBIMAGE_IMPLEMENTATION
#include "include/SDL_stbimage.h"

typedef struct {
    SDL_Texture *tex;
    SDL_Surface *surf;
} Image;

static int image_load(lua_State *L){
    const char *name = luaL_checkstring(L, 1);
    Image *image = (Image *)lua_newuserdata(L, sizeof(Image));
    if(access(name, F_OK) != 0) return luaL_error(L, "image file doesn't exist");
    image->surf = STBIMG_Load(name);
    image->tex = SDL_CreateTextureFromSurface(renderer, image->surf);

    luaL_getmetatable(L, "image");
    lua_setmetatable(L, -2);
    return 1;
}

static int image_display(lua_State *L){
    Image *image = (Image *)luaL_checkudata(L, 1, "image");

    SDL_FRect dest;
    dest.x = (float)luaL_checknumber(L, 2);
    dest.y = (float)luaL_checknumber(L, 3);
    SDL_GetTextureSize(image->tex, &dest.w, &dest.h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderTexture(renderer, image->tex, NULL, &dest);
    return 0;
}

static int image_gc(lua_State *L){
    Image *image = (Image *)luaL_checkudata(L, 1, "image");

    if(image->tex != NULL) SDL_DestroyTexture(image->tex);
    if(image->surf != NULL) SDL_DestroySurface(image->surf);
    return 0;
}

static const luaL_Reg image_lib[] = {
    {"load", image_load},
    {"display", image_display},
    {NULL, NULL}
};

static const luaL_Reg image_meta[] = {
    {"display", image_display},
    {"__gc", image_gc},
    {NULL, NULL}
};

LUALIB_API int luaL_openimage(lua_State *L) {
    luaL_newmetatable(L, "image");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    
    luaL_register(L, NULL, image_meta);

    luaL_register(L, "image", image_lib);
    return 1;
}