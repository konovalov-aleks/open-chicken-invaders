/*
 * interface.h
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
#include <objects/base/sprite.h>
#include <objects/text/text.h>

#include <memory>

namespace oci {
namespace levels {

class Interface : public objects::Active {
public:
    Interface() : mDisplayedScore(0) {}

    void Init();
    void Run() override;
private:
    int mDisplayedScore;
    std::shared_ptr<objects::Text> mScoreText;
};

} // namespace levels
} // namespace oci
