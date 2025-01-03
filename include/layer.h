/**************************************************************************************************
 *  File: layer.h
 *  Desc: Defines layers API, which are user defined functions that define main application functionality
 *  and being scheduled during game update stage.
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

#ifndef FEATHER_LAYER_H
#define FEATHER_LAYER_H

#include <stdint.h>
#include <stdbool.h>
#include <tllist.h>

/* 
 *  @brief - each layer is a user defined function that will be scheduled during update phase.
 *
 *  @fRun       - actual pointer to the layer function.
 *  @iPriority  - priority of which this layer is executed. Negative values are only executed on 
 *  initialization according to the value they have, i.e (-2) will be runned only two times. Negative 
 *  values also have the highest priority.
 *  @sName      - name of the layer provided by user.
 *  @uLastSleep - used by runtime to implement sleeping layers.
 * */
typedef struct {
    void (*fRun)(void *tRun);
    int iPriority;
    char* sName;
    uint32_t uLastSleep;
} tLayer;

/* 
 *  @brief - list of layer functions.
 * */
typedef tll(tLayer) tLayerList;

/* 
 *  @brief - compare implementation for the layer structure.
 * */
bool bLayerCmp(tLayer l1, tLayer l2);

/* 
 *  @brief - Returns a proper number, so that a layer will be executen N times.
 * */
#define iPerformNTimes(N) -(int)N

/* 
 *  @brief - defines and appends a new layer to the scene.
 *
 *  Allow to define a layer and append it to the existing scene. User may define any local data
 *  structure to use within this layer.
 * */
#define FEATHER_LAYER(sScene, iP, scName, anyLocal, ...)    \
    anyLocal;                                               \
    void scName(void *__tRun) __VA_ARGS__;                  \
    __attribute__((constructor))                            \
    void scName##_constructor() {                           \
        tLayer layer = {                                    \
            .fRun = scName,                                 \
            .iPriority = iP,                                \
            .sName = #scName,                               \
            .uLastSleep = 0                                 \
        };                                                  \
        vSceneAppendLayer(sScene, layer);                   \
    }

#endif
