/*
 * object_holder.h
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

#include <boost/noncopyable.hpp>
#include <portability/memory.h>

namespace oci {

template<typename T, template<typename> class Ptr = weak_ptr>
class ObjectHolder : boost::noncopyable {
public:
    ObjectHolder() {}
    ObjectHolder(const Ptr<T>& obj) : mObject(obj) {}

    ~ObjectHolder() {
        reset();
    }

    ObjectHolder& operator= (const Ptr<T>& obj) {
        reset();
        mObject = obj;
        return *this;
    }

    Ptr<T>& get() {
        return mObject;
    }

private:

    shared_ptr<T> get_ptr(weak_ptr<T>& ptr) const {
        return ptr.lock();
    }

    shared_ptr<T> get_ptr(shared_ptr<T>& ptr) const {
        return ptr;
    }

    void reset() {
        shared_ptr<T> obj = get_ptr(mObject);
        if(obj)
            obj->Storage().KillObject(mObject);
    }

    Ptr<T> mObject;
};

} // namespace oci
