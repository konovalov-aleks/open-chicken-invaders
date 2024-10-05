/*
 * sprite.cpp
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

#include "sprite.h"

#ifndef USE_SFML

#include "rect.h"
#include "texture.h"
#include "vector2.h"

#include <SDL_rect.h>      // for SDL_Rect

#ifdef ANDROID
#   include "window.h"
#endif

namespace oci {
namespace core {

FloatRect Sprite::getGlobalBounds() const noexcept {
    const Vector2u tex_size = mTexture ? mTexture->getSize() : Vector2u();
    return { getPosition() - getOrigin(), tex_size };
}

void Sprite::DoDraw(SDL_Renderer* renderer) const {
#ifdef ANDROID
    const float scale = static_cast<float>(Window::Instance().GetRealHeight()) /
                        Window::Instance().GetHeight();
#else
    const float scale = 1.0f;
#endif
    if(mTexture) {
        const Vector2u tex_size = mTexture->getSize();
        SDL_Rect dstrect;
        dstrect.x = (getPosition().x - getOrigin().x) * scale;
        dstrect.y = (getPosition().y - getOrigin().y) * scale;
        dstrect.w = tex_size.x * scale;
        dstrect.h = tex_size.y * scale;
        SDL_RenderCopy(renderer, mTexture->SDLObject(), nullptr, &dstrect);
    }
}

} // namespace core
} // namespace oci

#endif //USE_SFML
