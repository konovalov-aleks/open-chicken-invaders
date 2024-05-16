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

#include <boost/noncopyable.hpp>
#include <core/color.h>
#include <core/image.h>
#include <core/shape.h>
#include <core/sprite.h>
#include <core/window.h>
#include <stdexcept>

namespace oci {
namespace objects {

namespace {

class Gradient : public core::Sprite, boost::noncopyable {
public:
    static Gradient& Instance() {
        static Gradient instance;
        return instance;
    }
private:
    Gradient() {
        if(!mImage.LoadFromFile("res/images/gradient.bmp"))
            throw std::logic_error("Can not load image file "
                                   "\"res/images/gradient.bmp\"");
        mImage.SetSmooth(false);
        mImage.CreateMaskFromColor(Color::White);
        SetImage(mImage);
    }
private:
    Image mImage;
};

} // namespace

void UnshadeScreen::Draw() {
    Gradient& gradient = Gradient::Instance();
    Window& window = Window::Instance();
    const int tilesize = gradient.GetSize().y;
    int x = mPosition;
    unsigned int y = 0;
    for(; x > 0 && y < window.GetHeight(); x -= tilesize, y += tilesize) {
        gradient.SetPosition(x - gradient.GetSize().x, y);
        window.Draw(gradient);
        window.Draw(Shape::Rectangle(x, y, window.GetWidth(), y + tilesize,
                                         Color::Black));
    }
    if(y < window.GetHeight())
        window.Draw(Shape::Rectangle(x, y, window.GetWidth(),
                                         window.GetHeight(), Color::Black));
}

void UnshadeScreen::Run() {
    mPosition += Gradient::Instance().GetSize().y;
    if(++mPosition > Window::Instance().GetWidth() + Gradient::Instance().GetSize().x)
        Storage().KillObject(this);
}

} // namespace objects
} // namespace oci
