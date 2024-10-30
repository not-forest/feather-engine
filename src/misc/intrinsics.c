/**************************************************************************************************
 *  File: intrinsics.c
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

#include <stdio.h>
#include <stdlib.h>
#include "intrinsics.h"

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
char* __ext_ReadFile(const char *csPath) {
    FILE* f = fopen(csPath, "r");

    if (!f) 
        return NULL;

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);

    return buffer;
}
