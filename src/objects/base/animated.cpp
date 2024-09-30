/*
 * animated.cpp
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

#include "animated.h"

namespace oci {
namespace objects {

void Animated::Init(const char* spritename, const Vector2f& position,
                    float animation_speed) {
    Sprite::Init(spritename);
    mAnimationSpeed = animation_speed;
    setPosition(position);
}

void Animated::NextFrame() {
    mAnimationPosition += mAnimationSpeed * mAnimationDirection;
    if(mAnimationPosition < 0 || mAnimationPosition > FramesCount() - 1) {
        if(!AnimationInfo().need_repeat) {
            Storage().KillObject(this);
            return;
        } else
            mAnimationPosition =
                (mAnimationPosition < 0 && AnimationInfo().need_reverse) ||
                (mAnimationPosition > FramesCount() - 1 &&
                 !AnimationInfo().need_reverse) ? 0 : FramesCount() - 1;
        if(AnimationInfo().need_reverse)
            mAnimationDirection = -mAnimationDirection;
    }
    SetFrame(static_cast<int>(mAnimationPosition));
}

} // namespace objects
} // namespace oci
