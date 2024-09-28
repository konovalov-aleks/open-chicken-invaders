/*
 * jumping_asteroid.cpp
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

#include "jumping_asteroid.h"

#include <core/window.h>
#include <objects/particles/smoke.h>

#include <cmath>
#include <cstdlib>
#include <numbers>

namespace oci {
namespace objects {

void JumpingAsteroid::Init(const Vector2f& position, float angle,
                           float speed, short health, Type type, bool split,
                           const CreateSplinterCallback& cb) {
    Asteroid::Init(position, angle, speed, health, type);
    mSpeed = speed;
    mType = type;
    mSplit = split;
    mOnCreateSplinter = cb;
}

void JumpingAsteroid::Run() {
    if(std::rand() % 3 == 0) {
        // FIXME
        int angle = (std::atan2(mDX, mDY) * std::numbers::pi) / 180;
        int dx = std::rand() % GetWidth() - GetWidth() / 2;
        Storage().CreateObject<CSmoke>(
            Vector2f(GetPosition().x + dx, GetPosition().y),
            std::rand() % 3 + 0.5f, angle);
    }

    Move(mDX, mDY);
    if((GetPosition().x <= 0 && mDX < 0) ||
       (GetPosition().x >= Window::Instance().GetWidth() && mDX > 0))
        mDX = -mDX;
    if((GetPosition().y <= 0 && mDY < 0) ||
       (GetPosition().y >= Window::Instance().GetHeight() && mDY > 0))
        mDY = -mDY;
}

void JumpingAsteroid::OnBang(CollisionType ct) {
    if(mSplit && ct == ctFriendBullet)
        for(int i = 0; i < 3; ++i)
            CreateAsteroidSplinter(static_cast<float>(std::rand() ) / RAND_MAX * std::numbers::pi_v<float> * 2.0);
}

void JumpingAsteroid::CreateAsteroidSplinter(float angle) {
    std::shared_ptr<JumpingAsteroid> s = Storage().CreateObject<JumpingAsteroid>(
        GetPosition(), angle, mSpeed, 4, mType == tRockBig || mType == tRockSmall ? tRockSmall : tFireSmall,
        false, mOnCreateSplinter);
    if(mOnCreateSplinter)
        mOnCreateSplinter(s);
}

} // namespace objects
} // namespace oci
