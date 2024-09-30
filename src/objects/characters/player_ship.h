/*
 * player_ship.h
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

#include <core/window.h>
#include <objects/base/collision_object.h>
#include <objects/gun/gun.h>
#include "shield.h"
#include "ship.h"

#include <memory>

namespace oci {
namespace objects {

class PlayerShip : public Ship, public ICollisionObject,
                   public std::enable_shared_from_this<PlayerShip> {
public:
    void Init();

    void StartFire() { mFire = true; }
    void StopFire() { mFire = false; }

    void CreateShield();

    static int DEFAULT_Y_POS() {
        return Window::Instance().getSize().y - 80;
    }

protected:
    virtual CollisionType GetCollisionType() const override { return ctPlayerShip; };
    virtual int CollisionWith() const override;
    virtual void OnCollision(const CollisionObjectInfo& collisedWith) override;
    virtual float DoGetX() const override;
    virtual float DoGetY() const override;
    virtual int DoGetFrameWidth() const override;
    virtual int DoGetFrameHeight() const override;
    virtual void Run() override;

private:
    /// Перемещение влево
    void MoveLeft();
    /// Перемещение вправо
    void MoveRight();
    /// Выстрелить
    void Fire();
    /// Сменить тип оружия
    void SelectGun(TGunType gun_type);
    /// Запустить ракету
    void StartMissile();

    /// скорость
    float mDX, mDY;
    /// ускорение
    float mFDX, mFDY;
    /// куда летит корабль? (Например, координаты точки, которая нажата стилусом.)
    float mTargetX, mTargetY;
    /// временное смещение по y (например, после выстрела)
    float mDeltaY;

    /// включена ли стрельба
    bool mFire;
    /// время перезарядки
    static const int mReloadTime = 10;
    /// время до следующего выстрела
    int mFireTime;

    bool mEnterPressed;

    /// оружие
    std::unique_ptr<Gun> mGun;

    std::weak_ptr<Shield> mShield;
};

} // namespace objects
} // namespace oci
