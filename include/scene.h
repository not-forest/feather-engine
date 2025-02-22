/**************************************************************************************************
 *  File: scene.h
 *  Desc: Defines all datatypes and function related to one generic scene.
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

#ifndef FEATHER_SCENE_H
#define FEATHER_SCENE_H

#include <tllist.h>
#include <stdint.h>

/* 
 *  @brief - additional structure that provides a way to observe colliders.
 *
 *  Only four parameters is required.
 * */
typedef struct {
    double x, y, w, h;
    uint32_t uColliderId, uCollidersGroup;
} tColliderLabel;

typedef tll(tColliderLabel) tColliders;

#include <controller.h>
#include <layer.h>
#include <rect.h>


/* 
 *  @brief - defines a structure of one generic scene.
 *
 *  @lLayers - list of layers, which are user defined handler function for each scene.
 *
 *  Each scene contains a set of handler function to provide the main user program's
 *  logic. The main engine's runtime can handle only one scene at a time. A scene can have
 *  zero, one or more cameras.
 * */
typedef struct {
    char* sName;
    tLayerList lLayers;
    tControllerList lControllers;
    tRectList lRects;
    tColliders lColliders;

    uint32_t uCurrentRunningLayerId;
    uint32_t uCurrentRunningControllerId;
} tScene;

/* 
 *  @brief - pushes layer to the scene's list.
 *
 *  Each layer is a user defined function that will be scheduled during the update phase.
 *  All layers can access the shared resources and it's local variables.
 * */
void vSceneAppendLayer(tScene *sScene, tLayer vLayer) __attribute__((nonnull(1)));

/* 
 *  @brief - pushes new controller to the scene's list.
 *
 *  Each controller is being handled (or not) before all of the layers, but after the input handling phase.
 *  Controllers are not held by the scene and runtime, so they are preserved within the user defined structure.
 *  They can always be added and removed at runtime.
 * */
void vSceneAppendController(tScene *sScene, tController tCtrl) __attribute__((nonnull(1)));

/* 
 *  @brief - removes the controller from the scene's list.
 *
 *  Does nothing if the controller is not within the scene's list already.
 * */
void vSceneRemoveController(tScene *sScene, uint32_t uControllerID) __attribute__((nonnull(1)));

/* 
 *  @brief - defines new empty scene.
 * */
#define FEATHER_SCENE(scName)           \
    static tScene scName = (tScene) {   \
        .sName = #scName,               \
        .lLayers = tll_init(),          \
        .lControllers = tll_init(),     \
        .lRects = tll_init(),           \
        .lColliders = tll_init(),       \
        .uCurrentRunningLayerId = 0,    \
        .uCurrentRunningControllerId = 0\
    };                                  \

#endif
