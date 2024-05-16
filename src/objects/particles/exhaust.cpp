/*
 * exhaust.cpp
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

#include "exhaust.h"

namespace oci {
namespace objects {

void Exhaust::Init(const Vector2f& position) {
    Animated::Init("engine.xml", position);
    df = 0;
}

void Exhaust::NextFrame() {
    SetFrame(CurrentFrame() + 1);
    if(CurrentFrame() > 6)
        switch (df) {
            case  1:
            case -1: SetFrame(4); break;
            case  0: SetFrame(0); break;
        }
    else if(CurrentFrame() > 2 && df == 0)
        SetFrame(0);
    else if(df == 1) {
        df = -1;
        SetFrame(4);
    }
}

} // namespace objects
} // namespace oci
