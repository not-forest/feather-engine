/**************************************************************************************************
 *  File: font.c
 *  Desc: allows to generate text output within the application, with the use of any TrueType fonts (.ttf).
 *  It generates a textured rect for you, which can be manipulated just like any regular texture, with the
 *  ability of changing the text on run.
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

#include <log.h>
#include <rect.h>
#include <font.h>
#include <runtime.h>
#include <intrinsics.h>

/* 
 *  @brief - creates a new text unit.
 *
 *  @tRun       - currently running runtime.
 *  @tTxt       - pointer to unitialized text variable.
 *  @sInitText  - static iniitialization text. It can be modified on go
 *  @tCtx       - 2D context of the text block.
 *  @sFontPath  - either relative or full path to the font file.
 *  @uPriority  - priority of rendering the text block.
 * */
tText* tTextInit(struct tRuntime *tRun, tText *tTxt, const char *sInitText, tContext2D tCtx, const char *sFontPath, uint16_t uPriority) {
    tRuntime *_tRun = (tRuntime*) tRun;
    TTF_Font *sdlFont = TTF_OpenFont(sFontPath, 24);

    if (sdlFont == NULL) {
        vFeatherLogError("Unable to append font: %s. %s", strrchr(sFontPath, '/') + 1, TTF_GetError());
        return NULL;
    }

    tRect tRct = { 
        .tAnims = tll_init(), 
        .tCtx = tCtx, 
        .sTexturePath = "TEXTR",
        .tFr.uIdx = 0 
    };

    if (!strlen(sInitText)) {
        vFeatherLogError("Zero length texts are not allowed.");
        return NULL;
    }

    SDL_Surface* sdlSurf = TTF_RenderText_Solid(sdlFont, sInitText, __FEATHER__WHITE__);

    if (sdlSurf == NULL) {
        vFeatherLogError("Unable to create text surface: %s. %s", strrchr(sFontPath, '/') + 1, SDL_GetError());
        return NULL;
    }

    if (__vRectFromTextureRaw(_tRun, &tRct, sdlSurf))
        return NULL;

    // Insert the rectangle into the list with priority handling
    tll_foreach(_tRun->sScene->lRects, it) {
        if (it->item.uPriority > uPriority) {
            tll_insert_before(_tRun->sScene->lRects, it, tRct);
            tRct = *(tRect*)it->prev;
        }
    }

    // New higher priority rectangles are pushed to the front
    tll_push_back(_tRun->sScene->lRects, tRct);

    tTxt->tRct = (struct tRect*)_tRun->sScene->lRects.tail;
    tTxt->uFontSize = 24;
    vTextAppend(tTxt, (char*)sInitText);

    return tTxt; 
}


/* 
 *  @brief - append one char to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @cChar  - char letter to append.
 * */
void vTextAppendChar(tText *tTxt, char cChar) {
    tll_push_front(tTxt->sStr, cChar);
}

/* 
 *  @brief - append text slice to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @sSlice - text slice to append.
 * */
void vTextAppend(tText *tTxt, char *sSlice) {
    while (*sSlice)
        vTextAppendChar(tTxt, *sSlice++);
}
