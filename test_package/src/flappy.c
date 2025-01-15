/**************************************************************************************************
 *  File: flappy.c
 *  Desc: Implementation of a flappy bird clone, which shows how to use the Physics controller.
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

#include <context2d.h>
#include <feather.h>
#include <layer.h>
#include <physics.h>
#include <rect.h>
#include <runtime.h>

FEATHER_SCENE(BirdGame);

void cfg(tRuntime *tRun) {
    tRun->sScene = &BirdGame;
    tRun->cMainWindowName = "Flappy Bird";
}

RUNTIME_CONFIGURE(cfg)

static double dBackgroundOffset = 0;

FEATHER_LAYER(&BirdGame, iPerformNTimes(1), GameInitialize,
    void vFullScreen(void*,tController*);
    void vFly(void*,tController*);
    tRect *tRctBg[3];

    // Internal structure to operate the bird.
    struct {
        tKeyboardController tKeybCtrl;
        tPhysController tBirdPhysics;
        tRect *tRect;
    } Bird;
,{
    tContext2D tCtx = tContextInit();
    tRuntime* tRun = tThisRuntime();

    // Background is created in three same pieces.
    for (int i = 0; i < 3; ++i)
        tRctBg[i] = tInitRect(tRun, tCtx, 0, "assets/bird_bg.png");

    Bird.tRect = tInitRect(tRun, tCtx, 3, "assets/Birds.png");
    vRectIndexate(Bird.tRect, 0, 16, 16);
    vFullScreen(tRun, NULL);

    // Adjusting the whole contect scale to the fullscreen size.
    tControllerInit(tRun, SDL_WINDOWEVENT, NULL, fControllerHandler(vFullScreen));

    // Using keyboard controller to control bird's flight.
    vKeyboardControllerInit(tRun, &Bird.tKeybCtrl);
    vKeyboardOnPress(&Bird.tKeybCtrl, SDLK_SPACE, fControllerHandler(vFly));

    // Adding dynamic physics to the bird. This will cause bird to fall.
    Bird.tBirdPhysics.tGForce.dAccel = 5;
    Bird.tBirdPhysics.tGForce.dMaxSpeed = 10;
    vPhysicsInit(tRun, &Bird.tBirdPhysics, Bird.tRect, DYNAMIC, 0);
    vPhysicsSetDelay(tRun, &Bird.tBirdPhysics, 15);
});

/* Importing animation. */
FEATHER_LAYER(&BirdGame, iPerformNTimes(1), GameAnimationsInitialize,
#define BIRD_FLIGHT_ANIMATION (uint8_t[]){0, 1, 2, 3}
,{
    for (int i = 0; i < 8; ++i) // Eight same animations with different bird colors.
        vRectAppendAnimation(Bird.tRect, BIRD_FLIGHT_ANIMATION, 4);
});

/* Updates the animation */
FEATHER_LAYER(&BirdGame, 1, GameUpdate,, {
    tRuntime* tRun = tThisRuntime();
    vAnimateFrame(tRun, Bird.tRect, 0, 500);
});

/* Adjusts the size of the rects according to the proporsion of the screen. */
void vFullScreen(void *vRun, tController *tCtrl) {
    int w, h;
    tRuntime *tRun = (tRuntime*)vRun;

    // Adjust all content to match screen.
    vRuntimeGetWindowDimensions(tRun, &w, &h);
    Bird.tRect->tCtx.fScaleX = h/150.f;
    Bird.tRect->tCtx.fScaleY = h/150.f;
    Bird.tRect->tCtx.fX = w/4.f;

    // This allows the screen to move smoothly.
    for (int i = 0; i < 3; ++i) {
        tContext2D *tBgCtx = &tRctBg[i]->tCtx;
        vFullScreenRectHeight(tRctBg[i], tRun);
        tBgCtx->fX = tRctBg[i]->tFr.uWidth * tBgCtx->fScaleX * i + dBackgroundOffset; 
    }
}

/* Handling space event. */
void vFly(void *vRun, tController *tCtrl) {
    tForce tF = { .x = 0, .y = -1, .dSpeed = 10 };
    vPhysicsApplyForce(&Bird.tBirdPhysics, tF);
}
