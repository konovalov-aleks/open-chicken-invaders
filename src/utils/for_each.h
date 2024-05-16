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

#ifdef CPP11

namespace oci {

namespace {
    template<typename T, typename... Args>
    inline void __call(T& v, void (T::* func)(Args...), const Args& ...args) {
        (v.*func)(args...);
    }

    template<typename T, typename Func, typename... Args>
    inline void __call(T& v, Func func, const Args& ...args) {
        func(v, args...);
    }
} // namespace

/// Удаляет из контейнера все "мертвые" указатели, а для каждого из живых выполняет указанную функцию
template<typename T, typename Func, typename... Args>
void for_each(T& container, Func f, const Args& ...args) {
    for(auto iter = container.begin(); iter != container.end();) {
        auto current = iter++;
        auto ptr = current->lock();
        if(!ptr)
            container.erase(current);
        else
            __call(*ptr, f, args...);
    }
}

} // namespace oci

#else

#include <boost/preprocessor.hpp>
#include <portability/cpp11.h>
#include <portability/memory.h>

namespace oci {

#define _GEN_FOR_EACH_PP_FUN(N, I, U)                                               \
namespace {                                                                         \
    template<typename T BOOST_PP_COMMA_IF(I)                                        \
             BOOST_PP_ENUM_PARAMS(I, typename T)>                                   \
    inline void __call(T& v, void (T::* func)(BOOST_PP_ENUM_PARAMS(I, T))           \
                       BOOST_PP_COMMA_IF(I)                                         \
                       BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) {             \
        (v.*func)(BOOST_PP_ENUM_PARAMS(I, arg));                                    \
    }                                                                               \
    template<typename T, typename Func BOOST_PP_COMMA_IF(I)                         \
             BOOST_PP_ENUM_PARAMS(I, typename T)>                                   \
    inline void __call(T& v, Func func BOOST_PP_COMMA_IF(I)                         \
                       BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) {             \
        func(v BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, arg));                  \
    }                                                                               \
} /* namespace */                                                                   \
template<typename T, typename Func BOOST_PP_COMMA_IF(I)                             \
         BOOST_PP_ENUM_PARAMS(I, typename T)>                                       \
void for_each(T& container, Func f BOOST_PP_COMMA_IF(I)                             \
              BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) {                      \
    for(typename T::iterator iter = container.begin(); iter != container.end();) {  \
        typename T::iterator current = iter++;                                      \
        shared_ptr<typename T::value_type::element_type> ptr(current->lock());      \
        if(!ptr)                                                                    \
            container.erase(current);                                               \
        else                                                                        \
            __call(*ptr, f BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, arg));      \
    }                                                                               \
}

BOOST_PP_REPEAT_FROM_TO(0, MAX_PARAMS_COUNT, _GEN_FOR_EACH_PP_FUN, ~)

} // namespace oci

#endif // CPP11
