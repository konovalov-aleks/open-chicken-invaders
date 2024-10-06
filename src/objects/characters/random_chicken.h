/*
 * random_chicken.h
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

#include "chicken.h"
#include <core/vector2.h>

namespace oci {
namespace objects {

/// Кура, которая произвольно летает по экрану, пока ее не убъют
class RandomChicken : public Chicken {
public:
    /// конструктор, который загружает спрайт по-умолчанию
    void Init(const Vector2f& position, int period, Type type, int health, float speed);

    /// конструктор, который указывает начальную точку-цель
    /**     \param x0, y0 Начальные координаты
            \param period Частота "бомбардировки" яйцами
            \param type   Тип курицы (цвет)
            \param health "здоровье" курицы
            \param Speed  Скорость курицы
            \param tag    Необязательное поле. Сделано под свои нужды. При удалении курицы разрушает Tag (если он не NULL)
            \param X0, Y0 Начальные координаты точки-цели (в которую кура летит)
    */
    void Init(const Vector2f& position, int period, Type type, int health,
              float speed, float X0, float Y0);

private:
    void GenerateNewPoint();
    virtual void Run() override;

    float mSpeed;  /// модуль вектора скорости
    float mX1, mY1; /// координаты точки, в которую летит кура
    float mDX, mDY; /// проекции вектора скорости на оси

    /// Максимальная координата Y точки, в которую может полететь кура
    static const int MAX_Y_START_COORD;
};

} // namespace objects
} // namespace oci
