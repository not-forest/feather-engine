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

#include "tllist.h"
#include <SDL_ttf.h>
#include <log.h>
#include <rect.h>
#include <font.h>
#include <runtime.h>
#include <intrinsics.h>

// Function to convert sString to char*
char* sStringToCharPtr(const sString* sStr) {
    if (!sStr) 
        return NULL;

    size_t length = tll_length(*sStr);
    if (length == 0)
        return strdup("");

    char* result = (char*)malloc(length + 1);
    if (!result)
        return NULL;
    size_t i = 0;
    tll_foreach(*sStr, item)
        result[i++] = *(char*)item;
    result[length] = '\0';

    return result;
}

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
tText* tTextInit(tRuntime *tRun, tText *tTxt, const char *sInitText, tContext2D tCtx, const char *sFontPath, uint16_t uPriority) {
    tRuntime *_tRun = (tRuntime*) tRun;
    TTF_Font *sdlFont = TTF_OpenFont(sFontPath, 24);

    if (sdlFont == NULL) {
        vFeatherLogError("Unable to append font: %s. %s", strrchr(sFontPath, '/') + 1, TTF_GetError());
        return NULL;
    }

    tRect tRct = { 
        .tAnims = tll_init(), 
        .tCtx = tCtx, 
        .sTexturePath = "TTEXT",
        .tFr.uIdx = 0,
        .uRectId = uRectIDIncrementer,
    };

    tTxt->uRectID = tRct.uRectId;

    if (!strlen(sInitText)) {
        vFeatherLogError("Zero length texts are not allowed.");
        return NULL;
    }

    // Insert the rectangle into the list with priority handling
    tll_foreach(_tRun->sScene->lRects, it) {
        if (it->item.uPriority > uPriority) {
            tll_insert_before(_tRun->sScene->lRects, it, tRct);
        }
    }

    // New higher priority rectangles are pushed to the front
    tll_push_back(_tRun->sScene->lRects, tRct);

    tTxt->uFontSize = 24;
    tTxt->uLength = 0;
    tTxt->sdlFont = sdlFont;
    tTxt->sFontPath = sFontPath;
    vTextAppend(tRun, tTxt, (char*)sInitText);

    return tTxt; 
}

/* 
 *  @brief - allows to change the font and swap new text content.
 * */
void vChangeTextFont(tRuntime* tRun, tText* tTxt, const char* sNewFontPath, uint16_t uNewFontSize) {
    tRect *tRct;
    tRct = tGetRect(tRun, tTxt->uRectID);

    if (tRct == NULL) {
        vFeatherLogError("Unable to get text's Rect. Make sure that it is initialized.");
        return;
    }

    if (sNewFontPath != NULL || tTxt->uFontSize != uNewFontSize) {
        TTF_Font* oldFont = tTxt->sdlFont;
        if (oldFont) {
            TTF_CloseFont(oldFont);
            tTxt->sdlFont = NULL;
        }

        sNewFontPath = (sNewFontPath == NULL) ? tTxt->sFontPath : sNewFontPath;
        TTF_Font* newFont = TTF_OpenFont(sNewFontPath, uNewFontSize);
        if (!newFont) {
            vFeatherLogError("Unable to load new font: %s", TTF_GetError());
            return;
        }

        tTxt->sdlFont = newFont;
        tTxt->uFontSize = uNewFontSize;
    }

    SDL_Surface* sdlSurf = TTF_RenderUTF8(tTxt->sdlFont, sStringToCharPtr(&tTxt->sStr), __FEATHER__WHITE__, __FEATHER__BLACK__);
    tRct->tFr.uWidth = sdlSurf->w; 
    tRct->tFr.uHeight = sdlSurf->h;
    if (!sdlSurf) {
        vFeatherLogError("Unable to render text surface: %s", TTF_GetError());
        TTF_CloseFont(tTxt->sdlFont); // Clean up on error
        tTxt->sdlFont = NULL;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(tRun->sdlRenderer, sdlSurf);
    SDL_FreeSurface(sdlSurf);
    if (!textTexture) {
        vFeatherLogError("Unable to create text texture: %s", SDL_GetError());
        TTF_CloseFont(tTxt->sdlFont);
        tTxt->sdlFont = NULL;
        return;
    }

    tRct->idTextureID = (uintptr_t)textTexture;
}

void __vInnerAppendCharUpdate(tRuntime *tRun, tText *tTxt, char cChar, bool bUpdate) {
    if (cChar != '\0' && cChar != '\n') {
        tll_push_back(tTxt->sStr, cChar);
        tTxt->uLength++;
        if (bUpdate)
            vChangeTextFont(tRun, tTxt, NULL, tTxt->uFontSize);
    }
}

char __vInnerPopCharUpdate(tRuntime *tRun, tText *tTxt, bool bUpdate) {
    char c = '\0';
    if (tll_length(tTxt->sStr) - 1) {
        c = tll_pop_back(tTxt->sStr);
        tTxt->uLength--;
    }
    if (bUpdate)
        vChangeTextFont(tRun, tTxt, NULL, tTxt->uFontSize);
    return c;
}

/* 
 *  @brief - pops one char from the internal string.
 *
 *  Will always return '\0' when the string is empty.
 * */
char vTextPopChar(tRuntime *tRun, tText *tTxt) {
    return __vInnerPopCharUpdate(tRun, tTxt, true);
}

/* 
 *  @brief - clears the whole text.
 * */
char* sTextClear(tRuntime *tRun, tText *tTxt) {
    char* sStr = sStringToCharPtr(&tTxt->sStr); 
    while (vTextPopChar(tRun, tTxt) != '\0');
    return sStr;
}

/* 
 *  @brief - append one char to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @cChar  - char letter to append.
 * */
void vTextAppendChar(tRuntime *tRun, tText *tTxt, char cChar) {
    __vInnerAppendCharUpdate(tRun, tTxt, cChar, true);
}

/* 
 *  @brief - append text slice to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @sSlice - text slice to append.
 * */
void vTextAppend(tRuntime *tRun, tText *tTxt, char *sSlice) {
    while (*(sSlice+1))
        __vInnerAppendCharUpdate(tRun, tTxt, *sSlice++, false);
    if (*sSlice)
        __vInnerAppendCharUpdate(tRun, tTxt, *sSlice, true);
}
