#include "lj_lifeinit.h"

typedef struct {
	uint64_t elapsed;
	bool running;
	uint64_t current_time;
	uint64_t start_time;
	uint64_t pause_time;
	uint64_t pause_start;
	uint64_t pause_end;
} Timer;

uint64_t posix_GetProcessTimeWide(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((uint64_t)ts.tv_sec * 1000000ULL) + ((uint64_t)ts.tv_nsec / 1000ULL);
}

void updateTimer(Timer *timer){	
	if(!timer->running) return;
	
	timer->current_time = posix_GetProcessTimeWide();
	
	timer->elapsed = timer->current_time - timer->start_time;
}

static int timer_new(lua_State *L){
    Timer *timer = (Timer *)lua_newuserdata(L, sizeof(Timer));

	timer->start_time = posix_GetProcessTimeWide();
    
    luaL_getmetatable(L, "timer");
    lua_setmetatable(L, -2);
    return 1;
}

static int timer_start(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
	
	timer->start_time = posix_GetProcessTimeWide();
	timer->current_time = 0;
	timer->running = true;
    return 0;
}

static int timer_pause(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
	
	if(timer->running) timer->pause_start = posix_GetProcessTimeWide();
	else {
		timer->pause_end = posix_GetProcessTimeWide();
		timer->pause_time = timer->pause_end - timer->pause_start;
		timer->start_time += timer->pause_time;
	}
	
	timer->running = !timer->running;
    return 0;
}

static int timer_reset(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
    //this is how to reset a timer
	timer->elapsed = 0;
	timer->running = false;
	timer->current_time = 0;
	timer->pause_end = 0;
	timer->pause_start = 0;
	timer->pause_time = 0;
	timer->start_time = 0;
    return 0;
}

static int timer_stop(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
    timer->running = false;
    return 0;
}

static int timer_elapsed(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
    if (timer->running) updateTimer(timer);
	lua_pushnumber(L, (double)(timer->elapsed)/(1000*1000));
    return 1;
}

static int timer_running(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
    lua_pushboolean(L, timer->running);
    return 1;
}

static int timer_set(lua_State *L){
    Timer *timer = (Timer *)luaL_checkudata(L, 1, "timer");
    double seconds = luaL_checknumber(L, 2);

	uint64_t offset_ticks = (uint64_t)(seconds * 1000000); // seconds to microseconds
	timer->start_time = posix_GetProcessTimeWide() - offset_ticks;
	timer->current_time = posix_GetProcessTimeWide();
	timer->elapsed = offset_ticks;
	timer->running = true;
    return 0;
}

static const luaL_Reg timer_lib[] = {
    {"new", timer_new},
    {"start", timer_start},
    {"pause", timer_pause},
    {"stop", timer_stop},
    {"reset", timer_reset},
    {"set", timer_set},
    {"elapsed", timer_elapsed},
    {"running", timer_running},
    {NULL, NULL}
};

static const luaL_Reg timer_methods[] = {
    {"start", timer_start},
    {"pause", timer_pause},
    {"stop", timer_stop},
    {"reset", timer_reset},
    {"set", timer_set},
    {"elapsed", timer_elapsed},
    {"running", timer_running},
    {NULL, NULL}
};

LUALIB_API int luaL_opentimer(lua_State *L) {
	luaL_newmetatable(L, "timer");

    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    
    luaL_register(L, NULL, timer_methods);

	luaL_register(L, "timer", timer_lib);
    return 1;
}