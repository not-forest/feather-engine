/**************************************************************************************************
 *  File: feather.c
 *  Desc: Main library space for Feather Engine software runtime. Automatically initializes the runtime
 *  to simplify work from the user's size. Main function is declared here as a weak alias, therefore
 *  can be rewritten inside any other user module afterwards. 
 *
 *  This module is optional and can be used for simpler initialization or quick bindings. For manual
 *  runtime initialization "runtime.h" API shall be used.
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

#include "feather.h"
#include "log.h"
#include "intrinsics.h"
#include "runtime.h"

/* 
 *  @brief - Overrides the main function of the process if the header file of engine runtime is imported.
 * */
int iFeatherMain(void) {
    tRuntime rRuntime;

#ifndef NDEBUG
    // On release builds, logging verbosity is decreased to info.
    vFeatherLogSetLevel(lINFO);
#endif
    vFeatherLogInfo("Entering the 'iFeatherMain' function.");

    rRuntime = DEFAULT_RUNTIME();  // Creating the runtime environment.
#ifdef vRuntimeConfig
    vRuntimeConfig(&rRuntime);              // This function can be provided by user.
#else 
    vFeatherLogWarn("Runtime configuration not provided. Default configuration will be used.");
#endif

    // The runtime will loop until the game is exited or error.
    tEngineError errResult = errMainLoop(&rRuntime);
    if (errResult)
        vFeatherLogFatal("Unrecoverable error occured: %s. Aborting...", feather_errfmt(errResult));

    return errResult;
}

// Forcing the C compiler to link main function as 'iFeatherMain' during linking when the header is added.
__ext__WeakAlias(iFeatherMain, main) __attribute__((used));
