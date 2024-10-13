/**************************************************************************************************
 *  File: res.h
 *  Desc: Defines a resource data type. Resources are any type used by user in the main application,
 *  which shall be handled by this engine.
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

#ifndef FEATHER_RES_H
#define FEATHER_RES_H

#include <stdint.h>
#include <threads.h>
#include <tllist.h>

#include "lock.h"
#include "intrinsics.h"

/* 
 *  @brief - scene's resource data type.
 *
 *  @vRawData - nonnull pointer to internal user's data.
 *
 *  Any user data can be wrapped as a resource, which provides proper loading abilities with implemented
 *  mutual exclusion and allows to be used between different layers of the scene. Each user data is packed
 *  into resource, which is then obtained by the layer when the runtime schedules it. Only shared data should
 *  be packed into a resource.
 * */
typedef struct {
    void *vRawData;  
    uint32_t uResId;

    vLockFn vLock; 
} tResource;

/* 
 *  @brief - list of user resources.
 * */
typedef tll(tResource) tResList;

/* 
 *  @brief - declares user type as a resource.
 *
 *  @tType - defined user type.
 *  @fPtrC - function pointer to the constructor function. It should always return data of proper type.
 *  @fPtrD - function pointer to the destructor function.
 * */
#define FeatherResource(tType, fPtrC, fPtrD)            \
    cCOUNT = __COUNTER__;                               \
    __res_constructor_declare(tType, fPtrC, cCOUNT);    \
    __res_destructor_declare(tType, fPtrC, cCOUNT);     \

// Aliases user defined constructor for runtime use.
#define __res_constructor_declare(tType, fPtrC, cCOUNT) \  
    tType RES_ ## cCOUNT ## constructor(void)           \
    __attribute__((alias(#fPtrC)));                     \

// Aliases user defined destructor for runtime use.
#define __res_destructor_declare(tType, fPtrD, cCOUNT)  \ 
    tType RES_ ## cCOUNT ## destructor(void)            \
    __attribute__((alias(#fPtrD)));                     \

#define tReadResource(tRes, Type) \
    (Type*)__res_read_void(tRes)

#define vWriteResouce(tRes, tData) \
    __res_write_void(tRes, (void*)tData)

/* 
 *  @brief - creates a new resource instance for custom use data type.
 * */
tResource __res_new(void *vRawData);
/* 
 *  @brief - provides the inner data type of the underlined resource.
 *
 *  @tRes - pointer to the resource location. The provided pointer should never be null.
 * */
void* __res_read_void(tResource *tRes) __attribute__((visibility("protected"), nonnull(1)));
/* 
 *  @brief - writes user's data type into the resource.
 *
 *  @tRes - pointer to the resource location. The provided pointer should never be null.
 *  @vRawData - pointer to new data to be written into the resource.
 * */
void __res_write_void(tResource *tRes, void *vRawData) __attribute__((visibility("protected"), nonnull(1, 2)));

#endif
