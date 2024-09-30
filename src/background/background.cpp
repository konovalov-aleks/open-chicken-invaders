/*
 * background.cpp
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

#include "background.h"

#include <core/rect.h>
#include <core/window.h>

#include <cassert>

namespace oci {

namespace {

class StaticController : public Background::Controller {
public:
    virtual int GetX() const override { return 0; }
    virtual int GetY() const override { return 0; }
};

} // namespace

Background& Background::Instance() {
    static Background instance;
    return instance;
}

Background::Background() {
    SetController(std::make_shared<StaticController>());
}

void Background::Draw() {
    const Vector2u tex_size = mSprite.getTexture()->getSize();
    const int width = tex_size.x;
    const int height = tex_size.y;
    int xx = GetController().GetX() % width;
    if(xx > 0)
        xx -= width;
    int y0 = GetController().GetY() % height;
    if(y0 > 0)
        y0 -= height;
    while(xx < static_cast<int>(Window::Instance().getSize().x)) {
        int yy = y0;
        while(yy < static_cast<int>(Window::Instance().getSize().y)) {
            mSprite.setPosition(xx, yy);
            Window::Instance().draw(mSprite);
            yy += height;
        }
        xx += width;
    }
}

void Background::SetController(const std::shared_ptr<Controller>& controller) {
    assert(controller.get());
    mControllers.push_front(controller);
}

void Background::ResetController(const std::shared_ptr<Controller>& controller) {
    mControllers.remove(controller);
}

const Background::Controller& Background::GetController() const {
    assert(!mControllers.empty());
    return *mControllers.front();
}

} // namespace oci
