/*
 * vector2.h
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
#   include <SFML/System/Vector2.hpp> // IWYU pragma: export
#endif

namespace oci {

#ifdef USE_SFML

using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;

#else

template<typename T>
class Vector2 {
public:
    Vector2() : x(), y() {}

    Vector2(T x, T y) : x(x), y(y) {}

    template<typename T2>
    Vector2(const Vector2<T2>& other) : x(other.x), y(other.y) {}

    template<typename T2>
    Vector2& operator += (const Vector2<T2>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2 operator+ (const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator- (const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    T x, y;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

#endif

} // namespace oci
