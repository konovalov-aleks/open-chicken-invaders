/*
 * red_gun.cpp
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

#include "red_gun.h"

#include <audio/controller_holder.h>
#include <audio/player.h>
#include "player_bullet.h"

#include <numbers>

namespace oci {
namespace objects {

/// Скорость пули
static const float BULLET_SPEED = 12.0f;

RedGun::RedGun(short level) : Gun(level, gt_Red) {
}

void RedGun::Fire(float x, float y) {
    switch(level) {
        case 1:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x, y - 18, BULLET_SPEED, 0.0f, 0, 1);
            break;
        case 2:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, 0.0f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.0f, 0, 1);
            break;
        case 3:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x, y - 18, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0, 0, 1);
            break;
        case 4:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 0, 1);
            break;
        case 5:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 0, 1);
            break;
        case 6:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 0, 1);
            break;
        case 7:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 0, 1);
            break;
        case 8:
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 0, 1);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 1, 2);
            CreateBullet<CPlayerBullet>(
                "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 0, 1);
            break;
        default:
            if(level<20) {
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 1, 2);
            } else {
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.7f,  1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x - 6, y - 12, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.26f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x - 4, y - 16, BULLET_SPEED, (2 * std::numbers::pi_v<float>) - 0.13f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x,     y - 18, BULLET_SPEED, 0, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x + 4, y - 16, BULLET_SPEED, 0.13f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.26f, 1, 2);
                CreateBullet<CPlayerBullet>(
                    "gun1.xml", x + 6, y - 12, BULLET_SPEED, 0.7f, 1, 2);
            }
    }
    Storage().CreateObject<audio::ControllerHolder>(audio::Play("tr3_239.wav"));
}

} // namespace objects
} // namespace oci
