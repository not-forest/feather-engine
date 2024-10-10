/**************************************************************************************************
 *  File: __feather_log.c
 *  Desc: Engine __feather_logging library implementation. Logs information with different verbosity levels
 *      on engine's startup. Used in the main API, but can be used outside the engine itself as well.
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

#include "log.h"

typedef struct {
    vLogFn vFn;
    void *vUserData;
    int iLevel;
} tCallback;

static struct {
    void *vUserData;
    vLockFn vLock;
    int iLevel;
    bool bQuiet;
    tCallback tCallbacks[FEATHER_LOG_MAX_CALLBACKS];
} tLogger __attribute__((visibility("internal")));

static const char *cLevelStrings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static void vStdoutCallback(tLogEvent *tEv) {
    char cBuf[16];
    cBuf[strftime(cBuf, sizeof(cBuf), "%H:%M:%S", tEv->tTime)] = '\0';
    
#ifndef FEATHER_LOG_NO_COLOR
    fprintf(
        tEv->vUserData, "\x1b[36m%s\x1b[0m %s%-5s\x1b[0m \x1b[36m%s\x1b[0m:%d: ",
        cBuf, cLevelColors[tEv->iLevel], cLevelStrings[tEv->iLevel],
        tEv->cFile, tEv->iLine
    );
#else
    fprintf(
        tEv->vUserData, "%s %-5s %s:%d: ",
        cBuf, cLevelStrings[tEv->iLevel], tEv->cFile, tEv->iLine
    );
#endif
    vfprintf(tEv->vUserData, tEv->cFmt, tEv->vaAp);
    fprintf(tEv->vUserData, "\n");
    fflush(tEv->vUserData);
} __attribute__((visibility("internal")))

static void vFileCallback(tLogEvent *tEv) {
    char cBuf[64];
    cBuf[strftime(cBuf, sizeof(cBuf), "%Y-%m-%d %H:%M:%S", tEv->tTime)] = '\0';
    fprintf(
        tEv->vUserData, "%s %-5s %s:%d: ",
        cBuf, cLevelStrings[tEv->iLevel], tEv->cFile, tEv->iLine
    );
    vfprintf(tEv->vUserData, tEv->cFmt, tEv->vaAp);
    fprintf(tEv->vUserData, "\n");
    fflush(tEv->vUserData);
} __attribute__((visibility("internal")))

static void vLock(void) {
    if (tLogger.vLock) {
        tLogger.vLock(true, tLogger.vUserData);
    }
} __attribute__((visibility("internal")))

static void vUnlock(void) {
    if (tLogger.vLock) {
        tLogger.vLock(false, tLogger.vUserData);
    }
} __attribute__((visibility("internal")))

/* 
 *  @brief - based on the provided level id, returns its string representation. 
 */
const char* __feather_log_level_string(int iLevel) {
    return cLevelStrings[iLevel];
}

/* 
 *  @brief - additional lock logic function for multithreaded environment.
 */
void __feather_log_set_lock(vLockFn vFn, void *vUserData) {
    tLogger.vLock = vFn;
    tLogger.vUserData = vUserData;
}

/* 
 *  @brief - sets logging level for current environment.
 */
void __feather_log_set_level(int iLevel) {
    tLogger.iLevel = iLevel;
}

/* 
 *  @brief - enables or disables all log messages.
 */
void __feather_log_set_quiet(bool bEnable) {
    tLogger.bQuiet = bEnable;
}

/* 
 *  @brief - adds a callback for logging.
 */
int __feather_log_add_callback(vLogFn vFn, void *vUserData, int iLevel) {
    for (int i = 0; i < FEATHER_LOG_MAX_CALLBACKS; i++) {
        if (!tLogger.tCallbacks[i].vFn) {
            tLogger.tCallbacks[i] = (tCallback){ vFn, vUserData, iLevel };
            return 0;
        }
    }
    return -1;
}

/* 
 *  @brief - adds file pointer for logging output.
 */
int __feather_log_add_fp(FILE *fFp, int iLevel) {
    return __feather_log_add_callback(vFileCallback, fFp, iLevel);
}

static void vInitEvent(tLogEvent *tEv, void *vUserData) {
    if (!tEv->tTime) {
        time_t tCurrentTime = time(NULL);
        tEv->tTime = localtime(&tCurrentTime);
    }
    tEv->vUserData = vUserData;
} __attribute__((visibility("internal")))

/* 
 *  @brief - Performs the actual logging logic.
 */
void __feather_log(int iLevel, const char *cFile, int iLine, const char *cFmt, ...) {
    tLogEvent tEv = {
        .cFmt   = cFmt,
        .cFile  = cFile,
        .iLine  = iLine,
        .iLevel = iLevel,
    };

    vLock();

    if (!tLogger.bQuiet && iLevel >= tLogger.iLevel) {
        vInitEvent(&tEv, stderr);
        va_start(tEv.vaAp, cFmt);
        vStdoutCallback(&tEv);
        va_end(tEv.vaAp);
    }

    for (int i = 0; i < FEATHER_LOG_MAX_CALLBACKS && tLogger.tCallbacks[i].vFn; i++) {
        tCallback *tCb = &tLogger.tCallbacks[i];
        if (iLevel >= tCb->iLevel) {
            vInitEvent(&tEv, tCb->vUserData);
            va_start(tEv.vaAp, cFmt);
            tCb->vFn(&tEv);
            va_end(tEv.vaAp);
        }
    }

    vUnlock();
}

