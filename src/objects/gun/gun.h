/*
 * gun.h
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

#pragma once

#include <assert.h>
#include <objects/base/sprite.h>
#include <context/manager.h>
#include <context/objects_storage.h>

namespace oci {
namespace objects {

/// Тип оружия
enum TGunType {
    gt_Red,
    gt_Green,
    gt_Yellow,
    GUN_TYPE_COUNT
};

/// Базовый класс оружия
class Gun {
public:
    short level;

    TGunType Type();
    virtual void Fire(float x, float y) = 0;
    virtual ~Gun() {}
protected:
    /** \param  lev              Уровень (мощьность) оружия
        \param  gun_type         Тип оружия
    */
    Gun(short lev, TGunType gun_type);

    context::ObjectsStorage& Storage() {
        shared_ptr<context::Context> game_context(
            context::Manager::Instance().GetContext("game"));
        assert(game_context.get());
        return game_context->GetStorage("level");
    }

    template<typename T>
    void CreateBullet(const char* sprite_name, float x, float y, float speed,
                      float angle, int state, short power) {
        Storage().CreateObject<T>(sprite_name, Vector2f(x, y), speed, angle,
                                  state, power);
    }

    /// тип оружия
    TGunType guntype;
};

} // namespace objects
} // namespace oci
