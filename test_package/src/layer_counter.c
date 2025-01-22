/**************************************************************************************************
 *  File: layer_counter.c
 *  Desc: This example creates a single layer counter that will be incremented on each game update cycle. The
 *  counter is then printed to the console via logging API.
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

#include <feather.h>
#include <stdint.h>

// Creating a new scene.
FEATHER_SCENE(Scene1);

// Here we append the scene to runtime, so we won't obtain an error.
void cfg(tRuntime *tRun) {
    tRun->sScene = &Scene1;
    tRun->cMainWindowName = "Counter Example";
}

/* Here we define a layer. It will be executed each time the main loop enters the update phase. */
FEATHER_LAYER(&Scene1, 1, LocalCounterIncrementationLayer, // The name is provided here.  
    // Local structure declaration. Basically any C code can be defined here.
    struct {
        uint8_t counter;
    } myLocalStruct;
    const char MY_LOCAL_CONST[] = "USER";
    ,{
        tRuntime *tRun = tThisRuntime();

        vFeatherSleepThisLayerMs(tRun, 1000) {
            myLocalStruct.counter++;
            vFeatherLogInfo("Hello %s.Incrementing local counter: %d", MY_LOCAL_CONST, myLocalStruct.counter);

            if (myLocalStruct.counter > 10)
                vFeatherExit(0, tRun);
        }
    }
);

// Here we configure the runtime, so we wont obtain an error.
RUNTIME_CONFIGURE(cfg);
