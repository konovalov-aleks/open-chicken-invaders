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
#include <context/manager.h>
#include <core/window.h>
#include <objects/particles/smoke.h>
#include <objects/particles/spark.h>

#include <cmath>
#include <memory>
#include <numbers>

namespace oci {
namespace objects {

namespace {
    static const float MISSILE_MAX_SPEED = 10;
    static const float MISSILE_ACCELERATION = 1.0f;
    static const int MISSILE_POWER = 35;
    static const int MISSILE_MAX_LIFE_TIME = 30;
}

Missile::Missile() :
    mDX(0), mDY(-1), mAngle(0), mSpeed(1), mLifeTime(MISSILE_MAX_LIFE_TIME) {}

void Missile::Init(const Vector2f& position) {
    Sprite::Init("missile.xml", position);
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("gpolice_12.wav"));
}

void Missile::Run() {
    Move(mDX, mDY);

    float delta_x = (Window::Instance().GetWidth() / 2.0f) - GetPosition().x;
    float delta_y = GetPosition().y - (Window::Instance().GetHeight() / 2.0f);

    if(--mLifeTime < 0 ||
       (std::fabsf(delta_x) < mSpeed && std::fabsf(delta_y) < mSpeed)) {
        Storage().CreateObject<audio::ControllerHolder>(
            audio::Play("fx113.wav"));
        std::shared_ptr<context::Context> context =
            context::Manager::Instance().GetActiveContext();
        if(context)
            context->ColliseAll(ctMissile, MISSILE_POWER);
        Spark(Storage(), GetPosition(), 50);
        Smoke(Storage(), GetPosition(), 20);
        Storage().KillObject(this);
    } else {
        float ideal_angle = std::atan2f(delta_x, delta_y);
        mAngle = (mAngle * 10.0f + ideal_angle) / 11.0f;
        mDX = sinf(mAngle) * mSpeed;
        mDY = -cosf(mAngle) * mSpeed;

        mSpeed += MISSILE_ACCELERATION;
        if(mSpeed > MISSILE_MAX_SPEED)
            mSpeed = MISSILE_MAX_SPEED;

        int state = std::round(mAngle / (2 * std::numbers::pi_v<float>) * StatesCount());
        if(state < 0)
            state += StatesCount();
        if(state >= static_cast<int>(StatesCount()))
            state = StatesCount() - 1;
        SetState(state);
    }
}

} // namespace objects
} // namespace oci
