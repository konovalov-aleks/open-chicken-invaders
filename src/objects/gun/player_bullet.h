/*
 * player_bullet.h
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

#include "bullet.h"

namespace oci {
namespace objects {

/// пуля игрока
class CPlayerBullet : public Bullet {
public:
    /** \param sprite_name      Название спрайта
        \param pos              Начальные координаты пули
        \param speed            Скорость пули
        \param angle            Угол, под которым летит пуля
        \param State            Тип пули (номер строки в текстуре)
        \param power            Мощность пули
    */
    void Init(const char* sprite_name, const Vector2f& pos, float speed, float angle, int State, short power) {
       Bullet::Init(sprite_name, pos, speed, angle, State, power, ctAsteroid | ctChicken, ctFriendBullet);
    }
};

} // namespace objects
} // namespace oci
