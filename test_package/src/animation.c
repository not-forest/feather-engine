/**************************************************************************************************
 *  File: animation.c
 *  Desc: Example of using animations within the Rect structure.
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
#include <layer.h>
#include <rect.h>
#include <runtime.h>

FEATHER_SCENE(Animation);

void cfg(tRuntime *tRun) {
    tRun->sScene = &Animation;
    tRun->cMainWindowName = "Animation example";
}

FEATHER_LAYER(&Animation, iPerformNTimes(1), AnimationExampleInit,
    tRect *tAnimatedSprites;
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    tRect* tBg = tInitRect(tRun, tCtx, 0, NULL);
    vChangeRectColor(tRun, tBg, (SDL_Color){0, 255, 255, 255});
    vFullScreenRect(tBg, tRun);

    tCtx.fScaleX = 10;
    tCtx.fScaleY = 10;
    tAnimatedSprites = tInitRect(tRun, tCtx, 1, "assets/animals.png"); 
    vRectIndexate(tAnimatedSprites, 0, 32, 32);  // Indexate values shall be chosen manually from different image assets.
});

FEATHER_LAYER(&Animation, iPerformNTimes(1), AppendAnimations,
#define ANIMATION_VECTOR (uint8_t[]){0 + 3*i, 1 + 3*i, 2 + 3*i}
,{
    for (uint8_t i = 0; i < 34; ++i)
        vRectAppendAnimation(tAnimatedSprites, ANIMATION_VECTOR, 3);
});

FEATHER_LAYER(&Animation, 1, UpdateAnimation,
    uint8_t uAnimationId = 0;    
,{
    tRuntime *tRun = tThisRuntime();
    vAnimateFrame(tRun, tAnimatedSprites, uAnimationId, 400);
});

FEATHER_LAYER(&Animation, 1, IncrementAnimationId,, {
    tRuntime *tRun = tThisRuntime();

    vFeatherSleepThisLayerMs(tRun, 2000) 
        if (++uAnimationId >= 34)
            uAnimationId = 0;
});

RUNTIME_CONFIGURE(cfg)
