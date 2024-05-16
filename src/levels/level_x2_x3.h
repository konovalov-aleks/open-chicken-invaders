/*
 * level_x2_x3.h
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

namespace oci {
namespace levels {

/// Интерфейс 2-ых и 3-их уровней на планете
/**     Прилетают куры, собираются в стаю и слоняются из стороны в сторону.
        Если задано, то любая курица (сама, случайно) может слетать в
        какую-нибудь точку и вернуться затем обратно.

        Для использования нужно наследовать класс, в конструкторе создавать кур,
        в поле Tag передавать им указатель на заполненную структуру
        Level_x2_x3::ChickenInfo.

        Загружается список ресурсов 1_1
*/
class Level_x2_x3 : public GameLevel {
public:

    class LevelChicken : public objects::Chicken {
    public:
        float X, Y;         /// координаты точки, в которую летит кура
        float xPos, yPos;   /// позиция куры без учета смещения стаи
        char mode;
    };

protected:

    virtual void CreateChickens(std::list<weak_ptr<LevelChicken> >& chickens) = 0;

    /** \param flyenable Могут ли куры вылетать из своих ячеек.
                         Обычно в уровне 2 false, в уровне 3 true
        \param flock_x_speed  Скорость движения стаи
    */
    void Init(bool flyenable, float flock_x_speed);
private:
    virtual void Run() override;

    std::list<weak_ptr<LevelChicken> > mChickens;

    float mFlockXPos;    /// смещение всей стаи по x
    float mFlockXSpeed; /// скорость перемещения всей стаи по x
    bool mFlyEnable;    /// могут ли куры пойти улететь из своей ячейки 
};

} // namespace levels
} // namespace oci
