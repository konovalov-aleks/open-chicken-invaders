/*
 * intro_0.h
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

#include <background/background.h>
#include "intro_level.h"
#include <objects/base/active.h>
#include <objects/base/animated_collision_object.h>
#include <objects/base/sprite.h>
#include <objects/text/subtitle_text.h>
#include <solar_system.h>

#include <list>
#include <memory>

namespace oci {
namespace levels {

/// Стартовый мультик
class Intro_0 : public IntroLevel {
public:
    void Init();
private:
    virtual void RunIntro() override;

    class Exhaust : public objects::Animated {
    public:
        void Init(const Vector2f& position) { Animated::Init("engine.xml", position); }
        virtual DrawPriority GetDrawPriority() const override { return dpExhaust; }
    };

    /// Курица, которая в начале мультика убегает от персонажа
    /// кура ускоряется при попадании в нее пули
    class IntroChicken1 : public objects::AnimatedCollisionObject, public Active {
    public:
        void Init(const Vector2f& position);

        virtual CollisionType GetCollisionType() const override { return ctChicken; };
        virtual int CollisionWith() const override { return ctFriendBullet; };
        virtual DrawPriority GetDrawPriority() const override { return dpChicken; }
        void OnCollision(const objects::CollisionObjectInfo& collised_with) override;
        void Run() override;

    private:
        float mSpeed;
    };

    /// Кура. Такие куры летят за персонажем в конце мультика стаей.
    /// Кура летит вправо, после того, как окажется за экраном удаляется.
    class IntroChicken2 : public objects::Animated, public objects::Active {
    public:
        enum Type {
            tSmall,
            tMedium,
            tBig
        };

        /** \param position Начальные координаты куры
         *  \param type Тип курицы
         */
        void Init(const Vector2f& position, Type type);
        virtual DrawPriority GetDrawPriority() const override;
        virtual void Run() override;

    private:
        Type mType;
        float mSpeed;
    };

    /// Класс, управляющий движением фона
    class BackgroundController : public Background::Controller {
    public:
        int GetX() const override { return x; }
        int GetY() const override { return y; }

        int x, y;
    };

    std::weak_ptr<objects::Sprite> mShip;
    std::weak_ptr<Exhaust> mExhaust;
    std::weak_ptr<IntroChicken1> mChicken;
    std::weak_ptr<objects::SubtitleText> mSubtitle;
    Background::ControllerSetter<BackgroundController> mBackgroundController;
    std::list<std::weak_ptr<IntroChicken2> > mChickens2;
    char mMode;
};

} // namespace levels
} // namespace oci
