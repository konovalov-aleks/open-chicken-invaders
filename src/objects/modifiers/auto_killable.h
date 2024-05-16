/*
 * auto_killable.h
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

#include <core/window.h>
#include <objects/base/active.h>
#include <portability/type_traits.h>

namespace oci {
namespace objects {
namespace modifiers {

class BoundLeft {};
class BoundTop {};
class BoundRight {};
class BoundBottom {};
class BoundNone {};

namespace details {

template<typename T>
struct CheckAction {
    template<typename ObjT>
    void operator()(ObjT&) const {}
};

template<>
struct CheckAction<BoundLeft> {
    template<typename ObjT>
    void operator()(ObjT& obj) const {
        if(obj.GetPosition().x <= -obj.GetWidth() / 2)
            obj.Storage().KillObject(&obj);
    }
};

template<>
struct CheckAction<BoundTop> {
    template<typename ObjT>
    void operator()(ObjT& obj) const {
        if(obj.GetPosition().y <= -obj.Height() / 2)
            obj.Storage().KillObject(&obj);
    }
};

template<>
struct CheckAction<BoundRight> {
    template<typename ObjT>
    void operator()(ObjT& obj) const {
        if(obj.GetPosition().x >= Window::Instance().GetWidth() + obj.GetWidth() / 2)
            obj.Storage().KillObject(&obj);
    }
};

template<>
struct CheckAction<BoundBottom> {
    template<typename ObjT>
    void operator()(ObjT& obj) const {
        if(obj.GetPosition().y >= Window::Instance().GetHeight() + obj.GetHeight() / 2)
            obj.Storage().KillObject(&obj);
    }
};

struct Dummy {};

} // namespace details

template<typename T,
         typename B1 = BoundNone,
         typename B2 = BoundNone,
         typename B3 = BoundNone,
         typename B4 = BoundNone>
class AutoKillable : public T,
                     public conditional<is_base_of<Active, T>::value,
                                        details::Dummy, Active> {
public:
    virtual void Run() override {
        CallParentRun(is_base_of<Active, T>());
        details::CheckAction<B1>()(*this);
        details::CheckAction<B2>()(*this);
        details::CheckAction<B3>()(*this);
        details::CheckAction<B4>()(*this);
    }

private:

    void CallParentRun(integral_constant<bool, false>) {}
    void CallParentRun(integral_constant<bool, true>) { T::Run(); }
};

} // namespace modifiers
} // namespace objects
} // namespace oci
