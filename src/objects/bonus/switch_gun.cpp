/*
 * switch_gun.cpp
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

#include "switch_gun.h"

#include "bonus.h"
#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>
#include <core/vector2.h>
#include <objects/base/collision_object_types.h>
#include <objects/gun/gun.h>

#include <iterator>
#include <string_view>

namespace oci {
namespace objects {

namespace {

const char* const SPRITE_NAMES[] = {
    "giftbox_red.xml",      // gtRed
    "giftbox_green.xml",    // gtGreen
    "giftbox_yellow.xml"    // gtYellow
};

static_assert(std::size(SPRITE_NAMES) == GUN_TYPE_COUNT,
              "For each element of enumeration GunType must be exist "
              "element in SPRITE_NAMES");

} // namespace

void BonusSwitchGun::Init(const Vector2f& position, TGunType type,
                          CollisionType collision_type) {
    Bonus::Init(position, SPRITE_NAMES[type]);
    mCollisionType = collision_type;
}

void BonusSwitchGun::OnBonus() {
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("gpbleep.wav"));
}

} // namespace objects
} // namespace oci
