/**************************************************************************************************
 *  File: intrinsics.h
 *  Desc: Engines main intrinsic binding, macros and function for general purpose use. Completely
 *  hidden and shall not be used in the outside code.
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

#include <stdint.h>
#ifndef FEATHER_INTRINSICS_H
#define FEATHER_INTRINSICS_H

#define __FEATHER_OPENGL__ 0
#define __FEATHER_VULKAN__ 1
#define __FEATHER_DIRECTX__ 2

/* One byte value describing amount of frames per second. */
typedef uint8_t tFPS;

/* Creates a strong alias binding to another function. */
#define __ext__StrongAlias(TargetFunc, AliasDecl)               \
    extern __typeof__ (TargetFunc) AliasDecl                    \
    __attribute__ ((alias (#TargetFunc), copy (TargetFunc)))

/* Creates a weak alias binding to another function. */
#define __ext__WeakAlias(TargetFunc, AliasDecl) \
    __ext__StrongAlias(TargetFunc, AliasDecl)   \
    __attribute__((weak))

#endif
