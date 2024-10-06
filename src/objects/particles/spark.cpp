/*
 * spark.cpp
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

#include "spark.h"

#include "particle.h"
#include <context/object_storage.h>
#include <core/vector2.h>
#include <utils/fast_trigonometry.h>

#include <cstdlib>

namespace oci {
namespace objects {

static int counter = 0;

void Spark(context::ObjectStorage& storage, const Vector2f& pos, int size) {
    int count = std::rand() % (size / 2) + 3;
    if(counter + count > 20)
        count /= 4;
    for (int i = 0; i < count; ++i) {
        trigonometry::Degree angle(std::rand() % 360);
        float speed = static_cast<float>(std::rand()) / RAND_MAX * (size / 5.0f);
        float animation_speed = 1.5f / (1.0f + std::rand() % (size / 4));
        storage.CreateObject<CSpark>(pos, speed, angle, animation_speed);
    }
}

CSpark::~CSpark() {
    --counter;
}

void CSpark::Init(const Vector2f& pos, float speed, trigonometry::Degree angle,
                  float animation_speed) {
    ++counter;
    Particle::Init("sparc.xml", pos, speed, angle, animation_speed);
    SetState((angle.Value() * 32) / 360);
}

} // namespace objects
} // namespace oci
