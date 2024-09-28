/*
 * solar_system.h
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

#include <objects/base/active.h>
#include <objects/base/sprite.h>

#include <memory>

namespace oci {

enum TPlanet
{
    PLANET_SUN      = 1,
    PLANET_MERCURY  = 2,
    PLANET_VENUS    = 4,
    PLANET_EARTH    = 8,
    PLANET_MARS     = 16,
    PLANET_ASTEROID = 32,
    PLANET_JUPITER  = 64,
    PLANET_SATURN   = 128,
    PLANET_URANUS   = 256,
    PLANET_NEPTUNE  = 512,
    PLANET_PLUTO    = 1024,
    PLANET_ALL      = 2047
};

enum TPlanetIndex
{
    PLANET_IND_SUN      = 0,
    PLANET_IND_MERCURY  = 1,
    PLANET_IND_VENUS    = 2,
    PLANET_IND_EARTH    = 3,
    PLANET_IND_MARS     = 4,
    PLANET_IND_ASTEROID = 5,
    PLANET_IND_JUPITER  = 6,
    PLANET_IND_SATURN   = 7,
    PLANET_IND_URANUS   = 8,
    PLANET_IND_NEPTUNE  = 9,
    PLANET_IND_PLUTO    = 10
};

//
// SolarSystem
//

/// Солнечная система - отрисовывает планеты, управляет фоном
class SolarSystem : public objects::Active {
public:
    virtual void Init(float x0, float y0, short planets);

    /// \return Высоту планеты с номером ind.
    /** \warning Планета с индексом ind должна существовать */
    int GetPlanetHeight(short ind);

    /// Координаты X планет
    static const int PLANET_X_SUN, PLANET_X_MERCURY, PLANET_X_VENUS,
        PLANET_X_EARTH, PLANET_X_MARS, PLANET_X_ASTEROID,
        PLANET_X_JUPITER, PLANET_X_SATURN, PLANET_X_URANUS,
        PLANET_X_NEPTUNE, PLANET_X_PLUTO;

    static const int PLANETS_X[];

private:
    virtual void Run() override;

    std::weak_ptr<objects::Sprite> mPlanets[11]; /// Спрайты планет
};

} // namespace oci
