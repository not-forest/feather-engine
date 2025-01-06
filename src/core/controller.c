/**************************************************************************************************
 *  File: controller.c
 *  Desc: Allows to define some unique features to different scene objects based on the user input.
 *  By binding the controller to any structure, a set of functions can be defined on each pressed key,
 *  mouse movement, joystick, etc.
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

#include "log.h"
#include "tllist.h"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <stdint.h>
#include <controller.h>
#include <runtime.h>

static uint32_t CONT_COUNTER = 0;
/* 
 *  @brief - create a new controller.
 *
 *  @tRun           - currently running runtime.
 *  @vUserData      - pointer to user data used within the handler.
 *  @sdlEventType   - any event from the SDL library.
 *  @fHnd           - pointer to the declared handler function of type (void*)(void*,tController*)
 *
 *  @return - returns an id of the new controller.
 * */
uint32_t tControllerInit(tRuntime *tRun, SDL_EventType sdlEventType, void *vUserData, fHandler fHnd) {
    uint32_t uCCounter = ++CONT_COUNTER;
    tController tC = {
        .sdlEventType = sdlEventType,
        .fHnd = fHnd,
        .vUserData = vUserData,
        .uControllerID = uCCounter,
        .invoke = false,
    };
    vSceneAppendController(tRun->sScene, tC);
    return uCCounter;
}

/* 
 *  @brief - gives a pointer to the controller based on the provided ID.
 *
 *  Will return NULL if no controller is found under such ID.
 * */
tController* tControllerGet(tRuntime *tRun, uint32_t uCtrlID) {
    tController* tCtrlPtr = NULL;

    tll_foreach(tRun->sScene->lControllers, tCtrl) {
        if (tCtrl->item.uControllerID == uCtrlID) {
            tCtrlPtr = &tCtrl->item;
        }
    }

    return tCtrlPtr;
}

/* 
 *  @brief - Initializes a keyboard controller with a new controller.
 *
 *  Make sure that the keyboard controller outlives the layer, in which it is used.
 * */
void vKeyboardControllerInit(tRuntime *tRun, tKeyboardController *tKeybCtrl) {
    tController *tCtrlDown, *tCtrlUp;

    tKeybCtrl->tCtrlPair.uDown = \ 
        tControllerInit(tRun, SDL_KEYDOWN, NULL, fControllerHandler(__vKeyboardControllerHandlerFn));
    tKeybCtrl->tCtrlPair.uUp = \ 
        tControllerInit(tRun, SDL_KEYUP, NULL, fControllerHandler(__vKeyboardControllerHandlerFn));
    tKeybCtrl->tKeybHndPair.sdlPressed = (fKeyboardHandlerPairList) tll_init();
    tKeybCtrl->tKeybHndPair.sdlReleased = (fKeyboardHandlerPairList) tll_init();
   
    tCtrlDown = tControllerGet(tRun, tKeybCtrl->tCtrlPair.uDown);
    tCtrlUp = tControllerGet(tRun, tKeybCtrl->tCtrlPair.uUp);
    if (tCtrlDown == NULL || tCtrlUp == NULL)
        vFeatherLogError("Inner controller not found. Unable to initialize");

    tCtrlDown->vUserData = tKeybCtrl;
    tCtrlUp->vUserData = tKeybCtrl;
}

/* 
 *  @brief - Handles keyboard events based on the handler function provided to the keyboard controller.
 * */
void __vKeyboardControllerHandlerFn(void *vRun, tController *tCtrl) {
    tKeyboardController* tKeybCtrl = (tKeyboardController*)tCtrl->vUserData;

    switch (tCtrl->sdlEvent.type) {
        case SDL_KEYDOWN:
            tll_foreach(tKeybCtrl->tKeybHndPair.sdlPressed, tPair)
                if (tPair->item.sdlKey == tCtrl->sdlEvent.key.keysym.sym && tCtrl->sdlEvent.key.repeat == 0)
                    tPair->item.fHnd(vRun, (struct tController*) tCtrl);
            break;
        case SDL_KEYUP:
            tll_foreach(tKeybCtrl->tKeybHndPair.sdlReleased, tPair)
                if (tPair->item.sdlKey == tCtrl->sdlEvent.key.keysym.sym && tCtrl->sdlEvent.key.repeat == 0)
                    tPair->item.fHnd(vRun, (struct tController*) tCtrl);
            break;
        default:
            break;
    }
}

/* 
 *  @brief - append handler function for the keyboard controller on press event.  
 * */
void vKeyboardOnPress(tKeyboardController* tKeyboardCtrl, SDL_Keycode sdlKey, fHandler fHnd) {
    fKeyboardPair fPair = { .fHnd = fHnd, .sdlKey = sdlKey };
    tll_push_front(tKeyboardCtrl->tKeybHndPair.sdlPressed, fPair);
}

/* 
 *  @brief - append handler function for the keyboard controller on release event.  
 * */
void vKeyboardOnRelease(tKeyboardController* tKeyboardCtrl, SDL_Keycode sdlKey, fHandler fHnd) {
    fKeyboardPair fPair = { .fHnd = fHnd, .sdlKey = sdlKey };
    tll_push_front(tKeyboardCtrl->tKeybHndPair.sdlReleased, fPair);
}

/* 
 *  @brief - returns true if the provided key was pressed at the time of function call.
 * */
bool bFeatherIsKeyPressed(SDL_Scancode sdlKey) {
    const Uint8* uKeyState = SDL_GetKeyboardState(NULL);
    return uKeyState[sdlKey];
}
