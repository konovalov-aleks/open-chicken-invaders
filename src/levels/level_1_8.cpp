/*
 * level_1_8.cpp
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

#include "level_1_8.h"

#include "factory.h"
#include "game_level.h"
#include "level.h"
#include <context/object_storage.h>
#include <core/vector2.h>
#include <core/window.h>
#include <objects/characters/asteroid.h>
#include <objects/characters/jumping_asteroid.h>
#include <utils/cleanup_container.h>

#include <cstdlib>
#include <functional>
#include <string_view>

namespace oci {
namespace levels {

static Factory::Registrar<CLevel_1_8> reg("level_1_8", "game", 0);

static const float ASTEROID_SPEED = 8.0f;
static const float INITIAL_POS_Y = 20.0f;

void CLevel_1_8::Init(int Step) {
    GameLevel::Init("level_1_4");
    mAsteroids.push_front(
        Storage().CreateObject<objects::JumpingAsteroid>(
            Vector2f(static_cast<float>(std::rand() % Window::Instance().getSize().x),
                     INITIAL_POS_Y),     // position
            1,                           // angle
            ASTEROID_SPEED,              // speed
            10,                          // health
            objects::Asteroid::tRockBig, // type
            true,                        // split
            std::bind(static_cast<
                    void(AsteroidsList::*)(const AsteroidsList::value_type&)
                >(&AsteroidsList::push_front), &mAsteroids, std::placeholders::_1)
        )
    );
    ShowLevelInfo(Step == 0 ? "8" : "18", "asteroids!", "no, really", true);
}

void CLevel_1_8::Run() {
    Level::Run();
    cleanup(mAsteroids);
    if(mAsteroids.empty())
        EndLevel();
}

} // namespace levels
} // namespace oci
