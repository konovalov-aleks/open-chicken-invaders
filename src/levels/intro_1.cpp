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
//    s32 ufosound;
    std::shared_ptr<Ship> mShip;
//    const CSound enginesound;

    int mMode;
};

static Factory::Registrator<Intro_1> reg("intro_1", "intro");

void Intro_1::Init() {
    IntroLevel::Init("intro1");
    mMode = 0;
    const Vector2u wndSize = Window::Instance().getSize();
    mTrafficLights = Storage().CreateObject<CommonSprite<Visible::dpBonus> >(
        "trafficlight.xml", Vector2f(wndSize.x / 2, wndSize.y / 2 - 50));
    mUFO = Storage().CreateObject<CommonAnimated<Visible::dpChicken> >(
        "flyingsaucer.xml", Vector2f(-100, 140));
    mShip = Storage().CreateObject<Ship>(
        Vector2f(wndSize.x / 2 - 80, wndSize.y + 20));
    //ufosound(0),
    //enginesound("ufo.wav", true)
//    background->dx = background->dy = 0;
//    trafficlights.frame = 1; // yellow
}

#define INTRO1_UFOSPEED 6

void Intro_1::RunIntro() {
    if(mMode < 20)
        ++mMode; // mode = 0..19 - yellow traffic light
    else if(mMode == 20) {       // mode = 20 - red
        mTrafficLights->SetState(1);
        ++mMode;
    } else if(mMode < 30) {   // the ship flies up the the traffic light
                              // stay in the position (120;140)
        float sp = mShip->getPosition().x < 340 ?
            -6.0f + (340.0f - mShip->getPosition().y) / 10.0f : -6.0f; // slow down when approaching the traffic light
        mShip->move(0, sp);
        if(fabsf(mShip->getPosition().y - 310.0f) < 10.0f) {
            mMode = 30; // stop - the UFO is about to flight
            mShip->SetEngineState(0); // stop the engine
        }
    } else if(mMode == 30) { // UFO flies onto the screen - turn on the sound
        ++mMode;
//        ufosound = PLAYER.PlaySound(enginesound);
    } else if(mMode == 31) { // the UFO is flying
        mUFO->move(INTRO1_UFOSPEED, 0);
        if(mUFO->getPosition().x > Window::Instance().getSize().x + 30) { // the UFO flew away - switch the green light and turn of UFO sound
            //speaker.channel(ufosound)->stop();
//            ufosound = 0;
            mTrafficLights->SetState(2);
//            mShip->SetEngineState(1); // start engine
            ++mMode;
        }
    } else {    // last action: fly away from the screen
        float sp = mShip->getPosition().y > 240 ?
            (mShip->getPosition().y - 320.0f) / 6.7f : -6.0f; // acceleration on start
        mShip->move(0, sp);
        if(mShip->getPosition().y < -80) // flew away => finish the level
            EndLevel();
    }
}

} // namespace levels
} // namespace oci
