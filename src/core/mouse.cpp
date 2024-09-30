/*
 * mouse.cpp
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

#include "mouse.h"

#ifndef USE_SFML

#include <SDL.h>

#include "vector2.h"
#include "window.h"

namespace oci {

bool Mouse::isButtonPressed(Mouse::Button button) {
    return (SDL_GetMouseState(NULL, NULL) & button) != 0;
}

inline Vector2i GetMousePos() {
    Vector2i result;
    SDL_GetMouseState(&result.x, &result.y);
    return result;
}

inline int screenToLogical(const Window& wnd, int s) {
#ifdef ANDROID
    return (s * wnd.GetHeight()) / wnd.GetRealHeight();
#else
    (void)wnd;
    return s;
#endif
}

inline int logicalToScreen(const Window& wnd, int s) {
#ifdef ANDROID
    return (s * wnd.GetHeight()) / wnd.GetRealHeight();
#else
    (void)wnd;
    return s;
#endif
}

Vector2i Mouse::getPosition(const Window& relativeTo) {
    Vector2i pos = GetMousePos();
    return { screenToLogical(relativeTo, pos.x),
             screenToLogical(relativeTo, pos.y) };
}

void Mouse::setPosition(const Vector2i& pos, const Window& relativeTo)
{
    SDL_WarpMouseInWindow(
        relativeTo.SDLHandle(),
        logicalToScreen(relativeTo, pos.x),
        logicalToScreen(relativeTo, pos.y)
    );
}

} // namespace oci

#endif //USE_SFML
