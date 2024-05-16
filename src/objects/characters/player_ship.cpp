/*
 * player_ship.cpp
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

#include "player_ship.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <boost/lexical_cast.hpp>
#include <core/event.h>
#include <core/input.h>
#include <core/window.h>
#include <game/state.h>
#include <levels/manager.h>
#include <objects/gun/green_gun.h>
#include <objects/gun/red_gun.h>
#include <objects/gun/missile.h>
#include <objects/particles/bang.h>
#include <stdexcept>

namespace oci {
namespace objects {

namespace {
    class Constants {
    public:
        static int CollisionNormal() { return ctChicken | ctEnemyBullet |
                                              ctBonuses | ctAsteroid | ctBigEgg; }
        static int CollisionShield() { return ctBonuses; }

        /// максимальные скорости @{
        static float MaxSpeedDX() { return 14.0f; }
        static float MaxSpeedDY() { return 14.0f; }
        /// @}
        /// ускорения @{
        static float XAcceleration() { return 3.0f; }
        static float YAcceleration() { return 3.0f; }
        /// @}
        /// ускорения @{
        static float XBreakAcceleration() { return 6.0f; }
        static float YBreakAcceleration() { return 6.0f; }
        /// @}
        /// скорость возврата к исходному состоянию после выстрела
        /// корабль отъезжает на 3 * Ship_y_onfire_speed назад
        static float OnFireSpeedY() { return 1.4f; }

        static float ShipUnlimitedX() {
            static const float ship_unlimited_x =
                Window::Instance().GetWidth() * 2;
            return ship_unlimited_x;
        }

        static float ShipUnlimitedY() {
            static const float ship_unlimited_y =
                Window::Instance().GetHeight() * 3 / 2;
            return ship_unlimited_y;
        }
    };

    class BangEffect : public objects::Active {
    public:
        void Init(const shared_ptr<PlayerShip>& ship) {
            mShip = ship;
            mShip->StopFire();
            Bang(Storage(), mShip->GetPosition(), 20);
            Storage().CreateObject<audio::ControllerHolder>(
                    audio::Play("fx113.wav"));
            mShip->SetPosition(Vector2f(
                Window::Instance().GetWidth() / 2,
                Window::Instance().GetHeight() + mShip->GetSize().y));
            mShip->CreateShield();
        }

        void Run() override {
            float speed = std::min(
                (PlayerShip::DEFAULT_Y_POS() - mShip->GetPosition().y) / 8.0f,
                -1.0f);
            mShip->Move(0, speed);
            if(fabs(mShip->GetPosition().y - PlayerShip::DEFAULT_Y_POS()) < 1.0f) {
                mShip->StartFire();
                Storage().KillObject(this);
                mShip->SetPosition(mShip->GetPosition().x, PlayerShip::DEFAULT_Y_POS());
            }
        }

    private:
        shared_ptr<PlayerShip> mShip;
    };

    template<typename T, typename T2, typename T3>
    inline void fit_value_to_range(T& value, T2 min_val, T3 max_val) {
        if(value > static_cast<T>(max_val))
            value = static_cast<T>(max_val);
        else if(value < static_cast<T>(min_val))
            value = static_cast<T>(min_val);
    }
} // namespace

void PlayerShip::Init() {
    Ship::Init(Vector2f(100, DEFAULT_Y_POS()));
    ICollisionObject::Init(1);
    mDX = mDY = mFDX = mFDY = mTargetX = mTargetY = mDeltaY = 0;
    mFire = true;
    mFireTime = mReloadTime;
    mGun.reset(new RedGun(1));
    mEnterPressed = false;
    CreateShield();
}

void PlayerShip::CreateShield() {
    mShield = Storage().CreateObject<Shield>(GetPosition());
}

int PlayerShip::CollisionWith() const {
    return mShield.expired() ? Constants::CollisionNormal() :
                               Constants::CollisionShield();
}

void PlayerShip::OnCollision(const CollisionObjectInfo& collisedWith) {
    switch (collisedWith.type) {
        case ctRedBonus:
            if(mGun->Type() == gt_Red)
                ++mGun->level;
            else
                SelectGun(gt_Red);
            break;
        case ctGreenBonus:
            if(mGun->Type() == gt_Green)
                ++mGun->level;
            else
                SelectGun(gt_Green);
            break;

        case ctUpgradeBonus:
            ++mGun->level;
            break;

        case ctCoinBonus:
        case ctLegBonus:
        case ctBodyBonus:
            break;

        default:
            if(mShield.expired()) {
                mGun->level = mGun->level/2;
                if(mGun->level == 0)
                    mGun->level = 1;

                State::Instance().RemoveLive();
                Storage().CreateObject<BangEffect>(shared_from_this());
                if(State::Instance().Lives() <= 0)
                    levels::Manager::Instance().GameOver();
            }
            break;
    }
}

float PlayerShip::DoGetX() const {
    return GetPosition().x;
}

float PlayerShip::DoGetY() const {
    return GetPosition().y;
}

int PlayerShip::DoGetFrameWidth() const {
    return GetWidth();
}

int PlayerShip::DoGetFrameHeight() const {
    return GetHeight();
}

void PlayerShip::Run() {
    const Input& input = Window::Instance().GetInput();
    if(input.IsKeyDown(Key::Left))
        MoveLeft();
    else if(input.IsKeyDown(Key::Right))
        MoveRight();
    else if(input.IsKeyDown(Key::Return) && !mEnterPressed)
        StartMissile();
    else
        mFDX = 0;
    mEnterPressed = input.IsKeyDown(Key::Return);

    // стреляем, если надо
    if(--mFireTime < 0) {
        mFireTime = mReloadTime;
        Fire();
    }

    float x = GetPosition().x;
    float y = GetPosition().y;

    if(mFDX != 0) // если нажата кнопка, то ускоряемся
        mDX += mFDX;
    else
        if(mDX != 0) { // иначе, если скорость ненулевая - замедляемся
            if(mDX > 0)
                mDX -= mDX > Constants::XBreakAcceleration() ? Constants::XBreakAcceleration() : mDX;
            else
                mDX += -mDX > Constants::XBreakAcceleration() ? Constants::XBreakAcceleration() : -mDX;
        }
    fit_value_to_range(mDX, -Constants::MaxSpeedDX(), Constants::MaxSpeedDX());

    if(mFDY != 0)
        mDY += mFDY;
    else if(mDY != 0) {
        if(mDY > 0)
            mDY -= mDY > Constants::YBreakAcceleration() ? Constants::YBreakAcceleration() : mDY;
        else
            mDY += -mDY > Constants::YBreakAcceleration() ? Constants::YBreakAcceleration() : -mDY;
    }
    fit_value_to_range(mDY, -Constants::MaxSpeedDY(), Constants::MaxSpeedDY());

    if(fabsf(mTargetX - x) < fabsf(mDX)) {
        x = mTargetX;
        mFDX = mDX = 0;
    } else
        x += mDX;
    fit_value_to_range(x, 0, Window::Instance().GetWidth());

    if(fabsf(mTargetY - y) < fabsf(mDY)) {
        y = mTargetY;
        mFDY = mDY = 0;
    } else
        y += mDY;

    if(mDeltaY != 0) {
        float delta = std::min(mDeltaY, Constants::OnFireSpeedY());
        y -= delta;
        mDeltaY -= delta;
    }

    // номер кадра анимации высчитывается исходя из скорости по оси X
    int a = static_cast<int>(mDX * 2.0f) + 4;
    SetFrame(a < static_cast<int>(FramesCount()) ? (a >= 0 ? a : 0) : FramesCount() - 1);

    SetPosition(x, y);
    shared_ptr<Shield> shield = mShield.lock();
    if(shield)
        shield->SetPosition(GetPosition());
    Ship::Run();
}

void PlayerShip::MoveLeft() {
    mTargetX = Constants::ShipUnlimitedX();
    mTargetY = Constants::ShipUnlimitedY();
    mFDX = -Constants::XAcceleration();
}

void PlayerShip::MoveRight() {
    mTargetX = Constants::ShipUnlimitedX();
    mTargetY = Constants::ShipUnlimitedY();
    mFDX = Constants::XAcceleration();
}

void PlayerShip::StartMissile() {
    if(mFire && State::Instance().Missiles() > 0) {
        State::Instance().RemoveMissile();
        Storage().CreateObject<Missile>(GetPosition());
    }
}

void PlayerShip::Fire() {
    if(mFire) {
        mGun->Fire(GetPosition().x, GetPosition().y);
        // корабль отъезжает назад после выстрела
        mDeltaY = 3.0f * Constants::OnFireSpeedY();
        Move(0, mDeltaY);
    }
}

void PlayerShip::SelectGun(TGunType gun_type) {
    if(gun_type == mGun->Type())
        return;

    short level = mGun->level;
    switch(gun_type) {
        case gt_Red:
            mGun.reset(new RedGun(level));
            break;
        case gt_Green:
            mGun.reset(new GreenGun(level));
            break;
        default:
            throw std::logic_error("Unknown gun type: " +
                                   boost::lexical_cast<std::string>(gun_type));
    }
}

} // namespace objects
} // namespace oci
