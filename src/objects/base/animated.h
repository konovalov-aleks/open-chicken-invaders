/*
 * animated.h
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

#include "object.h"
#include "sprite.h"
#include "visible.h"
#include <core/vector2.h>

namespace oci {
namespace objects {

/// анимированный объект
class Animated : virtual public Object, public Sprite {
public:
    Animated() : mAnimationSpeed(0), mAnimationPosition(0), mAnimationDirection(1) {}
    void Init(const char* spritename, const Vector2f& position, float animation_speed = 1.0f);

    /// вызывается, когда нужно переключить кадр
    virtual void NextFrame();
    inline void SetAnimationSpeed(float speed) { mAnimationSpeed = speed; }
protected:
    float mAnimationSpeed;
    float mAnimationPosition;
    int mAnimationDirection;
};

template<Visible::DrawPriority draw_priority>
class CommonAnimated : public Animated {
public:
    void Init(const char* spritename, const Vector2f& position,
              float animation_speed = 1.0f) {
        Animated::Init(spritename, position, animation_speed);
    }

    DrawPriority GetDrawPriority() const override {
        return draw_priority;
    }
};

} // namespace objects
} // namespace oci
