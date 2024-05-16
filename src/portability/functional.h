/*
 * functional.h
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

#ifdef CPP11
#   include <functional>
#else
#   include <boost/bind.hpp>
#   include <boost/function.hpp>
#endif

namespace oci {

#ifdef CPP11

using std::mem_fn;
using std::bind;
using std::function;
namespace placeholders = std::placeholders;

#else

using boost::mem_fn;
using boost::bind;
using boost::function;

namespace placeholders {

using ::_1;
using ::_2;
using ::_3;

};

#endif

} // namespace oci
