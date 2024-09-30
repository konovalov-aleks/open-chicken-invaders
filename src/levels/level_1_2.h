/*
 * level_1_2.h
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
#include <objects/characters/chicken.h>

#include <list>
#include <memory>

namespace oci {
namespace levels {

class Level_1_2 : public GameLevel {
public:

    class LevelChicken : public objects::Chicken {
    public:
        void Init(float x, float y, int xpos, int ypos, char Mode, float y0) {
            Chicken::Init(Vector2f(x, y0), 800, Chicken::tRed, 1);
            X = x;
            Y = y;
            xPos = xpos;
            yPos = ypos;
            mode = Mode;
        }

        float X, Y; /// координаты точки, в которую летит кура
        int xPos, yPos; /// координаты куры в сетке
        char mode;
    };

    void Init(int Step);

private:
    std::list<std::weak_ptr<LevelChicken> > mChickens;

    virtual void Run() override;

    char mode;
    /// скорость движения всех куриц
    float dx;
};

} // namespace levels
} // namespace oci
