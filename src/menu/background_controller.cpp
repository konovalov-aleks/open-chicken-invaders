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

#include <background/simple_controller.h>
#include <core/mouse.h>
#include <core/vector2.h>
#include <core/window.h>

namespace oci {
namespace menu {

static const float BACKGROUND_MAX_SPEED = 3;

void BackgroundController::Run() {
    const Vector2u window_size = Window::Instance().getSize();
    const Vector2i mouse_pos = Mouse::getPosition(Window::Instance());
    SetSpeed(Vector2f(
        ((mouse_pos.x * 2 * BACKGROUND_MAX_SPEED) / window_size.x - BACKGROUND_MAX_SPEED),
        ((mouse_pos.y * 2 * BACKGROUND_MAX_SPEED) / window_size.y - BACKGROUND_MAX_SPEED)));
    SimpleController::Run();
}

} // namespace menu
} // namespace oci
