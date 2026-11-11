cmake_minimum_required(VERSION 3.16)
project(LifeLuaSimulator C)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Find Dependencies
# -----------------------------------------------------------------------------

# 1. SDL3
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    # Fallback pkg-config check if CMake module isn't in default path
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL3 REQUIRED sdl3)
endif()

# 2. LuaJIT 2.1
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

# -----------------------------------------------------------------------------
# Executable Setup
# -----------------------------------------------------------------------------

# Add your source files here
set(SOURCES
    src/lj_lifemain.c
    src/lj_lifeos.c
    src/lj_lifedraw.c
    src/lj_lifecolor.c
    src/lj_lifecontrols.c
    src/lj_lifeimage.c
    src/lj_lifetimer.c
)

add_executable(LifeLuaSimulator ${SOURCES})

# Target Include Directories
target_include_directories(LifeLuaSimulator PRIVATE
    ${SDL3_INCLUDE_DIRS}
    ${LUAJIT_INCLUDE_DIRS}
)

# Target Link Libraries
target_link_libraries(LifeLuaSimulator PRIVATE
    ${LUAJIT_LIBRARIES}
)

if(TARGET SDL3::SDL3)
    target_link_libraries(LifeLuaSimulator PRIVATE SDL3::SDL3)
else()
    target_link_libraries(LifeLuaSimulator PRIVATE ${SDL3_LIBRARIES})
    target_link_directories(LifeLuaSimulator PRIVATE ${SDL3_LIBRARY_DIRS})
endif()

# Include link directories for LuaJIT if pkg-config provided them
target_link_directories(LifeLuaSimulator PRIVATE ${LUAJIT_LIBRARY_DIRS})