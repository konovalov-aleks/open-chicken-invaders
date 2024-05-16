/*
 * type_traits.h
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
#   include <type_traits>
#else
#   include <boost/type_traits.hpp>
#endif

namespace oci {

#ifdef CPP11

using std::integral_constant;
using std::is_base_of;
using std::is_convertible;
using std::conditional;

#else

using boost::integral_constant;
using boost::is_base_of;
using boost::is_convertible;
using boost::conditional;

#endif

} // namespace oci
