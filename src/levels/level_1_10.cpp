/*
 * level_1_10.cpp
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

#include "level_1_10.h"

#include <core/window.h>
#include "factory.h"

namespace oci {
namespace levels {

using namespace objects;

namespace {
    /// Начальная координата Y босса
    static const float BOSS_INITIAL_Y_POS = -60.0f;
    /// Координата Y точки, к которой изначально летит босс
    static const float BOSS_INITIAL_DESTINATION_POINT_Y = 60.0f;
    /// Скорость босса
    extern const float BOSS_SPEED = 4.0f;

    static Factory::Registrator<Level_1_10> reg("level_1_10", "game");
} // namespace

void Level_1_10::Init() {
    GameLevel::Init("level_1_10");
    const Vector2u wndSize = Window::Instance().getSize();
    mBoss = Storage().CreateObject<Boss1>(
        Vector2f(wndSize.x / 2, BOSS_INITIAL_Y_POS),
        Vector2f(wndSize.x / 2, BOSS_INITIAL_DESTINATION_POINT_Y),
        BOSS_SPEED);
    ShowLevelInfo("10", "show'em who's boss!");
}

void Level_1_10::Run() {
    Level::Run();
    if(mBoss.expired())
        EndLevel();
}

} // namespace levels
} // namespace oci
