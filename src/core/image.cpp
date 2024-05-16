/*
 * image.cpp
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

#include "image.h"

#ifndef USE_SFML

#include "window.h"

namespace oci {

namespace {


} // namespace

Image::Image() {}

Image::Image(size_t width, size_t height, const Color& color) {
    mSurface = shared_ptr<SDL_Surface>(
        SDL_CreateRGBSurface(0, width, height, 32,
                             0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff),
        SurfaceDeleter());
    if(!mSurface)
        throw std::runtime_error(std::string("Cannot create SDL surface: ") +
                                 SDL_GetError());
    SDL_FillRect(mSurface.get(), NULL, SDL_MapRGB(mSurface->format,
                                                  color.r, color.g, color.b));
}

bool Image::LoadTexture() const {
    if(!mSurface) {
        printf("SDL error: %s\n", SDL_GetError());
        return false;
    }
    mTexture = shared_ptr<SDL_Texture>(
        SDL_CreateTextureFromSurface(Window::Instance().GetRenderer(), mSurface.get()),
        TextureDeleter());
    return mTexture != NULL;
}

bool Image::LoadFromMemory(const char* data, size_t datasize) {
    puts("Load from memory");
    mSurface = shared_ptr<SDL_Surface>(
        SDL_LoadBMP_RW(SDL_RWFromConstMem(data, datasize), 1),
        SurfaceDeleter());
    return mSurface != NULL;
}

bool Image::LoadFromFile(const std::string& filename) {
    printf("Load from file %s\n", filename.c_str());
    mSurface = shared_ptr<SDL_Surface>(
        SDL_LoadBMP(filename.c_str()),
        SurfaceDeleter());
    return mSurface != NULL;
}

void Image::CreateMaskFromColor(Color color_key) {
    if(mSurface) {
        SDL_SetColorKey(mSurface.get(), 1,
                        SDL_MapRGB(mSurface->format,
                                   color_key.r,
                                   color_key.g,
                                   color_key.b));
        mTexture = shared_ptr<SDL_Texture>();
    }
}

size_t Image::GetWidth() const {
    return mSurface ? mSurface->w : 0;
}

size_t Image::GetHeight() const {
    return mSurface ? mSurface->h : 0;
}

} // namespace oci

#endif //USE_SFML
