/*
 * asteroid.cpp
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

#include "asteroid.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>
#include <core/window.h>
#include <objects/particles/smoke.h>

#include <cstdlib>
#include <cmath>
#include <numbers>

namespace oci {
namespace objects {

void Asteroid::Init(const Vector2f& position, float angle, float speed,
                    short health, Type type) {
    static const char* const SPRITE_NAMES[] = {
        "asteroid_small_rock.xml", "asteroid_big_rock.xml",
        "asteroid_small_fire.xml", "asteroid_big_fire.xml"
    };
    static_assert(std::size(SPRITE_NAMES) == TYPE_COUNT,
                  "for each element in enumeration Asteroid::Type must "
                  "exists string in array SPRITE_NAMES");
    AnimatedCollisionObject::Init(SPRITE_NAMES[type], position, health),
    mDX = speed * std::sin(angle);
    mDY = speed * std::cos(angle);
    mType = type;
}

void Asteroid::Run() {
    if(std::rand() % 3 == 0) {
        // FIXME
        int angle = (std::atan2(mDX, mDY) * std::numbers::pi) / 180;
        int dx = std::rand() % GetWidth() - GetWidth() / 2;
        Storage().CreateObject<CSmoke>(
            Vector2f(getPosition().x + dx, getPosition().y),
            std::rand() % 3 + 0.5f, angle);
    }
    move(mDX, mDY);
}

void Asteroid::OnCollision(const CollisionObjectInfo& collisedWith) {
    if(collisedWith.type == ctFriendBullet || collisedWith.type == ctMissile)
        power -= collisedWith.power;
    else
        power = 0;

    if(power <= 0) {
        Smoke(Storage(), getPosition(),
              mType == tRockSmall || mType == tFireSmall ? 20 : 30);
        if(collisedWith.type != ctMissile)
            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("cannonfire.wav"));
        OnBang(collisedWith.type);
        Storage().KillObject(this);
    }
}

} // namespace objects
} // namespace oci
