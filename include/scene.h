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

#include "res.h"
#include "layer.h"

#pragma once

#ifndef FEATHER_SCENE_H
#define FEATHER_SCENE_H

/* 
 *  @brief - defines a structure of one generic scene.
 *
 *  @lLayers - list of layers, which are user defined handler function for each scene.
 *  @lResources - list of resources used between layers inside the scene.
 *
 *  Each scene contains a set of resources and handler function to provide the main user program's
 *  logic. The main engine's runtime can handle only one scene at a time.
 * */
typedef struct {
    tLayerList lLayers;
    tResList lResources;
} tScene;

#endif