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
#include "tllist.h"
#include <SDL_events.h>
#include <feather.h>
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
        .tGForce = { .x = 0, .y = 1, .dMaxSpeed = 1, .dSpeed = 0.0001, .dAccel = 9.8 },
        .tAdditionalForces = tll_init(),
        .uCollidersGroup = uCollidersGroup
    };

    tPhys->uCtrlId = tControllerInit(tRun, SDL_USEREVENT, tPhys, fControllerHandler(__vPhysicsControllerInternal));
    tCtrl = tControllerGet(tRun, tPhys->uCtrlId);
    tCtrl->invoke = true; // Physics controllers always invoke themselves.
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

void __vPhysicsControllerInternal(void *vRun, tController *tCtrl) {
    tCtrl->invoke = true; // self invoked.
    tPhysController *tPhys = (tPhysController*)tCtrl->vUserData;
    tRect *tRct = tPhys->tRct;

    switch (tPhys->eBodyType) {
        case DYNAMIC:
            // Updating the gravitational force each controller tick.
            vUpdateForce(&tPhys->tGForce);

            // Each additional supplied force is applied to the rect and removed.
            tll_foreach(tPhys->tAdditionalForces, tF) {
                vAppendForce(&tPhys->tGForce, &tF->item);
                tll_remove(tPhys->tAdditionalForces, tF);
            }

            vApplyForce(tRct, &tPhys->tGForce);
            break;
        case STATIC:
            break;
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
 *  @brief - updates force's bias based on current velocity and speed.
 *
 *  @tFc - pointer to the force, which shall be upgraded (for example once per game tick.)
 * */
void vUpdateForce(tForce *tFc) {
    if (tFc->dSpeed < tFc->dMaxSpeed)
        tFc->dSpeed *= tFc->dAccel;
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
    tMainForce->x += tForwardedForce->x;
    tMainForce->y += tForwardedForce->y;

    tMainForce->dSpeed += tForwardedForce->dSpeed;
    tMainForce->dAccel += tForwardedForce->dAccel;

    if (tMainForce->dSpeed > tMainForce->dMaxSpeed) {
        tMainForce->dSpeed = tMainForce->dMaxSpeed;
    }
}

