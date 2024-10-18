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

#include "layer.h"
#include <tllist.h>
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
} tScene;

/* 
 *  @brief - pushes layer to the scene's list.
 *
 *  Each layer is a user defined function that will be scheduled during the update phase.
 *  All layers can access the shared resources and it's local variables.
 * */
void vSceneAppendLayer(tScene *sScene, fLayer vLayer) __attribute__((nonnull(1)));

/* 
 *  @brief - defines and appends a new layer to the scene.
 *
 *  Allow to define a layer and append it to the existing scene. User may define any local data
 *  structure to use within this layer.
 * */
#define FEATHER_LAYER(sScene, scName, anyLocal, ...)    \
    anyLocal;                                           \
    void scName(void *tRun) __VA_ARGS__;                \
    __attribute__((constructor))                        \
    void scName ## _constructor() {                     \
        vSceneAppendLayer(sScene, scName);              \ 
    }

/* 
 *  @brief - defines new empty scene.
 * */
#define FEATHER_SCENE(scName)           \
    static tScene scName = (tScene) {   \
        .sName = #scName,               \
        .lLayers = tll_init(),          \
    };                                  \

#endif
