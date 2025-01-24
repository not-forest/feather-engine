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

int __vRectFromTextureRaw(tRuntime *tRun, tRect *tRct, SDL_Surface *sdlSurf) {
    if (tRct == NULL) {
        vFeatherLogError("Internal error. NULL Rect in __vRectFromTextureRaw function.");
    }

    tRct->tFr.uWidth = sdlSurf->w; 
    tRct->tFr.uHeight = sdlSurf->h;

    // Generate SDL_Texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(tRun->sdlRenderer, sdlSurf);
    if (!texture) {
        vFeatherLogError("Unable to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(sdlSurf);
        return -1;
    }
    SDL_FreeSurface(sdlSurf);

    // Store the texture in the rectangle
    tRct->idTextureID = (uintptr_t)texture;
    return 0;
}

// Initialize a rectangle
tRect* tInitRect(tRuntime *tRun, tContext2D tCtx, uint16_t uPriority, char* sTexturePath) {
    tRect tRct = { 
        .tAnims = tll_init(), 
        .tCtx = tCtx, 
        .sTexturePath = sTexturePath, 
        .uPriority = uPriority, 
        .tFr.uIdx = 0,
        .uRectId = uRectIDIncrementer++,
    };

    if (sTexturePath == NULL) {
        // Color can be adjusted later.
        vChangeRectColor(tRun, &tRct, (SDL_Color) { 255, 255, 255, 255 });
    } else {
        // Load texture using SDL_image
        SDL_Surface* sdlSurf;
        sdlSurf = IMG_Load(sTexturePath);

        if (!sdlSurf) {
            vFeatherLogError("Unable to load rect texture: %s", IMG_GetError());
            return NULL;
        }

        if (__vRectFromTextureRaw(tRun, &tRct, sdlSurf) < 0)
            return NULL;
        vFeatherLogInfo("Loading asset: Rect texture: %s...", strrchr(sTexturePath, '/') + 1);
    }

    // Insert the rectangle into the list with priority handling
    tll_foreach(tRun->sScene->lRects, it)
        if (it->item.uPriority > uPriority) {
            tll_insert_before(tRun->sScene->lRects, it, tRct);
            return (tRect*)it->prev;
        }

    // New higher priority rectangles are pushed to the front
    tll_push_back(tRun->sScene->lRects, tRct);
    return (tRect*)tRun->sScene->lRects.tail;
}

#include <SDL.h>
#include <stdint.h>

/* 
 *  @brief - Change the color of the rectangle.
 *
 *  @tRun          - Pointer to the runtime.
 *  @tRct          - Pointer to the rectangle.
 *  @fallbackColor - New fallback color to apply.
 *
 *  This also destroys the currently applied texture. Here context2D's scaleX and scaleY decides the size of the colored block.
 */
void vChangeRectColor(tRuntime* tRun, tRect* tRct, SDL_Color fallbackColor) {
    SDL_Texture* oldTexture = (SDL_Texture*)tRct->idTextureID;
    if (oldTexture) {
        SDL_DestroyTexture(oldTexture);
        tRct->idTextureID = 0;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 
            1 * (int)tRct->tCtx.fScaleX, 
            1 * (int)tRct->tCtx.fScaleY, 
            32, SDL_PIXELFORMAT_RGBA32
    );

    if (!surface) {
        vFeatherLogError("Unable to create surface for new color: %s", SDL_GetError());
        return;
    }

    Uint32 color = SDL_MapRGBA(surface->format, fallbackColor.r, fallbackColor.g, fallbackColor.b, fallbackColor.a);
    SDL_FillRect(surface, NULL, color);
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(tRun->sdlRenderer, surface);
    SDL_FreeSurface(surface);

    if (!newTexture) {
        vFeatherLogError("Unable to create new texture for rectangle: %s", SDL_GetError());
        return;
    }

    tRct->idTextureID = (uintptr_t)newTexture;
    tRct->tFr.uWidth = 1 * (int)tRct->tCtx.fScaleX;
    tRct->tFr.uHeight = 1 * (int)tRct->tCtx.fScaleY;
}

/* 
 *  @brief - choose the frame to draw in the next render cycle.
 * */
void vRectFrame(tRect *tRct, uint8_t uIdx) {
    tRct->tFr.uIdx = uIdx;
}

/* 
 *  @brief - indexates over the surface of this rect, allowing to use individual frames of different sizes.
 * */
void vRectIndexate(tRect *tRct, uint8_t uIdx, uint32_t uWidth, uint32_t uHeight) {
    tRct->tFr.uIdx = uIdx;
    tRct->tFr.uHeight = uHeight;
    tRct->tFr.uWidth = uWidth;
}

/* 
 *  @brief - Animates the rect according to the currently chosen rect.
 *
 *  @tRct           - the rect that shall be animated.
 *  @uAnimationId   - id of previously appended animations.
 *  @fSpeed         - amount of ms, before the next frame will appear.
 *
 * */
void vAnimateFrame(tRuntime *tRun, tRect *tRct, uint16_t uAnimationId, float fSpeed) {
    tRct->uAnimationId = uAnimationId;

    vFeatherSleepThisLayerMs(tRun, fSpeed) {
        uint16_t uIIdx = 0;
        tll_foreach(tRct->tAnims, tAnim) {
            if (uIIdx == uAnimationId) {
                uint16_t uFIdx = 0;
                ++tAnim->item.uCurrent;
                if (tll_length(tAnim->item.uFrames) == tAnim->item.uCurrent)
                    tAnim->item.uCurrent = 0;

                tll_foreach(tAnim->item.uFrames, tFrame) {
                    if (uFIdx == tAnim->item.uCurrent)
                        tRct->tFr.uIdx = tFrame->item;
                    ++uFIdx;
                }
            }
            uIIdx++;
        }
    }
}

/* 
 *  @brief - append animation to the rectangle
 * */
uint16_t vRectAppendAnimation(tRect *tRct, uint8_t *uIdx, uint8_t uIdxLength) {
    if (tRct == NULL) {
        vFeatherLogError("Rect is null. Impossible to append animation.");
        return 0;
    }

    tAnimation tAnim = { .uCurrent = 0, .uFrames = tll_init() };
    
    for (uint8_t i = 0; i < uIdxLength; ++i)
        if (uIdx != NULL)
            tll_push_back(tAnim.uFrames, *(uIdx + i));

    tll_push_back(tRct->tAnims, tAnim);
    return tll_length(tRct->tAnims) - 1;
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
    SDL_Rect srcRect, dstRect;
    SDL_Point sdlCenter;
    SDL_Renderer* sdlRend = tRun->sdlRenderer;
    SDL_Texture* texture = (SDL_Texture*)rect->idTextureID;

    int uWidth, uHeight, uColumns;
    SDL_QueryTexture(texture, NULL, NULL, &uWidth, &uHeight);

    // indexing for framing. 
    uColumns = uWidth / rect->tFr.uWidth;
    srcRect.x = (rect->tFr.uIdx % uColumns) * rect->tFr.uWidth;
    srcRect.y = (rect->tFr.uIdx / uColumns) * rect->tFr.uHeight;
    srcRect.w = rect->tFr.uWidth;
    srcRect.h = rect->tFr.uHeight;

    dstRect.x = (int)rect->tCtx.fX;
    dstRect.y = (int)rect->tCtx.fY;
    dstRect.w = (int)(srcRect.w * rect->tCtx.fScaleX);
    dstRect.h = (int)(srcRect.h * rect->tCtx.fScaleY);

    sdlCenter.x = dstRect.x + dstRect.w / 2;
    sdlCenter.y = dstRect.y + dstRect.h / 2;

    // Render the texture with the calculated position, scale, and rotation
    SDL_RenderCopyEx(
        sdlRend,
        texture,
        &srcRect, 
        &dstRect,     
        rect->tCtx.fRotation,
        &sdlCenter,
        SDL_FLIP_NONE
    );
}

void __vFullscreenInner(tRect *tRct, tRuntime *tRun, bool w, bool h) {
    int ww, wh, wr, hr;
    vRuntimeGetWindowDimensions(tRun, &ww, &wh);
    SDL_QueryTexture((SDL_Texture*)tRct->idTextureID, NULL, NULL, &wr, &hr);
    tRct->tCtx.fScaleX = w ? (float)ww / (float)wr : (float)wh / (float)hr;
    tRct->tCtx.fScaleY = h ? (float)wh / (float)hr : (float)ww / (float)wr;
}


/* 
 *  @brief - grows or shrinks rect's context to match with the full window size.
 *
 *  @tRct - object's rect.
 *  @tRun - current runtime.
 * */
void vFullScreenRect(tRect *tRct, tRuntime *tRun) {
    __vFullscreenInner(tRct, tRun, true, true);
}

/* 
 *  @brief - grows or shrinks rect's context to match with the full window width.
 *
 *  @tRct - object's rect.
 *  @tRun - current runtime.
 * */
void vFullScreenRectWidth(tRect *tRct, tRuntime *tRun) {
    __vFullscreenInner(tRct, tRun, true, false);
}

/* 
 *  @brief - grows or shrinks rect's context to match with the full window height.
 *
 *  @tRct - object's rect.
 *  @tRun - current runtime.
 * */
void vFullScreenRectHeight(tRect *tRct, tRuntime *tRun) {
    __vFullscreenInner(tRct, tRun, false, true);
}

/* 
 *  Provides a proper pointer to the rect, based on it's ID.
 *
 *  This must be used to prevent pointer problems, when more rects are added.
 * */
tRect *tGetRect(tRuntime *tRun, uint32_t uRectId) {
    tRect *tRctPtr = NULL;
    tll_foreach(tRun->sScene->lRects, tRct)
        if (tRct->item.uRectId == uRectId)
            tRctPtr = &tRct->item;

    return tRctPtr;
}
