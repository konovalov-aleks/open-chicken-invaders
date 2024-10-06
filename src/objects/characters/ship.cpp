/*
 * ship.cpp
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

#include "ship.h"

#include <context/object_storage.h>
#include <core/vector2.h>
#include <objects/base/sprite.h>
#include <objects/particles/exhaust.h>

#include <cassert>
#include <string>

namespace oci {
namespace objects {

const int Ship::EnginePosX = 10;
const int Ship::EnginePosY = 16;

// константы, для Ship::X и Ship::Y, когда не нажат стилус (например, управляем с GSensor)
const float Ship_unlimited_x = 100000.0f;
const float Ship_unlimited_y = 100000.0f;

// на сколько умножать вектор, полученный с акселлерометра
const float GSensorMultiplier = 20.0f;
// время после взрыва, за которое влетает корабль
const int Ship_fly_time = 35;

void Ship::Init(const Vector2f& pos) {
    Sprite::Init("fighter.xml", pos);
    e1 = Storage().CreateObject<Exhaust>(
        Vector2f(pos.x + EnginePosX, pos.y + EnginePosY));
    e2 = Storage().CreateObject<Exhaust>(
        Vector2f(pos.x - EnginePosX, pos.y + EnginePosY));
    dx_e = 0.0f;
    SetFrame(4);
}

void Ship::Run() {
    // пересчитываем координаты "выхлопа" двигателей
    e1->setPosition(getPosition().x + EnginePosX + dx_e, getPosition().y + EnginePosY);
    e2->setPosition(getPosition().x - EnginePosX - dx_e, getPosition().y + EnginePosY);
}

void Ship::SetEngineState(int st) {
    assert(st >= 0 && st <= 1);
    e1->df = e2->df = st;
}

} // namespace objects
} // namespace oci
