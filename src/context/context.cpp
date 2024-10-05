/*
 * context.cpp
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

#include "context.h"

#include <context/object_storage.h>
#include <core/window.h>
#include <objects/base/active.h>
#include <objects/base/animated.h>
#include <objects/base/collision_object.h>
#include <objects/base/visible.h>
#include <utils/for_each.h>

#include <cassert>
#include <list>
#include <map>

namespace oci {
namespace context {

namespace {

template<typename T>
std::shared_ptr<T> lock(const std::weak_ptr<T>& ptr) {
    return ptr.lock();
}

template<typename T1, typename T2>
std::shared_ptr<T2> lock(const std::pair<T1, std::weak_ptr<T2> >& ptr) {
    return ptr.second.lock();
}

template<typename ElementType, typename T, typename Func>
void process(T& container, Func f) {
    for(typename T::iterator iter = container.begin();
       iter != container.end();) {
        typename T::iterator current = iter++;
        std::shared_ptr<ElementType> ptr = lock(*current);
        if(!ptr)
            container.erase(current);
        else
            ((*ptr).*f)();
    }
}

} // namespace

class Context::Impl {
public:
    std::list<
        std::weak_ptr<objects::Active>
    > mActiveObjects;

    std::multimap<
        objects::Visible::DrawPriority,
        std::weak_ptr<objects::Visible>
    > mVisibleObjects;

    std::list<
        std::weak_ptr<objects::Animated>
    > mAnimatedObjects;

    typedef std::list<
        std::weak_ptr<objects::ICollisionObject>
    > CollisionObjectsList;
    CollisionObjectsList mCollisionObjects;

    typedef std::unordered_map<std::string, std::unique_ptr<ObjectStorage> > ObjectsMap;
    ObjectsMap mObjectStorages;
};

Context::Context() : mImpl(new Impl()) {
}

Context::~Context() {
}

void Context::RegisterActiveObject(const std::shared_ptr<objects::Active>& func) {
    mImpl->mActiveObjects.push_front(func);
}

void Context::RegisterVisible(const std::shared_ptr<objects::Visible>& obj) {
    mImpl->mVisibleObjects.insert(std::make_pair(obj->GetDrawPriority(), obj));
}

void Context::RegisterAnimated(const std::shared_ptr<objects::Animated>& obj) {
    mImpl->mAnimatedObjects.push_front(obj);
}

void Context::RegisterCollisionObject(const std::shared_ptr<objects::ICollisionObject>& obj) {
    mImpl->mCollisionObjects.push_front(obj);
}

void Context::Run() {
    process<objects::Active>(mImpl->mActiveObjects, &objects::Active::Run);
}

void Context::ProcessCollisions() {
    for(Context::Impl::CollisionObjectsList::iterator iter1 =
            mImpl->mCollisionObjects.begin();
            iter1 != mImpl->mCollisionObjects.end();) {
        std::shared_ptr<objects::ICollisionObject> obj1 = iter1->lock();
        if(!obj1) {
            Context::Impl::CollisionObjectsList::iterator old = iter1++;
            mImpl->mCollisionObjects.erase(old);
            continue;
        }
        for(Context::Impl::CollisionObjectsList::iterator iter2 = ++iter1;
                iter2 != mImpl->mCollisionObjects.end(); ++iter2) {
            std::shared_ptr<objects::ICollisionObject> obj2 = iter2->lock();
            if(obj2 && (obj1->GetCollisionType() & obj2->CollisionWith()) &&
               obj2->DetectCollision(*obj1)) {
                objects::CollisionObjectInfo ci = obj1->CollisionInfo();
                obj1->OnCollision(obj2->CollisionInfo());
                obj2->OnCollision(ci);
            }
        }
    }
}

void Context::Draw() {
    process<objects::Visible>(mImpl->mVisibleObjects, &objects::Visible::Draw);
}

void Context::Animate() {
    process<objects::Animated>(mImpl->mAnimatedObjects, &objects::Animated::NextFrame);
}

void Context::ColliseAll(CollisionType collision_type, int power) {
    for(Context::Impl::CollisionObjectsList::iterator iter =
       mImpl->mCollisionObjects.begin();
       iter != mImpl->mCollisionObjects.end(); ++iter) {
        std::shared_ptr<objects::ICollisionObject> obj( iter->lock() );
        if(obj && obj->CollisionWith() & collision_type) {
            objects::CollisionObjectInfo info(obj->DoGetX(), obj->DoGetY(),
                            obj->DoGetFrameWidth(), obj->DoGetFrameHeight(),
                            collision_type, power);
            obj->OnCollision(info);
        }
    }
}

ObjectStorage& Context::GetStorage(const std::string& name) {
    Context::Impl::ObjectsMap& storages = mImpl->mObjectStorages;
    Context::Impl::ObjectsMap::const_iterator iter = storages.find(name);
    if(iter == storages.end())
        iter = storages.insert(
                   std::make_pair(name, std::unique_ptr<ObjectStorage>(new ObjectStorage(*this)))
               ).first;
    return *iter->second;
}

} // namespace context
} // namespace oci
