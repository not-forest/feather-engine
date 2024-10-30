/**************************************************************************************************
 *  File: shader.h
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

#ifndef FEATHER_SHADER_H
#define FEATHER_SHADER_H

#include "err.h"
#include "runtime.h"

/* OPENGL */
#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengles2.h>
#endif

/* 
 *  @brief - initializes the shader program.
 * */
void vgraphInitShaderProgram(tRuntime *tRun); 

/* 
 *  @brief - Loads the requested shader for future use.
 *
 *  @sVertexPath, sFragmentPath - path to vertex and fragment glsl shader source files.
 *  Those paths will be automatically concatenated to the shaders folder.
 * */
tEngineError errGraphLoadShader(GLuint glShaderProgram, const char *sVertexPath, const char *sFragmentPath);

#endif

#endif
