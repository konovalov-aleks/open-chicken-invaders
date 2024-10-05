/*
 * window.h
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
#   include <SFML/Graphics/RenderWindow.hpp>
#else
#   include "drawable.h"
#   include "event.h"
#   include "video_mode.h"

#   include <SDL.h>

#   include <memory>
#   include <string>
#endif

namespace oci {

#ifdef USE_SFML

typedef sf::RenderWindow WindowImpl;

#else

class WindowImpl {
public:
    void create(VideoMode mode, const std::string& title, unsigned int style);
    void close();
    void display();
    void draw(const Drawable& obj);
    void clear();

    void setMouseCursorVisible(bool);

    bool pollEvent(Event& event_received);

    bool isOpen() const;

    Vector2u getSize() const noexcept { return mSize; }

    unsigned int GetRealWidth() const;
    unsigned int GetRealHeight() const;

    SDL_Window* SDLHandle() const noexcept { return mWindow.get(); }
    SDL_Renderer* GetRenderer() { return mRenderer.get(); }

    void setFramerateLimit(unsigned int limit);

private:
    struct WindowDeleter {
        void operator() (SDL_Window* window) const {
            if(window)
                SDL_DestroyWindow(window);
        }
    };

    struct RendererDeleter {
        void operator() (SDL_Renderer* renderer) const {
            if(renderer)
                SDL_DestroyRenderer(renderer);
        }
    };

    struct CursorDeleter {
        void operator() (SDL_Cursor* cursor) const {
            if(cursor)
                SDL_FreeCursor(cursor);
        }
    };

    struct SDLInit {
        SDLInit();
        ~SDLInit();
    };

    SDLInit mSdlInit;

    std::unique_ptr<SDL_Window, WindowDeleter> mWindow;
    std::unique_ptr<SDL_Renderer, RendererDeleter> mRenderer;
    Uint32 mLastFrameTimeMs = 0;
    Vector2u mSize = {};
    Uint32 mFrameMinTimeMs = 0;
};

#endif

class Window : public WindowImpl {
public:
    static Window& Instance() { return mInstance; }

    Window() = default;

    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;

private:
    static Window mInstance;
};

} // namespace oci
