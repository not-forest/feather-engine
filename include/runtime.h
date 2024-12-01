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

#include <stdint.h>
#include <err.h>

#include <res.h>
#include <scene.h>
#include <intrinsics.h>
#include <rect.h>

/* 
 *  @brief - engine's runtime datatype structure.
 *
 *  @uFps       - amount of frames per second in which all layers within the scene should be scheduled.
 *  @wRunWindow - inner SDL window pointer.
 *  @lResources - list of resources used between layers inside the scene.
 *  @lRects     - list of all rects for drawing.
 *  @sScene     - currently used scene. 
 *
 *  @glShaderProgram* - used when OpenGL is the chosen library for graphics. 
 *
 *  Defines the current active scene, processes the input, schedules all layers within that scene and
 *  renders the graphics.
 * */
typedef struct {
    tFPS uFps;
    char *cMainWindowName;

    SDL_Window *wRunWindow;
   
#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__
    GLuint glShaderProgram;
#endif

    tResList lResources;
    tRectList lRects;
    tScene *sScene;
} tRuntime;

#ifndef __EMSCRIPTEN__
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

#else
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
tEngineError errMainLoop(void *vData) __attribute__((nonnull(1)));
#endif

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
 *  @brief - creates a new instance of Rect, while initializing the drawing context.
 *
 *  Acts as a constructor for Rect structure. Rects are owned by the runtime and can only be referenced.
 *  If some rect shall be shared between the layers, wrap it under a resource structure.
 * */
tRect* tInitRect(tRuntime *tRun, tContext2D tCtx, uint16_t uPriority, char* sTexturePath) __attribute__((nonnull(1)));

/* 
 *  @brief - draws the rectangle to the screen.
 *
 *  The current position is defined by it's Context2D.
 * */
void vDrawRect(tRuntime *tRun, tRect *rect) __attribute__((nonnull(1)));

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
        .lRects = tll_init(),       \
    };

/* 
 *  @brief - Obtains the current runtime.
 *
 *  Runtime is actually always provided to every layer as a pointer, therefore
 *  it can always be accessed via it's argument name. It is not intuitive though, that's
 *  why this macro exists
 * */
#define tThisRuntime() tRun;

#endif
