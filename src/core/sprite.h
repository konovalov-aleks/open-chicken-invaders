/*
 * sprite.h
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

#ifdef USE_SFML
#   include<SFML/Graphics/Sprite.hpp>
#else
#   include "drawable.h"
#   include "rect.h"

#   include <SDL_render.h>

#   include <cassert>
#endif

#ifdef USE_SFML

namespace oci::core {
    using sf::Sprite;
} // namespace oci::core

#else

namespace oci {
    class Texture;
} // namespace oci

namespace oci::core {

class Sprite : public Drawable {
public:
    Sprite() = default;
    Sprite(const Texture& tex) {
        setTexture(tex, true);
    }

    const Texture* getTexture() const noexcept { return mTexture; }
    void setTexture(const Texture& texture, bool resetRect = false)
    {
        assert(resetRect); // resetRect == true is not used in the game and not implemented
        (void)resetRect;
        mTexture = &texture;
    }

    FloatRect getGlobalBounds() const noexcept;

    void DoDraw(SDL_Renderer* renderer) const override;

private:
    const Texture* mTexture = nullptr;
};

} // namespace oci::core

#endif
