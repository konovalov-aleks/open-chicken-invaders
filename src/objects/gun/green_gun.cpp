/*
 * green_gun.cpp
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

#include "green_gun.h"

#include "gun.h"
#include "player_bullet.h"
#include <audio/controller_holder.h>
#include <audio/player.h>
#include <context/object_storage.h>

#include <numbers>
#include <string_view>

namespace oci {
namespace objects {

/// Скорость пули
static const float BULLET_SPEED = 14.0f;

GreenGun::GreenGun(short level) : Gun(level, gt_Green) {
}

void GreenGun::Fire(float x, float y) {
    switch(level) {
        case 1:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x, y - 18, BULLET_SPEED, 0, 0, 1);
            break;
        case 2:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.017f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 1,  y - 12, BULLET_SPEED, 0.017f, 0, 1);
            break;
        case 3:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.017f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0.017f, 0, 1);
            break;
        case 4:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.017f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0.017f, 0, 1);
            break;
        case 5:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.017f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0.017f, 1, 2);
            break;
        case 6:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.017f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0.017f, 1, 2);
            break;
        case 7:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 2, 4);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0, 0, 1);
            break;
        case 8:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 2, 4);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0, 1, 2);
            break;
        case 9:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 20, y - 6, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.035f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 20, y - 6, BULLET_SPEED, 0.035f, 0, 1);
            break;
        default:
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 20, y - 6, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.035f, 2, 4);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x - 10, y - 12, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x,      y - 18, BULLET_SPEED, 0, 2, 4);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 10, y - 12, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun3.xml", x + 20, y - 6, BULLET_SPEED, 0.035f, 2, 4);
    }
    Storage().CreateObject<audio::ControllerHolder>(
        audio::Play("halflife_0040.wav"));
}

} // namespace objects
} // namespace oci
