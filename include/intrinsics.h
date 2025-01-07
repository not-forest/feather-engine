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

#define __FEATHER_SDL_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define __PUSH_WINDOW_FLAGS _Pragma("push_macro(\"__FEATHER_SDL_WINDOW_FLAGS\")") _Pragma("undef(\"__FEATHER_SDL_WINDOW_FLAGS\")")
#define __POP_WINDOW_FLAGS _Pragma("pop_macro(\"__FEATHER_SDL_WINDOW_FLAGS\")") __FEATHER_SDL_WINDOW_FLAGS
__PUSH_WINDOW_FLAGS

#ifdef __EMSCRIPTEN__

// SDL2 is not fully compatible with emscripten.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#endif

#ifndef FEATHER_FPS_UNLIMITED
// If true, FPS will be unlimited, therefore maximum perfomance is obtained. This will cause more energy
// to be used ofcource.
#define FEATHER_FPS_UNLIMITED false
#endif

#ifndef FEATHER_MS_PER_UPDATE
// Amount of update time given for the game. This prevents users with higher FPS to perform more calculations. This
// constant shall be given in milliseconds.
#define FEATHER_MS_PER_UPDATE 10
#endif

#define __FEATHER_SDL_DEFAULT SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO

/* Combination of all required SDL subsystems for the program's need.  */
#ifndef FEATHER_SDL_INIT
#define FEATHER_SDL_INIT __FEATHER_SDL_DEFAULT
#else
#define FEATHER_SDL_INIT __FEATHER_SDL_DEFAULT | FEATHER_SDL_INIT
#endif

/* All formats supported by SDL_image can be adjusted in Kconfig. */
#ifndef FEATHER_TEXTURE_FORMAT
#define FEATHER_TEXTURE_FORMAT 0

#if FEATHER_TEXTURE_JPG
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_JPG
#endif

#if FEATHER_TEXTURE_PNG
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_PNG
#endif

#if FEATHER_TEXTURE_TIF
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_TIF
#endif

#if FEATHER_TEXTURE_WEBP
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_WEBP
#endif

#if FEATHER_TEXTURE_JXL
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_JXL
#endif

#if FEATHER_TEXTURE_AVIF
#define FEATHER_TEXTURE_FORMAT FEATHER_TEXTURE_FORMAT | IMG_INIT_AVIF
#endif

#endif

/*
 *  @brief - internal engine's read function.
 *
 *  Reads the file based on the provided path. Returns NULL if file does not exist.
 *  Used internally by engine and shall not be used globally. This function creates
 *  heap allocations, therefore must be manually freed.
 *
 *  @csPath - path to the file location.
 *  @return - returns read data allocated in a buffer on heap.
 * */
char *__ext_ReadFile(const char *csPath);

/* One byte value describing amount of frames per second. */
typedef uint8_t tFPS;
/* Arbitrary game unit. Converted to required unit which is used by graphics library. */
typedef float tGameUnit;
/* Pointer to any void function with void pointer argument. */
typedef void (*fClosure)(void*);

/* Creates a strong alias binding to another function. */
#define __ext__StrongAlias(TargetFunc, AliasDecl)               \
    extern __typeof__ (TargetFunc) AliasDecl                    \
    __attribute__ ((alias (#TargetFunc), copy (TargetFunc)))

/* Creates a weak alias binding to another function. */
#define __ext__WeakAlias(TargetFunc, AliasDecl) \
    __ext__StrongAlias(TargetFunc, AliasDecl)   \
    __attribute__((weak))

/* Allows to count the amount of argument provided to the macro up to 10 */
#define __ext_ArgCounter10(...) __ext_ArgCounter10Helper(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __ext_ArgCounter10Helper(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...) COUNT

struct tRuntime;

#endif
