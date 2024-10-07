/*
 * for_each.h
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

#include <list>

namespace oci {

namespace details {
    template<typename T, typename... Args>
    void call(T& v, void (T::* func)(Args...), const Args& ...args) {
        (v.*func)(args...);
    }

    template<typename T, typename Func, typename... Args>
    void call(T& v, Func func, const Args& ...args) {
        func(v, args...);
    }
} // namespace details

/// Удаляет из контейнера все "мертвые" указатели, а для каждого из живых выполняет указанную функцию
template<typename T, typename Func, typename... Args>
void for_each(std::list<T>& container, Func f, const Args& ...args) {
    for(auto iter = container.begin(); iter != container.end();) {
        auto current = iter++;
        auto ptr = current->lock();
        if(!ptr)
            container.erase(current);
        else
            details::call(*ptr, f, args...);
    }
}

} // namespace oci
