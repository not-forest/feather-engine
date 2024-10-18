/**************************************************************************************************
 *  File: runtime.h
 *  Desc: Engine's runtime functions and data types. Runtime provides full abstraction over the main game
 *  loop, scheduling game tasks, handling graphics, physics, timings and can be seen as a core element of
 *  the whole engine structure.
 *
 *  User may implement their own main loop and define the runtime manually within their own 'main' function.
 *  All functions are marked as weak and can be easily redefined with custom implementations.
 **************************************************************************************************
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 * */

#pragma once

#ifndef FEATHER_RUNTIME_H
#define FEATHER_RUNTIME_H

#define __FEATHER_SDL_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define __PUSH_WINDOW_FLAGS _Pragma("push_macro(\"__FEATHER_SDL_WINDOW_FLAGS\")") _Pragma("undef(\"__FEATHER_SDL_WINDOW_FLAGS\")")
#define __POP_WINDOW_FLAGS _Pragma("pop_macro(\"__FEATHER_SDL_WINDOW_FLAGS\")") __FEATHER_SDL_WINDOW_FLAGS
__PUSH_WINDOW_FLAGS

#ifdef __EMSCRIPTEN__

// SDL2 is not fully compatible with emscripten.
#include <SDL/SDL.h>

#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__
#include <SDL/SDL_opengl.h>
#define __FEATHER_SDL_WINDOW_FLAGS (__POP_WINDOW_FLAGS | SDL_WINDOW_OPENGL)
#endif

#else

#include <SDL2/SDL.h>

#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__
#include <SDL2/SDL_opengl.h>
#define __FEATHER_SDL_WINDOW_FLAGS (__POP_WINDOW_FLAGS | SDL_WINDOW_OPENGL)
#endif

#endif


#include <stdint.h>
#include "err.h"

#include "scene.h"
#include "intrinsics.h"

#ifndef FEATHER_FPS_UNLIMITED
// If true, FPS will be unlimited, therefore maximum perfomance is obtained. This will cause more energy
// to be used ofcource.
#define FEATHER_FPS_UNLIMITED false
#endif

#ifndef FEATHER_UPDATE_AMOUNT
// Amount of update time given for the game. This prevents users with higher FPS to perform more calculations.
#define FEATHER_UPDATE_AMOUNT 1.0
#endif

#define __FEATHER_SDL_DEFAULT SDL_INIT_VIDEO

// Combination of all required SDL subsystems for the program's need. 
#ifndef FEATHER_SDL_INIT
#define FEATHER_SDL_INIT __FEATHER_SDL_DEFAULT
#else
#define FEATHER_SDL_INIT __FEATHER_SDL_DEFAULT | FEATHER_SDL_INIT
#endif

/* 
 *  @brief - engine's runtime datatype structure.
 *
 *  @uFps - amount of frames per second in which all layers within the scene should be scheduled.
 *  @wRunWindow - inner SDL window pointer.
 *  @lResources - list of resources used between layers inside the scene.
 *  @sScene - currently used scene. 
 *
 *  Defines the current active scene, processes the input, schedules all layers within that scene and
 *  renders the graphics.
 * */
typedef struct {
    tFPS uFps;
    char *cMainWindowName;

    SDL_Window *wRunWindow;

    tResList lResources;
    tScene *sScene;
} tRuntime;

/* 
 *  @brief - main engine loop. Schedules all layers within the current scene.
 *
 *  Main/game loop which handles all operations within the chosen scene. The full workflow can be interpreted
 *  as so:
 *  - Processing input (I/O);
 *  - Main program update (Schedules all layers with internal scheduler/Computes physics);
 *  - Renders the picture;
 *
 *  If some fatal error occurs, it will be thrown back as 'tEngineError'.
 * */
tEngineError errMainLoop(tRuntime *tRun) __attribute__((nonnull(1)));

/* 
 *  @brief - this function can be used to modify the runtime before entering the main loop.
 *
 *  Note that runtime can also be modified anywhere during the main loop. In manual implementations, any function
 *  can be used to configure the runtime.
 * */
#define RUNTIME_CONFIGURE(fConfig)      \
    void vRuntimeConfig(tRuntime *tRun) \
    __attribute__((nonnull(1), alias(#fConfig)));
void vRuntimeConfig(tRuntime *tRun) __attribute__((nonnull(1), weak));

/* 
 *  @brief - handles input operations to listen upcoming input from keyboard, mouse, joystick, etc.
 *
 *  Does not wait for inputs but listen to them. May work differently on different platform, especially on WASM targets.
 * */
tEngineError errEngineInputHandle(tRuntime *tRun) __attribute__((weak, nonnull(1)));

/* 
 *  @brief - updates the game state based on obtained inputs, physics, etc.
 *
 *  Runs the internal scheduler, which schedules over each provided layer and runs them
 *  once or infinite times. All input handlers are also executed during this stage to not
 *  stall the input function too much.
 * */
tEngineError errEngineUpdateHandle(tRuntime *tRun) __attribute__((weak, nonnull(1)));

/* 
 *  @brief - handles the rendering phase with graphics libraries based on provided physical resources.
 * */
tEngineError errEngineRenderHandle(tRuntime *tRun, double dDelay) __attribute__((weak, nonnull(1)));

/* 
 *  @brief - initialization part that executed before entering the main loop.
 *
 *  Initializes all content within the chosen scene and starts the loop.
 * */
tEngineError errEngineInit(tRuntime *tRun) __attribute__((nonnull(1)));

/* 
 *  @brief - default runtime value. Can be used and modified later.
 * */
#define DEFAULT_RUNTIME()           \
    (tRuntime) {                    \
        .uFps = 60,                 \
        .cMainWindowName = "window",\
        .wRunWindow = NULL,         \
        .sScene = NULL,             \
        .lResources = tll_init(),   \
    };

#endif
