/**************************************************************************************************
 *  File: feather.c
 *  Desc: Main engine entry bindings. If this header is included, then runtime is fully handled automatically
 *  by this engine. User shall only include his 'init', 'exit' and handlers functions. Loggin API defines six 
 *  verbosity levels, with ability to define callbacks functions for different levels, locking logic on 
 *  multithreading targets.
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

#ifndef FEATHER_H
#define FEATHER_H

int iFeatherMain(void) __attribute__((visibility("protected")));

/* 
 *  @brief - provides function for engine initialization.
 *
 *  User function should initialize any user-defined content, before building the first
 *  scene.
 * */
#define FEATHER_INIT(f)     \
void __feather_init(void) __attribute__((weak)) { f() } 

/* 
 *  @brief - feather engine initialization function.
 *
 *  Will be called before entering the engine loop. Should be defined by user with
 *  the use of 'FEATHER_INIT' macro.
 * */
void __feather_init(void);

// Default initialization function if no is provided.
void __default_init(void);

#endif
