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

#   include <SDL_surface.h>

#   include <cstddef>
#   include <memory>
#   include <string>
#endif

namespace oci {

#ifdef USE_SFML

using Image = sf::Image;

#else

class Image {
public:
    Image() = default;
    Image(Image&&) = default;
    Image& operator= (Image&&) = default;

    void create(unsigned width, unsigned height, const Color& color = Color::Black);
    bool loadFromMemory(const char* data, std::size_t datasize);
    bool loadFromFile(const std::string& filename);
    void createMaskFromColor(Color color_key);

    SDL_Surface* SDLHandle() const noexcept { return mSurface.get(); }

private:
    struct SurfaceDeleter {
        void operator() (SDL_Surface* surface) {
            if(surface)
                SDL_FreeSurface(surface);
        }
    };

    std::unique_ptr<SDL_Surface, SurfaceDeleter> mSurface;
};

#endif

} // namespace oci
