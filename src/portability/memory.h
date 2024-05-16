/*
 * memory.h
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

#ifndef CPP11
#   include <boost/enable_shared_from_this.hpp>
#   include <boost/make_shared.hpp>
#   include <boost/shared_ptr.hpp>
#endif

namespace oci {

#ifdef CPP11

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::unique_ptr;
using std::enable_shared_from_this;

#else

using boost::shared_ptr;
using boost::weak_ptr;
using boost::make_shared;
using boost::enable_shared_from_this;

template<typename T>
class DefaultDeleter {
public:
    void operator()(T* ptr) const { delete ptr; }
};

template<typename T, typename Deleter = DefaultDeleter<T> >
class unique_ptr {
public:
    typedef T element_type;

    explicit unique_ptr(T* p = 0) : mPtr(p), mDeleter() {}

    ~unique_ptr() { destroy(); }

    unique_ptr(const unique_ptr& other) : mPtr(other.mPtr) {
        const_cast<unique_ptr&>(other).mPtr = NULL;
    }

    void reset(T* p = 0) { destroy(); mPtr = p; }

    T& operator*() const { return *mPtr; }
    T* operator->() const { return mPtr; }
    T* get() const { return mPtr; }

    operator void*() const { return mPtr; }

    void swap(unique_ptr& b) { std::swap(b.mPtr, mPtr); }

private:
    void destroy() { mDeleter(mPtr); }

    T* mPtr;
    Deleter mDeleter;
};

#endif

} // namespace oci
