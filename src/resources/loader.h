/*
 * loader.h
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
#include <portability/memory.h>
#include <vector>

namespace oci {
namespace resources {

    class ResourcesLoader : boost::noncopyable {
    public:
        inline static ResourcesLoader& Instance() { return mInstance; }

        std::vector<char> GetData(const std::string& resource_name);

    private:
        ResourcesLoader();

        class Impl;
        unique_ptr<Impl> mImpl;
        static ResourcesLoader mInstance;
    };

} // namespace resources
} // namespace oci
