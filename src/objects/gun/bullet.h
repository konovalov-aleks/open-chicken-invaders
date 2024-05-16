/*
 * bullet.h
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

#include <objects/base/active.h>
#include <objects/base/collision_object.h>
#include <objects/base/sprite.h>
#include <objects/base/visible.h>

namespace oci {
namespace objects {

/// Пуля
class Bullet : public CCollisionObject, public Active {
public:
    /** \param sprite_name      Название спрайта
        \param pos              Начальные координаты пули
        \param speed            Скорость пули
        \param angle            Угол, под которым летит пуля
        \param State            Тип пули (номер строки в текстуре)
        \param power            Мощность пули
        \param collisionWith    Битовое поле, содержит флаги, указывающие, с какими объектами может сталкиваться пуля
        \param collisionType    Тип объекта (по коллизии)
    */
    void Init(const std::string& sprite_name, const Vector2f& pos, float speed,
              float angle, int State, short power, int collisionWith,
              CollisionType collisionType);

    virtual CollisionType GetCollisionType() const override { return collision_type; };
    virtual int CollisionWith() const override { return collision_with; };
    virtual DrawPriority GetDrawPriority() const override { return dpBullet; }

private:
    virtual void Run() override;
    virtual void OnCollision(const CollisionObjectInfo& collisedWith) override;

    /// Пуля летит с ускорением.
    /// Ускорение пули
    float dx0, dy0;
    /// Скорость пули
    float dx, dy;

    CollisionType collision_type;
    int collision_with;
};

} // namespace objects
} // namespace oci
