/*
 * mouse.h
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
#   include <SFML/Window/Mouse.hpp>
#else
#   include <SDL_mouse.h>

#   include "vector2.h"
#endif

namespace oci {

#ifdef USE_SFML

using sf::Mouse;

#else

class Window;

class Mouse {
public:
    enum Button {
        Left   = SDL_BUTTON_LMASK,
        Right  = SDL_BUTTON_RMASK,
        Middle = SDL_BUTTON_MMASK
    };

    static bool isButtonPressed(Button);
    static Vector2i getPosition(const Window& relativeTo);
    static void setPosition(const Vector2i&, const Window& relativeTo);
};

#endif

} // namespace oci
