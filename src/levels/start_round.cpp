/*
 * start_round.cpp
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

#include "start_round.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <background/background.h>
#include <core/window.h>
#include <levels/factory.h>
#include <levels/manager.h>
#include <objects/characters/player_ship.h>
#include <objects/effects/unshade_screen.h>
#include <objects/particles/smoke.h>

namespace oci {
namespace levels {

namespace {

static const float SHIP_DECELERATION = -3;
static const float SHIP_START_SPEED = -25.0;
static const float BACKGROUND_INITIAL_SPEED = 20.0f;
static const float BACKGROUND_FINISH_SPEED = 8.0f;
static const float BACKGROUND_DECELERATION = -0.2;
static const int LEVEL_TIME = 50;

static Factory::Registrator<StartRound> reg("start_round_1", "game", "");

} // namespace

StartRound::StartRound() : mDY(SHIP_START_SPEED), mTime(0) {}

void StartRound::Init(const std::string& /*round_title*/) {
    Level::Init("start_round");
    Storage().CreateObject<objects::UnshadeScreen>();
    mBackgroundController = Storage().CreateObject<SimpleController>();
    mBackgroundController->SetSpeed(Vector2f(0, BACKGROUND_INITIAL_SPEED));
    Background::Instance().SetController(mBackgroundController);
    objects::PlayerShip& player_ship = levels::Manager::Instance().GetPlayerShip();
    player_ship.StopFire();
    player_ship.SetPosition(Vector2f(Window::Instance().GetWidth() / 2.0f,
                                     Window::Instance().GetHeight() + player_ship.GetHeight()));
}

void StartRound::Run() {
    ++mTime;
    objects::PlayerShip& ship = levels::Manager::Instance().GetPlayerShip();

    int smoke_cnt = rand() % 5;
    while(smoke_cnt-- > 0)
        Storage().CreateObject<objects::CSmoke>(
            Vector2f(ship.GetPosition().x + rand() % 20 - 10, ship.GetPosition().y + 15 + rand() % 10), // позиция
            mBackgroundController->GetSpeed().y / 10.0f,  // скорость
            180 + 4 * (rand() % 20 - 10), // распределение углов
            1.0f / (3 + rand() % 15) // скорость анимации
        );

    if(ship.GetPosition().y <= objects::PlayerShip::DEFAULT_Y_POS()) {
        ship.SetPosition(ship.GetPosition().x, objects::PlayerShip::DEFAULT_Y_POS());
    } else {
        ship.Move(0, mDY);
        mDY -= SHIP_DECELERATION;
        if(mDY > 0)
            mDY = SHIP_DECELERATION;
    }

    mBackgroundController->Accelerate(0, BACKGROUND_DECELERATION);
    if(mBackgroundController->GetSpeed().y < BACKGROUND_FINISH_SPEED)
        mBackgroundController->SetSpeed(Vector2f(0, BACKGROUND_FINISH_SPEED));

    if(mTime == 15)
        Storage().CreateObject<audio::ControllerHolder>(
            audio::Play("gasengstop.wav"));

    if(mTime > LEVEL_TIME)
        EndLevel();
}

} // namespace levels
} // namespace oci
