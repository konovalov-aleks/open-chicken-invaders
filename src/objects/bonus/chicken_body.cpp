/*
 * chicken_body.cpp
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

#include "chicken_body.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>
#include <game/state.h>

namespace oci {
namespace objects {

void BonusChickenBody::Init(const Vector2f& position, float xspeed,
                            float yspeed) {
    BonusChickenPart::Init(position, xspeed, yspeed, "chicken_roast.xml");
}

void BonusChickenBody::OnBonus() {
    State::Instance().IncScore(10000);
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("toon40.wav"));
}

} // namespace objects
} // namespace oci
