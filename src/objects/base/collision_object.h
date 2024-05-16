/*
 * collision_object.h
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

#include "collision_object_types.h"
#include "object.h"
#include "sprite.h"

namespace oci {
namespace objects {

/// информация о сталкивающихся объектах
struct CollisionObjectInfo
{
    CollisionObjectInfo() {};
    CollisionObjectInfo(float _x, float _y, int _width, int _height,
                        CollisionType _type, short _power)
        : x(_x), y(_y), width(_width), height(_height), type(_type), power(_power) {};

    float x, y;
    int width, height;
    CollisionType type;
    short power;
};

class CollisionObjectHolder;

class ICollisionObject : virtual public Object {
public:
    void Init(short power);

    bool DetectCollision(ICollisionObject& obj);
    // \return флаги, указывающие на то, с кем может столкнуться объект
    virtual int CollisionWith() const = 0;
    // \return флаг, указывающий на тип объекта
    virtual CollisionType GetCollisionType() const = 0;
    CollisionObjectInfo CollisionInfo();

    virtual void OnCollision(const CollisionObjectInfo& collisedWith) = 0;

    /// \return координату x объекта
    virtual float DoGetX() const = 0;
    /// \return координату y объекта
    virtual float DoGetY() const = 0;
    /// \return ширину текущего кадра
    virtual int DoGetFrameWidth() const = 0;
    /// \return высоту текущего кадра
    virtual int DoGetFrameHeight() const = 0;

protected:
    /// мощность оружия или здоровье персонажа
    short power;
};

/// Неанимированный объект, у которого обрабатывается коллизия
class CCollisionObject : public ICollisionObject, public Sprite
{
public:
    void Init(const std::string& sprite_name, const Vector2f& pos,
              short power);

    virtual void OnCollision(const CollisionObjectInfo& collisedWith) = 0;

    /// \return координату X объекта
    virtual float DoGetX() const;
    /// \return координату Y объекта
    virtual float DoGetY() const;
    /// \return ширину текущего кадра
    virtual int DoGetFrameWidth() const;
    /// \return высоту текущего кадра
    virtual int DoGetFrameHeight() const;

protected:
    // флаги, указывающие на то, с кем может столкнуться объект
    int collision_with;
    // флаг, указывающий на тип объекта
    int collision_type;
};

} // namespace objects
} // namespace oci
