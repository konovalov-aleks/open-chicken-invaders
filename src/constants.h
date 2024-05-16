/*
 * constants.h
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

#include <core/window.h>

namespace oci {

/// Уровень высоты, на котором разбиваются яйца, на который падают бонусы и т.д.
inline float FloorLevel() {
    static const float floor_level = Window::Instance().GetHeight() - 20;
    return floor_level;
}

} // namespace oci

/// Координата Y первой строки текста (например, номер уровня)
extern const int FirstTextStringY;
/// Координата Y второй строки текста (например, название уровня)
extern const int SecondTextStringY;
/// Координата Y третьей строки текста
extern const int ThirdTextStringY;

namespace Boss2
{
    /// Скорость
    extern const float Speed;
} // namespace Boss2

namespace BigEgg
{
    /// Скорость
    extern const float Speed;
} // namespace BigEgg

