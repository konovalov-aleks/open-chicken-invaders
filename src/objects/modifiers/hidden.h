/*
 * hidden.h
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

#include "../base/visible.h"
#include <portability/type_traits.h>

namespace oci {
namespace objects {
namespace modifiers {

template<typename T>
class Hidden : public T {
public:

    static_assert((is_base_of<objects::Visible, T>::value),
                  "T must be visible object");

    virtual void Draw() override {
        if(mVisible)
            T::Draw();
    }

    bool Visible() const { return mVisible; }
    void SetVisible(bool visible) { mVisible = visible; }
    void Hide() { SetVisible(false); }
    void Show() { SetVisible(true); }

protected:
    Hidden() : mVisible(true) {}

    bool mVisible;
};

} // namespace modifiers
} // namespace objects
} // namespace oci
