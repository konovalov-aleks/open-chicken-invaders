/*
 * main.cpp
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

#include <background/background.h>
#include "context/context.h"
#include "context/manager.h"
#include <core/event.h>
#include <core/video_mode.h>
#include <core/window.h>
#include <core/window_style.h>
#include <diagnostics/benchmark/benchmark.h>
#include <diagnostics/fps.h>
#include <exception>
#include "levels/manager.h"
#include "menu/mainmenu.h"
#include <portability/unordered_map.h>
#include <stdio.h>

using namespace oci;

namespace {

    void onclose(const Event& /*event*/) {
        Window::Instance().Close();
    }

    typedef unordered_map<int, void (*)(const Event&)> EventsMap;
    static EventsMap event_handlers;

    void fill_handlers() {
        event_handlers.insert(std::make_pair(Event::Closed, onclose));
    };

    void unexpected_handler() {
        fputs("Unexpected handler called", stderr);
        exit(EXIT_FAILURE);
    }

    void terminate_handler() {
        fputs("Terminate handler called", stderr);
        exit(EXIT_FAILURE);
    }

} // namespace

int main(int argc, char* argv[]) {
    std::set_unexpected(unexpected_handler);
    std::set_terminate(terminate_handler);
    fill_handlers();

    using namespace oci;

    try {
        VideoMode vm(640, 480);
        Window::Instance().Create(vm, "Chicken invaders", Style::Close);
        if(argc == 2 && !strcmp(argv[1], "-benchmark"))
            benchmark::StartBenchmark();
        else {
            Window::Instance().SetFramerateLimit(25);
            MainMenu::InitMenu();
        }
        while(Window::Instance().IsOpened()) {
            Event event;
            while(Window::Instance().GetEvent(event)) {
                EventsMap::const_iterator it = event_handlers.find(event.Type);
                if(it != event_handlers.end())
                    it->second(event);
            }
            Window::Instance().Clear();
            shared_ptr<context::Context> context =
                context::Manager::Instance().GetActiveContext();
            Background::Instance().Draw();
            context->Run();
            context->ProcessCollisions();
            context->Animate();
            context->Draw();
            Window::Instance().Display();
        }
    } catch(const std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    } catch(...) {
        fputs("Unknown error :(\n", stderr);
    }
}
