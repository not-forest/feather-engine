/**************************************************************************************************
 *  File: lock.h
 *  Desc: General purpose bindings for internal locking mechanism. This module defines a lock interface,
 *  which allows to call locking functions and provide any possible mechanism individually for different
 *  engine structures.
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

#ifndef FEATHER_LOCK_H
#define FEATHER_LOCK_H

#include <stdbool.h>

/* 
 *  @brief - external locking callback function.
 *
 *  Calls the function 
 * */
typedef void (*vLockFn)(bool bLocked, void *vUserData);

/* 
 *  @brief -- locks the resource with an external locking algorithm.
 * */
#define vLOCK(vLock, uData) \
    __vlock(vLock, true, uData);

/* 
 *  @brief -- unlocks the resource with an external locking algorithm.
 * */
#define vUNLOCK(vLock, uData) \
    __vlock(vLock, false, uData);

/* 
 *  @brief - calls the external locking algorithm via pointer.
 *
 *  Based on the pointers any external locking algorithm can be used to
 *  handle some specific resource management. This locking interface is then applied
 *  to all engine's structures that do require mutual exclusion.
 * */
void __vlock(vLockFn vLock, bool bToLock, void *vUserData);

#endif
