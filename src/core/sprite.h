/*
 * sprite.h
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
#   include<SFML/Graphics/Sprite.hpp>
#else
#   include "drawable.h"
#   include "image.h"
#   include "rect.h"
#   include "vector2.h"
#endif

namespace oci {
namespace core {

#ifdef USE_SFML

using sf::Sprite;

#else

class Sprite : public Drawable {
public:
    Sprite() : mImage() {}
    Sprite(const Image& img, const Vector2f& position = Vector2f(0, 0))
        : Drawable(position), mImage(&img) {}

    Vector2f GetSize() const;
    void SetImage(const Image& img) { mImage = &img; }
    void SetSubRect(const IntRect& /*rect*/) {}

    void DoDraw(SDL_Renderer* renderer) const override;

private:
    const Image* mImage;
};

#endif

} // namespace core
} // namespace oci
