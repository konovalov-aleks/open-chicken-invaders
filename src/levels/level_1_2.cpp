/*
 * level_1_2.cpp
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

#include "level_1_2.h"

#include "factory.h"
#include "game_level.h"
#include "level.h"
#include <context/object_storage.h>
#include <core/vector2.h>
#include <core/window.h>
#include <utils/for_each.h>

#include <cmath>
#include <cstdlib>
#include <string>

namespace oci {
namespace levels {

/// Скорость движения стаи
static const float FLOCK_SPEED = 1.0f;
/// Расстояние между курами в линейке при влёте
static const float LINE_CELL_HEIGHT = 60.0f;

/// Начальная координата Y куры вычисляется по формуле: y = -x * CreateDX - y * CreateDY
/// @{
static const float CreateDX = 80;
static const float CreateDY = 200;
/// @}
/// Размер ячейки в сетке кур
static const float CellSize = 50;
/// Смещение ячейки в сетке
static const float CellOffset = 56;

static Factory::Registrar<Level_1_2> reg1("level_1_2", "game", 0);
static Factory::Registrar<Level_1_2> reg2("level_1_3", "game", 1);

void Level_1_2::Init(int Step) {
    GameLevel::Init("level_1_1");
    mode = 0;
    dx = FLOCK_SPEED;

    for(int y = 0; y < 5; ++y)
        for(int x = 0; x < 8; ++x) {
            mChickens.push_back(
                Storage().CreateObject<LevelChicken>(
                    y % 2 ? CellSize : Window::Instance().getSize().x - CellSize,
                    LINE_CELL_HEIGHT * (1 + y), x, y, 0, -(x * CreateDX + y * CreateDY)
                )
            );
        }
    if (!Step)
        ShowLevelInfo("2", "classic invasion");
    else
        ShowLevelInfo("3", "one more time");
}

namespace {
    class MoveAndCalcEnemiesFunctor {
        int& count;
    public:
        MoveAndCalcEnemiesFunctor(int& count) : count(count) {}

        void operator() (Level_1_2::LevelChicken& enemy) {
            if(enemy.mode < 2) {
                ++count;
                enemy.move((enemy.X - enemy.getPosition().x) / 8,
                           (enemy.Y - enemy.getPosition().y) / 8);
                if(std::abs(enemy.X - enemy.getPosition().x) < 1 &&
                   std::abs(enemy.Y - enemy.getPosition().y) < 1) {
                    enemy.Y = enemy.yPos * CellSize + CellOffset;
                    enemy.X = enemy.xPos * CellSize + CellOffset;
                    enemy.mode++;
                }
            }
        }
    };

    class DetermineFlockBoundsFunctor {
        float& xmax, &xmin;
    public:
        DetermineFlockBoundsFunctor(float& xmax, float& xmin)
            : xmax(xmax), xmin(xmin) {}

        void operator() (Level_1_2::LevelChicken& enemy) {
            if(enemy.getPosition().x > xmax)
                xmax = enemy.getPosition().x;
            if(enemy.getPosition().x < xmin)
                xmin = enemy.getPosition().x;
        }
    };

    class MoveChickenFunctor {
        float dx;
    public:
        MoveChickenFunctor(float dx) : dx(dx) {}
        void operator() (Level_1_2::LevelChicken& chicken) {
            chicken.move(dx, 0);
        }
    };
} // namespace

void Level_1_2::Run() {
    Level::Run();
    if(mChickens.empty()) {
        EndLevel();
        return;
    }

    switch (mode) {
    case 0:
        {
            int count = 0;
            for_each(mChickens, MoveAndCalcEnemiesFunctor(count));
            if(!count)
                mode = 1;
        } break;
    case 1:
        {
            const Vector2u wnd_size = Window::Instance().getSize();
            float xmin = wnd_size.x * 2.0f;
            float xmax = -static_cast<float>(wnd_size.x);
            for_each(mChickens, DetermineFlockBoundsFunctor(xmax, xmin));
            if(xmax + dx > wnd_size.x - CellSize || xmin + dx < CellSize)
                dx = -dx;
            for_each(mChickens, MoveChickenFunctor(dx));
        } break;
    }
}

} // namespace levels
} // namespace oci
