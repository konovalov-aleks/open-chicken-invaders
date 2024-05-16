/*
 * boss1.cpp
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

#include "boss1.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <core/window.h>
#include <objects/bonus/chicken_leg.h>
#include <objects/gun/bullet.h>
#include <objects/particles/smoke.h>
#include <portability/math.h>

namespace oci {
namespace objects {

namespace {
    static const float BOSS1_BULLET_SPEED = 8;
} // namespace

void Boss1::Init(const Vector2f& position, const Vector2f& destination,
                 float speed) {
    AnimatedCollisionObject::Init("boss1.xml", position, 50);
    mRealPosition = position;
    mAbsoluteSpeed = speed;
    mode = 0;
    mDestination = destination;
    CalculateSpeed();
}

void Boss1::Run() {
    if(!mode) {
        mRealPosition += mSpeed;
        if(fabs(mDestination.x - mRealPosition.x) <= fabs(mSpeed.x) &&
           fabs(mDestination.y - mRealPosition.y) <= fabs(mSpeed.y))
            mode = 1;
    } else if (mode > 9) {
        Fire();
        GenerateNewPoint();
    } else
        mOffset.x = static_cast<float>(((mode++ % 3) - 1) * 2);

    if(mOffset.y != 0)
        mOffset.y += 0.25f;

    SetPosition(mRealPosition + mOffset);
}

void Boss1::OnCollision(const CollisionObjectInfo& collisedWith) {
    mOffset.y = -3;
    if(collisedWith.type == ctFriendBullet || collisedWith.type == ctMissile) {
//        for(int i = 0; i < rand() % 3 + 1; ++i)
//            Storage().CreateObject<CFeather>(GetPosition());

        power -= collisedWith.power;
        if(power <= 0) {
            Smoke(Storage(), GetPosition(), 40);
            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("rdfx31-16k.wav"));
            for(int i = 0; i < 10; ++i) {
                float xx = ((float)rand() / RAND_MAX) * 8 - 4;
                float yy = ((float)rand() / RAND_MAX) * 4 - 2;
                Storage().CreateObject<BonusChickenLeg>(GetPosition(), xx, yy);
            }
            Storage().KillObject(this);
        }
    }
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("fx39trimmed-16k.wav"));
}

void Boss1::GenerateNewPoint() {
    do {
        mDestination = Vector2f(
            rand() % (Window::Instance().GetWidth() - GetWidth()) + GetWidth() / 2,
            rand() % (Window::Instance().GetHeight() - GetHeight()) + GetHeight() / 2);
    } while(!CalculateSpeed());
    mode = 0;
}

bool Boss1::CalculateSpeed() {
    float xx = mDestination.x - mRealPosition.x;
    float yy = mDestination.y - mRealPosition.y;
    float l = sqrtf(xx * xx + yy * yy);
    float sp = mAbsoluteSpeed + (float)rand() / RAND_MAX;
    if(l < sp)
        return false;
    mSpeed = Vector2f(mAbsoluteSpeed * xx / l, mAbsoluteSpeed * yy / l);
    return true;
}

void Boss1::Fire() {
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("laserLow.wav"));
    Storage().CreateObject<Bullet>(
        "gun4.xml", GetPosition(), BOSS1_BULLET_SPEED, M_PI, 0, 0,
        ctPlayerShip, ctEnemyBullet);
    Storage().CreateObject<Bullet>(
        "gun4.xml", GetPosition(), BOSS1_BULLET_SPEED, M_PI + 0.52f, 0, 0,
        ctPlayerShip, ctEnemyBullet);
    Storage().CreateObject<Bullet>(
        "gun4.xml", GetPosition(), BOSS1_BULLET_SPEED, M_PI - 0.52f, 0, 0,
        ctPlayerShip,ctEnemyBullet);
}

} // namespace objects
} // namespace oci
