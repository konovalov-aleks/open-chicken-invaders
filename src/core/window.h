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

#include <boost/noncopyable.hpp>

#ifdef USE_SFML
#   include <SFML/Graphics/RenderWindow.hpp>
#else
#   include "drawable.h"
#   include "event.h"
#   include "input.h"
#   include <portability/chrono.h>
#   include <portability/memory.h>
#   include <SDL2/SDL.h>
#   include <string>
#   include "video_mode.h"
#endif

namespace oci {

#ifdef USE_SFML

typedef sf::RenderWindow WindowImpl;

#else

class WindowImpl {
public:
    WindowImpl();
    ~WindowImpl();

    void Create(VideoMode mode, const std::string& title, unsigned int style);
    void Close();
    void Display();
    void Draw(const Drawable& obj);
    void Clear();

    void ShowMouseCursor(bool show);
    void SetCursorPosition(unsigned int x, unsigned int y);

    bool GetEvent(Event& event_received);

    bool IsOpened() const;

    unsigned int GetWidth() const { return mSize.x; }
    unsigned int GetHeight() const { return mSize.y; }

    unsigned int GetRealWidth() const;
    unsigned int GetRealHeight() const;

    const Input& GetInput() const { return mInput; }

    SDL_Renderer* GetRenderer() { return mRenderer.get(); }

    void SetFramerateLimit(unsigned int limit);

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

    Input mInput;
    unique_ptr<SDL_Window, WindowDeleter> mWindow;
    unique_ptr<SDL_Renderer, RendererDeleter> mRenderer;
    CHRONO::system_clock::time_point mLastFrameTime;
    Vector2<unsigned int> mSize;
    int mFrameMinTime;
};

#endif

class Window : public WindowImpl, boost::noncopyable {
public:
    static Window& Instance() { return mInstance; }

private:
    static Window mInstance;
};

} // namespace oci
