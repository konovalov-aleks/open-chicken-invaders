/*
 * cursor.cpp
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

#include "cursor.h"

#include <core/mouse.h>
#include <core/window.h>

#include <memory>

namespace oci {
namespace menu {

using namespace objects;

void Cursor::Init() {
    Window& wnd = Window::Instance();
    wnd.setMouseCursorVisible(false);
    const Vector2u wnd_size = wnd.getSize();
    Vector2i wnd_center(wnd_size.x / 2, wnd_size.y / 2);
    std::shared_ptr<CursorSpriteType> sprite = Storage().CreateObject<
                                    CommonSprite<Visible::dpForeground>
                                >("fork.xml", Vector2f(wnd_center));
    Mouse::setPosition(wnd_center, wnd);
    sprite->setOrigin(0, 0);
    mSprite = sprite;
}

void Cursor::Run() {
    std::shared_ptr<CursorSpriteType> p = mSprite.get().lock();
    if(p)
        p->setPosition(Vector2f(Mouse::getPosition(Window::Instance())));
}

} // namespace menu
} // namespace oci
