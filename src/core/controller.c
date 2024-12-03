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

#include <tllist.h>
#include <controller.h>

tController tControllerInit(SDL_EventType sdlEventType, void (fHandler)(void*)) {
    return (tController) {
        .sdlEventType = sdlEventType,
        .fHandler = fHandler,
        .invoke = false,
    };
}

/* 
 *  @brief - append handler function for the keyboard controller on press event.  
 * */
void vKeyboardOnPress(tKeyboardController* tKeyboardCtrl, SDL_Keysym sdlKey, void (*fHanler)(void *uData)) {
    tKeyboardHandlerPair tKHP = { sdlKey, fHanler };
    tll_push_front(tKeyboardCtrl->lOnPress, tKHP);
}

/* 
 *  @brief - append handler function for the keyboard controller on release event.  
 * */
void vKeyboardOnRelease(tKeyboardController* tKeyboardCtrl, SDL_Keysym sdlKey, void (*fHanler)(void *uData)) {
    tKeyboardHandlerPair tKHP = { sdlKey, fHanler };
    tll_push_front(tKeyboardCtrl->lOnRelease, tKHP);
}

/* 
 *  @brief - append handler function for the keyboard controller on click event.  
 * */
void vKeyboardOnClick(tKeyboardController* tKeyboardCtrl, SDL_Keysym sdlKey, void (*fHanler)(void *uData)) {
    tKeyboardHandlerPair tKHP = { sdlKey, fHanler };
    tll_push_front(tKeyboardCtrl->lOnClick, tKHP);
}
