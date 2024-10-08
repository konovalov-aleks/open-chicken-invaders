/*
 * indicator.h
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

#include <core/vector2.h>
#include <objects/base/active.h>
#include <objects/base/sprite.h> // IWYU pragma: keep
#include <objects/base/visible.h>
#include <objects/modifiers/hidden.h> // IWYU pragma: keep
#include <objects/text/text.h> // IWYU pragma: keep

// IWYU pragma: no_include <__fwd/string_view.h>
#include <memory>
#include <string_view> // IWYU pragma: keep

namespace oci {
namespace levels {
namespace interface {

class Indicator : public objects::Active {
public:
    void Init(std::string_view sprite_name, const Vector2f& position);
    void Run() override;

protected:
    virtual int GetValue() const = 0;

private:
    std::shared_ptr<objects::modifiers::Hidden<objects::Text> > mText;
    std::shared_ptr<
        objects::modifiers::Hidden<
            objects::CommonSprite<objects::Visible::dpForeground>
        >
    > mIcons[3];
};

} // namespace interface
} // namespace levels
} // namespace oci
