/*
 * ufo.cpp
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

#include "ufo.h"

#include <audio/player.h>
#include <core/window.h>
#include <objects/bonus/upgrade_gun.h>
#include <objects/gun/bullet.h>
#include <objects/particles/smoke.h>

#include <numbers>

namespace oci {
namespace objects {

static const int UFO_ATTACK_TIME = 15;
static const float UFO_BULLET_SPEED = 8.0f;
static const Vector2f UFO_INITIAL_POSITION(-20.0f, 60.0f);

void UFO::Init(float speed) {
    AnimatedCollisionObject::Init("flyingsaucer.xml", UFO_INITIAL_POSITION, 0);
    mTime = UFO_ATTACK_TIME;
    mSpeed = speed;
    mEngineSound = audio::Play("hb12.wav", true, true);
}

void UFO::Run() {
    if(--mTime <= 0) {
        mTime = UFO_ATTACK_TIME;
        Storage().CreateObject<audio::ControllerHolder>(
            audio::Play("laserLow.wav"));
        Storage().CreateObject<Bullet>(
            "gun4.xml", GetPosition(), UFO_BULLET_SPEED,
            std::numbers::pi_v<float>, 0, 0, ctPlayerShip, ctEnemyBullet);
    }
    Move(mSpeed, 0);
    if(GetPosition().x >= Window::Instance().GetWidth())
        Storage().KillObject(this);
}

void UFO::OnCollision(const CollisionObjectInfo& /*collised_with*/) {
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("gp82.wav"));
    Storage().CreateObject<BonusUpgradeGun>(GetPosition());
    Smoke(Storage(), GetPosition(), 15);
    Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
