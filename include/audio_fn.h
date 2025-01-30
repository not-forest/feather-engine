/**************************************************************************************************
 *  File: audio.h
 *  Desc: module for controlling audio within the system.
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

#ifndef FEATHER_AUDIO_FN_H
#define FEATHER_AUDIO_FN_H

#include <runtime.h>
#include <audio.h>

/* 
 *  @brief - load a sound effect from file to the sound chunk list.
 *
 *  @tRun       - currently running runtime.
 *  @sFilePath  - path to the audio file. Only files applicable to SDL_Music can be supported.
 *
 *  @returns    - returns an id of the loaded sound.
 * */
uint32_t uLoadMixerSound(tRuntime *tRun, const char* sFilePath);

/* 
 *  @brief - load music from file to the music list.
 *
 *  @tRun       - currently running runtime.
 *  @sFilePath  - path to the audio file. Only files applicable to SDL_Music can be supported.
 *
 *  @returns    - returns an id of the loaded music.
 * */
uint32_t uLoadMixerMusic(tRuntime *tRun, const char* sFilePath);

/* 
 *  @brief      - play a sound by it's id.
 *  @tRun       - currently running runtime.
 *  @uSoundID   - id value obtained by the loader function.
 *  @iChannel   - one of 42 available SDL channels
 *  @uLoopCount - amount of time, that this sound shall repeat itself.
 *
 *  Will return a warning and play nothing, if the provided sound ID does not exit.
 * */
void vPlaySound(tRuntime *tRun, uint32_t uSoundID, int8_t iChannel, uint32_t uLoopCount);

/* 
 *  @brief      - play a sound by it's id.
 *  @tRun       - currently running runtime.
 *  @uSoundID   - id value obtained by the loader function.
 *  @uLoopCount - amount of time, that this msuic shall repeat itself.
 *
 *  Will return a warning and play nothing, if the provided sound ID does not exit.
 * */
void vPlayMusic(tRuntime *tRun, uint32_t uSoundID, uint32_t uLoopCount);

/* 
 *  @brief      - sets the volume of the music.
 *  @tRun       - currently running runtime.
 *  @uVolume    - volume value in percents 0 - 100. Values above 100 are seen as 100.
 * */
void vSetMusicVolume(tRuntime *tRun, uint8_t uVolume);

/* 
 *  @brief      - sets the volume of the sound under a provided channel.
 *
 *  @tRun       - currently running runtime.
 *  @uVolume    - volume value in percents 0 - 100. Values above 100 are seen as 100.
 *  @iChannel   - one of 42 available SDL channels
 *
 *  To change the volume of the sound by it's ID, use the sound queue API instead. Note that
 *  it is not necessary to play the sound before changing the volume.
 * */
void vSetSoundVolume(tRuntime *tRun, int8_t iChannel, uint8_t uVolume);

/* 
 *  @brief - stops the sound on a specific channel.
 *  @tRun       - currently running runtime.
 *  @iChannel   - one of 42 available SDL channels
 *
 *  To stop the sound by it's ID, use the sound queue API instead.
 * */
void vStopSound(tRuntime *tRun, int8_t iChannel);

/* 
 *  @brief - stops currently running music, if there is some.
 *
 *  @tRun - currently running runtime.
 * */
void vStopMusic(tRuntime *tRun);

/* 
 *  @brief - Gets the current amount of channels.
 *
 *  @tRun - currently running runtime.
 * */
int iMixerAmountOfChannels(tRuntime *tRun);

/*  
 *  @brief      - sets the maximal amount of channels.
 *  @tRun       - currently running runtime.
 *  @iAmount    - maximum amount of channels to set.
 *
 *  Note that this shall be smaller or equal to FEATHER_RUNTIME_MIXER_MAX_CHANNELS.
 *  */
void vMixerSetChannelAmount(tRuntime *tRun, int iAmount);


/* 
 *  @brief      - queues a sound by it's id.
 *
 *  @tRun       - currently running runtime.
 *  @uSoundID   - id value obtained by the loader function.
 *  @uLoopCount - amount of time, that this sound shall repeat itself.
 *
 *  Will return a warning and play nothing, if the provided sound ID does not exit. if
 *  the queue is full, then the function will halt, until some channel will be freed.
 *  Do not play music here.
 * */
void vQueueSound(tRuntime *tRun, uint32_t uSoundID, uint32_t uLoopCount);

#endif
