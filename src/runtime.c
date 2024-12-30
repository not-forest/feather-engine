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

#include <time.h>
#include <unistd.h>
#include <tllist.h>

#include <log.h>
#include <runtime.h>
#include <intrinsics.h>
#include <err.h>

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
    time_t tCurrent, tLast, tSleep, tDelay = 0.0;
    tEngineError errResult;

    errResult = errEngineInit(tRun);
    if (errResult) return errResult;

    vFeatherLogDebug("Entering the main loop. MS_PER_UPDATE: %f", FEATHER_UPDATE_AMOUNT);

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
        tSleep = tCurrent + FEATHER_UPDATE_AMOUNT - time(NULL);
        if (tSleep > 0) usleep(tSleep);
#endif
    }

    return 0;
}
#else

typedef struct {
    time_t tCurrent, tLast, tSleep, tDelay;
    tRuntime *tRun;
} emscripten_helper_struct;

void errMainLoop(void *vData) {
    emscripten_helper_struct* sHelper = (emscripten_helper_struct*)(vData);

    sHelper->tCurrent = time(NULL);
    sHelper->tDelay += sHelper->tCurrent - sHelper->tLast;
    sHelper->tLast = sHelper->tCurrent;

    /* errResult = errEngineInputHandle(sHelper->tRun);
    if (errResult) return errResult; */

    // Updating the game for certain amount of time passed.
    while (sHelper->tDelay >= FEATHER_UPDATE_AMOUNT) {
        /* errResult = errEngineUpdateHandle(tRun);
        if (errResult) return errResult; */
        sHelper->tDelay -= FEATHER_UPDATE_AMOUNT;
    }

    /* errResult = errEngineRenderHandle(tRun, tDelay);
    if (errResult) return errResult; */

#if FEATHER_FPS_UNLIMITED == false
    // Sleeping for some amount of time, to not outrun the FPS amount.
    sHelper->tSleep = sHelper->tCurrent + FEATHER_UPDATE_AMOUNT - time(NULL);
    if (sHelper->tSleep > 0) usleep(sHelper->tSleep);
#endif
}

/* 
 * Additional helper function for emscripten to enter it's loop.
 * */
tEngineError errEmscriptenPreloop(tRuntime *tRun) {
    time_t tCurrent, tLast, tSleep, tDelay = 0.0;
    tEngineError errResult;

    errResult = errEngineInit(tRun);
    if (errResult) return errResult;

    vFeatherLogDebug("Entering the main loop. MS_PER_UPDATE: %f", FEATHER_UPDATE_AMOUNT);

    tLast = time(NULL);

    emscripten_helper_struct mainLoopArg = {
        .tCurrent = tCurrent,
        .tDelay = tDelay,
        .tLast = tLast,
        .tSleep = tSleep,
        .tRun = tRun,
    }

    emscripten_set_main_loop_arg(errMainLoop, mainLoopArg, 0, true);
}

#endif

tEngineError errEngineInit(tRuntime *tRun) { 
    // SDL environment is initialized here.
    if (SDL_Init( FEATHER_SDL_INIT ) < 0)
        return -errSDL_ERR;

    if (IMG_Init( FEATHER_TEXTURE_FORMAT ) < 0)
        return -errSDL_ERR;

    // Running user configuration here. 
    if (vRuntimeConfig)
        vRuntimeConfig(tRun); // This function shall be provided by user.
    else
        vFeatherLogWarn("Runtime configuration not provided. Default configuration will be used.");
 
    // Without a single scene, runtime shall abort.
    if (tRun->sScene == NULL) 
        return -errNO_SCENE;
    vFeatherLogInfo("Starting scene: <%s>", tRun->sScene->sName);

    // Creating the default window. Can be changed in 'vRuntimeConfig' 
    tRun->wRunWindow = SDL_CreateWindow(
        tRun->cMainWindowName,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        640, 
        480,
        __FEATHER_SDL_WINDOW_FLAGS
    );

    if (tRun->wRunWindow == NULL)
        return -errSDL_ERR;

    tRun->sdlRenderer = SDL_CreateRenderer(tRun->wRunWindow, -1, SDL_RENDERER_ACCELERATED);

    if (tRun->sdlRenderer == NULL)
        return -errSDL_ERR;

#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);                              // Updates synchronized with vertical retrace. 

    SDL_GL_CreateContext(tRun->wRunWindow);
/*     vFeatherLogInfo("Using GL version: %s", glGetString(GL_VERSION)); */
#endif
   
    // Sorting all appended layers.
    tll_sort(tRun->sScene->lLayers, bLayerCmp);

    // Appending all resources to the list.
    for (double dResCnt = 0; dResCnt < __COUNTER__; ++dResCnt) {
        vFeatherLogDebug("Constructing resource: %d", dResCnt);
        /* void* vResPtr = RES_ ## dResCnt ## constructor((void*) tRun); 
        tll_push_front(tRun->lResources, vResPtr); */
    }

    vFeatherLogDebug("Entering the initialization function.");

    return 0;
}

/* 
 *  @brief - swaps the current scene to another one.
 *
 *  The previous scene will be saved in the same state it was left. After this command the layers
 *  from the new scene are going to be scheduled. Initialization layers are not returned to the initial
 *  state after the swap, so they must be reset manually before calling this function.
 * */
void vRuntimeSwapScene(tRuntime *tRun, tScene *tSc) {
    tRun->sScene = tSc;
}

tEngineError errEngineInputHandle(tRuntime *tRun) {
    //vFeatherLogDebug("Entering the input handler function");
    SDL_Event sdlEvent;

    while (SDL_PollEvent( &sdlEvent )) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                vFeatherLogInfo("Exiting...");
                vFeatherExit(0, tRun);
            default:
                // Marking all handler function to invoke on update.
                tll_foreach(tRun->sScene->lControllers, c) {
                    if (!c->item.invoke) {
/*                         vFeatherLogInfo("%d, %d", c->item.sdlEventType, sdlEvent.type); */
                        c->item.invoke = c->item.sdlEventType == sdlEvent.type;
                    }
                }
        }
    }

    return 0;
}

tEngineError errEngineUpdateHandle(tRuntime *tRun) {
    //vFeatherLogDebug("Entering the update function");
    
    // Running all controller handler functions.
    tll_foreach(tRun->sScene->lControllers, c) {
        if (c->item.invoke) {
            c->item.fHandler(tRun);
            c->item.invoke = false;
        }
    }

    // Iterating over each user defined layer and updating the application logic.
    tll_foreach(tRun->sScene->lLayers, l) {
        if (l->item.iPriority)
            l->item.fRun(tRun);

        if (l->item.iPriority < 0)
            l->item.iPriority++;
    } 

    return 0;
}

tEngineError errEngineRenderHandle(tRuntime *tRun, double dDelay) {
    //vFeatherLogDebug("Entering the rendering function with delay: %f", dDelay);
    SDL_RenderClear(tRun->sdlRenderer);

    // Drawing all rect objects to the screen.
    tll_foreach(tRun->sScene->lRects, rect) {
        vDrawRect(tRun, (tRect*)rect);
    }

    SDL_RenderPresent(tRun->sdlRenderer);
    return 0;
}

/* 
 * @brief - exit the engine's runtime with some status.
 * */
void vFeatherExit(tEngineError tStatus, tRuntime *tRun) {
    tll_free(tRun->sScene->lControllers);
    tll_free(tRun->sScene->lLayers);
    tll_free(tRun->sScene->lRects);
    tll_free(tRun->lResources);

    SDL_Quit();
    exit(tStatus);
}

/* 
 *  @brief - changes the screen's title. Can be used any time during the runtime.
 * */
void vRuntimeSetWindowTitle(tRuntime *tRun, char* sTitle) {
    SDL_SetWindowTitle(tRun->wRunWindow, sTitle);
}


/* 
 *  @brief - gets the dimensions of the current running window.
 * */
void vRuntimeGetWindowDimensions(tRuntime *tRun, int *w, int *h) {
    SDL_GetWindowSize(tRun->wRunWindow, w, h);
}
