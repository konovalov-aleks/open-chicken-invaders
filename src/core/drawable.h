/*
 * drawable.h
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
#   include<SFML/Graphics/Drawable.hpp>
#else
#   include "vector2.h"

#   include <SDL.h>
#endif

namespace oci {

#ifdef USE_SFML

    using sf::Drawable;

#else

    class Drawable {
    public:
        Drawable() {}
        Drawable(const Vector2f& position) : mPosition(position) {}

        virtual ~Drawable() = default;

        const Vector2f& getPosition() const { return mPosition; }
        void setPosition(float x, float y) { mPosition.x = x; mPosition.y = y; }
        void setPosition(const Vector2f& position) { mPosition = position; }
        void move(float dx, float dy) { mPosition.x += dx; mPosition.y += dy; }
        void move(const Vector2f& delta) { move(delta.x, delta.y); }

        const Vector2f& getOrigin() const { return mOrigin; }
        void setOrigin(float x, float y) { mOrigin.x = x; mOrigin.y = y; }
        void setOrigin(const Vector2f& point) { mOrigin = point; }

        virtual void DoDraw(SDL_Renderer* target) const = 0;

    private:
        Vector2f mPosition;
        Vector2f mOrigin;
    };

#endif

}
