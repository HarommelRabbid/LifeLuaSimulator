#include "lj_lifeinit.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void luaL_lifelua_dofile(lua_State *L, const char *script_path){
    if (luaL_loadfile(L, script_path) || lua_pcall(L, 0, 0, 0) != LUA_OK) {
        const char *error = lua_tostring(L, -1) != NULL ? lua_tostring(L, -1) : "Unknown error";
        SDL_MessageBoxData *msgbox;
        msgbox->flags = SDL_MESSAGEBOX_ERROR | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
        msgbox->title = "LifeLua Error";
        msgbox->message = error;
        msgbox->numbuttons = 2;
        msgbox->colorScheme = NULL;
        SDL_MessageBoxButtonData button_data[3] = {
            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Close"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "Retry"},
            {0, 0, NULL}
        };
        msgbox->buttons = button_data;
        msgbox->window = window;
        int retid;
        SDL_ShowMessageBox(msgbox, &retid);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    SDL_SetAppMetadata("LifeLua Simulator", "1.0", "com.harommel.lifeluasimulator");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("LifeLua Simulator", 960, 544, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_SetRenderLogicalPresentation(renderer, 960, 544, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    lua_State *L = luaL_newstate();
    lua_pushstring(L, LL_VERSION); lua_setglobal(L, "LL_VERSION");
    luaL_openlibs(L);
    luaL_opencolor(L);
    luaL_opendraw(L);
    luaL_opencontrols(L);
    luaL_extendos(L);
    luaL_openimage(L);

    const char *script_path = "main.lua";
    if(argc > 1) script_path = argv[1];
    luaL_lifelua_dofile(L, script_path);

    lua_close(L);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}