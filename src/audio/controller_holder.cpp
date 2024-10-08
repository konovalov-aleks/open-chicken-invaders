/*
 * controller_holder.cpp
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

#include "controller_holder.h"

#include "controller.h"
#include <context/object_storage.h>

#include <utility>

namespace oci {
namespace audio {

void ControllerHolder::Init(std::unique_ptr<Controller>&& controller) {
    mController = std::move(controller);
}

void ControllerHolder::Run() {
    if(!mController || !mController->IsPlaying())
        Storage().KillObject(this);
}

} // namespace audio
} // namespace oci
