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
#include <tllist.h>

#include "context2d.h"
#include "runtime.h"
#include "rect.h"
#include "intrinsics.h"

// Vertex data (positions + texture coordinates)
const float RVERTICES[] = {
    // positions        // texture coordinates
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top right
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // top left 
};

const uint8_t DINDICES[] = {
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

tRect* tInitRect(tRuntime *tRun, tContext2D tCtx, uint16_t uPriority, char* sTexturePath) {
    // Allocate memory for the new rectangle
    tRect *rect = (tRect*)malloc(sizeof(tRect));
    if (!rect) return NULL; // Handle allocation failure

    rect->tCtx = tCtx;
    rect->sTexturePath = sTexturePath;
    rect->uPriority = uPriority;

    // Generate and bind VBO and EBO
    glGenBuffers(1, &rect->VBO);
    glGenBuffers(1, &rect->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, rect->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RVERTICES), RVERTICES, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DINDICES), DINDICES, GL_STATIC_DRAW);

    // Define the vertex attributes
    // Position attribute (3 floats for position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // Texture coordinate attribute (2 floats for texture)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind buffers to prevent accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Insert the rectangle into the list with priority handling
    tll_foreach(tRun->lRects, it) {
        if (it->item.uPriority > uPriority) {
            tll_insert_before(tRun->lRects, it, *rect);
            return (tRect*)it->prev;
        }
    }

    // New higher priority rectangles are pushed to the front
    tll_push_front(tRun->lRects, *rect);
    return (tRect*)tRun->lRects.head;
}

/* 
 *  @brief - draws the rectangle to the screen.
 *
 *  The current position is defined by its Context2D.
 */
void vDrawRect(tRuntime *tRun, tRect *rect) {
    glUseProgram(tRun->glShaderProgram);

    // Pass the transformation matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(tRun->glShaderProgram, "uTransform"), 1, GL_FALSE, (const float*)rect->tCtx.m4UniformMatrix);

    // Bind VBO and EBO before drawing
    glBindBuffer(GL_ARRAY_BUFFER, rect->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->EBO);

    // Enable vertex attributes and set pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Draw the rectangle using the index buffer
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    // Unbind the buffers after drawing
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
