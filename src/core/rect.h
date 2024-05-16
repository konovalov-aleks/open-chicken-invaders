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
#endif

namespace oci {

#ifdef USE_SFML

using sf::IntRect;

#else

template<typename T>
class Rect {
public:
    Rect() : Left(), Top(), Right(), Bottom() {}
    Rect(T left, T top, T right, T bottom)
        : Left(left), Top(top), Right(right), Bottom(bottom) {}

    T Left, Top, Right, Bottom;
};

typedef Rect<int> IntRect;

#endif

} // namespace oci
