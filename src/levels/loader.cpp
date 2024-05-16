/*
 * loader.cpp
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

#include "loader.h"

#include <set>
#include <string>

namespace oci {
namespace levels {

namespace {
    void FatalError(const char* description) {
        fprintf(stderr, "Error while loading \"description.xml\": %s\n",
                description);
        exit(1);
    }

    void LevelLoadFatalError(const std::string& levelname,
                             const char* description) {
        fprintf(stderr, "Error while loading level \"%s\": %s\n",
                levelname.c_str(), description);
        exit(1);
    }
} // namespace

Loader::Loader() {
    if(!xml_file.LoadFile("res/description.xml"))
        FatalError(xml_file.ErrorDesc());
    TiXmlNode* node = xml_file.FirstChild("ResourcesList");
    if(!node)
        FatalError("cannot find node \"ResourceList\"");
    ResourcesList = node->ToElement();
    if(!ResourcesList)
        FatalError("node \"ResourceList\" is not xml element");
}

void Loader::LoadLevel(const std::string& levelname) {
    assert(ResourcesList);
    TiXmlNode* level_node = ResourcesList->FirstChild(levelname);
    if(!level_node)
        LevelLoadFatalError(levelname, "cannot find level element");
    TiXmlElement* level = level_node->ToElement();
    if(!level)
        LevelLoadFatalError(levelname, "level node is not xml element");

    std::set<std::string> sprites_list, sounds_list;

    {
        TiXmlNode* node = level->FirstChild("sprite");
        if(node) {
            TiXmlNode* child = NULL;
            while((child = node->IterateChildren(child)) != NULL)
                sprites_list.insert(child->ToElement()->Attribute("name"));
        }
    }
    {
        TiXmlNode* node = level->FirstChild("sound");
        if(node) {
            TiXmlNode* child = NULL;
            while((child = node->IterateChildren(child)) != NULL)
                sounds_list.insert(child->ToElement()->Attribute("name"));
        }
    }

    int max_progr = sprites_list.size() + sounds_list.size();
    if(max_progr) {
//            CLoadScreen lscreen;
//            lscreen.InitMax(sprites_list.size() + sounds_list.size());
//            int progr = 0;

        for(std::set<std::string>::iterator pos = sprites_list.begin(); pos != sprites_list.end(); ++pos)
        {
//                spriteContainer.Load(*pos);
//                lscreen.SetProgress(++progr);
            }

            for(std::set<std::string>::iterator pos = sounds_list.begin(); pos != sounds_list.end(); ++pos)
            {
//                soundsContainer.Load(*pos);
//                lscreen.SetProgress(++progr);
            }
        }
}

} // namespace levels
} // namespace oci
