/*
 * chicken_part.cpp
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

#include "chicken_part.h"

#include <constants.h>
#include <core/window.h>

namespace oci {
namespace objects {

const int BonusChickenPart::LifeTime = 10;

void BonusChickenPart::Init(const Vector2f& position, float xspeed, float yspeed,
                             const char* sprite_name) {
    Bonus::Init(position, sprite_name);
    mTime = -1;
    mXSpeed = xspeed;
    mYSpeed = yspeed;
}

void BonusChickenPart::Run() {
    if(mTime < 0) {
        move(mXSpeed, mYSpeed);
        if(getPosition().x < 0 || getPosition().x > Window::Instance().getSize().x) {
            Storage().KillObject(this);
            return;
        }
        if(getPosition().y > FloorLevel()) {
            setPosition(getPosition().x, FloorLevel());
            if(mYSpeed < 1) {
                mYSpeed = mXSpeed = 0;
                mTime = LifeTime;
            } else
                mYSpeed *= -0.5f;
        } else
            mYSpeed += Gravity;
    } else if(--mTime == 0)
        Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
