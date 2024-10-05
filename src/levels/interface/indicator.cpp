/*
 * indicator.cpp
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

#include "lives_indicator.h"

#include <context/object_storage.h>

namespace oci {
namespace levels {
namespace interface {

using namespace objects;
using namespace modifiers;

void Indicator::Init(const std::string& sprite_name,
                     const Vector2f& position) {
    mText = Storage().CreateObject<Hidden<Text> >(
        "", Vector2f(position.x + 17, position.y),
        Font::GetFont("small.xml"), Text::haRight, Text::vaCenter
    );
    for(int i = 0; i < 3; ++i)
        mIcons[i] =
            Storage().CreateObject<Hidden<CommonSprite<Visible::dpForeground> > >(
                sprite_name, Vector2f(position.x + i * 12, position.y));
}

void Indicator::Run() {
    int value = GetValue();
    if(value <= 3) {
        mText->Hide();
        for(int i = 0; i < 3; ++i)
            mIcons[i]->SetVisible(i < value);
    } else {
        mIcons[0]->Show();
        mIcons[1]->Hide();
        mIcons[2]->Hide();
        mText->Show();
        mText->SetText(std::to_string(value));
    }
}

} // namespace interface
} // namespace levels
} // namespace oci
