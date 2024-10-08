/*
 * smoke.h
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

#pragma once

#include "particle.h"
#include <core/vector2.h>
#include <utils/fast_trigonometry.h>

namespace oci::context {
    class ObjectStorage;
} // namespace oci::context

namespace oci {
namespace objects {

void Smoke(context::ObjectStorage&, const Vector2f& pos, int size);

/// Дым
class CSmoke : public Particle {
public:
    ~CSmoke();
    /** \param pos            Начальные координаты объекта
        \param speed          Скорость объекта
        \param angle          Угол полета объекта
        \param animation_speed Скорость анимации
    */
    void Init(const Vector2f& pos, float speed, trigonometry::Degree angle,
              float animation_speed =  1.0f);
    virtual DrawPriority GetDrawPriority() const override { return dpSmoke; }
};

} // namespace objects
} // namespace oci
