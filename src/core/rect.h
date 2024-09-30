/*
 * rect.h
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
#   include<SFML/Graphics/Rect.hpp>
#else
#   include "vector2.h"

#   include <algorithm>
#endif

namespace oci {

#ifdef USE_SFML

using sf::IntRect;
using sf::FloatRect;

#else

template<typename T>
class Rect {
public:
    Rect() = default;
    Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight)
        : left(rectLeft)
        , top(rectTop)
        , width(rectWidth)
        , height(rectHeight)
    {}

    Rect(const Vector2<T>& position, const Vector2<T>& size)
        : left(position.x)
        , top(position.y)
        , width(size.x)
        , height(size.y)
    {}

    bool contains(const Vector2<T>& point) const noexcept
    {
        T minX = std::min(left, static_cast<T>(left + width));
        T maxX = std::max(left, static_cast<T>(left + width));
        T minY = std::min(top, static_cast<T>(top + height));
        T maxY = std::max(top, static_cast<T>(top + height));

        return (point.x >= minX) && (point.x < maxX) &&
               (point.y >= minY) && (point.y < maxY);
    }

    T left = 0;
    T top = 0;
    T width = 0;
    T height = 0;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#endif

} // namespace oci
