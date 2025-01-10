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

#include "tllist.h"
#include <runtime.h>
#include <intrinsics.h>
#include <log.h>

/* 
 *  @brief - load a sound effect from file to the sound chunk list.
 *
 *  @tRun       - currently running runtime.
 *  @sFilePath  - path to the audio file. Only files applicable to SDL_Music can be supported.
 *
 *  @returns    - returns an id of the loaded sound.
 * */
uint32_t uLoadMixerSound(tRuntime *tRun, const char* sFilePath) {
    Mix_Chunk tCh;

    if (sFilePath == NULL) {
        vFeatherLogError("Unable to load sound: %d. No such file or directory.", sFilePath);
        return 0;
    }

    vFeatherLogInfo("Loading asset: Sound: %s...", strrchr(sFilePath, '/') + 1);
    tCh = *Mix_LoadWAV(sFilePath);
    tll_push_back(tRun->tMixer.tChunks, tCh);
    return tll_length(tRun->tMixer.tChunks) - 1;
}

/* 
 *  @brief - load music from file to the music list.
 *
 *  @tRun       - currently running runtime.
 *  @sFilePath  - path to the audio file. Only files applicable to SDL_Music can be supported.
 *
 *  @returns    - returns an id of the loaded music.
 * */
uint32_t uLoadMixerMusic(tRuntime *tRun, const char* sFilePath) {
    Mix_Music *tMu;

    if (sFilePath == NULL) {
        vFeatherLogError("Unable to load music file: %d. No such file or directory.", sFilePath);
        return 0;
    }

    vFeatherLogInfo("Loading asset: Music: %s...", strrchr(sFilePath, '/') + 1);
    tMu = Mix_LoadMUS(sFilePath);
    tll_push_back(tRun->tMixer.tMusicList, tMu);
    return tll_length(tRun->tMixer.tMusicList) - 1;
}

/* 
 *  @brief      - play a sound by it's id.
 *  @tRun       - currently running runtime.
 *  @uSoundID   - id value obtained by the loader function.
 *  @iChannel   - one of 42 available SDL channels
 *  @uLoopCount - amount of time, that this sound shall repeat itself.
 *
 *  Will return a warning and play nothing, if the provided sound ID does not exit.
 * */
void vPlaySound(tRuntime *tRun, uint32_t uSoundID, int8_t iChannel, uint32_t uLoopCount) {
    uint32_t uId = 0;

    tll_foreach(tRun->tMixer.tChunks, tChunk) {
        if (uId == uSoundID) {
            // Cannot play until the channel is free. Use vStopSound first to prevent this.
            if (Mix_Playing(iChannel))
                return;

            Mix_PlayChannel(iChannel, &tChunk->item, uLoopCount);
            tRun->tMixer.bChannels[iChannel] = true;
            return;
        }
        ++uId;
    }

    vFeatherLogWarn("Unable to play the requested sound: uSoundId [%d] not found", uSoundID);
}

/* 
 *  @brief      - play a sound by it's id.
 *  @tRun       - currently running runtime.
 *  @uSoundID   - id value obtained by the loader function.
 *  @uLoopCount - amount of time, that this music shall repeat itself.
 *
 *  Will return a warning and play nothing, if the provided sound ID does not exit.
 * */
void vPlayMusic(tRuntime *tRun, uint32_t uMusicID, uint32_t uLoopCount) {
    uint32_t uId = 0;
    tll_foreach(tRun->tMixer.tMusicList, tMusic) {
        if (uId == uMusicID) {
            // Cannot play until the channel is free. Use vStopMusic first to prevent this.
            if (Mix_PlayingMusic())
                return;

            Mix_PlayMusic(tMusic->item, uLoopCount);
            return;
        }
        ++uId;
    }

    vFeatherLogWarn("Unable to play the requested sound: uSoundId [%d] not found", uMusicID);
}

/* 
 *  @brief      - sets the volume of the music.
 *  @tRun       - currently running runtime.
 *  @uVolume    - volume value in percents 0 - 128. Values above 128 are seen as 128.
 * */
void vSetMusicVolume(tRuntime *_, uint8_t uVolume) {
    Mix_VolumeMusic(uVolume);
}

/* 
 *  @brief      - sets the volume of the sound under a provided channel.
 *
 *  @tRun       - currently running runtime.
 *  @uVolume    - volume value in percents 0 - 100. Values above 100 are seen as 100.
 *  @iChannel   - one of 42 available SDL channels
 *
 *  To change the volume of the sound by it's ID, use the sound queue API instead. Note that
 *  it is not necessary to play the sound before changing the volume. Use -1 to affect all chanels
 * */
void vSetSoundVolume(tRuntime *_, int8_t iChannel, uint8_t uVolume) {
    Mix_Volume(iChannel, uVolume);
}

/* 
 *  @brief - stops the sound on a specific channel.
 *  @tRun       - currently running runtime.
 *  @iChannel   - one of 42 available SDL channels
 *
 *  To stop the sound by it's ID, use the sound queue API instead.
 * */
void vStopSound(tRuntime *_, int8_t iChannel) {
    Mix_HaltChannel(iChannel);
}

/* 
 *  @brief - stops currently running music, if there is some.
 *
 *  @tRun - currently running runtime.
 * */
void vStopMusic(tRuntime *_) {
    Mix_HaltMusic();
}

/* 
 *  @brief - Gets the current amount of channels from SDL.
 *
 *  @tRun - currently running runtime.
 * */
int iMixerAmountOfChannels(tRuntime *_) {
    return Mix_AllocateChannels(-1);
}

/*  
 *  @brief      - sets the maximal amount of channels.
 *  @tRun       - currently running runtime.
 *  @iAmount    - maximum amount of channels to set.
 *
 *  Note that this shall be smaller or equal to FEATHER_RUNTIME_MIXER_MAX_CHANNELS.
 *  */
void vMixerSetChannelAmount(tRuntime *_, int iAmount) {
    Mix_AllocateChannels(iAmount); 
}

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
void vQueueSound(tRuntime *tRun, uint32_t uSoundID, uint32_t uLoopCount) {
    for(;;) 
        for (uint8_t uCh = 0; uCh < FEATHER_RUNTIME_MIXER_MAX_CHANNELS; ++uCh)
            if (!Mix_Playing(uCh)) 
                vPlaySound(tRun, uSoundID, uCh, uLoopCount); 
}

#endif
