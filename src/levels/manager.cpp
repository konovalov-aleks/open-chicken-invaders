/*
 * manager.cpp
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

#include "manager.h"

#include "factory.h"
#include "interface/interface.h"
#include <context/context.h>
#include <context/manager.h>
#include <context/object_storage.h>
#include <core/critical_error.h>
#include <game/state.h>
#include <objects/characters/player_ship.h>

#include <cstdio>
#include <string>

namespace oci {
namespace levels {

Manager& Manager::Instance() {
    static Manager manager;
    return manager;
}

Manager::Manager() : current_level_index(0) {
}

namespace {
    const char* levels[] = {
        "intro_0",
        "start_round_1",
        "level_1_1",
        "level_1_2",
        "level_1_3",
        "level_1_4",
        "level_1_5",
        "level_1_6",
        "level_1_7",
        "level_1_8",
        "level_1_9",
        "level_1_10",
        "intro_1",
        "game_over"
    };
}

void Manager::NewGame() {
    context::Manager::Instance().KillContext("game");
    context::Manager::Instance().KillContext("intro");
    context::ObjectStorage& game_global_storage =
        context::Manager::Instance().GetContext("game")->GetStorage("global");
    mPlayerShip = game_global_storage.CreateObject<objects::PlayerShip>();
    game_global_storage.CreateObject<Interface>();
    State::Instance().Reset();
    SelectLevel(0);
}

void Manager::SelectLevel(int level_index) {
    std::printf("SelectLevel %d\n", level_index);
    Factory::Instance().Build(levels[level_index]);
    current_level_index = level_index;
/*    ObjectsStorage& storage = LevelLocalStorage();
std::printf("select level %llx\n", (long long)&storage);
    switch(level_index) {
//        case 0: new CWarp(10); break;
//        case 1: new CWarp(9); break;
//        case 2: new CWarp(8); break;
//        case 3: new CWarp(7); break;
//        case 4: new CWarp(6); break;
//        case 5: new CWarp(5); break;
//        case 6: new CWarp(4); break;
//        case 7: new CWarp(3); break;
//        case 8: new CWarp(2); break;
//        case 9: new CWarp(1); break;
//        case 10: new CWarp(0); break;
*/
/*        case 8:   new CLevel_1_8(0);            break;
        case 9:   new CLevel_1_9(0);            break;
        case 10:  new CLevel_1_10();            break;
case 11: new CEndGame(); break;
//        case 11:  new CWarp(PLANET_IND_PLUTO);  break;

        case 12:  new CIntro_1();               break;
        case 13:  new CLevel_2_1();             break;
        case 14:  new CLevel_2_2(0);            break;
        case 15:  new CLevel_2_2(1);            break;
        case 16:  new CLevel_2_4();             break;
        case 17:  new CLevel_2_5();             break;
        case 18:  new CLevel_2_6();             break;
        case 19:  new CLevel_2_7();             break;
        case 20:  new CLevel_1_8(1);            break;
        case 21:  new CLevel_1_9(1);            break;
        case 22:  new CLevel_2_10();            break;
        case 23:  new CWarp(PLANET_IND_NEPTUNE);break;
        case 24:  new CIntro_2();               break;
        case 25:  new CLevel_3_1();             break;
        case 26:  new CLevel_3_2(0);            break;
        case 27:  new CLevel_3_2(1);            break;
        case 28:  new CLevel_3_4();             break;
        case 29:  new CLevel_3_5(0);            break;
        case 30:  new CLevel_3_6();             break;
        case 31:  new CLevel_3_5(1);            break;
        case 32:  new CLevel_3_8();             break;
        case 33:  new CLevel_1_9(2);            break;
        case 34:  new CLevel_3_10();            break;
        case 35:  new CWarp(PLANET_IND_URANUS); break;
        default: new CEndGame();*/
//    }
}

void Manager::CreateNextLevel() {
    SelectLevel(current_level_index + 1);
}

void Manager::GameOver() {
    Factory::Instance().Build("game_over");
}

objects::PlayerShip& Manager::GetPlayerShip() const {
    if(!mPlayerShip) [[unlikely]]
        CriticalError("Player ship is not created");
    return *mPlayerShip;
}

} // namespace levels
} // namespace oci
