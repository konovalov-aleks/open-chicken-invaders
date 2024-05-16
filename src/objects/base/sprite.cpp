/*
 * sprite.cpp
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

#include "sprite.h"

#include <assert.h>
#include <boost/lexical_cast.hpp>
#include <core/color.h>
#include <core/rect.h>
#include <core/window.h>
#include <fstream>
#include <resources/loader.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <tinyxml.h>
#include <utils/cache.h>

namespace oci {
namespace objects {

namespace {
    /// Тип отрисовки изображения спрайта
    enum BitmapDrawingType
    {
        bdtCOPY         = 0, ///< простое копирование пикселей
        bdtCOLORKEY     = 1  ///< копирование с цветовым ключем (отрисовываются все пиксели, чей цвет отличен от ключа)
    };

    inline Color ColorFrom565(unsigned short color) {
        return Color((color >> 11) & 0x1f,
                     (color >> 5) & 0x3f,
                     color & 0x3f);
    }

#ifdef USE_SFML
    struct OldSpriteLoader {
        Sprite::SpriteData operator()(const std::string& name) {
            Sprite::SpriteData data;

            printf("Loading sprite \"%s\"\n", name.c_str());
            std::ifstream f(std::string("res/images/") + name, std::ios_base::binary);
            if(!f)
                throw std::logic_error("file \"" + name + "\" not found");
            std::string image_name;
            char c[2];
            while(f.read(c, 2) && c[0])
                image_name += c[0];
            puts(image_name.c_str());
            Image img;
            if(!img.LoadFromFile("res/images/" + image_name))
                throw std::logic_error("cannot load image \"" + image_name + "\"");
            img.SetSmooth(false);

            unsigned char draw_type;
            f.read((char*)&draw_type, 1);
            #ifdef DEBUG_SPRITE
            printf("Draw type: %hhd\n", draw_type);
            #endif
            switch(draw_type) {
                case bdtCOPY:
                    break;
                case bdtCOLORKEY: {
                    unsigned short color_key = 0;
                    f.read((char*)&color_key, sizeof(color_key));
                    img.CreateMaskFromColor(ColorFrom565(color_key));
                    break;
                }
                default:
                    assert(false); // неизвестный тип отрисовки
            }

            char statescount = 0;
            f.read(&statescount, 1);

            #ifdef DEBUG_SPRITE
            printf("States count: %hhd\n", statescount);
            #endif
            int top = 0;
            for(int i = 0; i < statescount; ++i) {
                int left = 0, max_height = 0;
                char framescount = 0;
                f.read(&framescount, 1);
                #ifdef DEBUG_SPRITE
                printf("\tFrames count: %hhd\n", framescount);
                #endif
                Sprite::Animation anim;
                char mode;
                f.read(&mode, 1);
                switch(mode) {
                    case 0:
                        anim.info.need_reverce = false;
                        anim.info.need_repeat = true;
                        break;
                    case 1:
                        anim.info.need_reverce = true;
                        anim.info.need_repeat = true;
                        break;
                    case 2:
                        anim.info.need_reverce = false;
                        anim.info.need_repeat = false;
                        break;
                    default:
                        throw std::logic_error("unknown animation mode (" +
                                boost::lexical_cast<std::string>(mode) + ")");
                };
                #ifdef DEBUG_SPRITE
                printf("\tAnimation mode: %hhd\n", anim.mode);
                #endif
                for(int k = 0; k < framescount;) {
                    char n = 0;
                    f.read(&n, 1);

                    short width = 0, height = 0;
                    f.read((char*)&width, 2);
                    f.read((char*)&height, 2);
                    #ifdef DEBUG_SPRITE
                    printf("\tFrame[%d x %hhd]: width = %hd, height = %hd\n", k, n, width, height);
                    #endif
                    for(int t = 0; t < n; ++t) {
                        anim.images.push_back(Image(width, height));
                        anim.images.back().Copy(img, 0, 0, IntRect(left, top, left + width, top + height));
                        #ifdef DEBUG_SPRITE
                        printf("\t\t[%d, %d, %d, %d]\n", left, top, left + width, top + height);
                        #endif
                        left += width;
                        if(height > max_height)
                            max_height = height;
                    }
                    k += n;
                }
                data.push_back(anim);
                top += max_height;
            }
            return data;
        }
    };
#endif

    class SpriteLoader {
    private:
        inline bool GetBoolValue(const TiXmlElement& e, const char* name, bool def = false) const {
            int value = 0;
            return e.QueryIntAttribute(name, &value) == TIXML_SUCCESS ? value != 0 : def;
        }

        Sprite::Animation LoadAnimation(const TiXmlElement& animation) const {
            Sprite::Animation anim;
            anim.info.need_reverce = GetBoolValue(animation, "reverce");
            anim.info.need_repeat = GetBoolValue(animation, "repeat");
            const TiXmlNode* node = NULL;
            while((node = animation.IterateChildren(node)) != NULL) {
                const TiXmlElement* frame = node->ToElement();
                if(frame && frame->ValueStr() == "frame") {
                    const char* filename = frame->Attribute("name");
                    if(filename) {
                        std::vector<char> data = resources::ResourcesLoader::Instance().GetData(filename);
                        if(data.empty())
                            throw std::logic_error("Image resource \"" + std::string(filename) + "\" is empty");
                        anim.images.push_back(Image());
                        if(!anim.images.back().LoadFromMemory(&data[0], data.size()))
                            throw std::logic_error("Could not open image resource \"" + std::string(filename) + "\"");
                        anim.images.back().SetSmooth(false);
                        const char* colorkey = frame->Attribute("colorkey");
                        if(colorkey) {
                            int ck = atoi(colorkey);
                            anim.images.back().CreateMaskFromColor(Color(ck >> 16 & 0xFF,
                                                                         ck >> 8  & 0xFF,
                                                                         ck & 0xFF));
                        }
                    }
                }
            }
            return anim;
        }

        Sprite::SpriteData LoadXml(const TiXmlNode& root) const {
            Sprite::SpriteData result;
            const TiXmlNode* node = NULL;
            while((node = root.IterateChildren(node)) != NULL) {
                const TiXmlElement* animation = node->ToElement();
                if(animation && animation->ValueStr() == "animation")
                    result.push_back(LoadAnimation(*animation));
            }
            return result;
        }

    public:
        Sprite::SpriteData operator()(const std::string& name) {
            TiXmlDocument xml;
            if(!xml.LoadFile("res/sprites/" + name))
                throw std::logic_error("sprite \"" + name + "\" not found");
            const TiXmlNode* root = xml.FirstChild("sprite");
            if(!root)
                throw std::logic_error("cannot find root tag \"sprite\" in file \"" + name + "\"");
            return LoadXml(*root);
        }
    };

} // namespace

void Sprite::Init(const std::string& filename, const Vector2f& pos) {
    Init(filename);
    #ifdef DEBUG_SPRITE
    printf("[%x] Sprite(\"%s\", %f, %f)\n", (unsigned int)this,
           filename.c_str(), pos.x, pos.y);
    #endif
    SetPosition(pos);
}

void Sprite::Init(const std::string& filename) {
    mCurrentState = mCurrentFrame = 0;
    #ifdef DEBUG_SPRITE
    printf("[%x] Sprite (\"%s\")\n", (unsigned int)this, filename);
    #endif
#ifdef USE_SFML
    if(strstr(filename.c_str(), ".sprite")) { 
        static Cache<SpriteData, OldSpriteLoader> old_cache;
        mData = &old_cache.Get(filename);
        printf("Old loader: loaded \"%s\"\n", filename.c_str());
    } else
#endif
    {
        static Cache<SpriteData, SpriteLoader> cache;
        mData = &cache.Get(filename);
    }
    // FIXME проверка, что есть кадры и изображения в кадрах
    assert(mData);
    assert(!mData->empty());
    SetImage(mData->front().images.front());
    bool res = SetState(0);
    assert(res);
}

Sprite::~Sprite() {
    #ifdef DEBUG_SPRITE
    printf("[%x] ~Sprite\n", (unsigned int)this);
    #endif
}

size_t Sprite::StatesCount() const {
    return mData ? mData->size() : 0;
}

size_t Sprite::CurrentState() const {
    return mCurrentState;
}

bool Sprite::SetState(size_t state) {
    assert(state < StatesCount());
    if(state >= StatesCount())
        return false;
    mCurrentState = state;
    return SetFrame(0);
}

size_t Sprite::FramesCount() const {
    return mData ? (*mData)[mCurrentState].images.size() : 0;
}

size_t Sprite::CurrentFrame() const {
    return mCurrentFrame;
}

bool Sprite::SetFrame(size_t frame) {
    if(frame >= FramesCount())
        throw std::logic_error(
           "Frames count = " + boost::lexical_cast<std::string>(FramesCount()) +
           ", but selected frame " + boost::lexical_cast<std::string>(frame));
    mCurrentFrame = frame;
    assert(mData);
    const Image& img = (*mData)[mCurrentState].images[mCurrentFrame];
    SetImage(img);
    SetSubRect(IntRect(0, 0, img.GetWidth(), img.GetHeight()));
    SetCenter(img.GetWidth() / 2, img.GetHeight() / 2);
    return true;
}

int Sprite::GetWidth() const {
    return mData ? (*mData)[mCurrentState].images[mCurrentFrame].GetWidth() : 0;
}

int Sprite::GetHeight() const {
    return mData ? (*mData)[mCurrentState].images[mCurrentFrame].GetHeight() : 0;
}

void Sprite::Draw() {
    Window::Instance().Draw(*this);
}

} // namespace objects
} // namespace oci
