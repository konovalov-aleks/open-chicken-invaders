/*
 * intro_1.cpp
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

#include <core/window.h>
#include "intro_level.h"
#include "factory.h"
#include <objects/base/animated.h>
#include <objects/base/sprite.h>
#include <objects/characters/ship.h>

#include <math.h>
#include <memory>

namespace oci {
namespace levels {

using namespace objects;

/// Мультик после Плутона
/**     Светофор переключается на красный. Корабль долетает до него и встает.
        Пролетает НЛО. Включается зеленый и корабль улетает.
*/
class Intro_1 : public IntroLevel {
public:
    void Init();

private:
    virtual void RunIntro() override;

    std::shared_ptr<Sprite> mTrafficLights;
    std::shared_ptr<Animated> mUFO;
//    s32 ufosound;            /// çâóê ÍËÎ
    std::shared_ptr<Ship> mShip;              /// êîðàáëü
//    const CSound enginesound;

    int mMode;
};

static Factory::Registrator<Intro_1> reg("intro_1", "intro");

void Intro_1::Init() {
    IntroLevel::Init("intro1");
    mMode = 0;
    mTrafficLights = Storage().CreateObject<CommonSprite<Visible::dpBonus> >(
        "trafficlight.xml", Vector2f(Window::Instance().GetWidth() / 2,
                                     Window::Instance().GetHeight() / 2 - 50));
    mUFO = Storage().CreateObject<CommonAnimated<Visible::dpChicken> >(
        "flyingsaucer.xml", Vector2f(-100, 140));
    mShip = Storage().CreateObject<Ship>(
        Vector2f(Window::Instance().GetWidth() / 2 - 80,
                 Window::Instance().GetHeight() + 20));
    //ufosound(0),
    //enginesound("ufo.wav", true)
//    background->dx = background->dy = 0;
//    trafficlights.frame = 1; // æåëòûé
}

#define INTRO1_UFOSPEED 6

void Intro_1::RunIntro() {
    if(mMode < 20)
        ++mMode; // mode = 0..19 - æåëòûé ñâåò
    else if(mMode == 20) {       // mode = 20 - êðàñíûé ñâåò
        mTrafficLights->SetState(1);
        ++mMode;
    } else if(mMode < 30) {   // êîðàáëü ëåòèò ê ñâåòîôîðó
                       // âñòàåì íà ïîçèöèè (120;140)
        float sp = mShip->GetPosition().x < 340 ?
            -6.0f + (340.0f - mShip->GetPosition().y) / 10.0f : -6.0f;// çàìåäëÿåìñÿ ïðè ïîäëåòå ê ñâåòîôîðó
        mShip->Move(0, sp);
        if(fabsf(mShip->GetPosition().y - 310.0f) < 10.0f) {
            mMode = 30; // âñòàåì - ñåé÷àñ ïîåäåò òàðåëêà
            mShip->SetEngineState(0); // âûêëþ÷àåì äâèãàòåëè
        }
    } else if(mMode == 30) { // òàðåëêà âëåòàåò - âêëþ÷àåì çâóê
        ++mMode;
//        ufosound = PLAYER.PlaySound(enginesound);
    } else if(mMode == 31) { // ëåòèò òàðåëêà!
        mUFO->Move(INTRO1_UFOSPEED, 0);
        if(mUFO->GetPosition().x > Window::Instance().GetWidth() + 30) { // òàðåëêà óëåòåëà - âêëþ÷àåì çåëåíûé è âûêëþ÷àåì åå çâóê
            //speaker.channel(ufosound)->stop();
//            ufosound = 0;
            mTrafficLights->SetState(2);
//            mShip->SetEngineState(1); // è çàïóñêàåì äâèãàòåëè
            ++mMode;
        }
    } else {    // ïîñëåäíåå äåéñòâèå: óëåòàåì ñ ýêðàíà
        float sp = mShip->GetPosition().y > 240 ?
            (mShip->GetPosition().y - 320.0f) / 6.7f : -6.0f; // óñêîðÿåìñÿ íà ñòàðòå
        mShip->Move(0, sp);
        if(mShip->GetPosition().y < -80) // óëåòåëè - çàâåðøàåìñÿ
            EndLevel();
    }
}

} // namespace levels
} // namespace oci
