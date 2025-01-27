/**************************************************************************************************
 *  File: font.h
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

#pragma once

#ifndef FEATHER_FONT_H
#define FEATHER_FONT_H

#include <tllist.h>
#include <intrinsics.h>
#include <context2d.h>

typedef tll(char) sString;
#include <rect.h>

/* 
 *  @brief - generate a text block with the use of any TrueType font available on your system, by adding the
 *  font file (.ttf) to your project.
 *
 *  @uFontSize  - font size for display. It is different than scaling the rect manually, since it does not change
 *  the quality of the texture.
 *  @tRct       - underlying rect pointer.
 *  @sStr       - text string allocated on heap.
 *
 *  This structire is a wrapper over regular rect structure, which allows to easily modify the text, written to
 *  the screen.
 * */
typedef struct {
    uint16_t uFontSize;
    uint16_t uLength;
    uint32_t uRectID;
    TTF_Font *sdlFont;

    const char *sFontPath;
    sString sStr;
} tText;

#include <runtime.h>

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
tText* tTextInit(tRuntime *tRun, tText *tTxt, const char *sInitText, tContext2D tCtx, const char *sFontPath, uint16_t uPriority);

/* 
 *  @brief - append one char to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @cChar  - char letter to append.
 * */
void vTextAppendChar(tRuntime *tRun, tText *tTxt, char cChar);

/* 
 *  @brief - pops one char from the internal string.
 *
 *  Will always return '\0' when the string is empty.
 * */
char vTextPopChar(tRuntime *tRun, tText *tTxt);

/* 
 *  @brief - clears the whole text.
 * */
char* sTextClear(tRuntime *tRun, tText *tTxt);

/* 
 *  @brief - append text slice to the currently written data within the text block.
 *
 *  @tTxt   - pointer to the text block to modify.
 *  @sSlice - text slice to append.
 * */
void vTextAppend(tRuntime *tRun, tText *tTxt, char *sSlice);

/* 
 *  @brief - allows to change the font and swap new text content.
 * */
void vChangeTextFont(tRuntime* tRun, tText* tTxt, const char* sNewFontPath, uint16_t uNewFontSize);

#endif
