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

#include <string>

namespace oci {

using namespace std::string_literals;

void Image::create(unsigned width, unsigned height, const Color& color) {
    mSurface.reset(
        SDL_CreateRGBSurface(0, width, height, 32,
                             0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff));
    if(!mSurface) [[unlikely]]
        throw std::runtime_error("Cannot create SDL surface: "s + SDL_GetError());
    SDL_FillRect(mSurface.get(), NULL, SDL_MapRGB(mSurface->format,
                                                  color.r, color.g, color.b));
}

bool Image::loadFromMemory(const char* data, std::size_t datasize) {
    mSurface.reset(SDL_LoadBMP_RW(SDL_RWFromConstMem(data, static_cast<int>(datasize)), 1));
    return mSurface != nullptr;
}

bool Image::loadFromFile(const std::string& filename) {
    mSurface.reset(SDL_LoadBMP(filename.c_str()));
    return mSurface != nullptr;
}

void Image::createMaskFromColor(Color color_key) {
    if(mSurface) {
        SDL_SetColorKey(mSurface.get(), 1,
                        SDL_MapRGB(mSurface->format,
                                   color_key.r,
                                   color_key.g,
                                   color_key.b));
    }
}

} // namespace oci

#endif //USE_SFML
