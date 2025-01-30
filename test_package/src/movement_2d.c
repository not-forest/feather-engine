/**************************************************************************************************
 *  File: movement.c
 *  Desc: Shows an example of a simple 2D rpg-like game with movement mechanics, conveniently
 *  utilizing the controller structure. All controllers are only defined within the scene, which means
 *  that they will have no effect in other scenes. Controllers can also be removed, if their ID is
 *  provided.
 *
 *  Assets: https://cupnooble.itch.io/sprout-lands-asset-pack
 *          https://freesound.org/people/Mrthenoronha/sounds/523725/
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

#include <audio_fn.h>
#include <context2d.h>
#include <controller.h>
#include <feather.h>
#include <rect.h>
#include <runtime.h>

// Here we define two main scenes, that will be used within this small example.
FEATHER_SCENE(Menu);
FEATHER_SCENE(Game);

void cfg(tRuntime *tRun) {
    tRun->sScene = &Menu;
    tRun->cMainWindowName = "Game: (Menu)";
}

static tRect *BackGround = NULL;

enum Direction { FRONT, BACK, LEFT, RIGHT };

static struct {
    tRect *tRct;
    tKeyboardController tKeybCtrl;
    tGameUnit velocityX, velocityY;
    enum Direction eDir;
} Player = {
    .tRct = NULL,
    .velocityX = 0.f,
    .velocityY = 0.f,
    .eDir = FRONT,
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
    uint32_t uStepSoundId;
    uint32_t u8BitMusicId;

    void handle_w(void*, tController*);
    void handle_a(void*, tController*);
    void handle_s(void*, tController*);
    void handle_d(void*, tController*);
    void stop_ws(void*, tController*);
    void stop_ad(void*, tController*);

    void apply_speed(tGameUnit,tGameUnit)
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();

    // Low priority to make sure that background will be drawn before everything else.
    BackGround = tInitRect(tRun, tCtx, 0, "assets/static_grass_bg.png");
    vFullScreenRect(BackGround, tRun);

    Player.tRct = tInitRect(tRun, tCtx, 1, "assets/BasicCharacterSpriteSet.png");
    vContextScale(&Player.tRct->tCtx, 5.f, 5.f);
    vRectIndexate(Player.tRct, 0, 48, 48);  // Indexate values shall be chosen manually from different image assets.

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

    /* Loading the stepping sound. */
    uStepSoundId = uLoadMixerSound(tRun, "assets/footstep.wav");
    u8BitMusicId = uLoadMixerMusic(tRun, "assets/movement_example_music8bit.wav");
    vPlayMusic(tRun, u8BitMusicId, -1); // Playing the music right away.

    vFeatherLogInfo("Game loaded successfully");
});

FEATHER_LAYER(&Game, iPerformNTimes(1), PlayerAnimationLoader,

#define ANIMATION_LENGTH 2
#define STAND_FRONT         (uint8_t[]){0, 1}
#define WALK_FRONT          (uint8_t[]){2, 3}
#define STAND_BACK          (uint8_t[]){4, 5}
#define WALK_BACK           (uint8_t[]){6, 7}
#define STAND_LEFT          (uint8_t[]){8, 9}
#define WALK_LEFT           (uint8_t[]){10, 11}
#define STAND_RIGHT         (uint8_t[]){12, 13}
#define WALK_RIGHT          (uint8_t[]){14, 15}

,{
    vRectAppendAnimation(Player.tRct, STAND_FRONT, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, WALK_FRONT, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, STAND_BACK, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, WALK_BACK, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, STAND_LEFT, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, WALK_LEFT, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, STAND_RIGHT, ANIMATION_LENGTH);
    vRectAppendAnimation(Player.tRct, WALK_RIGHT, ANIMATION_LENGTH);
});

FEATHER_LAYER(&Game, 1, UpdatePlayerMovement,

#define STAND_FRONT 0
#define WALK_FRONT  1
#define STAND_BACK  2
#define WALK_BACK   3
#define STAND_LEFT  4
#define WALK_LEFT   5
#define STAND_RIGHT 6
#define WALK_RIGHT  7

enum Direction eLastDir = FRONT;

,{
    tRuntime *tRun = tThisRuntime();
    tContext2D *tCtx = &Player.tRct->tCtx;

    vContextTranslate(tCtx, Player.velocityX, Player.velocityY);

    // For better animation responsibility when changing direction.
    if (eLastDir != Player.eDir)
        vRuntimeUnsleepCurrentLayer(tRun, true);

    // Updating the animation based on the direction.
    switch(Player.eDir) {
        case FRONT:
            if(Player.velocityY)
                vAnimateFrame(tRun, Player.tRct, WALK_FRONT, 200); 
            else
                vAnimateFrame(tRun, Player.tRct, STAND_FRONT, 700); 
            break;
        case BACK:
            if(Player.velocityY)
                vAnimateFrame(tRun, Player.tRct, WALK_BACK, 200); 
            else
                vAnimateFrame(tRun, Player.tRct, STAND_BACK, 700); 
            break;
        case LEFT:
            if(Player.velocityX)
                vAnimateFrame(tRun, Player.tRct, WALK_LEFT, 200); 
            else
                vAnimateFrame(tRun, Player.tRct, STAND_LEFT, 700); 
            break;
        case RIGHT:
            if(Player.velocityX)
                vAnimateFrame(tRun, Player.tRct, WALK_RIGHT, 200); 
            else
                vAnimateFrame(tRun, Player.tRct, STAND_RIGHT, 700); 
            break;
    }
    eLastDir = Player.eDir;
});

FEATHER_LAYER(&Game, 5, GameHandleSounds,, {
    if (Player.velocityX || Player.velocityY) {
        tRuntime *tRun = tThisRuntime();
        vQueueSound(tRun, uStepSoundId, 0);
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
    Player.eDir = BACK;
    apply_speed(0.f, -dV);
}
void handle_a(void *tRun, tController *tCtrl) { 
    Player.eDir = LEFT;
    apply_speed(-dV, 0.f);
}
void handle_s(void *tRun, tController *tCtrl) {
    Player.eDir = FRONT;
    apply_speed(0.f, dV);
}
void handle_d(void *tRun, tController *tCtrl) { 
    Player.eDir = RIGHT;
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
