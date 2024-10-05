/*
 * collision_object.cpp
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

#include "collision_object.h"

#include "sprite.h"
#include <core/vector2.h>

#include <cmath>
#include <cstdlib>

namespace oci {
namespace objects {

void ICollisionObject::Init(short Power) {
    power = Power;
}

CollisionObjectInfo ICollisionObject::CollisionInfo() {
    return CollisionObjectInfo(DoGetX(), DoGetY(), DoGetFrameWidth(),
                               DoGetFrameHeight(), GetCollisionType(), power);
}

bool ICollisionObject::DetectCollision(ICollisionObject& obj) {
    int w = (DoGetFrameWidth() + obj.DoGetFrameWidth()) / 2;
    int h = (DoGetFrameHeight() + obj.DoGetFrameHeight()) / 2;
    return (std::abs(DoGetX() - obj.DoGetX()) < w && std::abs(DoGetY() - obj.DoGetY()) < h);
}

void CCollisionObject::Init(const std::string& sprite_name,
                            const Vector2f& pos, short power) {
    ICollisionObject::Init(power);
    Sprite::Init(sprite_name, pos);
}

float CCollisionObject::DoGetX() const {
    return getPosition().x;
}

float CCollisionObject::DoGetY() const {
    return getPosition().y;
}

int CCollisionObject::DoGetFrameWidth() const {
    return GetWidth();
}

int CCollisionObject::DoGetFrameHeight() const {
    return GetHeight();
}

} // namespace objects
} // namespace oci
