/*
 * chicken.cpp
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

#include "chicken.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>
#include <core/vector2.h>
#include <egg.h>
#include <game/state.h>
#include <objects/base/animated_collision_object.h>
#include <objects/base/collision_object.h>
#include <objects/base/collision_object_types.h>
#include <objects/bonus/chicken_body.h>
#include <objects/bonus/chicken_leg.h>
#include <objects/bonus/coin.h>
#include <objects/bonus/switch_gun.h>
#include <objects/bonus/upgrade_gun.h>
#include <objects/gun/gun.h>
#include <objects/particles/smoke.h>

#include <cstdlib>
#include <iterator>
#include <string_view>

namespace oci {
namespace objects {

void Chicken::Init(const Vector2f& position, int period, Type type,
                   int health) {
    static const char* const SPRITES[] = {
        "chicken_red.xml",
        "chicken_orange.xml",
        "chicken_yellow.xml",
        "chicken_green.xml",
        "chicken_cyan.xml",
        "chicken_blue.xml",
        "chicken_purple.xml",
        "chicken_pink.xml"
    };
    static_assert(std::size(SPRITES) == TYPE_COUNT,
                  "For each elements of enumeration Chicken::Type must be "
                  "exists element in array SPRITES");

    AnimatedCollisionObject::Init(SPRITES[type], position, health);
    mMaxBombingPeriod = period;
    mTime = std::rand() % mMaxBombingPeriod;
    SetFrame(std::rand() % FramesCount());
}

void Chicken::Run() {
    if(mTime-- <= 0) {
//        if(GetX() > 0 && GetX() < Window::Instance().GetWidth() && GetY() > 0 && GetY() < FloorLevel)
        Storage().CreateObject<Egg>(getPosition());
        mTime = std::rand() % mMaxBombingPeriod;
    }
}

void Chicken::OnCollision(const CollisionObjectInfo& collisedWith) {
    switch(collisedWith.type) {
        case ctMissile:
            power -= collisedWith.power;
            break;
        case ctFriendBullet:
            power -= collisedWith.power;
            if(power <= 0)
                OnBang(collisedWith);
            break;
        default:
            power -= 10;
    }
    if(power <= 0) {
        State::Instance().IncScore(1000);
        Smoke(Storage(), getPosition(), 20);
        Storage().KillObject(this);
    }
}

void Chicken::OnBang(const CollisionObjectInfo& collisedWith) {
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play(std::rand() % 2 ? "rdfx31.wav" : "fx39trimmed.wav"));

    // теперь определимся с бонусом...
    if(std::rand() % 20) {
        // в 19 из 20 случаев выпадет нога
        float dx = -(collisedWith.x - getPosition().x) / (7.0f + 2 * (float)std::rand() / RAND_MAX);
        if(std::rand() % 8) // в 7 из 8 случаев выпадет нога, в 1 из 8 - тушка или монетки
            Storage().CreateObject<BonusChickenLeg>(getPosition(), dx, 0);
        else if(std::rand() % 2)
            Storage().CreateObject<BonusChickenBody>(getPosition(), dx, 0);
        else {
            for(int i = 0; i < std::rand() % 5 + 5; ++i) {
                float dx =  3.0f * (float)std::rand() / RAND_MAX;
                float dy = -5.0f * (float)std::rand() / RAND_MAX;
                Storage().CreateObject<BonusCoin>(getPosition(), dx, dy);
            }
        }
    } else {
        TGunType t;
        CollisionType ct;
        switch(std::rand() % 3) {
            case 0:
                t = gt_Red;
                ct = ctRedBonus;
                break;
            case 1:
                t = gt_Green;
                ct = ctGreenBonus;
                break;
            default:
                Storage().CreateObject<BonusUpgradeGun>(getPosition());
                return;
        }
        Storage().CreateObject<BonusSwitchGun>(getPosition(), t, ct);
    }
}

} // namespace objects
} // namespace oci
