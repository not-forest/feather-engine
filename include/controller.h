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

#ifndef FEATHER_CONTROLLER_H
#define FEATHER_CONTROLLER_H

#include <stdbool.h>
#include <intrinsics.h>
#include <tllist.h>

/* 
 *  @brief - controller structure that allows to define handler function for keyboard
 *  input event.
 *
 *  @fHandler - handler function for the specified event.
 *  @sdlEvent - specified event on which the handler function shall be invoked.
 * */
typedef struct {
    void (*fHandler)(void *tRun);
    SDL_EventType sdlEventType;
    bool invoke;
} tController;


/* 
 *  @brief - list of controllers.
 * */
typedef tll(tController) tControllerList;

#endif
