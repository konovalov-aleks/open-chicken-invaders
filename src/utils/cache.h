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

#include <utils/transparent_equal.h>
#include <utils/transparent_string_hash.h>

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace oci {

template<typename T, typename Loader>
class Cache {
public:
    T& Get(std::string_view name) {
        typename Storage::iterator iter = mCache.find(name);
        if(iter != mCache.end()) [[likely]]
            return iter->second;
        const auto [resIter, ok] =
            mCache.try_emplace(std::string(name), Loader()(name));
        assert(ok);
        return resIter->second;
    }

private:
    using Storage = std::unordered_map<
        std::string, T, TransparentStrHash, TransparentEqual
    >;
    Storage mCache;
};

} // namespace oci
