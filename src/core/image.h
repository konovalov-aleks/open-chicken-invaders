/*
 * image.h
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

#pragma once

#ifdef USE_SFML
#   include<SFML/Graphics/Image.hpp>
#else
#   include "color.h"
#   include <portability/memory.h>
#   include <SDL2/SDL.h>
#   include <string>
#   include "vector2.h"
#endif

namespace oci {

#ifdef USE_SFML

typedef sf::Image Image;

#else

class Image {
public:
    Image();
    Image(size_t width, size_t height, const Color& color = Color::Black);
    bool LoadFromMemory(const char* data, size_t datasize);
    bool LoadFromFile(const std::string& filename);
    void CreateMaskFromColor(Color color_key);
    void SetSmooth(bool /*smooth*/) {}

    size_t GetWidth() const;
    size_t GetHeight() const;

    SDL_Texture* GetTexture() const {
        if(!mTexture)
            LoadTexture();
        return mTexture.get();
    }

private:
    bool LoadTexture() const;

    struct TextureDeleter {
        void operator() (SDL_Texture* texture) {
            if(texture)
                SDL_DestroyTexture(texture);
        }
    };

    struct SurfaceDeleter {
        void operator() (SDL_Surface* surface) {
            if(surface)
                SDL_FreeSurface(surface);
        }
    };

    shared_ptr<SDL_Surface> mSurface;
    mutable shared_ptr<SDL_Texture> mTexture;
};

#endif

} // namespace oci
