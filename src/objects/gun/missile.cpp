/*
 * missile.cpp
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

#include "missile.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/context.h>
#include <context/manager.h>
#include <context/object_storage.h>
#include <core/vector2.h>
#include <core/window.h>
#include <objects/base/collision_object_types.h>
#include <objects/base/sprite.h>
#include <objects/particles/smoke.h>
#include <objects/particles/spark.h>

// IWYU pragma: no_include <__math/inverse_trigonometric_functions.h>
// IWYU pragma: no_include <__math/rounding_functions.h>
// IWYU pragma: no_include <__math/trigonometric_functions.h>
#include <cmath>
#include <cstdlib>
#include <numbers>
#include <string_view>

namespace oci {
namespace objects {

namespace {
    constexpr float MISSILE_MAX_SPEED = 10;
    constexpr float MISSILE_ACCELERATION = 1.0f;
    constexpr int MISSILE_POWER = 35;
    constexpr int MISSILE_MAX_LIFE_TIME = 30;
} // namespace

Missile::Missile() :
    mDX(0), mDY(-1), mAngle(0), mSpeed(1), mLifeTime(MISSILE_MAX_LIFE_TIME) {}

void Missile::Init(const Vector2f& position) {
    Sprite::Init("missile.xml", position);
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("gpolice_12.wav"));
}

void Missile::Run() {
    move(mDX, mDY);

    const Vector2u wnd_size = Window::Instance().getSize();
    float delta_x = (wnd_size.x / 2.0f) - getPosition().x;
    float delta_y = getPosition().y - (wnd_size.y / 2.0f);

    if(--mLifeTime < 0 ||
       (std::abs(delta_x) < mSpeed && std::abs(delta_y) < mSpeed)) {
        Storage().CreateObject<audio::ControllerHolder>(
            audio::Play("fx113.wav"));
        context::Context* context = context::Manager::Instance().GetActiveContext();
        if(context)
            context->CollideAll(ctMissile, MISSILE_POWER);
        Spark(Storage(), getPosition(), 50);
        Smoke(Storage(), getPosition(), 20);
        Storage().KillObject(this);
    } else {
        float ideal_angle = std::atan2(delta_x, delta_y);
        mAngle = (mAngle * 10.0f + ideal_angle) / 11.0f;
        mDX = std::sin(mAngle) * mSpeed;
        mDY = -std::cos(mAngle) * mSpeed;

        mSpeed += MISSILE_ACCELERATION;
        if(mSpeed > MISSILE_MAX_SPEED)
            mSpeed = MISSILE_MAX_SPEED;

        int state = std::round(mAngle / (2 * std::numbers::pi_v<float>) * StatesCount());
        if(state < 0)
            state += StatesCount();
        if(state >= static_cast<int>(StatesCount()))
            state = static_cast<int>(StatesCount() - 1);
        SetState(state);
    }
}

} // namespace objects
} // namespace oci
