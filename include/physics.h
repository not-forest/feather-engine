/**************************************************************************************************
 *  File: physics.h
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

#pragma once

#include "scene.h"
#ifndef FEATHER_PHYSICS_H
#define FEATHER_PHYSICS_H

#include <stdint.h>
#include <tllist.h>

/* 
 *  @brief - defines a force applied to some entity, which is a movement speed with acceleration and vector.
 *
 *
 *  @x,y        - vector directions and bias
 *  @dAccel     - acceleration for the physical object. Can be adjusted on run. Defaults to 9.8 GU/s².
 *  @dSpeed     - current speed of the entity.
 *  @dMaxSpeed  - maximal speed, which cannot be surpassed.
 * */
typedef struct {
    double x, y, dSpeed, dMaxSpeed;
    int32_t iTimes;
} tForce;

typedef tll(tForce) tForcesList;

#include <intrinsics.h>
#include <controller.h>
#include <rect.h>
#include <runtime.h>

/* 
 *  @brief - applies the force to the provided rect.
 * */
void vApplyForce(tRect *tRct, tForce *tFc) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - appends the forwarded force to the main force.
 *
 *  @tMainForce         - force that will be mutated by forwarded force.
 *  @tForwardedForce    - force that will mutate the main force *added*.
 *
 *  Note that the forwarded force wont be mutated at all after this operation.
 * */
void vAppendForce(tForce *tMainForce, tForce *tForwardedForce) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - defines all physical body types.
 *
 *  @DYNAMIC    - dynamic bodies are affected by internal gravity and also collide with all other bodies.
 *  @STATIC     - static bodies stay still, until moved by some other means (i.e keyboard controller). Dynamic bodies will collide with this one.
 *  @COLLIDER   - colliders wont be affected by gravity, but will collide with other dynamic and static objects.
 * */
typedef enum { DYNAMIC, STATIC, COLLIDER } ePhysicalBodyType;

/* 
 *  @brief - defines the direction in which the gravity shall be applied. 
 * */
typedef enum { 
    BOTTOM = 0x1, 
    TOP = 0x2, 
    LEFT = 0x4, 
    RIGHT = 0x8 
} eGravityDirection;

/* 
 *  @brief - simple physics controller that can be used to interact within the entities.
 *
 *  @uCtrlId            - ID value of the underlying controller.
 *  @tRct               - pointer to the rect entity, which will be affected by physics.
 *  @eBodyType          - physical type of this body. This defines a lot about the behavior of this entity
 *  @uCollidersGroup    - group in which this entity appears. Only entitties within the same group collide.
 *  @tAdditionalForces  - additional forces supplied by user.
 *  @uDelay             - delay in milliseconds, which prevents this controller from spamming too much.
 *
 * */
typedef struct {
    uint32_t uCtrlId, uDelay, uCollidersGroup;
    tRect *tRct;
    ePhysicalBodyType eBodyType;
    eGravityDirection eGravityDir;
    tForcesList tAdditionalForces;
    tColliders lCurrentlyCollides;
} tPhysController;

void __vPhysicsControllerInternal(void *vRun, tController *tCtrl);

/* 
 *  @brief - handles the physical layer of the application for the chosen entity.
 *
 *  @tRun               - currently running runtime.
 *  @tPhys              - uninitialized physics controller. 
 *  @tRct               - rect, which should be affected by physics.
 *  @eBodyType          - physical body type. Can be changed on run.
 *  @uCollidersGroup    - group number, in which collisions are managed.
 * */
void vPhysicsInit(tRuntime *tRun, tPhysController *tPhys, tRect *tRct, ePhysicalBodyType eBodyType, uint32_t uCollidersGroup) \
    __attribute__((nonnull(1, 2, 3)));


/* 
 *  @brief - allows to set the delay between physics controller calls.
 *
 *  @tRun   - currently running runtime.
 *  @tPhys  - uninitialized physics controller. 
 *  @dDelay - amount of delay time between controller calls in milis.
 * */
void vPhysicsSetDelay(tRuntime *tRun, tPhysController *tPhys, double dDelay) __attribute__((nonnull(1, 2)));

/* 
 *  @brief - applies the force to the physical body, handled by physics controller.
 *
 *  @tPhys  - uninitialized physics controller. 
 *  @tF     - force to apply.
 *
 *  The force is applied once, and removed after the first controller update.
 * */
void vPhysicsApplyForce(tPhysController *tPhys, tForce tF) __attribute__((nonnull(1)));

/* 
 *  @brief - returns true of something collides with a collider.
 * */
bool bPhysicsCurrentlyCollides(tPhysController *tPhys);

#endif
