/**************************************************************************************************
 *  File: rect.c
 *  Desc: Rects are engine's 2D primitives for drawing content onto the screen. They define bounding 
 *  box for each object or entity and apply coloring or texture.
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

#include "rect.h"
#include "context2d.h"

/* OPENGL */
#if FEATHER_GRAPHICS_MANAGER == __FEATHER_OPENGL__

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengles2.h>
#endif

GLuint VAO, VBO, EBO;

const float DVERTICES[] = {
    // Positions          // Texture Coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  // Top Right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // Bottom Left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f   // Top Left 
};

const uint8_t DINDICES[] = {
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

tRect tInitRect(tRuntime tRun, tContext2D tCtx, char* sTexturePath) {
    tRect rect;

    rect.tCtx = tCtx;
    rect.sTexturePath = sTexturePath;
    
    // Default rectangle representation expects the texture to be applied fully.
    memcpy(rect.fVertices, DVERTICES, sizeof(DVERTICES));
    memcpy(rect.uiIndices, DINDICES, sizeof(DINDICES));

    //glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect.fVertices), rect.fVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect.uiIndices), rect.uiIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //glBindVertexArray(0); 

    return rect;
}

#endif
