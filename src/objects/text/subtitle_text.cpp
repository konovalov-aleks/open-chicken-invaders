/*
 * subtitle_text.cpp
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

#include "subtitle_text.h"

#include "text.h"
#include <context/object_holder.h>
#include <context/object_storage.h>
#include <core/critical_error.h>
#include <core/vector2.h>
#include <core/window.h>
#include <font/font.h>

#include <tinyxml2.h>

#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>

namespace oci {
namespace objects {

using namespace std::literals;

static const int SUBTITLE_Y_POS = 400;

SubtitleText::SubtitleText() : mCurTask(-1), mCurTaskExpireTime(0) {}

void SubtitleText::Init(std::string_view filename) {
    tinyxml2::XMLDocument xml;
    if(xml.LoadFile((std::filesystem::path("res/subtitle/") / filename).string().c_str()) != tinyxml2::XML_SUCCESS) [[unlikely]]
        CriticalError("subtitle \"", filename, "\" not found");
    const tinyxml2::XMLNode* root = xml.FirstChildElement("subtitle");
    if(!root) [[unlikely]]
        CriticalError("cannot find root tag \"subtitle\" in file \"", filename, '"');
    for (const tinyxml2::XMLNode* node = root->FirstChild(); node; node = node->NextSiblingElement()) {
        const tinyxml2::XMLElement* sentence = node->ToElement();
        if(sentence && sentence->Value() == "sentence"sv) {
            int time = 0;
            if(sentence->QueryIntAttribute("time", &time) == tinyxml2::XML_SUCCESS) {
                const char* text = sentence->Attribute("text");
                mTasks.push_back(std::make_pair(time, text ? text : ""));
            }
        }
    }
}

void SubtitleText::Run() {
    if(--mCurTaskExpireTime < 0) {
        if(++mCurTask >= static_cast<int>(mTasks.size()))
            Storage().KillObject(this);
        else {
            mCurTaskExpireTime = mTasks[mCurTask].first;
            mText = Storage().CreateObject<Text>(
                mTasks[mCurTask].second,
                Vector2f(Window::Instance().getSize().x / 2, SUBTITLE_Y_POS),
                Font::GetFont("big.xml"), Text::haCenter, Text::vaCenter);
        }
    }
}

} // namespace objects
} // namespace oci
