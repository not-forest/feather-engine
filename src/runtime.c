/**************************************************************************************************
 *  File: runtime.c
 *  Desc: Engine's runtime functions and data types. Runtime provides full abstraction over the main game
 *  loop, scheduling game tasks, handling graphics, physics, timings and can be seen as a core element of
 *  the whole engine structure.
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

#include <SDL.h>
#include <SDL_video.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "runtime.h"
#include "err.h"

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
tEngineError errMainLoop(tRuntime *tRun) {
    const double MS_PER_FRAME = 1000. / (double)tRun->uFps;
    time_t tCurrent, tLast, tSleep, tDelay = 0.0;
    tEngineError errResult;

    errResult = errEngineInit(tRun);
    if (errResult) return errResult;

    vFeatherLogDebug("Entering the main loop. MS_PER_UPDATE: %f", MS_PER_FRAME);

    tLast = time(NULL);
    for(;;) {
        tCurrent = time(NULL);
        tDelay += tCurrent - tLast;
        tLast = tCurrent;
        
        errResult = errEngineInputHandle(tRun);
        if (errResult) return errResult;

        // Updating the game for certain amount of time passed.
        while (tDelay >= FEATHER_UPDATE_AMOUNT) {
            errResult = errEngineUpdateHandle(tRun);
            if (errResult) return errResult;
            tDelay -= FEATHER_UPDATE_AMOUNT;
        }

        errResult = errEngineRenderHandle(tRun, tDelay);
        if (errResult) return errResult;

#if FEATHER_FPS_UNLIMITED == false
        // Sleeping for some amount of time, to not outrun the FPS amount.
        tSleep = tCurrent + MS_PER_FRAME - time(NULL);
        if (tSleep > 0) usleep(tSleep);
#endif
    }

    return 0;
}
#else
// TODO!
#endif

tEngineError errEngineInit(tRuntime *tRun) { 
    // SDL environment is initialized here.
    if (SDL_Init( FEATHER_SDL_INIT ) < 0)
        return -errSDL_ERR;

    // Running user configuration here. 
    if (vRuntimeConfig)
        vRuntimeConfig(tRun); // This function shall be provided by user.
    else
        vFeatherLogWarn("Runtime configuration not provided. Default configuration will be used.");

    // Creating the default window. Can be changed in 'vRuntimeConfig' 
    tRun->wRunWindow = SDL_CreateWindow(
        tRun->cMainWindowName,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 
        640, 
        480,
        __FEATHER_SDL_WINDOW_FLAGS
    );

    vFeatherLogDebug("Entering the initialization function.");
    // Without a single scene, runtime shall abort.
    if (tRun->sScene == NULL) 
        return -errNO_SCENE;

    return 0;
}

tEngineError errEngineInputHandle(tRuntime *tRun) {
    //vFeatherLogDebug("Entering the input handler function");

    return 0;
}

tEngineError errEngineUpdateHandle(tRuntime *tRun) {
    //vFeatherLogDebug("Entering the update function");

    return 0;
}

tEngineError errEngineRenderHandle(tRuntime *tRun, double dDelay) {
    //vFeatherLogDebug("Entering the rendering function with delay: %f", dDelay);

    return 0;
}
