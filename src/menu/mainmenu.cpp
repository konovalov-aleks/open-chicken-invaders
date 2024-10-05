/*
 * mainmenu.cpp
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

#include "mainmenu.h"

#include "background_controller.h"
#include "cursor.h"
#include <context/manager.h>
#include <context/object_holder.h>
#include <core/mouse.h>
#include <core/window.h>
#include <font/font.h>
#include <levels/manager.h>
#include <objects/base/sprite.h>
#include <objects/effects/unshade_screen.h>
#include <objects/text/text.h>

#include <cstdlib>
#include <functional>
#include <memory>
#include <string>

namespace oci {

using namespace objects;

namespace {

    const char MENU_CONTEXT_NAME[] = "menu";

    class Button : public Sprite, public Active {
    public:

        enum State {
            sNormal,
            sChecked,
            sHover,
            sCheckedHover
        };

        Button() : mPressed(false) {}

        void Init(const std::string& caption, int ypos,
                  const std::function<void()>& on_press_callback) {
            Sprite::Init("menuitem.xml");
            setPosition(Window::Instance().getSize().x / 2, ypos);
            mCaption = Storage().CreateObject<Text>(
                caption, getPosition(), Font::GetFont("medium.xml"),
                Text::haCenter, Text::vaCenter);
            mOnPressCallback = on_press_callback;
        }

        DrawPriority GetDrawPriority() const override {
            return dpControlBackground;
        }

        void Run() override {
            const Vector2i mouse_pos = Mouse::getPosition(Window::Instance());
            bool mouse_in_button = getGlobalBounds().contains(Vector2f(mouse_pos));
            SetState(mouse_in_button ? sHover : sNormal);
            if(mouse_in_button && mPressed && !Mouse::isButtonPressed(Mouse::Left))
                mOnPressCallback();
            mPressed = mouse_in_button && Mouse::isButtonPressed(Mouse::Left);
        }

    private:
        ObjectHolder<Text> mCaption;
        bool mPressed;
        std::function<void()> mOnPressCallback;
    };

    class Logo : public CommonSprite<Visible::dpForeground> {
    public:
        void Init() {
            Sprite::Init("logo.xml");
            setPosition(Vector2f(Window::Instance().getSize().x / 2, 76));
        }
    };

    inline void doexit() {
        Window::Instance().close();
        std::exit(EXIT_SUCCESS);
    }
} // namespace

void MainMenu::Init() {
    Background::Instance().SetController(
        Storage().CreateObject<menu::BackgroundController>());

    Storage().CreateObject<Logo>();
    Storage().CreateObject<Button>("save the world", 260,
            std::bind(&levels::Manager::NewGame, &levels::Manager::Instance()));
    Storage().CreateObject<Button>("quit", 300, doexit);

    Storage().CreateObject<Text>(
        "main menu",
        Vector2f(Window::Instance().getSize().x / 2, 210),
        Font::GetFont("big.xml"), Text::haCenter, Text::vaCenter);

    Storage().CreateObject<UnshadeScreen>();
    Storage().CreateObject<menu::Cursor>();
}

void MainMenu::Run() {
}

void MainMenu::SwitchToMenu() {
    context::Manager::Instance().SetActiveContext(
        context::Manager::Instance().GetContext(MENU_CONTEXT_NAME));
}

void MainMenu::InitMenu() {
    std::shared_ptr<context::Context> context =
        context::Manager::Instance().GetContext(MENU_CONTEXT_NAME);
    context->GetStorage(context::storage::LOCAL).CreateObject<MainMenu>();
    context::Manager::Instance().SetActiveContext(context);
}

} // namespace oci
