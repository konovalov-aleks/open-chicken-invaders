/*
 * intro_level.h
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

#include "level.h"

namespace oci {
namespace levels {

/// Интерфейс для создания мультиков
/** вызывает методы RunIntro, пока не нажали кнопку и не щелкнули мышкой.
    для завершения надо вызывать метод EndLevel*/
class IntroLevel : public Level {
public:
    /// метод вызывается, когда интро должен выполнить свои действия
    /// (сдвинеть какие-либо объекты, проиграть звуки и т.д)
    virtual void RunIntro() = 0;

protected:
    void Init(const char* levelname);

private:
    virtual void Run() override;
};

} // namespace levels
} // namespace oci
