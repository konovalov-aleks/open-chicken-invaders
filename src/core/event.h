/*
 * event.h
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
#   include <SFML/Window/Event.hpp>
#else
#   include <SDL2/SDL.h>
#endif

namespace oci {

#ifdef USE_SFML

namespace Key = sf::Key;
namespace Mouse = sf::Mouse;
using sf::Event;

#else

class Event {
public:
    enum EventType {
        Closed = SDL_QUIT
    };

    EventType Type;
};

namespace Key {
    enum Code {
        Left   = SDL_SCANCODE_LEFT,
        Right  = SDL_SCANCODE_RIGHT,
        Return = SDL_SCANCODE_RETURN,
        Escape = SDL_SCANCODE_ESCAPE,
    };
};

namespace Mouse {
    enum Button {
        Left   = SDL_BUTTON_LMASK,
        Right  = SDL_BUTTON_RMASK,
        Middle = SDL_BUTTON_MMASK
    };
};

#endif

} // namespace oci
