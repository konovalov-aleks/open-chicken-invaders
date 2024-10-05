/*
 * unshade_screen.cpp
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

#include "unshade_screen.h"

#include <context/object_storage.h>
#include <core/color.h>
#include <core/critical_error.h>
#include <core/image.h>
#include <core/rect.h>
#include <core/rectangle_shape.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/vector2.h>
#include <core/window.h>

namespace oci {
namespace objects {

namespace {

class Gradient : public core::Sprite {
public:
    static Gradient& Instance() {
        static Gradient instance;
        return instance;
    }

    Vector2u getSize() const noexcept {
        return mTexture.getSize();
    }

private:
    Gradient() {
        Image img;
        if(!img.loadFromFile("res/images/gradient.bmp")) [[unlikely]]
            CriticalError("Can not load image file \"res/images/gradient.bmp\"");
        img.createMaskFromColor(Color::White);
        if(!mTexture.loadFromImage(img)) [[unlikely]]
            CriticalError("Can't create a texture");
        setTexture(mTexture, true);
    }

    Gradient(const Gradient&) = delete;
    Gradient& operator= (const Gradient&) = delete;
private:
    Texture mTexture;
};

} // namespace

void UnshadeScreen::Draw() {
    Gradient& gradient = Gradient::Instance();
    Window& window = Window::Instance();
    const Vector2u tex_size = gradient.getSize();
    const int tilesize = static_cast<int>(tex_size.y);
    int x = static_cast<int>(mPosition);
    unsigned y = 0;
    const Vector2u wnd_size = window.getSize();
    for(; x > 0 && y < wnd_size.y; x -= tilesize, y += tilesize) {
        gradient.setPosition(x - static_cast<int>(tex_size.x), y);
        window.draw(gradient);
        RectangleShape rs(Vector2f(static_cast<int>(wnd_size.x) - x, tilesize));
        rs.setPosition(x, y);
        rs.setFillColor(Color::Black);
        window.draw(rs);
    }
    if(y < window.getSize().y) {
        RectangleShape rs(Vector2f(wnd_size.x - x, wnd_size.y - y));
        rs.setPosition(x, y);
        rs.setFillColor(Color::Black);
        window.draw(rs);
    }
}

void UnshadeScreen::Run() {
    mPosition += Gradient::Instance().getSize().y;
    unsigned boundary = Window::Instance().getSize().x + Gradient::Instance().getSize().x;
    if(++mPosition > boundary)
        Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
