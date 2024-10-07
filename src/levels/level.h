/*
 * level.h
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

namespace oci {
namespace levels {

/// интерфейс уровня (или мультика)
class Level : public objects::Active {
public:
    void Init(const char* levelname);

    /// функция для удаления текущего уровеня и перехода к новому
    void EndLevel();

    virtual void Run() override;

protected:
    /** Выводит на экран номер и название уровня
     *     \param  index           Номер уровня (без wave)
     *     \param  name            Название
     *     \param  description     Необязательное описание уровня
     *     \param  descr_blink     Если true, то описание мигает
     */
    void ShowLevelInfo(const char* index, const char* name,
                       const char* description = nullptr,
                       bool descr_blink = false);
};

} // namespace levels
} // namespace oci
