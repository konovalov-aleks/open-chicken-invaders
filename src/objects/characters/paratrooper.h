/*
 * paratrooper.h
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

#include <objects/base/animated_collision_object.h>
#include <objects/base/active.h>
#include "random_chicken.h"

#include <functional>
#include <memory>

namespace oci {
namespace objects {

class Paratrooper : public AnimatedCollisionObject, public Active {
public:
    typedef std::function<void(const std::weak_ptr<RandomChicken>&)> CreateRndChickenCallbackFunc;

    void Init(const Vector2f& position, float speed, int p, short health);

    virtual CollisionType GetCollisionType() const override { return ctChicken; };
    virtual int CollisionWith() const override { return ctPlayerShip | ctFriendBullets; };
    virtual DrawPriority GetDrawPriority() const override { return dpChicken; }
    void SetOnCreateRandomChickenCallback(const CreateRndChickenCallbackFunc& cb);

private:

    void Run() override;
    void OnCollision(const CollisionObjectInfo& collisedWith) override;

    int MaxBombingPeriod;
    int time;
    float speed;
    CreateRndChickenCallbackFunc mOnCreateRandomChickenCallback;
};

} // namespace objects
} // namespace oci
