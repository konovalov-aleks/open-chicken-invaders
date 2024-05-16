/*
 * bonus.h
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
#include <objects/base/animated_collision_object.h>

namespace oci {
namespace objects {

class Bonus : public AnimatedCollisionObject, public Active {
public:
    void Init(const Vector2f& position, const char* sprite_name);

    virtual int CollisionWith() const override { return ctPlayerShip; };
    virtual void OnCollision(const CollisionObjectInfo& collisedWith) override;
    virtual void Run() override;
    virtual DrawPriority GetDrawPriority() const override { return dpBonus; }

    /** функция вызывается, когда игрок берет бонус - надо проиграть
     *  соответствующий звук и добавить очков */
    virtual void OnBonus() = 0;
protected:
    /// Скорость бонуса
    static const float Speed;
    /// Ускорение, с которым падают бонусы
    static const float Gravity;
};

} // namespace objects
} // namespace oci
