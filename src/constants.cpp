/*
 * constants.cpp
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

#include "constants.h"

const int FirstTextStringY = 120;
const int SecondTextStringY = 150;
const int ThirdTextStringY = 180;

namespace Boss2
{
    const float Speed = 6;
} // namespace Boss2

namespace BigEgg
{
    const float Speed = 4;
} // namespace BigEgg

namespace Intro_10
{
    const float ShipSpeed     = 4.8f;
    const float ShipBackSpeed = 8.0f;
    const float ChickenSpeed  = 4.0f;
    const float ChickenSpeed_after_fire = 6.0f;
    const float BulletSpeed       = 8.0f;
    const float ChickenRightSpeed = 4.0f;
    const float InitialShipPosX   = 800;
    const float InitialShipPosY   = 250;
    const int   SolarSystemSpeedX = 2;
    const float InitialSolarSystemPosX = -280 * 2;
    const int   ExhaustDeltaX       = 20;
    const int   TitleY              = 400;
    const int   SolarSystemStopPosX = 450;
    const float InitialChickenPosX  = 680;
    const float InitialChickenPosY  = 250;
    const float ShipFirePosX        = 620;
    const int   ShipReversePosX     = -90;
    const int   ShipEngineSoundPosX = -10;
    const float SmokeSpeed      = -2;
    const int   ShipFinishPosX  = 680;
    const int   RightChickensMaxX = 2 * 280;
} // namespace Intro_0
