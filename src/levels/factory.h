/*
 * factory.h
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

#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/noncopyable.hpp>
#include <context/objects_storage.h>
#include <string>
#include <portability/cpp11.h>
#include <portability/functional.h>
#include <portability/unordered_map.h>

#ifndef CPP11
#   include <boost/preprocessor.hpp>
#endif

namespace oci {
namespace levels {

class Factory : boost::noncopyable {
public:
    static Factory& Instance();

#ifdef CPP11

    template<typename T, typename... Args>
    void Register(const std::string& obj_name, const std::string& context_name,
                  Args... args) {
        mObjects.insert(
            std::make_pair(obj_name, unique_ptr<IGenerator>(
                           new Generator<T, Args...>(context_name, args...)))
        );
    }

#else
    #define _GEN_FACTORY_PP_REGISTER(N, I, U) \
    template<typename T BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, typename T)> \
    void Register(const std::string& obj_name, const std::string& context_name    \
                  BOOST_PP_COMMA_IF(I)                                            \
                  BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) {                \
        mObjects.insert(                                                          \
            std::make_pair(obj_name, unique_ptr<IGenerator>(                      \
                new Generator##I<T BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, T)>( \
                        context_name BOOST_PP_COMMA_IF(I)                         \
                        BOOST_PP_ENUM_PARAMS(I, arg))))                           \
        );                                                                        \
    }
    BOOST_PP_REPEAT(MAX_PARAMS_COUNT, _GEN_FACTORY_PP_REGISTER, ~)
#endif

    void Build(const std::string& obj_name);

    template<typename T>
    class Registrator {
    public:
    #ifdef CPP11

        template<typename... Args>
        Registrator(const std::string& obj_name,
                    const std::string& context_name, const Args& ...args) {
            Factory::Instance().Register<T>(obj_name, context_name, args...);
        }

    #else
        Registrator(const std::string& obj_name,
                    const std::string& context_name) {
            Factory::Instance().Register<T>(obj_name, context_name);
        }
        #define _GEN_REGISTRATOR_PP_CONSTRUCTOR(N, I, U)            \
        template<BOOST_PP_ENUM_PARAMS(I, typename T)>               \
        Registrator(const std::string& obj_name,                    \
                    const std::string& context_name,                \
                    BOOST_PP_ENUM_BINARY_PARAMS(I, const T, arg)) { \
            Factory::Instance().Register<T>(obj_name, context_name, \
                BOOST_PP_ENUM_PARAMS(I, arg));                      \
        }
        BOOST_PP_REPEAT_FROM_TO(1, MAX_PARAMS_COUNT, _GEN_REGISTRATOR_PP_CONSTRUCTOR, ~)
    #endif
    };

private:

    class IGenerator {
    public:
        IGenerator(const std::string& context_name) : mContextName(context_name) {}
        virtual ~IGenerator() {}
        void Build();

    protected:
        virtual void DoBuild(context::ObjectsStorage& storage) = 0;

    private:
        std::string mContextName;
    };

#ifdef CPP11

    template<typename T, typename ...Args>
    class Generator : public IGenerator {
    public:
        Generator(const std::string& context_name, const Args& ...args) :
            IGenerator(context_name), mArgs(args...) {}
    protected:
        virtual void DoBuild(context::ObjectsStorage& storage) override {
            boost::fusion::tuple<context::ObjectsStorage*> s(&storage);
            boost::fusion::invoke(
                mem_fn(&context::ObjectsStorage::CreateObject<T, Args...>),
                boost::fusion::joint_view<
                    boost::fusion::tuple<context::ObjectsStorage*>,
                    boost::fusion::tuple<Args...>
                >(s, mArgs));
        }
    private:
        boost::fusion::tuple<Args...> mArgs;
    };

#else
    #define _GEN_GENERATOR_PP_CLASS(N, I, U) \
    template<typename T BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, typename T)> \
    class Generator##I : public IGenerator {                                     \
    public:                                                                   \
        Generator##I(const std::string& context_name BOOST_PP_COMMA_IF(I)        \
                  BOOST_PP_ENUM_BINARY_PARAMS(I, const T, &arg)) :            \
            IGenerator(context_name), mArgs(BOOST_PP_ENUM_PARAMS(I, arg)) {}  \
    protected:                                                                \
        virtual void DoBuild(context::ObjectsStorage& storage) {              \
            boost::fusion::tuple<context::ObjectsStorage*> s(&storage);       \
            boost::fusion::invoke(                                            \
                mem_fn(&context::ObjectsStorage::CreateObject<                \
                    T BOOST_PP_COMMA_IF(I) BOOST_PP_ENUM_PARAMS(I, T)>),      \
                boost::fusion::joint_view<                                    \
                    boost::fusion::tuple<context::ObjectsStorage*>,           \
                    boost::fusion::tuple<BOOST_PP_ENUM_PARAMS(I, T)>          \
                >(s, mArgs));                                                 \
        }                                                                     \
    private:                                                                  \
        boost::fusion::tuple<BOOST_PP_ENUM_PARAMS(I, T)> mArgs;               \
    };
    BOOST_PP_REPEAT(MAX_PARAMS_COUNT, _GEN_GENERATOR_PP_CLASS, ~)
#endif

    typedef unordered_map<std::string, unique_ptr<IGenerator> > ObjectsMap;
    ObjectsMap mObjects;
};

} // namespace levels
} // namespace oci
