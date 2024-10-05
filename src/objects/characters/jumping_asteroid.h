/*
 * jumping_asteroid.h
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

#include "asteroid.h"

#include <core/vector2.h>
#include <objects/base/collision_object_types.h>

#include <functional>
#include <memory>

namespace oci {
namespace objects {

/// An asteroid that bounces off the screen edges and splits into 3 pieces
/// when it explodes (3 Asteroid objects are created)
class JumpingAsteroid : public Asteroid {
public:
    typedef std::function<void(std::shared_ptr<JumpingAsteroid>)> CreateSplinterCallback;

    /// \param split - whether to split the asteroid after an explosion
    void Init(const Vector2f& position, float angle, float speed, short health,
              Type type, bool split, const CreateSplinterCallback& cb);

protected:
    virtual void Run() override;
    virtual void OnBang(CollisionType ct) override;
    /// The method is called if the asteroid was exploded (need to create a splinter)
    /// \param angle - splinter movement angle
    virtual void CreateAsteroidSplinter(float angle);

private:
    bool mSplit;
    float mSpeed;

    CreateSplinterCallback mOnCreateSplinter;
};

} // namespace objects
} // namespace oci
