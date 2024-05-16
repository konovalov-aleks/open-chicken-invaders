/*
 * particle.h
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

#include <objects/base/animated.h>
#include <objects/base/active.h>
#include <utils/fast_trigonometry.h>

namespace oci {
namespace objects {

class Particle : public Animated, public Active {
public:
    /** \param sprite_name   Имя файла спрайта
        \param pos           Начальные координаты объекта
        \param speed         Скорость объекта
        \param angle         Угол полета объекта
        \param animation_speed Скорость анимации
    */
    void Init(const char* sprite_name, const Vector2f& pos, float speed,
              trigonometry::Degree angle, float animation_speed);
private:
    virtual void Run() override;

    /// Проекции вектора скорости на оси
    float mDX, mDY;
};

} // namespace objects
} // namespace oci
