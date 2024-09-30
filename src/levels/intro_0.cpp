/*
 * intro_0.cpp
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

#include "intro_0.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <core/window.h>
#include "factory.h"
#include "manager.h"
#include <objects/effects/unshade_screen.h>
#include <objects/gun/player_bullet.h>
#include <objects/modifiers/auto_killable.h>
#include <objects/particles/smoke.h>
#include <utils/cleanup_container.h>

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <numbers>
#include <stdexcept>

namespace oci {
namespace levels {

using namespace objects;
using namespace modifiers;

/// скорость корабля, когда он летит влево
static const float SHIP_SPEED = 4.8f;
/// скорость корабля, когда он возвращается назад (вправо)
static const float SHIP_BACK_SPEED = 8.0f;
/// начальная скорость курицы, летящей влево
static const float CHICKEN_SPEED = 4.0f;
/// скорость курицы, летящей влево, после попадания пули
static const float CHICKEN_SPEED_AFTER_SHOT = 6.0f;
/// скорость пули
static const float BULLET_SPEED = 8.0f;
/// начальная позиция корабля (за экраном)
static const Vector2f INITIAL_SHIP_POS(800, 250);
/// Начальное положение Солнечной системы
static const Vector2f INITIAL_SOLAR_SYSTEM_POS(-460, 0);
/// Насколько смещен выхлоп относительно координаты корабля
static const int EXHAUST_DELTA_X = 20;
/// Скорость перемещения вдоль Солнечной системы
static const int SOLAR_SYSTEM_MOVE_SPEED = 2;
/// Сначала мы просто перемещаемся вдоль солнечной системы. На этой координате останавливаемся
static const int SOLAR_SYSTEM_STOP_POS_X = 450;
/// Начальные координаты курицы, летящей влево
static const Vector2f INITIAL_CHICKEN_POS(680, 250);
/// Позиция, в которой игрок выстреливает по курице
static const float SHIP_SHOT_POS = 620;
/// Координата Х корабля, в которой он разворачивается
static const int SHIP_REVERSE_POS = -90;
/// Координата Х корабля, в которой воспроизводится звук мотора (после разворота)
static const int SHIP_ENGINE_SOUND_POS_X = -10;
/// Скорость движения дыма
static const float SMOKE_SPEED = -2;
/// Максимальная координата кур, летящих вправо, за экраном слева (то есть создаются они случайно, от MIN_CHICKEN_POS до 0)
static const int MIN_CHICKEN_START_POS = 560;

static Factory::Registrator<Intro_0> reg("intro_0", "intro");

void Intro_0::IntroChicken1::Init(const Vector2f& position) {
    AnimatedCollisionObject::Init("chicken_intro.xml", position, 1000, 0.3f);
    mSpeed = CHICKEN_SPEED;
}

void Intro_0::IntroChicken1::OnCollision(const CollisionObjectInfo&) {
    SetAnimationSpeed(1.0f);
    mSpeed = CHICKEN_SPEED_AFTER_SHOT;
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("rdfx31.wav"));
}

void Intro_0::IntroChicken1::Run() {
    move(-mSpeed, 0);
}

void Intro_0::IntroChicken2::Init(const Vector2f& position, Type type) {
    static const float CHICKEN_SPEEDS[] = { 3.0f, 3.5f, 4.0f };
    Animated::Init("chicken_intro.xml", position);
    assert(type >= 0 && type <= std::size(CHICKEN_SPEEDS));
    mSpeed = CHICKEN_SPEEDS[type];
    mType = type;
    SetState(type + 1);
    SetFrame(std::rand() % FramesCount());
}

Visible::DrawPriority Intro_0::IntroChicken2::GetDrawPriority() const {
    return static_cast<DrawPriority>(dpChicken + mType);
}

void Intro_0::IntroChicken2::Run() {
    move(mSpeed, 0);
}

void Intro_0::Init() {
    IntroLevel::Init("intro0");
    mShip = Storage().CreateObject<CommonSprite<Visible::dpShip> >(
                "fighter_sideview.xml", INITIAL_SHIP_POS);
    mExhaust = Storage().CreateObject<Exhaust>(Vector2f(INITIAL_SHIP_POS.x + EXHAUST_DELTA_X, INITIAL_SHIP_POS.y));
    mMode = 0;
    Storage().CreateObject<SolarSystem>(INITIAL_SOLAR_SYSTEM_POS.x, INITIAL_SOLAR_SYSTEM_POS.y,
        PLANET_SUN | PLANET_MERCURY | PLANET_VENUS | PLANET_EARTH | PLANET_MARS);
    mSubtitle = Storage().CreateObject<SubtitleText>("intro_0.xml");
    Storage().CreateObject<UnshadeScreen>();
    mBackgroundController->x = INITIAL_SOLAR_SYSTEM_POS.x;
    mBackgroundController->y = INITIAL_SOLAR_SYSTEM_POS.y;
}

void Intro_0::RunIntro() {
    std::shared_ptr<Sprite> ship(mShip.lock());
    if(!ship) [[unlikely]]
        throw std::logic_error("Attempt to use a destroyed Ship object");
    std::shared_ptr<Exhaust> exhaust(mExhaust.lock());
    if(!exhaust) [[unlikely]]
        throw std::logic_error("Attempt to use a destroyed Exhaust object");
    switch(mMode) {
    case 0:
        if(mBackgroundController->x < SOLAR_SYSTEM_STOP_POS_X)
            mBackgroundController->x += SOLAR_SYSTEM_MOVE_SPEED;
        else {
            ++mMode;
            mChicken = Storage().CreateObject<IntroChicken1>(INITIAL_CHICKEN_POS);
        }
        break;

    case 1:
        if(std::fabsf(ship->getPosition().x - SHIP_SHOT_POS) <= SHIP_SPEED) {
            ++mMode;
            Storage().CreateObject<CPlayerBullet>(
                "gun1.xml", ship->getPosition(), BULLET_SPEED, std::numbers::pi_v<float> * 1.5f, 0, 0);
            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("tr3_239.wav"));
        }
        // break тут не должно быть!
    case 2:
        ship->move(-SHIP_SPEED, 0);
        exhaust->setPosition(ship->getPosition().x + EXHAUST_DELTA_X, exhaust->getPosition().y);

        if(ship->getPosition().x < SHIP_REVERSE_POS) {
            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("recordscratch.wav"));
            ++mMode;
            ship->SetState(1);
            Storage().KillObject(mChicken);
        }
        break;

    case 3:        // чуток ждем и проигрываем звук (шум мотора)
        ship->move(SHIP_BACK_SPEED, 0);
        if(ship->getPosition().x > SHIP_ENGINE_SOUND_POS_X) {
            Storage().CreateObject<audio::ControllerHolder>(
                audio::Play("gasengstart.wav"));
            mMode++;
        }
        break;

    case 4:
        ship->move(SHIP_BACK_SPEED, 0);
        exhaust->setPosition(ship->getPosition().x - EXHAUST_DELTA_X,
                             exhaust->getPosition().y);
        if(std::rand() % 3)
            Storage().CreateObject<CSmoke>(
                Vector2f(ship->getPosition().x - EXHAUST_DELTA_X,
                         ship->getPosition().y),
                SMOKE_SPEED, 96 * (std::rand() % 20 - 10)
            );
        if(ship->getPosition().x > Window::Instance().getSize().x + 30) {
            //    создаем тучу кур...
            for(int i = 0; i < 10; ++i)
                // большие
                mChickens2.push_back(
                    Storage().CreateObject<AutoKillable<IntroChicken2, BoundRight> >(
                        Vector2f(-std::rand() % MIN_CHICKEN_START_POS,
                                 std::rand() % Window::Instance().getSize().y),
                        IntroChicken2::tBig
                    )
                );
            for(int i = 0; i < 20; ++i)
                // средние
                mChickens2.push_back(
                    Storage().CreateObject<AutoKillable<IntroChicken2, BoundRight> >(
                        Vector2f(-std::rand() % MIN_CHICKEN_START_POS,
                                 std::rand() % Window::Instance().getSize().y),
                        IntroChicken2::tMedium
                    )
                );
            for(int i = 0; i < 30; ++i)
                // мелкие
                mChickens2.push_back(
                    Storage().CreateObject<AutoKillable<IntroChicken2, BoundRight> >(
                        Vector2f(-std::rand() % MIN_CHICKEN_START_POS,
                                 std::rand() % Window::Instance().getSize().y),
                        IntroChicken2::tSmall
                        )
                );
            mMode++;
        }
        break;

    case 5:
        cleanup(mChickens2);
        if(mChickens2.empty() && mSubtitle.expired())
            EndLevel();
        break;
    }
}

} // namespace levels
} // namespace oci
