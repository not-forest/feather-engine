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

#include "audio.h"
#include "controller.h"
#include <SDL_keycode.h>
#ifndef FEATHER_RUNTIME_H
#define FEATHER_RUNTIME_H

#include <stdint.h>
#include <err.h>

#include <scene.h>
#include <intrinsics.h>
#include <rect.h>

/* 
 *  @brief - engine's runtime datatype structure.
 *
 *  @uFps               - amount of frames per second in which all layers within the scene should be scheduled.
 *  @cMainWindowName    - name shown on the currently opened SDL window.
 *  @wRunWindow         - inner SDL window pointer.
 *  @sScene             - currently used scene. 
 *  @sdlRenderer        - SDL renderer for drawing rects.
 *  @tMixer             - runtime sound mixer.
 *
 *  Defines the current active scene, processes the input, schedules all layers within that scene and
 *  renders the graphics.
 * */
typedef struct {
    tFPS uFps;
    char *cMainWindowName;

    SDL_Window *wRunWindow;
    SDL_Renderer *sdlRenderer;
    tRuntimeMixer tMixer;

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
 *  @brief - sleep for a certain amount of milliseconds
 *
 *  Provided time will be same, regarding the fps limit. This function shall only be used
 *  within the layers.
 * */
void __vFeatherSleepLayerMs(tRuntime *tRun, const char *sLayerName, uint32_t ms) __attribute__((nonnull(1)));

/* 
 *  @brief - returns the amount of ms the provided layer has to sleep.
 *
 *  This function also decrements the sleeping amount of the layer each time count.
 * */
int __vFeatherCheckLayerSleepMs(tRuntime *tRun, const char *sLayerName) __attribute__((nonnull(1)));

/* 
 *  @brief - allows to define a clause that will be executed after a sleep delay.
 *
 *  Only content within this clause will be delayed, which won't affect anything written under it. For now several such clauses
 *  won't work properly.
 * */
#define vFeatherSleepLayerMs(tRun, sLayerName, ms)                          \
    int ___sleep_time___ = __vFeatherCheckLayerSleepMs(tRun, #sLayerName);  \
    if (___sleep_time___ == 0) {                                            \
        __vFeatherSleepLayerMs(tRun, #sLayerName, ms);                      \
    } else if (___sleep_time___ > 0) {} else

/* 
 *  @brief - Same as the above, but do not require the layer's name.
 * */
#define vFeatherSleepThisLayerMs(tRun, ms)                                  \
    const char* sLayerName = tRuntimeGetCurrentLayer(tRun)->sName;          \
    int ___sleep_time___ = __vFeatherCheckLayerSleepMs(tRun, sLayerName);   \
    if (___sleep_time___ == 0) {                                            \
        __vFeatherSleepLayerMs(tRun, sLayerName, ms);                       \
    } else if (___sleep_time___ > 0) {} else

/* 
 *  @brief - returns a pointer to the currently running layer.
 * */
tLayer* tRuntimeGetCurrentLayer(tRuntime *tRun) __attribute__((nonnull(1)));

/* 
 *  @brief - Removes any remained sleep time from the currently running layer.
 * */
void vRuntimeUnsleepCurrentLayer(tRuntime *tRun, bool ignoreNextSleep);

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
 * @brief - exit the engine's runtime with some status.
 * */
void vFeatherExit(tEngineError tStatus, tRuntime *tRun);

/* 
 *  @brief - creates a new instance of Rect, while initializing the drawing context.
 *
 *  Acts as a constructor for Rect structure. Rects are owned by the runtime and can only be referenced.
 *  If some rect shall be shared between the layers, wrap it under a resource structure.
 * */
tRect* tInitRect(tRuntime *tRun, tContext2D tCtx, uint16_t uPriority, char* sTexturePath) __attribute__((nonnull(1)));

/* 
 *  @brief - create a new controller.
 *
 *  @tRun           - currently running runtime.
 *  @vUserData      - pointer to user data used within the handler.
 *  @sdlEventType   - any event from the SDL library.
 *  @fHnd           - pointer to the declared handler function of type (void*)(void*,tController*)
 *
 *  @return - returns an id of the new controller.
 * */
uint32_t tControllerInit(tRuntime *tRun, SDL_EventType sdlEventType, void *vUserData, fHandler fHnd);

/* 
 *  @brief - gives a pointer to the controller based on the provided ID.
 *
 *  Will return NULL if no controller is found under such ID.
 * */
tController* tControllerGet(tRuntime *tRun, uint32_t uCtrlID);

/* 
 *  @brief - initializes the freshly clean keyboard controller.
 * */
void vKeyboardControllerInit(tRuntime *tRun, tKeyboardController *tKeybCtrl);

/* 
 *  @brief - append handler function for the keyboard controller on press event.  
 * */
void vKeyboardOnPress(tKeyboardController* tKeyboardCtrl, SDL_Keycode sdlKey, fHandler fHnd);
/* 
 *  @brief - append handler function for the keyboard controller on release event.  
 * */
void vKeyboardOnRelease(tKeyboardController* tKeyboardCtrl, SDL_Keycode sdlKey, fHandler fHnd);

/* 
 *  @brief - draws the rectangle to the screen.
 *
 *  @tRun - currently running runtime.
 *  @tRct - pointer to the rectange to draw.
 *
 *  The current position is defined by it's Context2D.
 * */
void vDrawRect(tRuntime *tRun, tRect *rect) __attribute__((nonnull(1)));

/* 
 *  @brief - changes the texture of the rect.
 *
 *  @tRun            - currently running runtime.
 *  @tRct            - pointer to the rectange we wish to change
 *  @sNewTexturePath - path to the new texture source.
 * */
void vChangeRectTexture(tRuntime* tRun, tRect* tRct, char* sNewTexturePath) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - Animates the rect according to the currently chosen rect.
 *
 *  @tRct           - the rect that shall be animated.
 *  @uAnimationId   - id of previously appended animations.
 *  @fSpeed         - amount of ms, before the next frame will appear.
 * */
void vAnimateFrame(tRuntime *tRun, tRect *tRct, uint16_t uAnimationId, float fSpeed) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - swaps the current scene to another one.
 *
 *  The previous scene will be saved in the same state it was left. After this command the layers
 *  from the new scene are going to be scheduled. Initialization layers are not returned to the initial
 *  state after the swap, so they must be reset manually before calling this function.
 * */
void vRuntimeSwapScene(tRuntime *tRun, tScene *tSc);

/* 
 *  @brief - changes the screen's title. Can be used any time during the runtime.
 * */
void vRuntimeSetWindowTitle(tRuntime *tRun, char* sTitle);

/* 
 *  @brief - gets the dimensions of the current running window.
 * */
void vRuntimeGetWindowDimensions(tRuntime *tRun, int *w, int *h);

/* 
 *  @brief - grows or shrinks rect's context to match with the full window size.
 *
 *  @tRct - object's rect.
 *  @tRun - current runtime.
 * */
void vFullScreenRect(tRect *tRct, tRuntime *tRun);

/* 
 *  @brief - Change the color of the rectangle.
 *
 *  @tRun          - Pointer to the runtime.
 *  @tRct          - Pointer to the rectangle.
 *  @fallbackColor - New fallback color to apply.
 *
 *  This also destroys the currently applied texture. Here Ctx width and height decides the size of the color.
 */
void vChangeRectColor(tRuntime* tRun, tRect* tRct, SDL_Color fallbackColor) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - default runtime value. Can be used and modified later.
 * */
#define DEFAULT_RUNTIME()                       \
    (tRuntime) {                                \
        .uFps = 60,                             \
        .cMainWindowName = "Feather App",       \
        .sdlRenderer = NULL,                    \
        .wRunWindow = NULL,                     \
        .sScene = NULL,                         \
        .tMixer = { tll_init(), tll_init() }    \
    };

/* 
 *  @brief - Obtains the current runtime.
 *
 *  Runtime is actually always provided to every layer as a pointer, therefore
 *  it can always be accessed via it's argument name. It is not intuitive though, that's
 *  why this macro exists
 * */
#define tThisRuntime() (tRuntime*)(__tRun);

#endif
