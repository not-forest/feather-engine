/**************************************************************************************************
 *  File: context2d.h
 *  Desc: The context for 2D objects.
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

#ifndef FEATHER_CONTEXT2D_H
#define FEATHER_CONTEXT2D_H

#include <intrinsics.h>

/* 
 *  @brief - 2D context, which describes the current location of the object  
 *
 *  Defines the location of the object within the 2D space, it's rotation and scale. The context allows
 *  to simplify the description of the object or entity and manipulate on it's graphical representation
 *  without calling internal graphics library.
 *
 *  Simplifies the process of modifying object without having to deal with graphics library from the user's
 *  side. Game controllers are straightly connected to context and completely hide all graphical logic.
 *  
 *  @uX, uY             - coordinates of the object.
 *  @fScaleX, fScaleY   - scale scalar of the object.
 *  @fRotation          - rotation in radians of the object.
 * */
typedef struct {
    tGameUnit fX, fY;
    double fScaleX, fScaleY;
    double fRotation;
} tContext2D;

/* 
 *  @brief - Translates the context to some provided delta values.
 *
 *  @tCtx - object's context.
 *  @uDx - delta X direction (horizontal).
 *  @uDy - delta Y direction (vertical).
 *
 *  When applying the transformation, higher Y values will correspond to moving object higher up. 
 * */
void vContextTranslate(tContext2D *tCtx, tGameUnit fDx, tGameUnit fDy);

/* 
 *  @brief - scales the context based on the provided scalar values.
 *
 *  @tCtx - object's context.
 *  @fDsx - X axis scale factor.
 *  @fDsy - Y axis scale factor.
 * */
void vContextScale(tContext2D *tCtx, float fDsx, float fDsy);

/* 
 *  @brief - rotates the context based on the provided rotation angle.
 *
 *  @tCtx - object's context.
 *  @tDr  - delta angle in radians.
 * */
void vContextRotate(tContext2D *tCtx, float fDr);

/* 
 *  @brief - allows to construct the context based on the provided arguments.
 *
 *  Allows to create a context by providing up to 5 variables. When some of variables are not
 *  provided, it will use the default values for them. The variables shall be provided in the
 *  order below:
 *  
 *  @uX, uY             - coordinates of the object. Defaults to (0, 0) if not provided.
 *  @fScaleX, fScaleY   - scale scalar of the object. Defaults to (1.f, 1.f) if not provided.
 *  @fRotation          - rotation in radians of the object. Defaults to 0.f if not provided.
 * */
#define tContextInit()                              \
    (tContext2D) {                                  \
        .fX = 0,                                    \
        .fY = 0,                                    \
        .fScaleX = 1.,                              \
        .fScaleY = 1.,                              \
        .fRotation = 0.,                            \
    }

#endif
