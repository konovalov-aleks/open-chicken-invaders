/*
 * cache.h
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

#include <string>
#include <unordered_map>
#include <utility>

namespace oci {

template<typename T, typename Loader>
class Cache {
public:
    const T& Get(const std::string& name) {
        typename Storage::const_iterator iter = mCache.find(name);
        if(iter != mCache.end())
            return iter->second;
        std::pair<typename Storage::const_iterator, bool> res =
            mCache.insert(std::make_pair(name, Loader()(name)));
        return res.first->second;
    }

private:
    typedef std::unordered_map<std::string, T> Storage;
    Storage mCache;
};

} // namespace oci
