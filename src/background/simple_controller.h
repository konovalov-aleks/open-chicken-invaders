/*
 * simple_controller.h
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

#include "background.h"
#include <objects/base/active.h>

namespace oci {

class SimpleController : public Background::Controller, public objects::Active {
public:
    int GetX() const override { return mPos.x; }
    int GetY() const override { return mPos.y; }

    void SetSpeed(const Vector2f& speed) { mSpeed = speed; }
    const Vector2f& GetSpeed() const { return mSpeed; }
    void Accelerate(float dx, float dy) {
        mSpeed.x += dx;
        mSpeed.y += dy;
    }
protected:
    void Run() override { mPos += mSpeed; }
private:
    Vector2f mPos, mSpeed;
};

} // namespace oci
