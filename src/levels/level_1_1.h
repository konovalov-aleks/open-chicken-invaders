/*
 * level_1_1.h
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

#include <memory>

namespace oci::objects {
    class RandomChicken;
} // namespace oci::objects

namespace oci {
namespace levels {

/// уровень, в котором одна кура летает по экрану
class Level_1_1 : public GameLevel {
public:
    void Init();
private:
    virtual void Run() override;

    std::weak_ptr<objects::RandomChicken> mChicken;
    int mMode;
};

} // namespace levels
} // namespace oci
