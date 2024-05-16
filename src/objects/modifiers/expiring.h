/*
 * expiring.h
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

#include "../base/active.h"
#include <portability/cpp11.h>
#include <portability/type_traits.h>

#ifndef CPP11
#   include <boost/preprocessor.hpp>
#endif

namespace oci {
namespace objects {
namespace modifiers {

template<typename T>
class Expiring : public T, virtual public Active {
public:

#ifdef CPP11
    template<typename... Args>
    void Init(int life_time, const Args&... args) {
        T::Init(args...);
        mLifeTime = life_time;
    }
#else
    void Init(int life_time) {
        T::Init();
        mLifeTime = life_time;
    }

    #define _GEN_EXPIRING_PP_CONSTRUCTOR(N, I, U)                             \
    template<BOOST_PP_ENUM_PARAMS(I, typename T)>                             \
    void Init(int life_time, BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) { \
        T::Init(BOOST_PP_ENUM_PARAMS(I, arg));                                \
        mLifeTime = life_time;                                                \
    }
    BOOST_PP_REPEAT_FROM_TO(1, MAX_PARAMS_COUNT, _GEN_EXPIRING_PP_CONSTRUCTOR, ~)
#endif

    virtual void Run() override {
        if(--mLifeTime < 0)
            this->Storage().KillObject(this);
        else
            CallParentRun(is_base_of<Active, T>());
    }

private:
    void CallParentRun(integral_constant<bool, false>) {}
    void CallParentRun(integral_constant<bool, true>) { T::Run(); }

    int mLifeTime;
};

} // namespace modifiers
} // namespace objects
} // namespace oci
