/**************************************************************************************************
 *  File: shader.c
 *  Desc: Internal shader function bindings for the engine. Those functions are used by engine internal
 *  structures, yet may be used directly to manipulate on graphics manually.
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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "intrinsics.h"
#include "shader.h"

#ifndef PROJECT_ROOT_PATH
#error Shader feature cannot be used without the project's root path.
#endif

#define SHADERS_ROOT_PATH PROJECT_ROOT_PATH"/shaders/"

#define FEATHER_CORE_SHADERS    \
                        "rect"  \

/* OPENGL */
#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__

#include "log.h"
#include "err.h"

GLuint __compile_shader(const char* sShaderSrc, GLenum tShaderType) {
    int res;
    GLuint glShader = glCreateShader(tShaderType);

    glShaderSource(glShader, 1, &sShaderSrc, NULL);
    glCompileShader(glShader);

    glGetShaderiv(glShader, GL_COMPILE_STATUS, &res);
    if (!res) {
        char infoLog[512];
        glGetShaderInfoLog(glShader, 512, NULL, infoLog);
        vFeatherLogError("Unable to compile shader:\n%s", infoLog);
    }

    return glShader;
}

/* 
 *  @brief - Loads the requested shader for future use.
 *
 *  @sVertexPath, sFragmentPath - path to vertex and fragment glsl shader source files.
 *  Those paths will be automatically concatenated to the shaders folder.
 * */
tEngineError errGraphLoadShader(GLuint glShaderProgram, const char *sVertexPath, const char *sFragmentPath) {
    int res;
    char sVertexPathFull[256], sFragmentPathFull[256];

    strcpy(sVertexPathFull, SHADERS_ROOT_PATH);
    strcat(sVertexPathFull, sVertexPath);
    strcpy(sFragmentPathFull, SHADERS_ROOT_PATH);
    strcat(sFragmentPathFull, sFragmentPath);

    vFeatherLogInfo("Loading core shaders:\n%s\n%s\n", sVertexPathFull, sFragmentPathFull);

    char *sVertexSrc = __ext_ReadFile(sVertexPathFull);
    char *sFragmentSrc = __ext_ReadFile(sFragmentPathFull);

    if (sVertexSrc == NULL || sFragmentSrc == NULL) {
        vFeatherLogFatal("Vertex, fragment source pair cannot be created.");
        return -errNO_FILE;
    }

    GLuint glVertexShader = __compile_shader(sVertexSrc, GL_VERTEX_SHADER);
    GLuint glFragmentShader = __compile_shader(sFragmentSrc, GL_FRAGMENT_SHADER);

    free(sVertexSrc);
    free(sFragmentSrc);

    glAttachShader(glShaderProgram, glVertexShader);
    glAttachShader(glShaderProgram, glFragmentShader);
    glDeleteShader(glVertexShader);
    glDeleteShader(glFragmentShader);

    return 0;
}

void vgraphInitShaderProgram(tRuntime *tRun) { 
    GLuint glShaderProgram = glCreateProgram();
    char sFraqBuf[256], sVertBuf[256], sCore[] = { FEATHER_CORE_SHADERS };

    // Appending all core shaders.
    for (uint8_t i = 0; i < sizeof(sCore); ++i) {
        strcpy(sFraqBuf, sCore);
        strcpy(sVertBuf, sCore);
        strcat(sFraqBuf, ".frag.glsl");
        strcat(sVertBuf, ".vert.glsl");

        errGraphLoadShader(glShaderProgram, sVertBuf, sFraqBuf);
    }     

    tRun->glShaderProgram = glShaderProgram;
}

#endif
