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
#include <runtime.h>
#include <time.h>

FEATHER_SCENE(BirdGame);

void cfg(tRuntime *tRun) {
    tRun->sScene = &BirdGame;
    tRun->cMainWindowName = "Flappy Bird";
}

RUNTIME_CONFIGURE(cfg)

static double dBackgroundOffset = 0;
static struct {
    tRect *uTopTubeRects[3], *uBottomTubeRects[3];
    int uTubeOffsetVals[3];
} Tubes = { .uTubeOffsetVals = {1000, 1500, 2000} };

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
    vPhysicsInit(tRun, &Bird.tBirdPhysics, Bird.tRect, DYNAMIC, 0);
    vPhysicsSetDelay(tRun, &Bird.tBirdPhysics, 5);

    tForce tGForce = { .x = 0, .y = 1, .dSpeed = 5, .iTimes = -1 };
    vPhysicsApplyForce(&Bird.tBirdPhysics, tGForce);
});

/* Importing animation. */
FEATHER_LAYER(&BirdGame, iPerformNTimes(1), GameAnimationsInitialize,
#define BIRD_FLIGHT_ANIMATION (uint8_t[]){0, 1, 2, 3}
,{
    srand(time(NULL));

    for (int i = 0; i < 8; ++i) // Eight same animations with different bird colors.
        vRectAppendAnimation(Bird.tRect, BIRD_FLIGHT_ANIMATION, 4);
});

/* Updates the animation */
FEATHER_LAYER(&BirdGame, 1, GameUpdate,, {
    tRuntime* tRun = tThisRuntime();
    vAnimateFrame(tRun, Bird.tRect, 0, 100);
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

/* Spawns three set of tubes, for moving. */
FEATHER_LAYER(&BirdGame, iPerformNTimes(1), SpawnTubes,
    tPhysController tTubePhys[6];
,{
    tRuntime* tRun = tThisRuntime();
    tContext2D tTubeCtx = tContextInit();

    for (int i = 0; i < 3; ++i) {
        tTubeCtx.fX = Tubes.uTubeOffsetVals[i];
        tTubeCtx.fY = rand() % 500 - 650;
        tRect *tRctTop = tInitRect(tRun, tTubeCtx, 1, "assets/flappy_tubes.png");
        vFullScreenRectHeight(tRctTop, tRun);
        vRectIndexate(tRctTop, 0, 89, 526);

        tTubeCtx.fY += tRctTop->tFr.uHeight + 200;
        tRect *tRctBottom = tInitRect(tRun, tTubeCtx, 1, "assets/flappy_tubes.png");
        vFullScreenRectHeight(tRctBottom, tRun);
        vRectIndexate(tRctBottom, 1, 89, 526);

        Tubes.uTopTubeRects[i] = tRctTop;
        Tubes.uBottomTubeRects[i] = tRctBottom;

        vPhysicsInit(tRun, &tTubePhys[i], Tubes.uTopTubeRects[i], STATIC, 0); 
        vPhysicsInit(tRun, &tTubePhys[5 - i], Tubes.uBottomTubeRects[i], STATIC, 0); 
        vPhysicsSetDelay(tRun, &tTubePhys[i], 20);
        vPhysicsSetDelay(tRun, &tTubePhys[5 - i], 20);
    }
});

FEATHER_LAYER(&BirdGame, 1, MoveTubes,, {
    tRuntime* tRun = tThisRuntime();
    vFeatherSleepThisLayerMs(tRun, 10) {
        for (int i = 0; i < 3; ++i) {
            int x = Tubes.uTubeOffsetVals[i] -= 10;
            Tubes.uTopTubeRects[i]->tCtx.fX = x;
            Tubes.uBottomTubeRects[i]->tCtx.fX = x;

            if (x <- 300) {
                uint32_t uTubeHeight = Tubes.uTopTubeRects[i]->tFr.uHeight;
                Tubes.uTubeOffsetVals[i] = 1000;
                Tubes.uTopTubeRects[i]->tCtx.fY = rand() % uTubeHeight/2. - uTubeHeight;
                Tubes.uBottomTubeRects[i]->tCtx.fY = 
                    Tubes.uTopTubeRects[i]->tCtx.fY + uTubeHeight + rand() % 300 + 100;
            }
        }

        if (bPhysicsCurrentlyCollides(&Bird.tBirdPhysics)) {
            tContext2D tCtx = tContextInit();
            tRect *tGameOver = tInitRect(tRun, tCtx, 1, "assets/flappy_gameover.png");

            tForce tGOForce = { .x = -1, .y = 2, .iTimes = -1, .dSpeed = 5 };
            vPhysicsApplyForce(&Bird.tBirdPhysics, tGOForce);
            vContextRotate(&Bird.tRect->tCtx, 1);
            vFullScreenRect(tGameOver, tRun);
        }
    }
});

FEATHER_LAYER(&BirdGame, 1, MoveBackground,,{
    tRuntime* tRun = tThisRuntime();
    vFeatherSleepThisLayerMs(tRun, 10) {
        dBackgroundOffset -= 5;
        for (int i = 0; i < 3; ++i) {
            tContext2D *tBgCtx = &tRctBg[i]->tCtx;
            tBgCtx->fX = tRctBg[i]->tFr.uWidth * tBgCtx->fScaleX * i + dBackgroundOffset; 
        }
        if (dBackgroundOffset < -465)
            dBackgroundOffset = 0;
    }
})

/* Handling space event. */
void vFly(void *vRun, tController *tCtrl) {
    tForce tF1 = { .x = 0, .y = -1, .iTimes = 20, .dSpeed = 10 };
    tForce tF2 = { .x = 0, .y = -1, .iTimes = 10, .dSpeed = 5 };
    vPhysicsApplyForce(&Bird.tBirdPhysics, tF1);
    vPhysicsApplyForce(&Bird.tBirdPhysics, tF2);
}
