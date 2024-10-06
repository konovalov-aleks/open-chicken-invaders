/*
 * level_1_9.cpp
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

#include "game_level.h"
#include "level.h"
#include <context/object_storage.h>
#include <core/critical_error.h>
#include <levels/factory.h>
#include <objects/characters/ufo.h>

#include <memory>
#include <string_view>

namespace oci {
namespace levels {

/// Бонусный уровень - летит НЛО, в нем бонус
class Level_1_9 : public GameLevel {
public:
    /**  \param Step        Номер создаваемого уровня
         \param UFO_speed   Скорость НЛО
    */
    void Init(int step) {
        GameLevel::Init("level_1_9");
        float ufo_speed = 0;
        const char* str;
        switch(step) {
        case 0:
            str = "9";
            ufo_speed = 4;
            break;
        case 1:
            str = "19";
            ufo_speed = 4.6f;
            break;
        case 2:
            str = "29";
            ufo_speed = 6;
            break;
        default: [[unlikely]]
            CriticalError("level 1.9: unknown level step (", step, ')');
        }
        mUFO = Storage().CreateObject<objects::UFO>(ufo_speed);
        ShowLevelInfo(str, "bonus");
    }

protected:
    void Run() override {
        Level::Run();
        if(mUFO.expired())
            EndLevel();
    }
private:
    std::weak_ptr<objects::UFO> mUFO;
};

Factory::Registrar<Level_1_9> reg1("level_1_9", "game", 0);
Factory::Registrar<Level_1_9> reg2("level_2_9", "game", 1);
Factory::Registrar<Level_1_9> reg3("level_3_9", "game", 2);

} // namespace levels
} // namespace oci
