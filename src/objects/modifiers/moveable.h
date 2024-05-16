/*
 * moveable.h
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

#include <type_traits>

namespace oci {
namespace objects {
namespace modifiers {

namespace details {
    struct Dummy;
} // namespace details

template<typename T>
class Moveable : public T,
                 public std::conditional<std::is_base_of<Active, T>::value,
                                         details::Dummy, Active> {
public:
    static_assert(std::is_base_of<Sprite, T>::value,
                  "T must be derived from Sprite");

    template<typename... Args>
    void Init(const sf::Vector2f& speed, Args... args) {
        T::Init(args...);
        mSpeed = speed;
    }

    void SetSpeed(const sf::Vector2f& speed) {
        mSpeed = speed;
    }

    void Run() override {
        this->Move(mSpeed);
        CallParentRun(std::is_base_of<Active, T>());
    }

private:
    void CallParentRun(std::integral_constant<bool, false>) {}
    void CallParentRun(std::integral_constant<bool, true>) { T::Run(); }

    sf::Vector2f mSpeed;
};

template<typename T, int XSpeedMul10, int YSpeedMul10>
class Moveable2 : public T,
                  public std::conditional<std::is_base_of<Active, T>::value,
                                          details::Dummy, Active> {
public:
    static_assert(std::is_base_of<Sprite, T>::value,
                  "T must be derived from Sprite");

    void Run() override {
        this->Move(XSpeedMul10 / 10.0f, YSpeedMul10 / 10.0f);
    }

private:
    void CallParentRun(std::integral_constant<bool, false>) {}
    void CallParentRun(std::integral_constant<bool, true>) { T::Run(); }
};

} // namespace modifiers
} // namespace objects
} // namespace oci
