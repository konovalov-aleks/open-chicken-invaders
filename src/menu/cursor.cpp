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

#include <core/input.h>
#include <core/window.h>

#include <memory>

namespace oci {
namespace menu {

using namespace objects;

void Cursor::Init() {
    Window::Instance().ShowMouseCursor(false);
    Vector2f wnd_center(Window::Instance().GetWidth() / 2.0,
                        Window::Instance().GetHeight() / 2.0);
    std::shared_ptr<CursorSpriteType> sprite = Storage().CreateObject<
                                    CommonSprite<Visible::dpForeground>
                                >("fork.xml", wnd_center);
    Window::Instance().SetCursorPosition(wnd_center.x, wnd_center.y);
    sprite->SetCenter(0, 0);
    mSprite = sprite;
}

void Cursor::Run() {
    const Input& input = Window::Instance().GetInput();
    std::shared_ptr<CursorSpriteType> p = mSprite.get().lock();
    if(p)
        p->SetPosition(Vector2f(input.GetMouseX(), input.GetMouseY()));
}

} // namespace menu
} // namespace oci
