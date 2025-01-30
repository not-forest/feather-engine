/**************************************************************************************************
 *  File: terminal.c
 *  Desc: Custom terminal example, that shows how to create a simple terminal environment for custom
 *  debuggers or shells.
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

#include <feather.h>
#include <stdlib.h>

FEATHER_SCENE(Shell)

void cfg(tRuntime *tRun) {
    tRun->sScene = &Shell;
    tRun->cMainWindowName = "Feather Powered Terminal";
}

FILE *fPipe;

FEATHER_LAYER(&Shell, iPerformNTimes(1), TerminalInit,
    tText tTxt;
    tContext2D tCtx = tContextInit();

    void vHandleTextInput(void*, tController*);
    void vHandleSpecialKeys(void*, tController*);
    void vHandleCmd(tRuntime*, const char*);
,{
    tRuntime *tRun = tThisRuntime();

    tTxt = *tTextInit(tRun, &tTxt, "> ", tCtx, "assets/FiraCode-Bold.ttf", 1);
    // Initialize controller for text input and special keys
    tControllerInit(tRun, SDL_TEXTINPUT, NULL, fControllerHandler(vHandleTextInput));
    tControllerInit(tRun, SDL_KEYDOWN, NULL, fControllerHandler(vHandleSpecialKeys));

    SDL_StartTextInput(); // Enable text input handling
});

void vHandleTextInput(void *vRun, tController *tCtrl) {
    tRuntime *tRun = (tRuntime*)vRun;
    // Append the text input (captured as a string) to the text buffer
    vTextAppend(tRun, &tTxt, tCtrl->sdlEvent.text.text);
}

void vHandleSpecialKeys(void *vRun, tController *tCtrl) {
    tRuntime *tRun = (tRuntime*)vRun;
    const char *cCmd;

    switch (tCtrl->sdlEvent.key.keysym.sym) {
        case SDLK_BACKSPACE:
            vTextPopChar(tRun, &tTxt); // Remove the last character
            break;
        case SDLK_RETURN:
        case SDLK_RETURN2:
            // Move to the next line and execute the command
            tCtx.fX = 0; 
            tCtx.fY += tTxt.uFontSize;

            // Reset the text buffer for the new prompt
            tTxt = *tTextInit(tRun, &tTxt, " ", tCtx, "assets/FiraCode-Bold.ttf", 1);
            cCmd = sTextClear(tRun, &tTxt);

            // Retrieve and execute the command
            vHandleCmd(tRun, cCmd);
            break;
    }
}

void vHandleCmd(tRuntime *tRun, const char* cCmd) {
    cCmd++;

    char cmdWithStderr[1056] = {0};
    snprintf(cmdWithStderr, sizeof(cmdWithStderr), "%s 2>&1", cCmd);

    fPipe = popen(cmdWithStderr, "r");
    if (!fPipe) {
        vFeatherLogError("Unable to perform command!");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fPipe) != NULL) {
        vTextAppend(tRun, &tTxt, buffer);
    }

    if (tTxt.uLength > 0) {
        tCtx.fX = 0; 
        tCtx.fY += tTxt.uFontSize;
        tTxt = *tTextInit(tRun, &tTxt, " ", tCtx, "assets/FiraCode-Bold.ttf", 1);
        tCtx.fX = 0; 
        tCtx.fY += tTxt.uFontSize;
        tTxt = *tTextInit(tRun, &tTxt, " ", tCtx, "assets/FiraCode-Bold.ttf", 1);
    }
    sTextClear(tRun, &tTxt);
}

RUNTIME_CONFIGURE(cfg);
