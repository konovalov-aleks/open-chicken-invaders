/*
 * boss1.h
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

/// Босс из уровней 1-10 и 3-10
class Boss1 : public AnimatedCollisionObject, public Active {
public:
    /** \param  position  Начальные координаты
        \param  destination  Координаты точки, в которую изначально летит кура
        \param  speed Скорость курицы
    */
    void Init(const Vector2f& position, const Vector2f& destination, float speed);

    virtual CollisionType GetCollisionType() const override { return ctChicken; }
    virtual int CollisionWith() const override { return ctFriendBullets | ctPlayerShip; }
    virtual DrawPriority GetDrawPriority() const override { return dpChicken; }

protected:
    void Run() override;
    void OnCollision(const CollisionObjectInfo& collisedWith) override;
    void GenerateNewPoint();
    /// Функция расчитывает скорость куры, необходимую для того, чтобы лететь в
    /// заданную точку
    /// \return false, если кура уже находится близко к этой точке
    bool CalculateSpeed();
    void Fire();

private:
    Vector2f mDestination;
    Vector2f mSpeed; /// проекции скорости на оси
    Vector2f mRealPosition; /// реальные координаты без учета смещений
    Vector2f mOffset; /// смещения, для всяких понтов
    float mAbsoluteSpeed;  /// скорость
    int mode;
};

} // namespace objects
} // namespace oci
