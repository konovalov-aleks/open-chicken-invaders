/*
 * spark.h
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

namespace oci {
namespace objects {

/// Функция создаёт несколько искр с разными скоростями и направлениями
/**     \param pos  Координаты точки, в которой будут создаваться искры
        \param size  Размер взрыва, чем больше это число, тем больше искр будет создано */
void Spark(context::ObjectsStorage& storage, const Vector2f& pos, int size);

/// Анимированный объект, который движется с определенной скоростью под определенным углом.
/// Когда пролистываются все кадры анимации объект удаляется
/// Искра
class CSpark : public Particle {
public:
    virtual ~CSpark();
    /** \param pos           Начальные координаты объекта
        \param speed         Скорость объекта
        \param angle         Угол полета объекта
        \param animation_speed Скорость анимации
    */
    void Init(const Vector2f& pos, float speed, trigonometry::Degree angle,
              float animation_speed = 1.0f);
    DrawPriority GetDrawPriority() const override { return dpSparc; }
};

} // namespace objects
} // namespace oci
