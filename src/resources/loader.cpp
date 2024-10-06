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

#include <core/critical_error.h>
#include <utils/transparent_equal.h>
#include <utils/transparent_string_hash.h>

// IWYU pragma: no_include <__fwd/fstream.h>
// IWYU pragma: no_include <__fwd/ios.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace oci {
namespace resources {

    ResourceLoader ResourceLoader::mInstance;

    class ResourceLoader::Impl {
    public:
        virtual ~Impl() {}

        Impl() {
            mFile.open("ChickenInvaders2.dat", std::ios_base::in | std::ios_base::binary);
            if(!mFile) {
                std::fputs("FATAL ERROR: Could not open resource file \"ChickenInvaders2.dat\"\n", stderr);
                std::exit(1);
            }
            if(!ReadTOC()) {
                std::fputs("FATAL ERROR: file \"ChickenInvaders2.dat\" corrupted\n", stderr);
                std::exit(1);
            }
        }

        inline std::vector<char> GetData(std::string_view resource_name) {
            TOCType::const_iterator iter = mTOC.find(resource_name);
            if(iter == mTOC.end()) [[unlikely]]
                CriticalError("Cannot find file \"", resource_name, '"');
            std::vector<char> v;
            if(iter->second.second > 0) {
                v.resize(iter->second.second);
                mFile.seekg(iter->second.first);
                mFile.read(&v[0], iter->second.second);
            }
            return v;
        }

    private:

        bool ReadTOC() {
            static const char TRANSLATION_TABLE[] =
                    "###############################################################"
                    "###############################################################"
                    "##L##O#I##DEF###BC###_X#######P##Slmnohijkdefg#abc####xyz#tuvwp"
                    "#rs############\0####################-./################89##456"
                    "70123";
            mFile.seekg(0, std::ios::end);
            const int file_size = static_cast<int>(mFile.tellg());
            mFile.seekg(0, std::ios::beg);
            int items_count = 0;
            if(!mFile.read(reinterpret_cast<char*>(&items_count), 4))
                return false;
            for(int i = 0; i < items_count; ++i) {
                char buffer[80];
                int pos = 0, len = 0;
                if(!mFile.read(buffer, sizeof(buffer)) ||
                   !mFile.read(reinterpret_cast<char*>(&pos), 4) ||
                   !mFile.read(reinterpret_cast<char*>(&len), 4) ||
                   len < 0 || pos < 0 || len + pos > file_size)
                    return false;
                for(std::size_t i = 0; i < sizeof(buffer); ++i)
                    buffer[i] = TRANSLATION_TABLE[static_cast<unsigned char>(buffer[i])];
                buffer[sizeof(buffer) - 1] = '\0';
                mTOC.insert(std::make_pair(buffer, std::make_pair(pos, len)));
            }
            return true;
        }

        std::ifstream mFile;
        using TOCType = std::unordered_map<
            std::string, std::pair<int, int>,
            TransparentStrHash, TransparentEqual
        >;
        TOCType mTOC;
    };

    ResourceLoader::ResourceLoader() : mImpl(new Impl) {
    }

    std::vector<char> ResourceLoader::GetData(std::string_view resource_name) {
        return mImpl->GetData(resource_name);
    }

} // namespace resources
} // namespace oci
