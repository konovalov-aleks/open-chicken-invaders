/*
 * game_over.cpp
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

#include <core/window.h>
#include "factory.h"
#include "level.h"
#include <objects/effects/unshade_screen.h>
#include <objects/modifiers/blinking.h>
#include <objects/text/text.h>

namespace oci {
namespace levels {

using namespace objects;
using namespace modifiers;

class GameOver : public Level {
public:
    void Init() {
        Storage().CreateObject<UnshadeScreen>();
        Storage().CreateObject<Blinkable<Text> >(
            20, // blink period
            "game over",
            Vector2f(Window::Instance().getSize().x / 2,
                     Window::Instance().getSize().y / 2),
            Font::GetFont("big.xml"),
            Text::haCenter, Text::vaCenter);
    }
};

static Factory::Registrar<GameOver> reg("game_over", "");

} // namespace levels
} // namespace oci
