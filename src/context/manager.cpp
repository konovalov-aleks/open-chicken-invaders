/*
 * manager.cpp
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

#include "manager.h"

#include <stdexcept>

namespace oci {
namespace context {

Manager Manager::mInstance;

Manager::Manager() {
}

std::shared_ptr<Context> Manager::GetContext(const std::string& context_name) {
    ContextsMap::const_iterator iter = mContexts.find(context_name);
    if(iter == mContexts.end())
        iter = mContexts.insert(std::make_pair(context_name, std::make_shared<Context>())).first;
    return iter->second;
}

void Manager::KillContext(const std::string& context_name) {
    mContexts.erase(context_name);
}

void Manager::SetActiveContext(const std::shared_ptr<Context>& context) {
    if(!context)
        throw std::logic_error("Error: attempt to activate NULL context");
    mActiveContext = context;
}

} // namespace context
} // namespace oci
