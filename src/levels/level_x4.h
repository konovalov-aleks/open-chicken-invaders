/*
 * level_x4.h
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
#include <objects/characters/asteroid.h>

#include <forward_list>
#include <memory>

namespace oci {
namespace levels {

/// Интерфейс 4-ых уровней на планете
/**     В уровне летают астероиды.

        Класс вызывает CreateAsteroid, когда должен создаться новый астероид и
        уменьшает количество астероидов, которые нужно создать.
        Когда количество астероидов становится нулевым, вызывается EndLevel.

        Для использования нужно создать класс-наследник, в нем определить
        метод CreateAsteroid. В нем создавать астероиды.

        Загружается список ресурсов 1_4
*/
class Level_x4 : public GameLevel {
protected:
    /** \param p   1/вероятность того, что в данный момент времени создастся астероид. То есть в одном случае из P создастся астероид.
        \param cnt Количество астероидов в уровне
    */
    void Init(int p, int cnt);
    /// В этом методе должны создаваться астероиды
    virtual std::weak_ptr<objects::Asteroid> CreateAsteroid() = 0;

private:
    virtual void Run() override;

    int mP;     /// вероятность того, что сейчас создастся новый астероид
    int mCount; /// кол-во астероидов, которые прилетят
    std::forward_list<std::weak_ptr<objects::Asteroid> > mAsteroids;
};

} // namespace levels
} // namespace oci
