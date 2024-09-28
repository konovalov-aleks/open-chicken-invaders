/*
 * factory.cpp
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

#include "factory.h"

#include <stdexcept>
#include <context/manager.h>

namespace oci {
namespace levels {

Factory& Factory::Instance() {
    static Factory instance;
    return instance;
}

void Factory::Build(const std::string& obj_name) {
    ObjectsMap::iterator iter = mObjects.find(obj_name);
    if(iter == mObjects.end())
        throw std::logic_error("Cannot find level object by name \"" +
                               obj_name + "\"");
    iter->second->Build();
}

void Factory::IGenerator::Build() {
    std::shared_ptr<context::Context> context =
        context::Manager::Instance().GetContext(mContextName);
    DoBuild(context->GetStorage(context::storage::LOCAL));
    context::Manager::Instance().SetActiveContext(context);
}

} // namespace levels
} // namespace oci
