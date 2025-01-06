/**************************************************************************************************
 *  File: movement.c
 *  Desc: Shows an example of a simple 2D rpg-like game with movement mechanics, conveniently
 *  utilizing the controller structure. All controllers are only defined within the scene, which means
 *  that they will have no effect in other scenes. Controllers can also be removed, if their ID is
 *  provided.
 *
 *  Assets: https://cupnooble.itch.io/sprout-lands-asset-pack
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
#include <controller.h>
#include <feather.h>
#include <runtime.h>

// Here we define two main scenes, that will be used within this small example.
FEATHER_SCENE(Menu);
FEATHER_SCENE(Game);

void cfg(tRuntime *tRun) {
    tRun->sScene = &Menu;
    tRun->cMainWindowName = "Game: (Menu)";
}

static tRect *BackGround = NULL;

static struct {
    tRect *tRct;
    tKeyboardController tKeybCtrl;
    tGameUnit velocityX, velocityY;
} Player = {
    .tRct = NULL,
    .velocityX = 0.f,
    .velocityY = 0.f,
};

/* Main menu layer: it will wait until any key is pressed and then change to the game. */
FEATHER_LAYER(&Menu, iPerformNTimes(1), MainMenuLayer,
    void vStartGame(void *vRun, tController* tCtrl);
    void vFullScreen(void *vRun, tController* tCtrl);
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    BackGround = tInitRect(tRun, tCtx, 0, "assets/MainMenu1.jpg");
    vFullScreenRect(BackGround, tRun);

    // Creating controller that waits for any key to be pressed.
    tControllerInit(tRun, SDL_KEYDOWN, NULL, fControllerHandler(vStartGame));
    // Added controller to adjust full screen to the main menu.
    tControllerInit(tRun, SDL_WINDOWEVENT, NULL, fControllerHandler(vFullScreen));
    vFeatherLogInfo("Main menu initialized successfully.");
});

/* Small animation for main menu button */
FEATHER_LAYER(&Menu, 1, MainMenuAnimate, bool flag, {
    tRuntime *tRun = tThisRuntime();
    vFeatherSleepLayerMs(tRun, MainMenuAnimate, 1000) {
        char *sMenuTexture = flag ? "assets/MainMenu1.jpg" : "assets/MainMenu2.jpg";

        if (BackGround != NULL)
        vChangeRectTexture(tRun, BackGround, sMenuTexture);
        flag = !flag;
    }
});

// Adjusting the main menu image to fullscreen.
void vFullScreen(void *vRun, tController* tCtrl) {
    tRuntime *tRun = (tRuntime*)vRun;
    vFullScreenRect(BackGround, tRun);
}

// Changing the scene.
void vStartGame(void *vRun, tController* tCtrl) {
    tRuntime *tRun = (tRuntime*)vRun;
    vRuntimeSwapScene(tRun, &Game);
    vRuntimeSetWindowTitle(tRun, "Game: (Game)");
    // Removing the controller to save RAM.
    vSceneRemoveController(tRun->sScene, tCtrl->uControllerID);
}
/***************************************************************************************/

/* Game handling layers. Those will be only executed once the game is on. */
FEATHER_LAYER(&Game, iPerformNTimes(1), InitGameLayer,
    void handle_w(void*, tController*);
    void handle_a(void*, tController*);
    void handle_s(void*, tController*);
    void handle_d(void*, tController*);
    void stop_ws(void*, tController*);
    void stop_ad(void*, tController*);

    void apply_speed(tGameUnit,tGameUnit)
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtxBg = tContextInit(), tCtxPlayer = tContextInit();

    // Low priority to make sure that background will be drawn before everything else.
    BackGround = tInitRect(tRun, tCtxBg, 0, "assets/static_grass_bg.png");
    vFullScreenRect(BackGround, tRun);
    Player.tRct = tInitRect(tRun, tCtxPlayer, 1, "assets/BasicCharacterSpriteSet.png");

    /* Initializing the keyboard controller. */
    vKeyboardControllerInit(tRun, &Player.tKeybCtrl);

    /* For each corresponding key, mapping the handler function. */
    vKeyboardOnPress(&Player.tKeybCtrl, SDLK_w, fControllerHandler(handle_w));
    vKeyboardOnPress(&Player.tKeybCtrl, SDLK_a, fControllerHandler(handle_a));
    vKeyboardOnPress(&Player.tKeybCtrl, SDLK_s, fControllerHandler(handle_s));
    vKeyboardOnPress(&Player.tKeybCtrl, SDLK_d, fControllerHandler(handle_d));
    /* Releasing shall be the opposite of pressing. */
    vKeyboardOnRelease(&Player.tKeybCtrl, SDLK_w, fControllerHandler(stop_ws));
    vKeyboardOnRelease(&Player.tKeybCtrl, SDLK_a, fControllerHandler(stop_ad));
    vKeyboardOnRelease(&Player.tKeybCtrl, SDLK_s, fControllerHandler(stop_ws));
    vKeyboardOnRelease(&Player.tKeybCtrl, SDLK_d, fControllerHandler(stop_ad));

    vFeatherLogInfo("Game loaded successfully");
});

FEATHER_LAYER(&Game, 1, UpdatePlayerMovement,,{
    if (Player.tRct != NULL) {
        tContext2D *tCtx = &Player.tRct->tCtx;
        vContextTranslate(tCtx, Player.velocityX, Player.velocityY);
    }
});

#define dV 2.f
#define Vmin -10.f
#define Vmax 10.f

void stop_ws(void *tRun, tController *tCtrl) {
    Player.velocityY = 0.f;
}
void stop_ad(void *tRun, tController *tCtrl) {
    Player.velocityX = 0.f;
}

void handle_w(void *tRun, tController *tCtrl) {
    apply_speed(0.f, -dV);
}
void handle_a(void *tRun, tController *tCtrl) { 
    apply_speed(-dV, 0.f);
}
void handle_s(void *tRun, tController *tCtrl) {
    apply_speed(0.f, dV);
}
void handle_d(void *tRun, tController *tCtrl) { 
    apply_speed(dV, 0.f);
}

void apply_speed(tGameUnit dX, tGameUnit dY) {
    tContext2D *tCtx = &Player.tRct->tCtx;
    tGameUnit cX = tCtx->fX, cY = tCtx->fY;

    if (Vmin < Player.velocityX + dX && Player.velocityX + dX < Vmax)
        Player.velocityX += dX;
    else
        Player.velocityX = 0.f;

    if (Vmin < Player.velocityY + dY && Player.velocityY + dY < Vmax)
        Player.velocityY += dY;
    else
        Player.velocityY = 0.f;
}

RUNTIME_CONFIGURE(cfg);
