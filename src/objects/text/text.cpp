/*
 * text.cpp
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

#include "text.h"

#include <core/critical_error.h>
#include <core/window.h>

#include <algorithm>
#include <array>
#include <cassert>

namespace oci {
namespace objects {

namespace {

float TopAlign(float coord, float /*size*/) {
    return coord;
}

float CenterAlign(float coord, float size) {
    return coord - size / 2.0f;
}

float BottomAlign(float coord, float size) {
    return coord - size;
}


template<typename T>
struct PairFilter {
    PairFilter(T val) : mVal(val) {}

    template<typename T2>
    bool operator() (const std::pair<T, T2>& elem) {
        return elem.first == mVal;
    }
private:
    T mVal;
};

} // namespace

class Text::Aligner {
public:
    Aligner(Text::VerticalAlign va, Text::HorizontalAlign ha) {
        typedef std::array<
            std::pair<Text::VerticalAlign, AlignerFunc>,
            3
        > VerticalArr;
        static const VerticalArr vertical = {
            std::make_pair(vaTop, TopAlign),
            std::make_pair(vaCenter, CenterAlign),
            std::make_pair(vaBottom, BottomAlign)
        };

        typedef std::array<
            std::pair<Text::HorizontalAlign, AlignerFunc>,
            3
        > HorizontalArr;
        static const HorizontalArr horizontal = {
            std::make_pair(haLeft, TopAlign),
            std::make_pair(haCenter, CenterAlign),
            std::make_pair(haRight, BottomAlign)
        };

        VerticalArr::const_iterator viter =
            std::find_if(vertical.begin(), vertical.end(),
                         PairFilter<Text::VerticalAlign>(va));
        if(viter == vertical.end()) [[unlikely]]
            CriticalError("unknown vertical align type");
        mVAligner = viter->second;

        HorizontalArr::const_iterator hiter =
            std::find_if(horizontal.begin(), horizontal.end(),
                        PairFilter<Text::HorizontalAlign>(ha));
        if(hiter == horizontal.end()) [[unlikely]]
            CriticalError("unknown horizontal align type");
        mHAligner = hiter->second;
    }

    Vector2f Align(const Vector2f& pos, const Vector2f& sz) const {
        return Vector2f(mHAligner(pos.x, sz.x), mVAligner(pos.y, sz.y));
    }

private:
    typedef float (*AlignerFunc)(float, float);
    AlignerFunc mVAligner, mHAligner;
};

void Text::Init(const std::string& text, const Vector2f& position,
                const Font& font, HorizontalAlign ha, VerticalAlign va) {
    mAligner.reset(new Aligner(va, ha));
    mFont = &font;
    mPosition = position;
    SetText(text);
}

Text::Text() {
}

Text::~Text() {
}

void Text::Draw() {
    for(size_t i = 0; i < mSprites.size(); ++i)
        Window::Instance().draw(mSprites[i]);
}

void Text::SetText(const std::string& text) {
    assert(mFont);
    assert(mAligner);
    mSprites.clear();
    mSprites.reserve(text.length());
    Vector2f size(0, 0);
    for(size_t i = 0; i < text.length(); ++i) {
        const Texture& glyph = (*mFont)[text[i]];
        mSprites.push_back(core::Sprite(glyph));
        size.x += glyph.getSize().x + 1;
        if(glyph.getSize().y > size.y)
            size.y = glyph.getSize().y;
    }
    Vector2f pos = mAligner->Align(mPosition, size);
    for(std::vector<core::Sprite>::iterator glyph = mSprites.begin();
       glyph != mSprites.end(); ++glyph) {
        glyph->setPosition(pos);
        pos.x += glyph->getTexture()->getSize().x + 1;
    }
}

} // namespace objects
} // namespace oci
