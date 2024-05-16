/*
 * level_x2_x3.cpp
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

#include "level_x2_x3.h"

#include <core/window.h>
#include <math.h>
#include <utils/for_each.h>
#include <utils/math.h>

namespace oci {
namespace levels {

void Level_x2_x3::Init(bool flyenable, float flock_x_speed) {
    GameLevel::Init("level_1_1");
    mFlockXPos = 0;
    mFlockXSpeed = flock_x_speed;
    mFlyEnable = flyenable;
}

namespace {

class DetermineFlockBoundsFunctor {
    float &xmax, &xmin;
public:
    DetermineFlockBoundsFunctor(float& xmax, float& xmin) : xmax(xmax), xmin(xmin) {}

    void operator() (Level_x2_x3::LevelChicken& enemy) {
        if(enemy.GetPosition().x > xmax)
            xmax = enemy.GetPosition().x;
        if(enemy.GetPosition().x < xmin)
            xmin = enemy.GetPosition().x;
    }
};

class MoveChickenFunctor {
    float mFlockXSpeed, mFlockXPos;
    bool mFlyEnable;
public:
    MoveChickenFunctor(float flock_x_speed, float flock_x_pos, bool flyenable)
        : mFlockXSpeed(flock_x_speed), mFlockXPos(flock_x_pos), mFlyEnable(flyenable) {}

    void operator() (Level_x2_x3::LevelChicken& chicken) {
        switch(chicken.mode) {
            case 0: // летим на свое место
            {
                float deltaX = chicken.X - chicken.xPos;
                // подлетаем со скоростью 10, когда подлетаем - замедляемся
                chicken.xPos += fabs(deltaX) < 80 ? deltaX / 4 : utils::sgn(deltaX) * 20;
                float deltaY = chicken.Y - chicken.yPos;
                chicken.yPos += fabsf(deltaY) < 80 ? deltaY / 4 : utils::sgn(deltaY) * 20;
                if(fabsf(deltaX) < 2 && fabsf(deltaY) < 2) {
                    chicken.yPos = chicken.Y;
                    chicken.xPos = chicken.X;
                    ++(chicken.mode);
                }
                chicken.SetPosition(chicken.GetPosition().x, chicken.yPos);
            }
            // внимание! break'а тут нет - так надо :)
            case 1: // теперь прилетели - просто баландаемся вместе со всеми
                chicken.SetPosition(chicken.xPos + mFlockXPos, chicken.GetPosition().y);
                if(mFlyEnable && !(rand() % 1500)) { // кура может вылететь из своей "ячейки"
                    chicken.mode = 2;
                    chicken.xPos = static_cast<float>(rand() % Window::Instance().GetWidth()); // сюда полетит наша кура
                    chicken.yPos = static_cast<float>(rand() % Window::Instance().GetHeight());
                    // в info->X и info->Y координаты, куда будем возвращаться
                }
                break;
            case 2:
                // куре стукнуло в голову порезвиться - летим к точке...
                chicken.Move((chicken.xPos - chicken.GetPosition().x) / 20,
                             (chicken.yPos - chicken.GetPosition().y) / 20);
                if(fabsf(chicken.xPos - chicken.GetPosition().x) < 2 &&
                   fabsf(chicken.yPos - chicken.GetPosition().y) < 2) {
                    chicken.mode = 0;
                    chicken.xPos = chicken.GetPosition().x - mFlockXPos; // теперь кура будет болтаться
                    chicken.yPos = chicken.GetPosition().y;            // со всеми, поэтому восстанавливаем ее координаты
                    // убираем общее смещение
                }
                break;
        }
    }
};

} // namespace

void Level_x2_x3::Run() {
    Level::Run();
    /// найдем левую и правую координаты стаи
    float xmin = static_cast<float>(Window::Instance().GetWidth() * 2);
    float xmax = static_cast<float>(-Window::Instance().GetWidth());
    for_each(mChickens, DetermineFlockBoundsFunctor(xmax, xmin));
    mFlockXPos += mFlockXSpeed;
    // проверяем, не улетела ли стая за экран
    if(mFlockXPos > 0 ? xmax + mFlockXSpeed >= Window::Instance().GetWidth() - 40 : xmin + mFlockXSpeed < 60)
        mFlockXSpeed = -mFlockXSpeed;

    for_each(mChickens, MoveChickenFunctor(mFlockXSpeed, mFlockXPos, mFlyEnable));
}

} // namespace levels
} // namespace oci
