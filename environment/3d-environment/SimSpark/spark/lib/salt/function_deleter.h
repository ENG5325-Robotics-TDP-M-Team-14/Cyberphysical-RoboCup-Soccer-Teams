/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of rcssserver3D
   Mon Feb 26 2024
   Copyright (C) 2024 RoboCup Soccer Server 3D Maintenance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef SALT_FUNCTION_DELETER_H_
#define SALT_FUNCTION_DELETER_H_

#include <memory>

namespace salt
{

/**
 * A zero size deleter which calls a deleter specified at compile time for each
 * passed pointer. Mainly useful for C functions which delete a pointer.
 *
 * @tparam DeleterFunc A function or function object to call to delete each
 * passed pointer
 */
template <auto DeleterFunc>
struct FunctionDeleter
{
    template <typename T>
    void operator()(T *p)
    {
        DeleterFunc(p);
    }
};

/**
 * A unique_ptr which receives a deleter function as its second template
 * argument, useful for encapsulating C dynamic objects.
 */
template <typename T, auto DeleterFunc>
using unique_ptr_d = std::unique_ptr<T, FunctionDeleter<DeleterFunc>>;

} // namespace salt

#endif /* SALT_FUNCTION_DELETER_H_ */
