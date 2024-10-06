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

#include <context/object_storage.h>

// IWYU pragma: no_include <__fwd/string_view.h>
#include <cassert>
#include <memory>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace oci {
namespace levels {

class Factory {
public:
    static Factory& Instance();

    Factory() = default;

    Factory(const Factory&) = delete;
    Factory& operator= (const Factory&) = delete;

    template<typename T, typename... Args>
    void Register(std::string_view obj_name, std::string_view context_name, Args&&... args) {
        [[maybe_unused]] bool ok =
            mObjects.emplace(obj_name, std::unique_ptr<IGenerator>(
                new Generator<T, Args...>(context_name, std::forward<Args>(args)...))).second;
        assert(ok);
    }

    void Build(std::string_view obj_name);

    template<typename T>
    class Registrar {
    public:
        template<typename... Args>
        Registrar(std::string_view obj_name,
                  std::string_view context_name, Args... args) {
            Factory::Instance().Register<T>(obj_name, context_name, std::forward<Args>(args)...);
        }
    };

private:

    class IGenerator {
    public:
        IGenerator(std::string_view context_name)
            : mContextName(context_name)
        {}

        virtual ~IGenerator() = default;
        void Build();

    protected:
        virtual void DoBuild(context::ObjectStorage& storage) = 0;

    private:
        std::string_view mContextName;
    };

    template<typename T, typename... Args>
    class Generator : public IGenerator {
    public:
        Generator(std::string_view context_name, Args... args)
            : IGenerator(context_name)
            , mArgs(args...)
        {}
    protected:
        virtual void DoBuild(context::ObjectStorage& storage) override {
            auto args = std::tuple_cat(std::tuple(&storage), mArgs);
            std::apply(&context::ObjectStorage::CreateObject<T, Args...>,
                       std::tuple(args));
        }
    private:
        std::tuple<Args...> mArgs;
    };

    typedef std::unordered_map<std::string_view, std::unique_ptr<IGenerator> > ObjectsMap;
    ObjectsMap mObjects;
};

} // namespace levels
} // namespace oci
