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
#   include <SDL2/SDL.h>
#   include "vector2.h"
#endif

namespace oci {

#ifdef USE_SFML

    using sf::Drawable;

#else

    class Drawable {
    public:
        Drawable() {}
        Drawable(const Vector2f& position) : mPosition(position) {}

        const Vector2f& GetPosition() const { return mPosition; }
        void SetPosition(float x, float y) { mPosition.x = x; mPosition.y = y; }
        void SetPosition(const Vector2f& position) { mPosition = position; }
        void SetX(float x) { mPosition.x = x; }
        void SetY(float y) { mPosition.y = y; }
        void Move(float dx, float dy) { mPosition.x += dx; mPosition.y += dy; }
        void Move(const Vector2f& delta) { Move(delta.x, delta.y); }

        const Vector2f& GetCenter() const { return mCenter; }
        void SetCenter(float x, float y) { mCenter.x = x; mCenter.y = y; }
        void SetCenter(const Vector2f& point) { mCenter = point; }

        Vector2f TransformToLocal(const Vector2f& point) const {
            return point - mPosition + mCenter;
        }

        virtual void DoDraw(SDL_Renderer* target) const = 0;

    private:
        Vector2f mPosition;
        Vector2f mCenter;
    };

#endif

}
