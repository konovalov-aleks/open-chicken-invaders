/*
 * paratrooper.cpp
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

#include "paratrooper.h"

#include <constants.h>
#include <core/window.h>
#include <egg.h>
#include <game/state.h>
#include <objects/particles/smoke.h>

#include <memory>

namespace oci {
namespace objects {

static const float CHICKEN_SPEED = 2.0f;

void Paratrooper::Init(const Vector2f& position, float Speed, int p, short health) {
    AnimatedCollisionObject::Init("umbrella.xml", position, health);
    MaxBombingPeriod = p;
    speed = Speed;
    SetFrame(rand() % FramesCount());
    time = rand() % MaxBombingPeriod;
}

void Paratrooper::Run() {
    Move(0, speed);
    if(GetPosition().y - GetSize().y / 2 > Window::Instance().GetHeight()) {
        Storage().KillObject(this);
        return;
    }

    if(time <= 0) {
        if(GetPosition().x > 0 && GetPosition().x < Window::Instance().GetWidth() &&
           GetPosition().y > 0 && GetPosition().y < FloorLevel())
            Storage().CreateObject<Egg>(GetPosition());
        time = rand() % MaxBombingPeriod;
    } else time--;
}

void Paratrooper::OnCollision(const CollisionObjectInfo& collisedWith) {
    Smoke(Storage(), GetPosition(), 10);
    if(collisedWith.type == ctFriendBullet || CollisionType() == ctMissile) {
        power -= collisedWith.power;
        if(power <= 0) {
            State::Instance().IncScore(1000);
            std::weak_ptr<RandomChicken> chicken(
                Storage().CreateObject<RandomChicken>(
                    GetPosition(), MaxBombingPeriod, Chicken::tCyan, 1, CHICKEN_SPEED)
                );
            if(mOnCreateRandomChickenCallback)
                mOnCreateRandomChickenCallback(chicken);
            Storage().KillObject(this);
        }
    } else
        Storage().KillObject(this);
}

void Paratrooper::SetOnCreateRandomChickenCallback(const CreateRndChickenCallbackFunc& cb) {
    mOnCreateRandomChickenCallback = cb;
}

} // namespace objects
} // namespace oci
