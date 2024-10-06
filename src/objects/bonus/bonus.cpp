/*
 * bonus.cpp
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

#include "bonus.h"

#include <context/object_storage.h>
#include <core/vector2.h>
#include <core/window.h>
#include <objects/base/animated_collision_object.h>

#include <cstdlib>

namespace oci {
namespace objects {

const float Bonus::Speed   = 4.0f;
const float Bonus::Gravity = 0.4f;

void Bonus::Init(const Vector2f& position, const char* sprite_name) {
    AnimatedCollisionObject::Init(sprite_name, position, 0);
    SetFrame(std::rand() % FramesCount());
}

void Bonus::Run() {
    move(0, Speed);
    if(getPosition().y > Window::Instance().getSize().y)
        Storage().KillObject(this);
}

void Bonus::OnCollision(const CollisionObjectInfo&) {
    OnBonus();
    Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
