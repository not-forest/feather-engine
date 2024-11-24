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

#include <cglm/cglm.h>

#include "intrinsics.h"
#include "context2d.h"

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
    glm_translate(tCtx->m4UniformMatrix, (vec3){ tCtx->fX, tCtx->fY, 0.f});
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
    glm_scale(tCtx->m4UniformMatrix, (vec3){ fDsx, fDsy, 1.0f });
}

/* 
 *  @brief - rotates the context based on the provided rotation angle.
 *
 *  @tCtx - object's context.
 *  @tDr  - delta angle in radians.
 * */
void vContextRotate(tContext2D *tCtx, float fDr) {
    tCtx->fRotation += fDr;
    glm_rotate_z(tCtx->m4UniformMatrix, fDr, tCtx->m4UniformMatrix);
}

/* 
 *  @brief - transforms the context based on the provided transformation matrix.
 *
 *  @tCtx               - object's context.
 *  @m4TransformMatrix  - custom tranformation matrix to apply. 
 * */
void vApplyMatrix(tContext2D *tCtx, mat4 m4TransformMatrix) {
    glm_mat4_mul(tCtx->m4UniformMatrix, m4TransformMatrix, tCtx->m4UniformMatrix);
}
