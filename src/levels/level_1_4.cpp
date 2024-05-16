/*
 * level_1_4.cpp
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

#include "level_1_4.h"

#include <core/window.h>
#include "factory.h"
#include <objects/modifiers/auto_killable.h>

namespace oci {
namespace levels {

using namespace objects;
using namespace modifiers;

static Factory::Registrator<Level_1_4> reg("level_1_4", "game");

static const float ASTEROID_SPEED = 8.0f;

void Level_1_4::Init() {
    Level_x4::Init(10, 20);
    ShowLevelInfo("4", "meteor shower", "watch out!", true);
}

weak_ptr<objects::Asteroid> Level_1_4::CreateAsteroid() {
    int x = Window::Instance().GetWidth() + 10;
    int y = -10;
    if(rand() % 3)
        x = rand() % Window::Instance().GetWidth() * 3 / 4 +
            Window::Instance().GetWidth() / 4;
    else
        y = rand() % (Window::Instance().GetHeight() / 2);
    bool big = rand() & 1;
    return Storage().CreateObject<AutoKillable<Asteroid, BoundLeft, BoundBottom> >(
                Vector2f(x, y), // position
                6.1f,               // angle
                ASTEROID_SPEED,     // speed
                big ? 5 : 2,        // health
                big ? objects::Asteroid::tRockBig :
                      objects::Asteroid::tRockSmall);   // type
}

} // namespace levels
} // namespace oci
