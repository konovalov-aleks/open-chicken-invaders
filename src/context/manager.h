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

#include "context.h"

// IWYU pragma: no_include <__fwd/string_view.h>
#include <string_view>
#include <unordered_map>

namespace oci {
namespace context {

class Manager {
public:
    inline static Manager& Instance() { return mInstance; }

    Manager(const Manager&) = delete;
    Manager& operator= (const Manager&) = delete;

    Context& GetContext(std::string_view context_name);
    void KillContext(std::string_view context_name);

    Context* GetActiveContext() const { return mActiveContext; }
    void SetActiveContext(Context&);
private:
    Manager() = default;

    Context* mActiveContext = nullptr;
    typedef std::unordered_map<std::string_view, Context> ContextsMap;
    ContextsMap mContexts;
    static Manager mInstance;
};

} // namespace context
} // namespace oci
