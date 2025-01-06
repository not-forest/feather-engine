/**************************************************************************************************
 *  File: life.c
 *  Desc: Implementation of Conway's Game of Life, with the use of several simple rules handler by
 *  feather layers:
 *     - Any live cell with fewer than two live neighbours dies, as if by underpopulation.
 *     - Any live cell with two or three live neighbours lives on to the next generation.
 *     - Any live cell with more than three live neighbours dies, as if by overpopulation.
 *     - Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 *
 *  This example also shows the performance under a large amount of rects.
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
#include <runtime.h>
#include <stdlib.h>
#include <time.h>

#define CANVAS_SIZE 50
#define BLOCK_SIZE 15

struct {
    tRect tRcts[CANVAS_SIZE][CANVAS_SIZE];
    bool tBools[CANVAS_SIZE][CANVAS_SIZE];
} tGameCanvas;

FEATHER_SCENE(GoL);

void cfg(tRuntime *tRun) {
    tRun->sScene = &GoL;
    tRun->cMainWindowName = "Conway's Game of Life";
}

RUNTIME_CONFIGURE(cfg);

/* Will the canvas with rects 50/50 (black | white) */
FEATHER_LAYER(&GoL, iPerformNTimes(1), InitGameCanvas,
    void vRestartBoard(void*,tController*);
    void vGameOfLife(tRuntime*,int,int);    
    tKeyboardController tRestartButton; 
,{
    srand(time(NULL));
    tRuntime *tRun = tThisRuntime();
    vRestartBoard((void*)tRun, NULL);

    /* Adding a restart button handling to the game. */
    vKeyboardControllerInit(tRun, &tRestartButton);
    vKeyboardOnPress(&tRestartButton, SDLK_r, fControllerHandler(vRestartBoard));
});

FEATHER_LAYER(&GoL, 1, GameOfLifeUpdate,,{
    tRuntime *tRun = tThisRuntime();
    vFeatherSleepThisLayerMs(tRun, 100)
        for (int i = 0; i < CANVAS_SIZE; ++i) 
            for (int j = 0; j < CANVAS_SIZE; ++j)
                vGameOfLife(tRun, i, j);
})

/* Restarts the board */
void vRestartBoard(void *vRun, tController* tCtrl) {
    tRuntime* tRun = (tRuntime*)vRun;
    tContext2D tCtx = tContextInit();
    tRect *tRct = NULL;
    tCtx.fScaleX = BLOCK_SIZE;
    tCtx.fScaleY = BLOCK_SIZE;

    for (int i = 0; i < CANVAS_SIZE; ++i) 
        for (int j = 0; j < CANVAS_SIZE; ++j) {
            tCtx.fX = i * BLOCK_SIZE;
            tCtx.fY = j * BLOCK_SIZE;

            tRct = tInitRect(tRun, tCtx, 1, NULL);
            tGameCanvas.tBools[i][j] = false;

            if (rand() % 8 == 0) {
                vChangeRectColor(tRun, tRct, (SDL_Color) { 255, 255, 255, 255 });
                tGameCanvas.tBools[i][j] = true;
            } else 
                vChangeRectColor(tRun, tRct, (SDL_Color) { 0, 0, 0, 255 });


            tGameCanvas.tRcts[i][j] = *tRct;
        }
}

// The main game logic.
void vGameOfLife(tRuntime *tRun, int i, int j) {
    uint8_t uNeighb = 0;

    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0)
                continue;

            int ni = i + di;
            int nj = j + dj;

            if (ni >= 0 && ni < CANVAS_SIZE && nj >= 0 && nj < CANVAS_SIZE) {
                if (tGameCanvas.tBools[ni][nj] == 1) {
                    uNeighb++;
                }
            }
        }
    }

    // Apply the Game of Life rules:
    if (tGameCanvas.tBools[i][j] == 1) {
        // Rule 1: Any live cell with fewer than two live neighbours dies (underpopulation).
        // Rule 3: Any live cell with more than three live neighbours dies (overpopulation).
        if (uNeighb < 2 || uNeighb > 3) {
            vChangeRectColor(tRun, &tGameCanvas.tRcts[i][j], (SDL_Color) {0, 0, 0, 255});
            tGameCanvas.tBools[i][j] = false;
        }
        // Rule 2: Any live cell with two or three live neighbours lives on to the next generation.
        // (No need to change color as it's already alive)
    } else {
        // Rule 4: Any dead cell with exactly three live neighbours becomes a live cell (reproduction).
        if (uNeighb == 3) {
            vChangeRectColor(tRun, &tGameCanvas.tRcts[i][j], (SDL_Color) {255, 255, 255, 255});
            tGameCanvas.tBools[i][j] = true;
        }
    }
}


