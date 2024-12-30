/**************************************************************************************************
 *  File: err.h
 *  Desc: Defines an error type for engine and a set of error codes.
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

#ifndef FEATHER_ERR_H
#define FEATHER_ERR_H

/* Engine defined error integer. */
typedef int tEngineError;

/* 
 *  @brief - formats feather engine's error into string format.
 *
 *  @err - error input. Function ignores error's sign, therefore any format is allowed.
 * */
char* feather_errfmt(tEngineError err);

#define __ERR_OFFSET 6000

#define errNO_SCENE         __ERR_OFFSET + 0    // Scene is not defined, i.e pointer is NULL. 
#define errSDL_ERR          __ERR_OFFSET + 1    // Error obtained from the SDL, sometimes could be fatal.
#define errNO_FILE          __ERR_OFFSET + 2    // Unable to read from/write to file due to it's inexistence.
#define errBROKEN_SHADER    __ERR_OFFSET + 3    // Unable to compile the shader. This error will come straight from the graphics library.

#endif
