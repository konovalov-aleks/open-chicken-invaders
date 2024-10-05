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

#include <memory>
#include <string>
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
    void Register(std::string&& obj_name, std::string&& context_name, Args... args) {
        mObjects.insert(
            std::make_pair(std::move(obj_name), std::unique_ptr<IGenerator>(
                new Generator<T, Args...>(std::move(context_name), args...)))
        );
    }

    void Build(const std::string& obj_name);

    template<typename T>
    class Registrar {
    public:
        template<typename... Args>
        Registrar(std::string&& obj_name,
                  std::string&& context_name, Args... args) {
            Factory::Instance().Register<T>(std::move(obj_name),
                                            std::move(context_name),
                                            args...);
        }
    };

private:

    class IGenerator {
    public:
        IGenerator(std::string&& context_name)
            : mContextName(std::move(context_name))
        {}

        virtual ~IGenerator() = default;
        void Build();

    protected:
        virtual void DoBuild(context::ObjectStorage& storage) = 0;

    private:
        std::string mContextName;
    };

    template<typename T, typename... Args>
    class Generator : public IGenerator {
    public:
        Generator(std::string&& context_name, Args... args)
            : IGenerator(std::move(context_name))
            , mArgs(args...)
        {}
    protected:
        virtual void DoBuild(context::ObjectStorage& storage) override {
            const auto args =
                std::tuple_cat(std::tuple(&storage), mArgs);
            std::apply(&context::ObjectStorage::CreateObject<T, Args...>, args);
        }
    private:
        std::tuple<Args...> mArgs;
    };

    typedef std::unordered_map<std::string, std::unique_ptr<IGenerator> > ObjectsMap;
    ObjectsMap mObjects;
};

} // namespace levels
} // namespace oci
