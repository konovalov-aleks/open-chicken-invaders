/*
 * font.cpp
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

#include "font.h"

#include <core/color.h>
#include <resources/loader.h>
#include <stdexcept>
#include <string_view>
#include <tinyxml2.h>
#include <utils/cache.h>

namespace oci {

using namespace std::literals;

struct FontLoader {
    Font operator()(const std::string& name) {
        return Font(name);
    }
};

const Font& Font::GetFont(const std::string& name) {
    static Cache<Font, FontLoader> cache;
    return cache.Get(name);
}

Font::Font(const std::string& name) {
    Load(name);
}

const Image& Font::operator[] (char s) const {
    static const Image empty;
    unordered_map<char, Image>::const_iterator res = mImages.find(s);
    return res == mImages.end() ? empty : res->second;
}

void Font::Load(const std::string& name) {
    tinyxml2::XMLDocument xml;
    if(xml.LoadFile(("res/fonts/" + name).c_str()) != tinyxml2::XML_SUCCESS) [[unlikely]]
        throw std::logic_error("font \"" + name + "\" not found");
    const tinyxml2::XMLNode* root = xml.FirstChildElement("font");
    if(!root)
        throw std::logic_error("cannot find root tag \"font\" in file \"" + name + "\"");
    for (const tinyxml2::XMLNode* node = root->FirstChildElement(); node; node = node->NextSiblingElement()) {
        const tinyxml2::XMLElement* glyph = node->ToElement();
        if(glyph && glyph->Value() == "glyph"sv) {
            const char* symbol = glyph->Attribute("symbol");
            if(!symbol || strlen(symbol) != 1)
                throw std::logic_error("Invalid value for attribute \"symbol\" in tag \"glyph\"");
            const char* name = glyph->Attribute("name");
            if(!name)
                throw std::logic_error("Invalid value for attribute \"name\" in tag \"glyph\"");
            LoadGlyph(*symbol, name);
        }
    }
    if(mImages.find(' ') == mImages.end()) {
        int sum_width = 0;
        for(unordered_map<char, Image>::const_iterator iter = mImages.begin();
           iter != mImages.end(); ++iter)
            sum_width += iter->second.GetWidth();
        Image img(sum_width / mImages.size(), 1, Color::Black);
        img.CreateMaskFromColor(Color::Black);
        mImages.insert(std::make_pair(' ', img));
    }
}

void Font::LoadGlyph(char s, const std::string& filename) {
    std::vector<char> data = resources::ResourcesLoader::Instance().GetData(filename);
    if(data.empty())
        throw std::logic_error("Image resource \"" + filename + "\" is empty");
    std::pair<unordered_map<char, Image>::iterator, bool> res =
        mImages.insert(std::make_pair(s, Image()));
    if(res.second) {
        Image& img = res.first->second;
        if(!img.LoadFromMemory(&data[0], data.size()))
            throw std::logic_error("Cannot load image \"" + filename + "\"");
        img.SetSmooth(false);
        img.CreateMaskFromColor(Color::Black);
    }
}

} // namespace oci
