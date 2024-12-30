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

#include <context2d.h>
#include <runtime.h>
#include <rect.h>
#include <intrinsics.h>
#include <log.h>

// Initialize a rectangle
tRect* tInitRect(tRuntime *tRun, tContext2D tCtx, uint16_t uPriority, char* sTexturePath) {
    tRect rect;
    rect.tCtx = tCtx;
    rect.sTexturePath = sTexturePath;
    rect.uPriority = uPriority;

    // Load texture using SDL_image
    SDL_Surface* surface = IMG_Load(sTexturePath);
    if (!surface) {
        vFeatherLogError("Unable to load rect texture: %s", IMG_GetError());
        return NULL;
    }

    // Generate SDL_Texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(tRun->sdlRenderer, surface);
    if (!texture) {
        vFeatherLogError("Unable to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);

    // Store the texture in the rectangle
    rect.idTextureID = (uintptr_t)texture;

    // Insert the rectangle into the list with priority handling
    tll_foreach(tRun->sScene->lRects, it) {
        if (it->item.uPriority > uPriority) {
            tll_insert_before(tRun->sScene->lRects, it, rect);
            return (tRect*)it->prev;
        }
    }

    // New higher priority rectangles are pushed to the front
    tll_push_front(tRun->sScene->lRects, rect);
    return (tRect*)tRun->sScene->lRects.head;
}

// Draw the rectangle using SDL renderer
void vDrawRect(tRuntime *tRun, tRect *rect) {
    SDL_Renderer* renderer = tRun->sdlRenderer;
    SDL_Texture* texture = (SDL_Texture*)rect->idTextureID;

    float fTranslateX = rect->tCtx.m4UniformMatrix[3][0];
    float fTranslateY = rect->tCtx.m4UniformMatrix[3][1];
    float fScaleX = sqrtf(rect->tCtx.m4UniformMatrix[0][0] * rect->tCtx.m4UniformMatrix[0][0] +
                          rect->tCtx.m4UniformMatrix[0][1] * rect->tCtx.m4UniformMatrix[0][1]);
    float fScaleY = sqrtf(rect->tCtx.m4UniformMatrix[1][0] * rect->tCtx.m4UniformMatrix[1][0] +
                          rect->tCtx.m4UniformMatrix[1][1] * rect->tCtx.m4UniformMatrix[1][1]);
    float fRotation = atan2f(rect->tCtx.m4UniformMatrix[1][0], rect->tCtx.m4UniformMatrix[0][0]);

    SDL_Rect dstRect;
    dstRect.x = (int)fTranslateX;
    dstRect.y = (int)fTranslateY;
    SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
    dstRect.w = (int)(dstRect.w * fScaleX);
    dstRect.h = (int)(dstRect.h * fScaleY);

    float rotationAngle = fRotation * (180.0f / M_PI);

    SDL_RenderCopyEx(
        renderer,
        texture,
        NULL,
        &dstRect,
        rotationAngle,
        NULL,
        SDL_FLIP_NONE
    );
}
