/*
 * background.h
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
#include <list>
#include <memory>
#include <objects/base/sprite.h>

namespace oci {

/// Синглтон, отвечающий за фон
class Background : boost::noncopyable {
public:
    /// Интерфейс класса, управляющего движением фонового изображения
    class Controller {
    public:
        /// \return Координату Х фона
        virtual int GetX() const = 0;
        /// \return Координату Y фона
        virtual int GetY() const = 0;
    };

    /// Класс, автоматически устанавливающий и сбрасывающий контроллер
    template<typename T>
    class ControllerSetter {
    public:
        ControllerSetter() : mController(new T()){
            Background::Instance().SetController(mController);
        }

        ~ControllerSetter() {
            Background::Instance().ResetController(mController);
        }

        T& operator*() {
            return *mController;
        }

        const T& operator*() const {
            return *mController;
        }

        T* operator->() {
            return mController.get();
        }

        const T* operator->() const {
            return mController.get();
        }

    private:
        shared_ptr<T> mController;
    };

    static Background& Instance();

    /// Нарисовать фон
    void Draw();
    /// Установить контроллер фона
    void SetController(const shared_ptr<Controller>& controller);
    /// Сбросить контроллер, использовать предыдущий
    void ResetController(const shared_ptr<Controller>& controller);
    /// Получить используемый контроллер
    const Controller& GetController() const;

private:
    class BackgroundSprite : public objects::Sprite {
    public:
        BackgroundSprite() {
            Sprite::Init("background.xml");
            SetCenter(Vector2f(0, 0));
        }

        virtual DrawPriority GetDrawPriority() const override {
            return dpForeground;
        }

        virtual context::ObjectsStorage& Storage() override {
            return *static_cast<context::ObjectsStorage*>(NULL);
        }
    } mSprite;

    Background();
    std::list<shared_ptr<Controller> > mControllers;
};

} // namespace oci
