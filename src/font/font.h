/*
 * font.h
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

#include <boost/noncopyable.hpp>
#include <core/image.h>
#include <portability/unordered_map.h>
#include <string>

namespace oci {

class Font {
public:
    static const Font& GetFont(const std::string& name);

    const Image& operator[] (char s) const;

private:
    Font(const std::string& name);

    void Load(const std::string& name);
    void LoadGlyph(char s, const std::string& filename);

    unordered_map<char, Image> mImages;

    friend class FontLoader;
};

} // namespace oci
