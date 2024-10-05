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
#include <core/critical_error.h>
#include <core/image.h>
#include <core/texture.h>
#include <resources/loader.h>
#include <utils/cache.h>

#include <tinyxml2.h>

#include <cstring>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

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

const Texture& Font::operator[] (char s) const {
    static const Texture empty;
    std::unordered_map<char, Texture>::const_iterator res = mTextures.find(s);
    return res == mTextures.end() ? empty : res->second;
}

void Font::Load(const std::string& name) {
    tinyxml2::XMLDocument xml;
    if(xml.LoadFile(("res/fonts/" + name).c_str()) != tinyxml2::XML_SUCCESS) [[unlikely]]
        CriticalError("font \"", name, "\" not found");
    const tinyxml2::XMLNode* root = xml.FirstChildElement("font");
    if(!root) [[unlikely]]
        CriticalError("cannot find root tag \"font\" in file \"", name, "\"");
    for (const tinyxml2::XMLNode* node = root->FirstChildElement(); node; node = node->NextSiblingElement()) {
        const tinyxml2::XMLElement* glyph = node->ToElement();
        if(glyph && glyph->Value() == "glyph"sv) {
            const char* symbol = glyph->Attribute("symbol");
            if(!symbol || std::strlen(symbol) != 1) [[unlikely]]
                CriticalError("Invalid value for attribute \"symbol\" in tag \"glyph\"");
            const char* name = glyph->Attribute("name");
            if(!name) [[unlikely]]
                CriticalError("Invalid value for attribute \"name\" in tag \"glyph\"");
            LoadGlyph(*symbol, name);
        }
    }
    if(mTextures.find(' ') == mTextures.end()) {
        int sum_width = 0;
        for(std::unordered_map<char, Texture>::const_iterator iter = mTextures.begin();
           iter != mTextures.end(); ++iter)
            sum_width += iter->second.getSize().x;
        Image img;
        img.create(sum_width / mTextures.size(), 1, Color::Black);
        img.createMaskFromColor(Color::Black);
        Texture tex;
        if(!tex.loadFromImage(img)) [[unlikely]]
            CriticalError("Unable to create a texture");
        mTextures.insert(std::make_pair(' ', std::move(tex)));
    }
}

void Font::LoadGlyph(char s, const std::string& filename) {
    std::vector<char> data = resources::ResourceLoader::Instance().GetData(filename);
    if(data.empty()) [[unlikely]]
        CriticalError("Image resource \"", filename, "\" is empty");
    const auto [iter, inserted] = mTextures.insert(std::make_pair(s, Texture()));
    if(inserted) {
        Image img;
        if(!img.loadFromMemory(&data[0], data.size())) [[unlikely]]
            CriticalError("Cannot load image \"", filename, '"');
        img.createMaskFromColor(Color::Black);
        if(!iter->second.loadFromImage(img)) [[unlikely]]
            CriticalError("Unable to create a texture");
    }
}

} // namespace oci
