/*
 * interface.cpp
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

#include "interface.h"

#include <core/window.h>
#include <game/state.h>
#include "lives_indicator.h"
#include "missiles_indicator.h"
#include <objects/base/sprite.h>

namespace oci {
namespace levels {

using namespace objects;

namespace {
    std::string IntToStrWithComma(int value) {
        if(value <= 0)
            return "0";
        char buffer[64];
        buffer[sizeof(buffer) - 1] = '\0';
        char* ptr = buffer + sizeof(buffer) - 1;
        int cnt = 1;
        while(value > 0) {
            *(--ptr) = value % 10 + '0';
            value /= 10;
            if(value && cnt++ % 3 == 0)
                *(--ptr) = ',';
        }
        return ptr;
    }
} // namespace

void Interface::Init() {
    // Top panel
    Storage().CreateObject<CommonSprite<Visible::dpForeground> >(
        "hud.xml", Vector2f(Window::Instance().GetWidth() - 50, 0));
    // Bottom panel
    Storage().CreateObject<CommonSprite<Visible::dpForeground> >(
        "hud.xml", Vector2f(Window::Instance().GetWidth() - 50,
                                Window::Instance().GetHeight()));
    mScoreText = Storage().CreateObject<Text>("0",
        Vector2f(Window::Instance().GetWidth() - 2, 2),
        Font::GetFont("medium.xml"), Text::haRight, Text::vaTop);

    Storage().CreateObject<interface::LivesIndicator>();
    Storage().CreateObject<interface::MissilesIndicator>();
}

void Interface::Run() {
    int score = State::Instance().Score();
    if(score != mDisplayedScore) {
        mDisplayedScore += (score - mDisplayedScore > 1500 ? 500 : 100);
        if(mDisplayedScore > score)
            mDisplayedScore = score;
        mScoreText->SetText(IntToStrWithComma(mDisplayedScore));
    }
}

} // namespace levels
} // namespace oci
