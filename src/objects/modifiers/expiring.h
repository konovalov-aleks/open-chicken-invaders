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

#include <type_traits>

namespace oci {
namespace objects {
namespace modifiers {

template<typename T>
class Expiring : public T, virtual public Active {
public:

    template<typename... Args>
    void Init(int life_time, const Args&... args) {
        T::Init(args...);
        mLifeTime = life_time;
    }

    virtual void Run() override {
        if(--mLifeTime < 0)
            this->Storage().KillObject(this);
        else
            CallParentRun(std::is_base_of<Active, T>());
    }

private:
    void CallParentRun(std::integral_constant<bool, false>) {}
    void CallParentRun(std::integral_constant<bool, true>) { T::Run(); }

    int mLifeTime;
};

} // namespace modifiers
} // namespace objects
} // namespace oci
