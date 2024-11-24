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
#include <tllist.h>

/* 
 *  @brief - each layer is a user defined function that will be scheduled during update phase.
 *
 *  @fRun - actual pointer to the layer function.
 *  @iPriority - priority of which this layer is executed. Negative values are only executed on 
 *  initialization according to the value they have, i.e (-2) will be runned only two times. Negative 
 *  values also have the highest priority.
 * */
typedef struct {
    void (*fRun)(void *tRun);
    int iPriority;  
} tLayer;

/* 
 *  @brief - list of layer functions.
 * */
typedef tll(tLayer) tLayerList;


#endif
