/*
 * shape.cpp
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

#include "shape.h"

#ifndef USE_SFML

namespace oci {

namespace {

    class RectangleShapeImpl : public Shape::Impl {
    public:
        RectangleShapeImpl(float p1x, float p1y, float p2x, float p2y,
                           const Color& color) : mColor(color) {
            mRect.x = p1x;
            mRect.y = p1y;
            mRect.w = p2x - p1x;
            mRect.h = p2y - p1y;
        }

        void Draw(SDL_Renderer* renderer) const override {
            SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, 255);
            SDL_RenderFillRect(renderer, &mRect);
        }

    private:
        Color mColor;
        SDL_Rect mRect;
    };

} // namespace

Shape::Shape(Impl* impl) : mImpl(impl) {}

Shape Shape::Rectangle(float p1x, float p1y, float p2x, float p2y,
                       const Color& color) {
    return Shape(new RectangleShapeImpl(p1x, p1y, p2x, p2y, color));
}

} // namespace oci

#endif // !USE_SFML
