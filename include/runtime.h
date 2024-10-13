/**************************************************************************************************
 *  File: runtime.h
 *  Desc: Engine's runtime functions and data types. Runtime provides full abstraction over the main game
 *  loop, scheduling game tasks, handling graphics, physics, timings and can be seen as a core element of
 *  the whole engine structure.
 *
 *  
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

#include <stdint.h>
#include "err.h"

#include "scene.h"
#include "intrinsics.h"

/* 
 *  @brief - engine's runtime datatype structure.
 *
 *  @sScene - currently used scene. 
 *  @uFps - amount of frames per second in which all layers within the scene should be scheduled.
 *
 *  Defines the current active scene, processes the input, schedules all layers within that scene and
 *  renders the graphics.
 * */
typedef struct {
    tFPS uFps;
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
tEngineError errMainLoop(tRuntime *tRun);

/* 
 *  @brief - to not create custom runtime, this function can be used to modify the runtime before entering the
 *  main loop.
 *
 *  Note that runtime can also be modified during the main loop.
 * */
void vRuntimeConfig(tRuntime *tRun);

/* 
 *  @brief - default runtime value. Can be used and modified later.
 * */
#define DEFAULT_RUNTIME()           \
    (tRuntime) {                    \
        .uFps = 60,                 \
        .sScene = NULL,             \
    };
