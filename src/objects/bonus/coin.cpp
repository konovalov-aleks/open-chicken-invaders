/*
 * coin.cpp
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

#include "coin.h"

#include "bonus.h"
#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>
#include <core/vector2.h>
#include <core/window.h>
#include <game/state.h>

#include <string>

namespace oci {
namespace objects {

void BonusCoin::Init(const Vector2f& position, float xspeed, float yspeed) {
    Bonus::Init(position, "coin.xml");
    mXSpeed = xspeed;
    mYSpeed = yspeed;
}

void BonusCoin::Run() {
    move(mXSpeed, mYSpeed);
    mYSpeed += Gravity;
    const Vector2u wnd_size = Window::Instance().getSize();
    if(getPosition().x < 0 || getPosition().x > wnd_size.x ||
       getPosition().y > wnd_size.y)
        Storage().KillObject(this);
}

void BonusCoin::OnBonus() {
    State::Instance().IncScore(100);
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("coinbnce.wav"));
}

} // namespace objects
} // namespace oci
