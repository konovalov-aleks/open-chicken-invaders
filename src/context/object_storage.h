/*
 * objects_storage.h
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

#include "context.h"
#include <objects/base/object.h>

#include <cassert>
#include <concepts>
#include <memory>
#include <unordered_set>

namespace oci {

namespace context {

namespace storage {
    const char LOCAL[] = "local";
} // namespace storage

class ObjectStorage {
public:
    ~ObjectStorage() { Clean(); }

    ObjectStorage(const ObjectStorage&) = delete;
    ObjectStorage& operator= (const ObjectStorage&) = delete;

    template<std::derived_from<objects::Object> T, typename... Values>
    std::shared_ptr<T> CreateObject(Values&&... args) {
        std::shared_ptr<T> obj(std::make_shared<T>());
        obj->mStorage = this;
        obj->Init(std::forward<Values>(args)...);
        [[maybe_unused]] bool ok = mObjects.insert(obj).second;
        assert(ok);
        RegisterObject(obj);
        return obj;
    }

    void KillObject(objects::Object* obj) {
        if (auto iter = mObjects.find(obj); iter != mObjects.end())
            mObjects.erase(iter);
    }

    template<typename T>
    void KillObject(const std::weak_ptr<T>& obj) {
        if (auto iter = mObjects.find(obj.lock()); iter != mObjects.end())
            mObjects.erase(iter);
    }

    void Clean() {
        for (const std::shared_ptr<objects::Object>& obj : mObjects) {
            assert(obj);
            obj->mStorage = nullptr;
        }
        mObjects.clear();
    }

    Context& GetContext() const { return *mContext; }

private:
    ObjectStorage(Context& context) : mContext(&context) {}

    template<typename Func, typename T>
    void do_register(std::integral_constant<bool, false>, Func, const T&) {}

    template<typename Func, typename T>
    void do_register(std::integral_constant<bool, true>, Func f, const T& obj) {
        (mContext->*f)(obj);
    }

    template<typename T>
    void RegisterObject(const std::shared_ptr<T>& obj) {
        do_register(typename std::is_convertible<T*, objects::Visible*>::type(),
                    &Context::RegisterVisible, obj);
        do_register(typename std::is_convertible<T*, objects::Active*>::type(),
                    &Context::RegisterActiveObject, obj);
        do_register(typename std::is_convertible<T*, objects::ICollisionObject*>::type(),
                    &Context::RegisterCollisionObject, obj);
        do_register(typename std::is_convertible<T*, objects::Animated*>::type(),
                    &Context::RegisterAnimated, obj);
    }

    struct Hash {
        using is_transparent = void;

        template <typename T>
        std::size_t operator() (const T& ptr) const {
            return std::hash<objects::Object*>()(&*ptr);
        }
    };

    struct Equal {
        using is_transparent = void;

        template <typename T1, typename T2>
        bool operator() (const T1& a, const T2& b) const {
            return &*a == &*b;
        }
    };

    std::unordered_set<std::shared_ptr<objects::Object>, Hash, Equal> mObjects;
    Context* mContext;

    friend class Context;
};

} // namespace context
} // namespace oci
