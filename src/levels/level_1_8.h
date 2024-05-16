/*
 * level_1_8.h
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

#include "game_level.h"
#include <objects/characters/jumping_asteroid.h>
#include <portability/forward_list.h>

namespace oci {
namespace levels {

/// Уровень, в котором летает астероид, отскакивающий от границ экрана
class CLevel_1_8 : public GameLevel {
public:
    void Init(int step);
protected:
    virtual void Run() override;
private:
    typedef forward_list<weak_ptr<objects::JumpingAsteroid> > AsteroidsList;
    AsteroidsList mAsteroids;
};

} // namespace levels
} // namespace oci
