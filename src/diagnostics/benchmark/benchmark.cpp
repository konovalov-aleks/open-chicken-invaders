/*
 * benchmark.cpp
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

#include "benchmark.h"

#include <core/window.h>
#include <levels/factory.h>
#include <objects/base/animated.h>
#include <objects/characters/chicken.h>

#include <chrono>
#include <cstdlib>

namespace oci {
namespace benchmark {

    static levels::Factory::Registrar<Benchmark> reg("benchmark", "benchmark");

    class BenchmarkChicken : public objects::Animated {
    public:
        void Init() {
            Animated::Init(
                "chicken_red.xml",
                Vector2f(std::rand() % Window::Instance().getSize().x,
                         std::rand() % Window::Instance().getSize().y)
            );
        }

        virtual DrawPriority GetDrawPriority() const override { return dpChicken; }
    };

    Benchmark::Benchmark() : mFramesCount(0), mChickensCount(0),
                             mLastStatisticTime(Clock::now()),
                             mLastSceneUpdateTime(Clock::now()) {
    }

    void Benchmark::CreateNewChickens() {
        static const int CHICKENS_COUNT = 10;
        //for(int i = 0; i < CHICKENS_COUNT; ++i)
        for(int i = 0; i < mChickensCount; ++i)
            //Storage().CreateObject<BenchmarkChicken>();
            Storage().CreateObject<objects::Chicken>(
                Vector2f(std::rand() % Window::Instance().getSize().x,
                         std::rand() % Window::Instance().getSize().y),
                100, objects::Chicken::tCyan, 10);
        mChickensCount += CHICKENS_COUNT;
    }

    void Benchmark::Run() {
        ++mFramesCount;
        std::chrono::nanoseconds time = Clock::now() - mLastStatisticTime;
//        for(int i = 0; i < mChickensCount; ++i)
//            Storage().KillObject(Storage().CreateObject<BenchmarkChicken>());
        Storage().GetContext().ColliseAll(ctFriendBullet, 100500);
        if(time >= std::chrono::seconds(1)) {
            float FPS = mFramesCount / std::chrono::duration_cast<std::chrono::seconds>(time).count();
            printf("[%d chickens] FPS: %f\n", mChickensCount, FPS);
            mFramesCount = 0;
            mLastStatisticTime = Clock::now();
        }
        if(Clock::now() - mLastSceneUpdateTime >= std::chrono::seconds(5)) {
            CreateNewChickens();
            mLastSceneUpdateTime = Clock::now();
        }
    }

    void StartBenchmark() {
        levels::Factory::Instance().Build("benchmark");
    }

} // namespace benchmark
} // namespace oci
