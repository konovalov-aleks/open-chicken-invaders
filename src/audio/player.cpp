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
#elif __EMSCRIPTEN__
#   include <SDL/SDL_audio.h>
#   include <SDL/SDL_mixer.h>
#   include <SDL/SDL_mixer.h>
#else
#   include <SDL_audio.h>
#   include <SDL_mixer.h>
#   include <SDL_rwops.h>
#endif

#include "controller.h"
#include <core/critical_error.h>
#include <resources/loader.h>
#include <utils/cache.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace oci {
namespace audio {

#ifdef USE_SFML

namespace {

class Loader {
public:
    sf::SoundBuffer operator()(std::string_view name) {
        std::filesystem::path path = std::filesystem::path("sfx/") / name;
        std::vector<char> data =
            resources::ResourceLoader::Instance().GetData(path.string());
        if(data.empty()) [[unlikely]]
            CriticalError("Sound resource \"sfx/", name, "\" is empty");
        sf::SoundBuffer sb;
        if(!sb.loadFromMemory(&data[0], data.size())) [[unlikely]]
            CriticalError("Cannot load sound \"", name, '"');
        return sb;
    }
};

class ControllerImpl : public Controller {
public:
    ControllerImpl(const sf::SoundBuffer& sb, bool autoplay, bool loop) :
        mSound(sb)
    {
        mSound.setLoop(loop);
        if(autoplay)
            mSound.play();
    }

    virtual bool IsPlaying() const override {
        return mSound.getStatus() == sf::Sound::Playing;
    }

private:
    sf::Sound mSound;
};

} // namespace

std::unique_ptr<Controller> Play(std::string_view name, bool autoplay,
                                 bool loop) {
    static Cache<sf::SoundBuffer, Loader> cache;
    const sf::SoundBuffer& sb = cache.Get(name);
    return std::make_unique<ControllerImpl>(sb, autoplay, loop);
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
    std::shared_ptr<Mix_Chunk> operator()(std::string_view name) {
        std::filesystem::path path = std::filesystem::path("sfx/") / name;
        std::vector<char> data =
            resources::ResourceLoader::Instance().GetData(path.string());
        if(data.empty()) [[unlikely]]
            CriticalError("Sound resource \"sfx/", name, "\" is empty");
        std::shared_ptr<Mix_Chunk> res = std::shared_ptr<Mix_Chunk>(
            Mix_LoadWAV_RW(SDL_RWFromConstMem(&data[0], static_cast<int>(data.size())), 1),
            MixChunkDeleter());
        if(!res) [[unlikely]]
            CriticalError("Cannot load sound \"", name, "\"\n", Mix_GetError());
        return res;
    }
};

class ControllerImpl : public Controller {
public:
    ControllerImpl(Mix_Chunk& mix, bool autoplay, bool loop)
        : mChannel(-1) {
        if(autoplay)
            mChannel = Mix_PlayChannel(-1, &mix, loop ? -1 : 0);
    }

    virtual ~ControllerImpl() {
        if(IsPlaying())
            Mix_HaltChannel(mChannel);
    }

    virtual bool IsPlaying() const override {
        return mChannel != -1 && Mix_Playing(mChannel);
    }

private:
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

std::unique_ptr<Controller> Play(std::string_view name, bool autoplay, bool loop) {
    static Cache<std::shared_ptr<Mix_Chunk>, Loader> cache;
    const std::shared_ptr<Mix_Chunk>& mix = cache.Get(name);
    return std::make_unique<ControllerImpl>(*mix, autoplay, loop);
}

#endif

} // namespace audio
} // namespace oci
