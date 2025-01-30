/**************************************************************************************************
 *  File: physics.c
 *  Desc: Simple physics controller.
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
#include "scene.h"
#include "tllist.h"
#include <SDL_events.h>
#include <feather.h>
#include <math.h>
#include <physics.h>

/* 
 *  @brief - handles the physical layer of the application for the chosen entity.
 *
 *  @tRun               - currently running runtime.
 *  @tPhys              - uninitialized physics controller. 
 *  @tRct               - rect, which should be affected by physics.
 *  @eBodyType          - physical body type. Can be changed on run.
 *  @uCollidersGroup    - group number, in which collisions are managed.
 * */
void vPhysicsInit(tRuntime *tRun, tPhysController *tPhys, tRect *tRct, ePhysicalBodyType eBodyType, uint32_t uCollidersGroup) {
    tController *tCtrl;

    *tPhys = (tPhysController){
        .tRct = tRct,
        .eBodyType = eBodyType,
        .eGravityDir = BOTTOM,
        // Initial state of object is unchanged. Can be manually manipulated within the user space.
        .tAdditionalForces = tll_init(),
        .lCurrentlyCollides = tll_init(),
        .uCollidersGroup = uCollidersGroup,
    };

    tPhys->uCtrlId = tControllerInit(tRun, SDL_USEREVENT, tPhys, fControllerHandler(__vPhysicsControllerInternal));
    tCtrl = tControllerGet(tRun, tPhys->uCtrlId);
    tCtrl->invoke = true; // Physics controllers always invoke themselves.

    tColliderLabel tClbl = { 
        .x = tRct->tCtx.fX, 
        .y = tRct->tCtx.fY,
        .uCollidersGroup = uCollidersGroup,
        .w = tRct->tCtx.fScaleX * tRct->tFr.uWidth,
        .h = tRct->tCtx.fScaleY * tRct->tFr.uHeight,
        .uColliderId = tPhys->uCtrlId,
    };
    vFeatherLogInfo("Appending new collider for the current scene: %d", tClbl.uColliderId);
    tll_push_front(tRun->sScene->lColliders, tClbl);
}

/* 
 *  @brief - allows to set the delay between physics controller calls.
 *
 *  @tRun   - currently running runtime.
 *  @tPhys  - uninitialized physics controller. 
 *  @dDelay - amount of delay time between controller calls in milis.
 * */
void vPhysicsSetDelay(tRuntime *tRun, tPhysController *tPhys, double dDelay) {
    tController *tCtrl;
    tCtrl = tControllerGet(tRun, tPhys->uCtrlId);
    tPhys->uDelay = dDelay;
    tCtrl->uDelay = dDelay;
}

int checkCollision(tColliderLabel a, tColliderLabel b) {
    vFeatherLogInfo("%f, %f, %f, %f", a.x, b.x, a.y, b.y);
    return !(a.x + a.w <= b.x ||  // A is completely to the left of B
             a.x >= b.x + b.w ||  // A is completely to the right of B
             a.y + a.h <= b.y ||  // A is completely above B
             a.y >= b.y + b.h);   // A is completely below B
}


void __vPhysicsControllerInternal(void *vRun, tController *tCtrl) {
    tCtrl->invoke = true; // self invoked.
    tRuntime *tRun = (tRuntime*)vRun;
    tPhysController *tPhys = (tPhysController*)tCtrl->vUserData;
    tRect *tRct = tPhys->tRct;

    tColliderLabel tCol;
    tll_foreach(tRun->sScene->lColliders, tData) {
        tCol = tData->item;
        if (tData->item.uColliderId == tPhys->uCtrlId) {
            tData->item.x = tPhys->tRct->tCtx.fX;
            tData->item.y = tPhys->tRct->tCtx.fY;
            tData->item.w = tPhys->tRct->tCtx.fScaleX * tPhys->tRct->tFr.uWidth;
            tData->item.h = tPhys->tRct->tCtx.fScaleY * tPhys->tRct->tFr.uHeight;
            break;
        }
    }

    switch (tPhys->eBodyType) {
        case DYNAMIC:
            // Each additional supplied force is applied to the rect and removed.
            tll_foreach(tPhys->tAdditionalForces, tF) {
                vApplyForce(tRct, &tF->item);
                if (tF->item.iTimes == 0)
                    tll_remove(tPhys->tAdditionalForces, tF);
                else if (tF->item.iTimes > 0)
                    tF->item.iTimes--;
            }

        case STATIC:
            tll_foreach(tRun->sScene->lColliders, tData) {
                tColliderLabel tCol2 = tData->item;
                if (tCol2.uCollidersGroup == tPhys->uCollidersGroup) {
                    if (tCol2.uColliderId != tCol.uColliderId && checkCollision(tCol, tCol2)) {
                        tll_push_front(tPhys->lCurrentlyCollides, tCol2);
                    }
                }
            }
        case COLLIDER:
            break;
    }
}

/* 
 *  @brief - applies the force to the physical body, handled by physics controller.
 *
 *  @tPhys  - uninitialized physics controller. 
 *  @tF     - force to apply.
 *
 *  The force is applied once, and removed after the first controller update.
 * */
void vPhysicsApplyForce(tPhysController *tPhys, tForce tF) {
    tll_push_front(tPhys->tAdditionalForces, tF);
}

/* 
 *  @brief - applies the force to the provided rect.
 * */
void vApplyForce(tRect *tRct, tForce *tFc) {
    tRct->tCtx.fX += tFc->x * tFc->dSpeed;
    tRct->tCtx.fY += tFc->y * tFc->dSpeed;
}

/* 
 *  @brief - appends the forwarded force to the main force.
 *
 *  @tMainForce         - force that will be mutated by forwarded force.
 *  @tForwardedForce    - force that will mutate the main force *added*.
 *
 *  Note that the forwarded force wont be mutated at all after this operation.
 * */

void vAppendForce(tForce *tMainForce, tForce *tForwardedForce) {
    double dDotProd = tMainForce->x * tForwardedForce->x + tMainForce->y * tForwardedForce->y;

    tMainForce->dSpeed = sqrt(pow(tMainForce->dSpeed, 2) + 
                              pow(tForwardedForce->dSpeed, 2) + 
                              2 * tMainForce->dSpeed * tForwardedForce->dSpeed * dDotProd);

    if (tMainForce->dSpeed > tMainForce->dMaxSpeed)
        tMainForce->dSpeed = tMainForce->dMaxSpeed;

    vFeatherLogInfo("%f", tMainForce->dSpeed);
}

/* 
 *  @brief - returns true of something collides with a collider.
 * */
bool bPhysicsCurrentlyCollides(tPhysController *tPhys) {
    return tll_length(tPhys->lCurrentlyCollides) > 0;
}
