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

#include "context/context.h"
#include "context/manager.h"
#include "levels/manager.h"
#include "menu/mainmenu.h"
#include <background/background.h>
#include <core/event.h>
#include <core/video_mode.h>
#include <core/window.h>
#include <core/window_style.h>
#include <diagnostics/benchmark/benchmark.h>
#include <diagnostics/fps.h>

#include <cstring>

int main(int argc, char* argv[]) {
    using namespace oci;

    VideoMode vm(640, 480);
    Window::Instance().create(vm, "Chicken invaders", Style::Close);
    if(argc == 2 && !std::strcmp(argv[1], "-benchmark"))
        benchmark::StartBenchmark();
    else {
        Window::Instance().setFramerateLimit(25);
        MainMenu::InitMenu();
    }
    while(Window::Instance().isOpen()) {
        Event event;
        while(Window::Instance().pollEvent(event)) {
            if (event.type == Event::Closed)
                Window::Instance().close();
        }
        Window::Instance().clear();
        std::shared_ptr<context::Context> context =
            context::Manager::Instance().GetActiveContext();
        Background::Instance().Draw();
        context->Run();
        context->ProcessCollisions();
        context->Animate();
        context->Draw();
        Window::Instance().display();
    }
}
