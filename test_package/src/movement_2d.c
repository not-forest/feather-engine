/**************************************************************************************************
 *  File: movement.c
 *  Desc: Shows an example of a simple 2D rpg-like game with movement mechanics, conveniently
 *  utilizing the controller structure.
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
#include <controller.h>
#include <runtime.h>

// Here we define two main scenes, that will be used within this small example.
FEATHER_SCENE(Menu);
FEATHER_SCENE(Game);

void cfg(tRuntime *tRun) {
    tRun->sScene = &Menu;
    tRun->cMainWindowName = "Game: (Menu)";
}

// Creating mutual background resource
FEATHER_RESOURCE(tRect*, BackgroundImage, NULL, NULL);

/* Main menu layer: it will wait until any key is pressed and then change to the game. */
FEATHER_LAYER(&Menu, iPerformNTimes(1), MainMenuLayer,
    void vStartGame(void *vRun);
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    tRect *tRct = tInitRect(tRun, tCtx, 0, "assets/baboon.bmp");
    vFullScreenRect(tRct, tRun);

    // Creating controller that waits for any key to be pressed.
    uint32_t tCtrl_ID = tControllerInit(tRun, SDL_KEYDOWN, &vStartGame); 
    vFeatherLogInfo("Main menu initialized successfully.");
});

// Changing the scene.
void vStartGame(void *vRun) {
    tRuntime *tRun = (tRuntime*)vRun;
    vRuntimeSwapScene(tRun, &Game);
    vRuntimeSetWindowTitle(tRun, "Game: (Game)");
}
/***************************************************************************************/

/* Game handling layers. Those will be only executed once the game is on. */
FEATHER_LAYER(&Game, iPerformNTimes(1), InitGameLayer,,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    // Priority -1 to make sure that background will be drawn before everything else.
    tRect *tRct = tInitRect(tRun, tCtx, -1, "assets/baboon.bmp");
    vFullScreenRect(tRct, tRun);

    vFeatherLogInfo("Game loaded successfully");
});

FEATHER_LAYER(&Game, 1, UpdateGameLayer,, { 
    vFeatherLogInfo("Updating game...");
});

RUNTIME_CONFIGURE(cfg);
