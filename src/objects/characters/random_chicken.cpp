/*
 * random_chicken.cpp
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

#include "random_chicken.h"

#include <core/window.h>
#include <math.h>

namespace oci {
namespace objects {

const int RandomChicken::MAX_Y_START_COORD = 400;

void RandomChicken::Init(const Vector2f& position, int period, Type type,
                          int health, float Speed) {
    Chicken::Init(position, period, type, health);
    mSpeed = Speed;
    GenerateNewPoint();
}

void RandomChicken::Init(const Vector2f& position, int period, Type type,
                          int health, float speed, float X0, float Y0) {
    Chicken::Init(position, period, type, health);
    mSpeed = speed;
    mX1 = X0;
    mY1 = Y0;
    float xx = mX1 - GetPosition().x;
    float yy = mY1 - GetPosition().y;
    float l = sqrtf(xx * xx + yy * yy);
    mDX = mSpeed * xx / l;
    mDY = mSpeed * yy / l;
}

void RandomChicken::GenerateNewPoint() {
    float xx, yy, l, sp;
    do {
        mX1 = static_cast<float>(rand() % Window::Instance().GetWidth());
        mY1 = static_cast<float>(rand() % MAX_Y_START_COORD);
        xx = mX1 - GetPosition().x;
        yy = mY1 - GetPosition().y;
        l = sqrtf(xx * xx + yy * yy);
        sp = mSpeed + static_cast<float>(rand()) / RAND_MAX;
    } while(l < sp);
    mDX = mSpeed * xx / l;
    mDY = mSpeed * yy / l;
}

void RandomChicken::Run() {
    if(fabs(mX1 - GetPosition().x) <= fabs(mDX) && fabs(mY1 - GetPosition().y) <= fabs(mDY))
        GenerateNewPoint();
    Move(mDX, mDY);
    Chicken::Run();
}

} // namespace objects
} // namespace oci
