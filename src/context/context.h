/*
 * context.h
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

#include <memory>
#include <boost/noncopyable.hpp>
#include <objects/base/collision_object_types.h>
#include <portability/memory.h>

namespace oci {

namespace objects {

class Active;
class Visible;
class Animated;
class ICollisionObject;

} // namespace objects

namespace context {

class ObjectsStorage;

class Context : boost::noncopyable {
public:
    Context();
    ~Context();

    void RegisterActiveObject(const shared_ptr<objects::Active>& func);
    void RegisterVisible(const shared_ptr<objects::Visible>& obj);
    void RegisterAnimated(const shared_ptr<objects::Animated>& obj);
    void RegisterCollisionObject(const shared_ptr<objects::ICollisionObject>& obj);

    void Run();
    void Draw();
    void Animate();
    void ProcessCollisions();

    /// "Столкнуть" все повреждающиеся объекты с данным объектом
    void ColliseAll(CollisionType CollisionType, int power);

    ObjectsStorage& GetStorage(const std::string& name);

private:
    class Impl;
    unique_ptr<Impl> mImpl;
};

} // namespace context
} // namespace oci
