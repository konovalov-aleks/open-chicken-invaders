/*
 * sprite.h
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

#include <assert.h>
#include <core/image.h>
#include <core/sprite.h>
#include <core/vector2.h>
#include <string>
#include <vector>
#include "visible.h"

//#define DEBUG_SPRITE

namespace oci {
namespace objects {

class Sprite : public core::Sprite, public Visible {
public:
    struct AnimationInfo {
        bool need_reverce;
        bool need_repeat;
    };

    struct Animation {
        struct AnimationInfo info;
        std::vector<Image> images;
    };

    typedef std::vector<Animation> SpriteData;

    void Init(const std::string& filename);
    void Init(const std::string& filename, const Vector2f& pos);

    virtual ~Sprite();
    size_t StatesCount() const;
    size_t CurrentState() const;
    bool SetState(size_t state);
    size_t FramesCount() const;
    size_t CurrentFrame() const;
    bool SetFrame(size_t frame);

    AnimationInfo AnimationInfo() const { assert(mData); return (*mData)[mCurrentState].info; }
    bool AnimationRepeated() const;
    bool AnimationReverced() const;

    int GetWidth() const;
    int GetHeight() const;

protected:
    virtual void Draw() override;

private:
    const SpriteData* mData;
    size_t mCurrentState;
    size_t mCurrentFrame;
};

template<Visible::DrawPriority draw_priority>
class CommonSprite : public Sprite {
public:
    void Init(const std::string& filename) {
        Sprite::Init(filename);
    }

    void Init(const std::string& filename, const Vector2f& pos) {
        Sprite::Init(filename, pos);
    }

    DrawPriority GetDrawPriority() const override {
        return draw_priority;
    }
};

} // namespace objects
} // namespace oci
