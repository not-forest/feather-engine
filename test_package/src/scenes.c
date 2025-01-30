/**************************************************************************************************
 *  File: scenes.c
 *  Desc: Shows a way to combine several scene in order to properly arrange different parts of the
 *  application logic. This is also an example for MouseController.
 *
 *  Menu assets: https://applesauce-tree.itch.io/applesauces-beach-start-menu
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

#include <controller.h>
#include <feather.h>
#include <font.h>
#include <layer.h>
#include <rect.h>
#include <runtime.h>
#include <context2d.h>

FEATHER_SCENE(Menu);
FEATHER_SCENE(Settings);
FEATHER_SCENE(Start);

void cfg(tRuntime *tRun) {
    tRun->sScene = &Menu;
    tRun->cMainWindowName = "Game: (Menu)";
}

tMouseController tMCtrl;

/* Menu defines a list of buttons, which can be pressed to move between windows. */
FEATHER_LAYER(&Menu, iPerformNTimes(1), InitializeMenu,
    void vClickStart(void*,tController*);
    void vClickSettings(void*,tController*);
    void vClickExit(void*,tController*);
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    tRect *tBg = tInitRect(tRun, tCtx, 0, "assets/ScenesTestMenuBg.png");
    vFullScreenRect(tBg, tRun);
    
    int w,h;
    vRuntimeGetWindowDimensions(tRun, &w, &h);
    
    vContextTranslate(&tCtx, w/3., h/4.);
    vContextScale(&tCtx, 6, 6);
    tRect *tStartButton = tInitRect(tRun, tCtx, 1, "assets/MenuAssets.png");
    vRectIndexate(tStartButton, 0, 48, 16);
    vContextTranslate(&tCtx, 0, tStartButton->tFr.uHeight * tCtx.fScaleY);

    tRect *tSettingsButton = tInitRect(tRun, tCtx, 1, "assets/MenuAssets.png");
    vRectIndexate(tSettingsButton, 1, 48, 16);

    vContextTranslate(&tCtx, 0, tStartButton->tFr.uHeight * tCtx.fScaleY);
    tRect *tExitButton = tInitRect(tRun, tCtx, 1, "assets/MenuAssets.png");
    vRectIndexate(tExitButton, 2, 48, 16);
    
    vMouseControllerInit(tRun, &tMCtrl);

    vMouseOnPress(&tMCtrl, 1, tStartButton, fControllerHandler(vClickStart));
    vMouseOnPress(&tMCtrl, 1, tSettingsButton, fControllerHandler(vClickSettings));
    vMouseOnPress(&tMCtrl, 1, tExitButton, fControllerHandler(vClickExit));
});

/* Goes to the main application scene. */
void vClickStart(void *vRun, tController *tCtrl) {
    vRuntimeSwapScene((tRuntime*)vRun, &Start);
    vFeatherLogInfo("Going to the main app...");
}

/* Goes to the settings scene. */
void vClickSettings(void *vRun, tController *tCtrl) {
    vRuntimeSwapScene((tRuntime*)vRun, &Settings);
    vFeatherLogInfo("Going to settings...");
}

/* Exits the application on click. */
void vClickExit(void *vRun, tController *tCtrl) {
    vFeatherExit(0, (tRuntime*)vRun);
}

tMouseController tGoBackCtrl;

void vGoBack(void *vRun, tController *tRun) {
    vRuntimeSwapScene((tRuntime*)vRun, &Menu);
    vFeatherLogInfo("Going back");
}

/* Other scenes are defines just to show how you can go back and fourth between them. */
FEATHER_LAYER(&Start, iPerformNTimes(1), InitializeMainApp,
    tText tGoBackText;
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    tRect *tBg = tInitRect(tRun, tCtx, 0, "assets/SomeMainApp.jpg");
    vFullScreenRect(tBg, tRun);

    tTextInit(tRun, &tGoBackText, "Go Back To Main Menu", tCtx, "assets/FiraCode-Bold.ttf", 1);

    vMouseControllerInit(tRun, &tGoBackCtrl);
    tRect *tTextRect = tGetRect(tRun, tGoBackText.uRectID);

    vMouseOnPress(&tGoBackCtrl, 1, tTextRect, fControllerHandler(vGoBack));
});

FEATHER_LAYER(&Settings, iPerformNTimes(1), InitializeSettings,
    tText tGoBackText;
,{
    tRuntime *tRun = tThisRuntime();
    tContext2D tCtx = tContextInit();
    tRect *tBg = tInitRect(tRun, tCtx, 0, "assets/SettingsBackground.jpg");
    vFullScreenRect(tBg, tRun);

    tTextInit(tRun, &tGoBackText, "Go Back To Main Menu", tCtx, "assets/FiraCode-Bold.ttf", 1);

    vMouseControllerInit(tRun, &tGoBackCtrl);
    tRect *tTextRect = tGetRect(tRun, tGoBackText.uRectID);

    vMouseOnPress(&tGoBackCtrl, 1, tTextRect, fControllerHandler(vGoBack));
});

RUNTIME_CONFIGURE(cfg);
