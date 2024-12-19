/**************************************************************************************************
 *  File: rect.vert.glsl
 *  Desc: Vertex shader for handling Rects. Scales and positions the rectangle. Applies the transformation
 *  matrix. Passes the texture coordinates for each vertices
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

#version 420 core

uniform vec2 pan;           // Panning offset
uniform float zoom;         // Zoom factor
uniform float aspect;       // Aspect ratio

layout(location = 0) in vec4 position; // Input vertex attribute
out vec2 texCoord;          // Output texture coordinates

void main() {
    // Apply transformations to the vertex position
    gl_Position = vec4(position.xyz, 1.0);  
    gl_Position.xy += pan;                  // Apply panning
    gl_Position.xy *= zoom;                 // Apply zoom
    gl_Position.y *= aspect;                // Apply aspect ratio adjustment
    
    // Calculate texture coordinates
    texCoord = vec2(gl_Position.x, -gl_Position.y);
}

