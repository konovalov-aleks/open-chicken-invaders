/*
 * animated_collision_object.cpp
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

#include "animated_collision_object.h"

namespace oci {
namespace objects {

void AnimatedCollisionObject::Init(const char* sprite_name,
                                   const Vector2f& position,
                                   short power, float animation_speed) {
    ICollisionObject::Init(power);
    Animated::Init(sprite_name, position, animation_speed);
}

float AnimatedCollisionObject::DoGetX() const {
    return GetPosition().x;
}

float AnimatedCollisionObject::DoGetY() const {
    return GetPosition().y;
}

int AnimatedCollisionObject::DoGetFrameWidth() const {
    return GetSize().x;
}

int AnimatedCollisionObject::DoGetFrameHeight() const {
    return GetSize().y;
}

} // namespace objects
} // namespace oci
