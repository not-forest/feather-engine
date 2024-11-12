/**************************************************************************************************
 *  File: rect.h
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

#pragma once

#ifndef FEATHER_RECT_H
#define FEATHER_RECT_H

#include <stdint.h>
#include "context2d.h"
#include "runtime.h"

/* 
 *  @brief - Rect data type.
 *
 *  Rects are main boxes for holding information about something that shall be drawn on the screen, 
 *  it's boundaries and coordinates.
 * */
typedef struct {
    float fVertices[16];       // Vertex data
    uint8_t uiIndices[6];      // Index data
    char* sTexturePath;        // Path to texture
    tContext2D tCtx;           // Transformation context
    GLuint VAO, VBO, EBO;      // OpenGL objects
} tRect;

/* 
 *  @brief - creates a new instance of Rect, while initializing the drawing context.
 *
 *  Acts as a constructor for Rect structure. Once created, engine will draw it onto the screen.
 * */
tRect tInitRect(tContext2D tCtx, char* sTexturePath);

/* 
 *  @brief - draws the rectangle to the screen.
 *
 *  The current position is defined by it's Context2D.
 * */
void vDrawRect(tRect *rect, GLuint shaderProgram);

#endif
