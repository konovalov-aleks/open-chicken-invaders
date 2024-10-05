/*
 * solar_system.cpp
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

#include "solar_system.h"

#include <background/background.h>
#include <context/object_storage.h>
#include <core/critical_error.h>
#include <core/window.h>

#include <cassert>
#include <memory>

namespace oci {

const int SolarSystem::PLANET_X_SUN       = 2 * ( 66 + 188 / 2);
const int SolarSystem::PLANET_X_MERCURY   = 2 * ( 20 + 19 / 2);
const int SolarSystem::PLANET_X_VENUS     = 2 * (-35 + 32 / 2);
const int SolarSystem::PLANET_X_EARTH     = 2 * (-82 + 34 / 2);
const int SolarSystem::PLANET_X_MARS      = 2 * (-131 + 29 / 2);
const int SolarSystem::PLANET_X_ASTEROID  = 2 * (-210 + 29 / 2);
const int SolarSystem::PLANET_X_JUPITER   = 2 * (-310 + 52 / 2);
const int SolarSystem::PLANET_X_SATURN    = 2 * (-440 + 70 / 2);
const int SolarSystem::PLANET_X_URANUS    = 2 * (-530 + 32 / 2);
const int SolarSystem::PLANET_X_NEPTUNE   = 2 * (-630 + 40 / 2);
const int SolarSystem::PLANET_X_PLUTO     = 2 * (-710 + 20 / 2);

const int SolarSystem::PLANETS_X [] = {
    PLANET_X_SUN,
    PLANET_X_MERCURY,
    PLANET_X_VENUS,
    PLANET_X_EARTH,
    PLANET_X_MARS,
    PLANET_X_ASTEROID,
    PLANET_X_JUPITER,
    PLANET_X_SATURN,
    PLANET_X_URANUS,
    PLANET_X_NEPTUNE,
    PLANET_X_PLUTO
};

namespace {
    class Planet : public objects::CommonSprite<objects::Visible::dpPlanet> {
    public:
        void Init(const std::string& name, int index) {
            Sprite::Init(name,
                         Vector2f(
                             Background::Instance().GetController().GetX() +
                             SolarSystem::PLANETS_X[index],
                             Background::Instance().GetController().GetY() +
                             (Window::Instance().getSize().y / 2)));
        }
    };

    inline std::weak_ptr<objects::Sprite> CreatePlanet(context::ObjectStorage& storage, const char* name, int ind) {
        return storage.CreateObject<Planet>(name, ind);
    }
} // namespace

void SolarSystem::Init(float /*x0*/, float /*y0*/, short planets) {
    if(planets & PLANET_SUN)
        mPlanets[PLANET_IND_SUN] = CreatePlanet(Storage(), "sun.xml", PLANET_IND_SUN);
    if(planets & PLANET_MERCURY)
        mPlanets[PLANET_IND_MERCURY] = CreatePlanet(Storage(), "mercury.xml", PLANET_IND_MERCURY);
    if(planets & PLANET_VENUS)
        mPlanets[PLANET_IND_VENUS] = CreatePlanet(Storage(), "venus.xml", PLANET_IND_VENUS);
    if(planets & PLANET_EARTH)
        mPlanets[PLANET_IND_EARTH] = CreatePlanet(Storage(), "earth.xml", PLANET_IND_EARTH);
    if(planets & PLANET_MARS)
        mPlanets[PLANET_IND_MARS] = CreatePlanet(Storage(), "mars.xml", PLANET_IND_MARS);
    if(planets & PLANET_ASTEROID)
        mPlanets[PLANET_IND_ASTEROID] = CreatePlanet(Storage(), "asteroids.xml", PLANET_IND_ASTEROID);
    if(planets & PLANET_JUPITER)
        mPlanets[PLANET_IND_JUPITER] = CreatePlanet(Storage(), "jupiter.xml", PLANET_IND_JUPITER);
    if(planets & PLANET_SATURN)
        mPlanets[PLANET_IND_SATURN] = CreatePlanet(Storage(), "saturn.xml", PLANET_IND_SATURN);
    if(planets & PLANET_URANUS)
        mPlanets[PLANET_IND_URANUS] = CreatePlanet(Storage(), "uranus.xml", PLANET_IND_URANUS);
    if(planets & PLANET_NEPTUNE)
        mPlanets[PLANET_IND_NEPTUNE] = CreatePlanet(Storage(), "neptune.xml", PLANET_IND_NEPTUNE);
    if(planets & PLANET_PLUTO)
        mPlanets[PLANET_IND_PLUTO] = CreatePlanet(Storage(), "pluto.xml", PLANET_IND_PLUTO);
}

int SolarSystem::GetPlanetHeight(short ind) {
    assert(ind >= PLANET_IND_SUN && ind <= PLANET_IND_PLUTO);
    std::shared_ptr<objects::Sprite> planet(mPlanets[ind].lock());
    if(!planet) [[unlikely]]
        CriticalError("attempt to access to a non-existent planet (index: ", ind, ')');
    return planet->GetHeight();
}

void SolarSystem::Run() {
    /// Двигаем все планеты исходя из положения фона
    for(int i = 0; i < 11; ++i) {
        std::shared_ptr<objects::Sprite> planet(mPlanets[i].lock());
        if(planet) {
            planet->setPosition(PLANETS_X[i] + Background::Instance().GetController().GetX(),
                                Background::Instance().GetController().GetY() +
                                (Window::Instance().getSize().y >> 1));
        }
    }
}

} // namespace oci
