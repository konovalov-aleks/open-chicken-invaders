/*
 * level_1_5.cpp
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

#include "level_1_5.h"

#include <core/window.h>
#include <objects/characters/paratrooper.h>
#include "factory.h"
#include <utils/cleanup_container.h>

namespace oci {
namespace levels {

using namespace objects;

const float CLevel_1_5::ChickenSpeed = 2.0f;

static Factory::Registrator<CLevel_1_5> reg1("level_1_5", "game", 0);
static Factory::Registrator<CLevel_1_5> reg2("level_1_6", "game", 1);
static Factory::Registrator<CLevel_1_5> reg3("level_1_7", "game", 2);

inline void CLevel_1_5::CreateParatrooper(float x, float y) {
    weak_ptr<Paratrooper> paratrooper = Storage().CreateObject<Paratrooper>(
        Vector2f(x, y), ChickenSpeed, 400, 1);
    shared_ptr<Paratrooper>(paratrooper)->SetOnCreateRandomChickenCallback(
        bind(static_cast<
                void(ChickensList::*)(const ChickensList::value_type&)
            >(&ChickensList::push_front), mChickens, placeholders::_1));
    mChickens.push_front(paratrooper);
}

void CLevel_1_5::Init(char Step) {
    GameLevel::Init("level_1_5");
    switch(Step) {
    case 0:
        ShowLevelInfo("5", "paratroopers!");
        for(int y = 0; y < 4; ++y) {
            float y0 = -40.0f - 150.0f * y;
            float dy = 40;
            for(int x = 0; x < 7; ++x) {
                CreateParatrooper(x * Window::Instance().GetWidth() / 7 + 32.0f, y0);
                y0 += dy;
                dy -= 14;
            }
        }
        break;
    case 1:
        ShowLevelInfo("6", "death from above");
        for(int n = 0; n < 16; ++n) {
            int x = rand() % Window::Instance().GetWidth();
            int y = -(rand() % Window::Instance().GetHeight());
            CreateParatrooper(x, y);
        } break;
    case 2:
        {
            ShowLevelInfo("7", "third time lucky");
            float y0 = -20;
            for(int n = 0; n < 3; ++n) {
                float x0 = Window::Instance().GetWidth() - 30;
                for(int k = 0; k < 6; ++k) {
                    CreateParatrooper(x0, y0);
                    x0 -= 100 + rand() % 20;
                    y0 -= 40;
                }
                y0 -= 240;
            }
            y0 = -260;
            for(int n = 0; n < 3; ++n) {
                float x0 = 30;
                for(int k = 0; k < 6; ++k) {
                    CreateParatrooper(x0, y0);
                    x0 += 100 + rand() % 20;
                    y0 -= 40;
                }
                y0 -= 240;
            }
        } break;
    }
}

void CLevel_1_5::Run() {
    Level::Run();
    cleanup(mChickens);
    if(mChickens.empty())
        EndLevel();
}

} // namespace levels
} // namespace oci
