/*
 * manager.h
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

#include <boost/noncopyable.hpp>
#include "context.h"
#include <portability/memory.h>
#include <portability/unordered_map.h>
#include <string>

namespace oci {
namespace context {

class Manager : boost::noncopyable {
public:
    inline static Manager& Instance() { return mInstance; }

    shared_ptr<Context> GetContext(const std::string& context_name);
    void KillContext(const std::string& context_name);

    shared_ptr<Context> GetActiveContext() { return mActiveContext; }
    void SetActiveContext(const shared_ptr<Context>& context);
private:
    Manager();

    shared_ptr<Context> mActiveContext;
    typedef unordered_map<std::string, shared_ptr<Context> > ContextsMap;
    ContextsMap mContexts;
    static Manager mInstance;
};

} // namespace context
} // namespace oci
