/*
 * texture.cpp
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

#include "texture.h"

#ifndef USE_SFML

#include "critical_error.h"
#include "window.h"

#include <cassert>

namespace oci {

bool Texture::loadFromImage(const Image& img)
{
    SDL_Surface* surface = img.SDLHandle();
    if(!surface) [[unlikely]]
        return false;

    mTexture = std::unique_ptr<SDL_Texture, TextureDeleter>(
        SDL_CreateTextureFromSurface(Window::Instance().GetRenderer(), surface));
    return mTexture.get();
}

Vector2u Texture::getSize() const
{
    assert(mTexture);
    int w, h;
    if(SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &w, &h)) [[unlikely]]
        CriticalError("SDL_QueryTexture failed\n", SDL_GetError());
    return Vector2u(w, h);
}

} // namespace oci

#endif //USE_SFML
