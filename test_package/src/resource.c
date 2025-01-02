/**************************************************************************************************
 *  File: resource.c
 *  Desc: Defines the use of Feather's resources, as a mutual exclusive user defined data types, which are
 *  shared between all layers of all scenes. Multiple scene can use one resource, meanwhile engine will make
 *  sure no collisions are met.
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

// Creating a new scene.
FEATHER_SCENE(Scene1);

// Here we append the scene to runtime, so we wont obtain an error.
void cfg(tRuntime *tRun) {
    vFeatherLogInfo("My custom runtime configuration function!!!!");
    tRun->sScene = &Scene1;
    tRun->cMainWindowName = "Resource Example";
}

typedef struct {
    int hp;
    int speed;
} tCharacter;
static tCharacter Player = { .hp = 100, .speed = 5};

// Healing
FEATHER_LAYER(&Scene1, 1, PlayerHealerFunction,,{
    if (Player.hp > 0) { 
        Player.hp++;
        vFeatherLogInfo("Player healed HP = %d", Player.hp);
    }
});

// Loosing Hp
FEATHER_LAYER(&Scene1, 2, PlayerHurtFunction,,{
    if (Player.hp > 0) {
        Player.hp -= 25; 
        vFeatherLogInfo("Player hurt HP = %d", Player.hp);
    } else
        vFeatherLogFatal("Player is dead!");

});

// Here we configure the runtime, so we wont obtain an error.
RUNTIME_CONFIGURE(cfg);
