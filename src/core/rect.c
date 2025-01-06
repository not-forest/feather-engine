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
    tll_push_back(tRun->sScene->lRects, rect);
    return (tRect*)tRun->sScene->lRects.tail;
}

/* 
 *  @brief - changes the texture of the rect.
 *
 *  @tRun            - currently running runtime.
 *  @tRct            - pointer to the rectange we wish to change
 *  @sNewTexturePath - path to the new texture source.
 * */
void vChangeRectTexture(tRuntime* tRun, tRect* tRct, char* sNewTexturePath) {
    // Unload the existing texture
    SDL_Texture* oldTexture = (SDL_Texture*)tRct->idTextureID;
    if (oldTexture) {
        SDL_DestroyTexture(oldTexture);
        tRct->idTextureID = 0; // Reset the texture ID
    }
    SDL_Surface* surface = IMG_Load(sNewTexturePath);
    if (!surface) {
        vFeatherLogError("Unable to load new texture: %s", IMG_GetError());
        return;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(tRun->sdlRenderer, surface);
    SDL_FreeSurface(surface);

    if (!newTexture) {
        vFeatherLogError("Unable to create new texture from surface: %s", SDL_GetError());
        return;
    }

    tRct->idTextureID = (uintptr_t)newTexture;
    tRct->sTexturePath = strdup(sNewTexturePath);
} __attribute__((nonnull(1, 2)))

// Draw the rectangle using SDL renderer
void vDrawRect(tRuntime *tRun, tRect *rect) {
    SDL_Renderer* renderer = tRun->sdlRenderer;
    SDL_Texture* texture = (SDL_Texture*)rect->idTextureID;

    SDL_Rect dstRect;
    dstRect.x = rect->tCtx.fX;
    dstRect.y = rect->tCtx.fY;

    SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
    dstRect.w = (dstRect.w * rect->tCtx.fScaleX);
    dstRect.h = (dstRect.h * rect->tCtx.fScaleY);

    // Convert the rotation to degrees for SDL_RenderCopyEx
    float rotationAngle = rect->tCtx.fRotation * (180.0f / M_PI);

    // Render the texture with the calculated position, scale, and rotation
    SDL_RenderCopyEx(
        renderer,
        texture,
        NULL,            // No source rectangle (full texture)
        &dstRect,        // Destination rectangle
        rotationAngle,   // Rotation angle in degrees
        NULL,            // No center (rotate around the top-left corner)
        SDL_FLIP_NONE    // No flipping
    );
}

