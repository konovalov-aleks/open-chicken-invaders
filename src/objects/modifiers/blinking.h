/*
 * blinking.h
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

#include "../base/visible.h"

#include <concepts>

namespace oci {
namespace objects {
namespace modifiers {

template<std::derived_from<Visible> T>
class Blinkable : public T {
public:

    Blinkable() : mBlinkPeriod(), mCurrentTime() {}

    template<typename... Args>
    void Init(int blink_period, const Args&... args) {
        mBlinkPeriod = blink_period;
        T::Init(args...);
    }

    virtual void Draw() override {
        ++mCurrentTime;
        if(mCurrentTime * 2 < mBlinkPeriod)
            T::Draw();
        else if(mCurrentTime >= mBlinkPeriod)
            mCurrentTime = 0;
    }

private:
    int mBlinkPeriod, mCurrentTime;
};

} // namespace modifiers
} // namespace objects
} // namespace oci
