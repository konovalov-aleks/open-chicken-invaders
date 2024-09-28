/*
 * window.cpp
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

#include "window.h"

namespace oci {
    Window Window::mInstance;
} // namespace oci

#ifndef USE_SFML

#include <stdexcept>
#include <thread>

namespace oci {

WindowImpl::WindowImpl() : mFrameMinTime(0) {
    SDL_Init(SDL_INIT_EVERYTHING);
}

WindowImpl::~WindowImpl() {
    SDL_Quit();
}

void WindowImpl::Create(VideoMode mode, const std::string& title, unsigned int) {
    mSize.x = mode.Width();
    mSize.y = mode.Height();
    mWindow.reset(SDL_CreateWindow(title.c_str(), 0, 0,
                                   mode.Width(), mode.Height(),
                                   SDL_WINDOW_SHOWN));
    if(!mWindow)
        throw std::runtime_error("Cannot create window");

    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED));
    if(!mRenderer)
        throw std::runtime_error("Cannot create renderer");
}

void WindowImpl::Close() {
    mRenderer.reset();
    mWindow.reset();
}

void WindowImpl::Display() {
    SDL_RenderPresent(mRenderer.get());
#ifndef ANDROID
    if(mFrameMinTime > 0) {
        CHRONO::system_clock::time_point current_time = CHRONO::system_clock::now();
        CHRONO::nanoseconds delta = current_time - mLastFrameTime;
        if(delta < CHRONO::microseconds(mFrameMinTime)) {
            std::this_thread::sleep_for(
                CHRONO::microseconds(mFrameMinTime) - delta);
        }
        mLastFrameTime = current_time;
    }
#endif
}

void WindowImpl::Draw(const Drawable& obj) {
    obj.DoDraw(mRenderer.get());
}

void WindowImpl::Clear() {
    SDL_RenderClear(mRenderer.get());
}

void WindowImpl::ShowMouseCursor(bool show) {
    SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

void WindowImpl::SetCursorPosition(unsigned int x, unsigned int y) {
#ifdef ANDROID
    const float scale = static_cast<float>(GetRealHeight() / GetHeight());
#else
    const float scale = 1.0f;
#endif
    SDL_WarpMouseInWindow(mWindow.get(), x * scale, y * scale);
}

bool WindowImpl::GetEvent(Event& event_received) {
    SDL_Event event;
    if(SDL_PollEvent(&event)) {
        event_received.Type = static_cast<Event::EventType>(event.type);
        return true;
    }
    return false;
}

bool WindowImpl::IsOpened() const {
    return mWindow.get() != NULL;
}

namespace {
    inline Vector2i GetWindowSize(SDL_Window* wnd) {
        Vector2i result;
        SDL_GetWindowSize(wnd, &result.x, &result.y);
        return result;
    }
}

unsigned int WindowImpl::GetRealWidth() const {
    return GetWindowSize(mWindow.get()).x;
}

unsigned int WindowImpl::GetRealHeight() const {
    return GetWindowSize(mWindow.get()).y;
}

void WindowImpl::SetFramerateLimit(unsigned int limit) {
    mFrameMinTime = limit > 0 ? 1000000 / limit : 0;
}

} // namespace oci

#endif
