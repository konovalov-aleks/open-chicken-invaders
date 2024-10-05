/*
 * bullet.cpp
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

#include "bullet.h"

#include <context/object_storage.h>
#include <core/window.h>
#include <objects/particles/spark.h>

#include <cmath>
#include <numbers>

namespace oci {
namespace objects {

void Bullet::Init(const std::string& sprite_name, const Vector2f& pos,
                  float speed, float angle, int _state, short power,
                  int collisionWith, CollisionType collisionType) {
    CCollisionObject::Init(sprite_name, pos, power);
    collision_type = collisionType;
    collision_with = collisionWith;
    SetState(_state);
    dx0 =  speed * std::sin(angle);
    dy0 = -speed * std::cos(angle);
    dx = 0.4f * dx0;
    dy = 0.4f * dy0;

    int frame = std::round(angle / (std::numbers::pi_v<float> * 2.0f) * 32.0f);
    if(frame >= static_cast<int>(FramesCount()))
        frame = 0;
    SetFrame(frame);
}

void Bullet::OnCollision(const CollisionObjectInfo& /*collisedWith*/) {
    Spark(Storage(), getPosition(), 20);
    Storage().KillObject(this);
}

void Bullet::Run() {
    dx += 0.03f * dx0;
    dy += 0.03f * dy0;
    if(std::abs(dx) > std::abs(dx0))
        dx = dx0;
    if(std::abs(dy) > std::abs(dy0))
        dy = dy0;

    move(dx, dy);
    const Vector2u wnd_size =  Window::Instance().getSize();
    if(getPosition().x > wnd_size.x || getPosition().x < 0 ||
       getPosition().y > wnd_size.y || getPosition().y < 0)
        Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
