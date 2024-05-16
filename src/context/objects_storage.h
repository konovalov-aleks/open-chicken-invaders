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
#include <boost/noncopyable.hpp>
#include <memory>
#include <portability/cpp11.h>
#include <portability/memory.h>
#include <portability/type_traits.h>
#include <portability/unordered_map.h>

#ifndef CPP11
#   include <boost/preprocessor.hpp>
#endif

namespace oci {

namespace objects {
    class Object;
} // namespace objects

namespace context {

namespace storage {
    const char LOCAL[] = "local";
} // namespace storage

class ObjectsStorage : boost::noncopyable {
public:

#ifdef CPP11
    template<typename T, typename... Values>
    shared_ptr<T> CreateObject(const Values&... args) {
        static_assert((is_base_of<objects::Object, T>::value),
                      "T must be derived from Object");
        shared_ptr<T> obj(make_shared<StorageSetter<T>>(*this));
        obj->Init(args...);
        mObjects.insert({obj.get(), obj});
        RegisterObject(obj);
        return obj;
    }
#else

    #define __GEN_OBJSTOR_PP_CONSTRUCTOR(N, I, U)                 \
    template<typename T BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, typename Value)> \
    shared_ptr<T> CreateObject(BOOST_PP_ENUM_BINARY_PARAMS(I, const Value, &arg)) {   \
        static_assert((is_base_of<objects::Object, T>::value),    \
                      "T must be derived from Object");           \
        shared_ptr<T> obj(new StorageSetter<T>(*this));           \
        obj->Init(BOOST_PP_ENUM_PARAMS(I, arg));                  \
        mObjects.insert(std::make_pair(obj.get(), obj));          \
        RegisterObject(obj);                                      \
        return obj;                                               \
    }
    BOOST_PP_REPEAT_FROM_TO(0, MAX_PARAMS_COUNT, __GEN_OBJSTOR_PP_CONSTRUCTOR, ~)

#endif // CPP11

    void KillObject(objects::Object* obj) {
        mObjects.erase(obj);
    }

    template<typename T>
    void KillObject(const weak_ptr<T>& obj) {
        if(!obj.expired())
            mObjects.erase(obj.lock().get());
    }

    void Clean() {
        mObjects.clear();
    }

    Context& GetContext() const { return *mContext; }

private:
    template<typename Parent>
    class StorageSetter : public Parent {
    public:
        StorageSetter(ObjectsStorage& storage) : mStorage(storage) {}

        virtual ~StorageSetter() {}
        virtual ObjectsStorage& Storage() override { return mStorage; }
    private:
        ObjectsStorage& mStorage;
    };

    ObjectsStorage(Context& context) : mContext(&context) {}

    template<typename Func, typename T>
    inline void do_register(integral_constant<bool, false>, Func, T) {}

    template<typename Func, typename T>
    inline void do_register(integral_constant<bool, true>, Func f, T obj) {
        (mContext->*f)(obj);
    }

    template<typename T>
    void RegisterObject(shared_ptr<T> obj) {
        do_register(typename is_convertible<T*, objects::Visible*>::type(),
                    &Context::RegisterVisible, obj);
        do_register(typename is_convertible<T*, objects::Active*>::type(),
                    &Context::RegisterActiveObject, obj);
        do_register(typename is_convertible<T*, objects::ICollisionObject*>::type(),
                    &Context::RegisterCollisionObject, obj);
        do_register(typename is_convertible<T*, objects::Animated*>::type(),
                    &Context::RegisterAnimated, obj);
    }

    unordered_map<objects::Object*, shared_ptr<objects::Object> > mObjects;
    Context* mContext;

    friend class Context;
};

} // namespace context
} // namespace oci
