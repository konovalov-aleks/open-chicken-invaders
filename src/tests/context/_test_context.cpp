/*
 * _test_context.cpp
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

#include <context/manager.h>
#include <objects/base/game_object.h>
#include <stdio.h>

class TestObject : public GameObject {
public:
    TestObject() { puts("TestObject::TestObject()"); }
    virtual ~TestObject() { puts("TestObject::~TestObject()"); }
    void Init() { puts("TestObject::Init();"); }
};

int main() {
    puts("<Start>");
    puts("<Create context>");
    {
        std::shared_ptr<Context> ctx(ContextManager::Instance().GetContext("test_context"));
	puts("<Create object>");
        ctx->GetStorage("test_storage").CreateObject<TestObject>();
    }
    puts("<Destroy context>");
    ContextManager::Instance().KillContext("test_context");
    puts("<Finish>");
}
