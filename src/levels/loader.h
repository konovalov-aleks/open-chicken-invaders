/*
 * loader.h
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

#include <tinyxml2.h>

#include <string>

namespace oci {
namespace levels {

/// Класс, умеющий загружать ресурсы для уровня
class Loader
{
protected:
    Loader();

private:
    /// Загружает ресурсы уровня с именем levelname
    void LoadLevel(const std::string& levelname);

    tinyxml2::XMLDocument xml_file;
    tinyxml2::XMLElement* ResourcesList;

    friend class Level;
};

} // namespace levels
} // namespace oci
