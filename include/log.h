/**************************************************************************************************
 *  File: log.h
 *  Desc: Engine logging library bindings. Loggin API defines six verbosity levels, with ability to
 *  define callbacks functions for different levels, locking logic on multithreading targets.
 *
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

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include "lock.h"

// Maximum amount of callbacks functions.
#ifndef FEATHER_LOG_MAX_CALLBACKS
#warning "Undefined [FEATHER_LOG_MAX_CALLBACKS] will be defaulted to 16. Prefer defining it to the maximal amount your specific implementation requires to prevent useless overhead."
#define FEATHER_LOG_MAX_CALLBACKS 16
#endif

// Colorful logging.
#ifndef FEATHER_LOG_NO_COLOR
static const char *cLevelColors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

/* 
 *  @brief - defines a simple structure to perform a generic log.
 *
 *  @vaAp       - holds extra arguments for formatting (va_list).
 *  @cFmt       - formatted message value to print or save file. 
 *  @cFile      - chosen output file. Defaults to stdout.
 *  @tTime      - current time of log print.
 *  @vUserData  - user data pointer provided to callback function after logging.
 *  @iLine      - current code's line obtained from preprocessor __LINE__.
 *  @iLevel     - level of verbosity for this log.
 *
 *  Overall structure for describing a log event. Each log operation will
 *  use one to properly handle all functionality, such as saving to file,
 *  callbacks and locking.
 * */
typedef struct {
    va_list vaAp;
    const char *cFmt;
    const char *cFile;
    struct tm *tTime;
    void *vUserData;
    int iLine;
    int iLevel;
} tLogEvent;

// Custom type for log callback function.
typedef void (*vLogFn)(tLogEvent *ev);

// Six logging verbosity levels.
enum { lTRACE, lDEBUG, lINFO, lWARN, lERROR, lFATAL };

/* Definitions of useful macros for different logging levels. Those must be used in the actual code. */
#define feather_log_trace(...) __feather_log(lTRACE, __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_debug(...) __feather_log(lDEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_info(...)  __feather_log(lINFO,  __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_warn(...)  __feather_log(lWARN,  __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_error(...) __feather_log(lERROR, __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_fatal(...) __feather_log(lFATAL, __FILE__, __LINE__, __VA_ARGS__)
#define feather_log_set_level(N) __feather_log_set_level(N)
#define feather_log_quiet(enable) __feather_log_set_quiet(enable)
#define feather_log_add_callback(fn, udata, level) __feather_log_add_callback(fn, udata, level)
#define feather_log_add_file(fp, level) __feather_log_add_fp(fp, level)
#define feather_log_level_string(level) __feather_log_level_string(level)

/* 
 *  @brief - based on the provided level id, returns it's string representation. 
 * */
const char* __feather_log_level_string(int iLevel);
/* 
 *  @brief - additional lock logic function for multithreaded environment.
 *
 *  The provided function will be called on each lock/unlock sequence, i.e on each
 *  read/write. The function will be supplied with lock boolean and pointer to user's
 *  lock data.
 * */
void __feather_log_set_lock(vLockFn vFn, void *vUserData);
/* 
 *  @brief - sets logging level for current environment.
 *
 *  Any logging below such level is ignored. By default LOG_TRACE is used.
 * */
void __feather_log_set_level(int iLevel);
/* 
 *  @brief - enables or disables all log messages.
 * */
void __feather_log_set_quiet(bool bEnable);
/* 
 *  @brief - enables or disables all log messages.
 * */
int __feather_log_add_callback(vLogFn vFn, void *vUserData, int iLevel);
/* 
 *  @brief - adds file pointer for logging output.
 *
 *  Several file sources can be provided this way. Any messages below
 *  provided level will be ignored and not written to the file.
 * */
int __feather_log_add_fp(FILE *fFp, int iLevel);

/* 
 *  @brief - Performs the actual logging logic.
 * */
void __feather_log(int iLevel, const char *fout, int line, const char *cFmt, ...);

#endif
