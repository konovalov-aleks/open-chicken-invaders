/*
 * smoke.cpp
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

#include "smoke.h"

#include <context/object_storage.h>

#include <cstddef>
#include <cstdlib>

namespace oci {
namespace objects {

static int counter = 0;

void Smoke(context::ObjectStorage& storage, const Vector2f& pos, int size) {
    std::size_t count = std::rand() % (size - 4) + 4;
    if(counter + count > 25)
        count /= 4;
    for(std::size_t i = 0; i < count; ++i) {
        trigonometry::Degree angle(std::rand() % 360);
        float speed = static_cast<float>(rand()) / RAND_MAX * (static_cast<float>(size) / 20.0f);
        storage.CreateObject<CSmoke>(pos, speed, angle, 1.5f);
    }
}

CSmoke::~CSmoke() {
    --counter;
}

void CSmoke::Init(const Vector2f& pos, float speed, trigonometry::Degree angle,
                  float animation_speed) {
    ++counter;
    Particle::Init("smoke.xml", pos, speed, angle, animation_speed);
}

} // namespace objects
} // namespace oci
