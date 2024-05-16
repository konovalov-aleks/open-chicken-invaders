/*
 * animated_collision_object.h
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

#include "animated.h"
#include "collision_object.h"

namespace oci {
namespace objects {

/// анимированный объект, у которого обрабатывается коллизия
class AnimatedCollisionObject : public ICollisionObject, public Animated
{
public:
    /**   \param sprite_name          Имя файла спрайта
          \param [in] position        Координаты
          \param [in] power           Мощность оружия или "здоровье" персонажа */
    void Init(const char* sprite_name, const Vector2f& position, short power, float animation_speed = 1.0f);

protected:
    /// \return координату x объекта
    virtual float DoGetX() const override;
    /// \return координату y объекта
    virtual float DoGetY() const override;
    /// \return ширину текущего кадра
    virtual int DoGetFrameWidth() const override;
    /// \return высоту текущего кадра
    virtual int DoGetFrameHeight() const override;
};

} // namespace objects
} // namespace oci
