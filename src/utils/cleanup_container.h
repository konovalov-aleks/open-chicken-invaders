/*
 * cleanup_container.h
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

#include <functional>
#include <forward_list>
#include <list>

namespace oci {

/// Удаляет из контейнера все "мертвые" указатели
template<typename T>
void cleanup(std::list<T>& container) {
    container.remove_if(std::mem_fn(&T::expired));
}

/// Удаляет из контейнера все "мертвые" указатели
template<typename T>
void cleanup(std::forward_list<T>& container) {
    container.remove_if(std::mem_fn(&T::expired));
}

/// Удаляет из контейнера все "мертвые" указатели
template<typename T>
void cleanup(T& container) {
    for(typename T::iterator iter = container.begin();
       iter != container.end();) {
        typename T::iterator current = iter++;
        if(current->expired())
            container.erase(current);
    }
}

} // namespace oci
