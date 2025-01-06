/**************************************************************************************************
 *  File: context2d.c
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

#include "runtime.h"
#include <cglm/cglm.h>

#include <intrinsics.h>
#include <context2d.h>

/* 
 *  @brief - Translates the context to some provided delta values.
 *
 *  @tCtx - object's context.
 *  @uDx - delta X direction (horizontal).
 *  @uDy - delta Y direction (vertical).
 *
 *  When applying the transformation, higher Y values will correspond to moving object higher up. 
 * */
void vContextTranslate(tContext2D *tCtx, tGameUnit fDx, tGameUnit fDy) {
    tCtx->fX += fDx;
    tCtx->fY += fDy;
}

/* 
 *  @brief - scales the context based on the provided scalar values.
 *
 *  @tCtx - object's context.
 *  @fDsx - X axis scale factor.
 *  @fDsy - Y axis scale factor.
 * */
void vContextScale(tContext2D *tCtx, float fDsx, float fDsy) {
    tCtx->fScaleX *= fDsx;
    tCtx->fScaleY *= fDsy;
}

/* 
 *  @brief - rotates the context based on the provided rotation angle.
 *
 *  @tCtx - object's context.
 *  @tDr  - delta angle in radians.
 * */
void vContextRotate(tContext2D *tCtx, float fDr) {
    tCtx->fRotation += fDr;
}

/* 
 *  @brief - grows or shrinks rect's context to match with the full window size.
 *
 *  @tRct - object's rect.
 *  @tRun - current runtime.
 * */
void vFullScreenRect(tRect *tRct, tRuntime *tRun) {
    int w, h, wr, hr;
    vRuntimeGetWindowDimensions(tRun, &w, &h);
    SDL_QueryTexture((SDL_Texture*)tRct->idTextureID, NULL, NULL, &wr, &hr);
    tRct->tCtx.fScaleX = (float)w / (float)wr;
    tRct->tCtx.fScaleY = (float)h / (float)hr;
}
