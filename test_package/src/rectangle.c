/**************************************************************************************************
 *  File: rectange.c
 *  Desc: Shows how to create a single rectange with given texture and color. Engine then handles the
 *  with the drawing logic.
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
#include <rect.h>

// Creating a new scene.
FEATHER_SCENE(Scene1);

// Here we append the scene to runtime, so we won't obtain an error.
void cfg(tRuntime *tRun) {
    tRun->sScene = &Scene1;
    tRun->cMainWindowName = "Rectangle example";
}

// This layer just creates a new rectange instance.
FEATHER_LAYER(&Scene1, RectangeLayer,
    ,{
        tContext2D ctx = tContextInit();                        // Creating a new 2D contexts.
        tRuntime* this_runtime = tThisRuntime();                // Obtaining the current runtime.
        tRect* rect = tInitRect(this_runtime, ctx, 0, NULL);    // Creating new rect.
        rect->tCtx.fX += 1;
    }
);

RUNTIME_CONFIGURE(cfg)
