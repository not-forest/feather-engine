/**************************************************************************************************
 *  File: controller.h
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

#pragma once

#include <SDL_keyboard.h>
#ifndef FEATHER_CONTROLLER_H
#define FEATHER_CONTROLLER_H

#include <stdbool.h>
#include <intrinsics.h>
#include <tllist.h>


struct tController;
typedef void (*fHandler)(void *tRun, struct tController *tCtrl);

/* 
 *  @brief - controller structure that allows to define handler function for keyboard
 *  input event.
 *
 *  @fHnd           - handler function for the specified event.
 *  @vUserData      - pointer to user data used within the handler.
 *  @sdlEvent       - specified event on which the handler function shall be invoked.
 *  @uControllerID  - identifier of this controller.
 *  @uDelay         - used by runtime to allow delays between controllers.
 *  @invoke         - inner flag used to identify invoked controllers.
 *
 *  Implementeation must ensure, that the user data still exists, while the controller is added to
 *  the runtime environment.
 * */
typedef struct {
    fHandler fHnd;
    void *vUserData;
    SDL_EventType sdlEventType;
    uint32_t uControllerID;
    SDL_Event sdlEvent;
    uint32_t uDelay, uControllerLastCalled;
    bool invoke;
} tController;

/* 
 *  @brief - list of controllers.
 * */
typedef tll(tController) tControllerList;
typedef struct { fHandler fHnd; SDL_Keycode sdlKey; } fKeyboardPair; 
typedef tll(fKeyboardPair) fKeyboardHandlerPairList;

/* 
 *  @brief - convenient controller for handling keyboard input.
 *
 *  @tKeyboardController - holds an ID of this keyboard controller handler function.
 *
 *  Note that several functions can be hooked up to use for the same key. To rebind, previously created controllers
 *  must be cleaned up.
 * */
typedef struct {

    struct {
        uint32_t uDown;
        uint32_t uUp;
    } tCtrlPair;

    struct {
        fKeyboardHandlerPairList sdlPressed;
        fKeyboardHandlerPairList sdlReleased;
    } tKeybHndPair;

} tKeyboardController;

/* 
 *  @brief - shorthand macro to cast the function to fHandler.
 * */
#define fControllerHandler(fHnd) (fHandler)&fHnd

void __vKeyboardControllerHandlerFn(void *vRun, tController *tCtrl);

/* 
 *  @brief - returns true if the provided key was pressed at the time of function call.
 * */
bool bFeatherIsKeyPressed(SDL_Scancode sdlKey);

#endif
