/*
 * input.cpp
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

#include "input.h"

#ifndef USE_SFML

#include "vector2.h"

#ifdef ANDROID
#   include "window.h"
#endif

namespace oci {

bool Input::IsKeyDown(Key::Code key) const {
    return SDL_GetKeyboardState(NULL)[key];
}

bool Input::IsMouseButtonDown(Mouse::Button button) const {
    return (SDL_GetMouseState(NULL, NULL) & button) != 0;
}

namespace {

    inline Vector2i GetMousePos() {
        Vector2i result;
        SDL_GetMouseState(&result.x, &result.y);
        return result;
    }

    inline int Scale(int s) {
    #ifdef ANDROID
        return (s * Window::Instance().GetHeight()) /
                Window::Instance().GetRealHeight();
    #else
        return s;
    #endif
    }

} // namespace

int Input::GetMouseX() const {
    return Scale(GetMousePos().x);
}

int Input::GetMouseY() const {
    return Scale(GetMousePos().y);
}

} // namespace oci

#endif //USE_SFML
