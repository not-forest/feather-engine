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

#include <stdbool.h>
#ifndef FEATHER_AUDIO_H
#define FEATHER_AUDIO_H

#include <intrinsics.h>
#include <tllist.h>

#ifndef FEATHER_RUNTIME_MIXER_MAX_CHANNELS
#define FEATHER_RUNTIME_MIXER_MAX_CHANNELS 42
#endif

typedef tll(Mix_Chunk) tsdlChunks;
typedef tll(Mix_Music*) tsdlMusicList;

/* 
 *  @brief - audio mixer for feather engine's runtime.
 *
 *  @tChunks    - holds different sound chunks for playing (effects, sound, ambience, etc.)
 *  @tMusicList - holds all loaded music for playing.
 *  @bChannels  - bitfield of currently used channels.
 *
 *  Normally one mixer is expected per runtime. It can be handler manually within some local module
 *  in the userspace. By default mixer is working with the maximal value of channels, which is 42.
 *  It can be decreased at any time during the runtime or increase above the maximum at compile time
 *  in Kconfig.
 * */
typedef struct {
    tsdlChunks tChunks;
    tsdlMusicList tMusicList;
    bool bChannels[FEATHER_RUNTIME_MIXER_MAX_CHANNELS];
} tRuntimeMixer;

#endif
