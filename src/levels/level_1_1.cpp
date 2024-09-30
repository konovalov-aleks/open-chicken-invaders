/*
 * level_1_1.cpp
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

#include "level_1_1.h"

#include <constants.h>
#include <core/window.h>
#include "factory.h"
#include <objects/characters/player_ship.h>
#include <objects/modifiers/expiring.h>
#include <objects/text/text.h>

#include <cstdlib>

namespace oci {
namespace levels {

using namespace objects;

namespace {
    /// Начальное положение куры (по Y)
    const float INITIAL_CHICKEN_Y_POS = -30.0f;
    /// Скорость куры
    const float CHICKEN_SPEED = 2.0f;

    Factory::Registrar<Level_1_1> reg("level_1_1", "game");
} // namespace

void Level_1_1::Init() {
    GameLevel::Init("level_1_1");
    mMode = 0;
    mChicken = Storage().CreateObject<RandomChicken>(
                    Vector2f(static_cast<float>(std::rand() % Window::Instance().getSize().x),
                             INITIAL_CHICKEN_Y_POS), 100, Chicken::tRed, 1, CHICKEN_SPEED);
}

void Level_1_1::Run() {
    Level::Run();
    if(mMode < 50) { // вначале прилетаем, показываем текст
        ++mMode;
        if(mMode < 40) {
            if(mMode == 10) {
                const int window_center = Window::Instance().getSize().x / 2;
                Storage().CreateObject<modifiers::Expiring<Text> >(
                    20, "on approach to",
                    Vector2f(window_center, FirstTextStringY),
                    Font::GetFont("medium.xml"),
                    Text::haCenter, Text::vaCenter);
                Storage().CreateObject<modifiers::Expiring<Text> >(
                    20, "pluto",
                    Vector2f(window_center, SecondTextStringY),
                    Font::GetFont("big.xml"),
                    Text::haCenter, Text::vaCenter);
            }
        } else if(mMode == 50)
            ShowLevelInfo("1", "first contact");
    } else if(mChicken.expired())
        EndLevel();
}

} // namespace levels
} // namespace oci
