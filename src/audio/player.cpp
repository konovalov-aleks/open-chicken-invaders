/*
 * player.cpp
 * This file is part of OCI (Open Chicken Invaders)
 *
 * Copyright (C) 2010-2014 - Aleksey Konovalov (konovalov.aleks@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "player.h"

#ifdef USE_SFML
#   include <SFML/Audio/Sound.hpp>
#   include <SFML/Audio/SoundBuffer.hpp>
#else
#   include <SDL2/SDL_mixer.h>
#endif

#include <portability/cpp11.h>
#include <resources/loader.h>
#include <stdexcept>
#include <utils/cache.h>

namespace oci {
namespace audio {

#ifdef USE_SFML

namespace {

class Loader {
public:
    sf::SoundBuffer operator()(const std::string& name) {
        std::vector<char> data =
            resources::ResourcesLoader::Instance().GetData("sfx/" + name);
        if(data.empty())
            throw std::logic_error("Sound resource \"sfx/" + name +
                                   "\" is empty");
        sf::SoundBuffer sb;
        if(!sb.LoadFromMemory(&data[0], data.size()))
            throw std::logic_error("Cannot load sound \"" + name + "\"");
        return sb;
    }
};

class ControllerImpl : public Controller {
public:
    ControllerImpl(const sf::SoundBuffer& sb, bool autoplay, bool loop) :
        mSound(sb, loop)
    {
        if(autoplay)
            mSound.Play();
    }

    virtual bool IsPlaying() const override {
        return mSound.GetStatus() == sf::Sound::Playing;
    }

private:
    sf::Sound mSound;
};

} // namespace

std::shared_ptr<Controller> Play(const std::string& name, bool autoplay,
                                 bool loop) {
    static Cache<sf::SoundBuffer, Loader> cache;
    const sf::SoundBuffer& sb = cache.Get(name);
    return std::make_shared<ControllerImpl>(sb, autoplay, loop);
}

#else

namespace {

class MixChunkDeleter {
public:
    void operator()(Mix_Chunk* chunk) {
        if(chunk)
            Mix_FreeChunk(chunk);
    }
};

class Loader {
public:
    shared_ptr<Mix_Chunk> operator()(const std::string& name) {
        std::vector<char> data =
            resources::ResourcesLoader::Instance().GetData("sfx/" + name);
        if(data.empty())
            throw std::logic_error("Sound resource \"sfx/" + name +
                                   "\" is empty");
        shared_ptr<Mix_Chunk> res = shared_ptr<Mix_Chunk>(
            Mix_LoadWAV_RW(SDL_RWFromConstMem(&data[0], data.size()), 1),
            MixChunkDeleter());
    printf("load %s\n", name.c_str());
        if(!res)
            throw std::logic_error("Cannot load sound \"" + name + "\": " +
                                   Mix_GetError());
        return res;
    }
};

class ControllerImpl : public Controller {
public:
    ControllerImpl(shared_ptr<Mix_Chunk> mix, bool autoplay, bool loop)
        : mMixChunk(mix), mChannel(-1) {
        if(autoplay)
            mChannel = Mix_PlayChannel(-1, mMixChunk.get(), loop ? -1 : 0);
    }

    virtual ~ControllerImpl() {
        if(IsPlaying())
            Mix_HaltChannel(mChannel);
    }

    virtual bool IsPlaying() const override {
        return mChannel != -1 && Mix_Playing(mChannel);
    }

private:
    shared_ptr<Mix_Chunk> mMixChunk;
    int mChannel;
};

class SDLMixerInitializer {
public:
    SDLMixerInitializer() {
        Mix_Init(0);
        Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 1, 1024);
    }

    ~SDLMixerInitializer() {
        Mix_Quit();
    }
};

static SDLMixerInitializer sdl_mixer_initializer;

} // namespace

shared_ptr<Controller> Play(const std::string& name, bool autoplay,
                            bool loop) {
    static Cache<shared_ptr<Mix_Chunk>, Loader> cache;
    const shared_ptr<Mix_Chunk>& mix = cache.Get(name);
    return make_shared<ControllerImpl>(mix, autoplay, loop);
}

#endif

} // namespace audio
} // namespace oci
