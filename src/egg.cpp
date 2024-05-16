/*
 * egg.cpp
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

#include "egg.h"

#include "audio/controller_holder.h"
#include "audio/player.h"
#include "constants.h"

namespace oci {
namespace objects {

namespace {
    static const CHRONO::seconds EGG_LIFE_TIME = CHRONO::seconds(1);
    static const int EGG_SPEED = 3;
}

void Egg::Init(const Vector2f& position) {
    AnimatedCollisionObject::Init("egg.xml", position, 0, 0.0f);
    in_floor = false;
    speed = EGG_SPEED;
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("fx110.wav"));
}

void Egg::NextFrame() {
    if(CurrentFrame() != FramesCount() - 1)
        AnimatedCollisionObject::NextFrame();
}

void Egg::Run() {
    if(speed) {
        Move(0, speed);
        if(GetPosition().y > FloorLevel()) {
            SetY(FloorLevel());
            SetAnimationSpeed(1.0f);
            speed = 0;

            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("fx11.wav"));

            lifetime = CHRONO::system_clock::now();
            in_floor = true;
        }
    }
    if(in_floor && CHRONO::system_clock::now() - lifetime >= EGG_LIFE_TIME)
        Storage().KillObject(this);
}

void Egg::OnCollision(const CollisionObjectInfo&) {
    Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
