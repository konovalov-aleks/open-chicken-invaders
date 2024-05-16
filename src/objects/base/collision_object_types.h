/*
 * collision_object_types.h
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

enum CollisionType
{
    ctPlayerShip   = 1,
    ctChicken      = 2,
    ctFriendBullet = 4,
    ctEnemyBullet  = 8,
    ctAsteroid     = 16,

    ctBonuses      = 4064,
    ctRedBonus     = 32,
    ctYellowBonus  = 64,
    ctGreenBonus   = 128,
    ctUpgradeBonus = 256,
    ctCoinBonus    = 512,
    ctLegBonus     = 1024,
    ctBodyBonus    = 2048,

    ctBigEgg       = 4096,
    ctMissile      = 8192,

    ctFriendBullets = ctFriendBullet | ctMissile
};
