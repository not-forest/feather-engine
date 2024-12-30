/**************************************************************************************************
 *  File: scene.h
 *  Desc: Scene defines the current context, layers and state of the program. Atleast one scene must be
 *  provided to initialize the engine.
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

#include <scene.h>
#include <tllist.h>

/* 
 *  @brief - pushes layer to the scene's list.
 *
 *  Each layer is a user defined function that will be scheduled during the update phase.
 *  All layers can access the shared resources and it's local variables.
 * */
void vSceneAppendLayer(tScene *sScene, tLayer vLayer) {
    tll_push_front(sScene->lLayers, vLayer);
}

/* 
 *  @brief - pushes new controller to the scene's list.
 *
 *  Each controller is being handled (or not) before all of the layers, but after the input handling phase.
 *  Controllers are not held by the scene and runtime, so they are preserved within the user defined structure.
 *  They can always be added and removed at runtime.
 * */
void vSceneAppendController(tScene *sScene, tController tCtrl) {
    tll_push_front(sScene->lControllers, tCtrl);
}

/* 
 *  @brief - removes the controller from the scene's list.
 *
 *  Does nothing if the controller is not within the scene's list already.
 * */
void vSceneRemoveController(tScene *sScene, uint32_t uControllerID) {
    tll_foreach(sScene->lControllers, c)
        if (c->item.uControllerID == uControllerID)
            tll_remove(sScene->lControllers, c);
}

/* 
 *  @brief - compare implementation for the layer structure.
 * */
bool bLayerCmp(tLayer l1, tLayer l2) {
    return l1.iPriority < l2.iPriority;
}
