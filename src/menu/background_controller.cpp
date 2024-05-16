/*
 * background_controller.cpp
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

#include "background_controller.h"

#include <core/input.h>
#include <core/window.h>

namespace oci {
namespace menu {

static const float BACKGROUND_MAX_SPEED = 3;

void BackgroundController::Run() {
    const int window_width = Window::Instance().GetWidth();
    const int window_height = Window::Instance().GetHeight();
    const Input& input = Window::Instance().GetInput();
    SetSpeed(Vector2f(
        ((input.GetMouseX() * 2 * BACKGROUND_MAX_SPEED) / window_width - BACKGROUND_MAX_SPEED),
        ((input.GetMouseY() * 2 * BACKGROUND_MAX_SPEED) / window_height - BACKGROUND_MAX_SPEED)));
    SimpleController::Run();
}

} // namespace menu
} // namespace oci
