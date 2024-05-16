/*
 * level.cpp
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

#include "level.h"

#include <assert.h>
#include <constants.h>
#include <core/event.h>
#include <core/window.h>
#include "manager.h"
#include <menu/mainmenu.h>
#include <objects/modifiers/blinking.h>
#include <objects/modifiers/expiring.h>
#include <objects/text/text.h>
#include <stddef.h>

namespace oci {
namespace levels {

using namespace objects;
using namespace modifiers;

void Level::Init(const std::string& levelname) {
    levels::Manager::Instance().LoadLevel(levelname);
}

void Level::EndLevel() {
    puts("End level");
    Storage().KillObject(this);
    levels::Manager::Instance().CreateNextLevel();
}

void Level::ShowLevelInfo(const std::string& index, const std::string& name,
                          const std::string& description, bool descr_blink) {

    printf("show level info %s %s\n", index.c_str(), name.c_str());
    const int window_center = Window::Instance().GetWidth() / 2;
    Storage().CreateObject<Expiring<Text> >(
                          20,
                          "wave " + index,
                          Vector2f(window_center, FirstTextStringY),
                          Font::GetFont("medium.xml"),
                          Text::haCenter, Text::vaCenter);
    Storage().CreateObject<Expiring<Text> >(
                          20,
                          name,
                          Vector2f(window_center, SecondTextStringY),
                          Font::GetFont("big.xml"),
                          Text::haCenter, Text::vaCenter);
    if(!description.empty()) {
        if(descr_blink)
            Storage().CreateObject<Expiring<Blinkable<Text> > >(
                                20, // life time
                                8,  // blink period
                                description,
                                Vector2f(window_center, ThirdTextStringY),
                                Font::GetFont("medium.xml"),
                                Text::haCenter, Text::vaCenter);
        else
            Storage().CreateObject<Expiring<Text> >(
                                20, // life time
                                description,
                                Vector2f(window_center, ThirdTextStringY),
                                Font::GetFont("medium.xml"),
                                Text::haCenter, Text::vaCenter);
    }
}

void Level::Run() {
    if(Window::Instance().GetInput().IsKeyDown(Key::Escape))
        MainMenu::SwitchToMenu();
}

} // namespace levels
} // namespace oci
