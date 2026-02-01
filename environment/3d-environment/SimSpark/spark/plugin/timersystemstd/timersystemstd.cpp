/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of rcssserver3D
   Thu Mar 24 2011
   Copyright (C) 2003-1011 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#include "timersystemstd.h"
#include <cmath>
#include <ratio>
#include <thread>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;

void TimerSystemStd::Initialize()
{
    // Check if the steady_clock is precise enough.
    // Note: even with a sufficient precision, the duration for the clock to
    // update its value may be higher.
    static_assert(
        std::ratio_less_equal<std::chrono::steady_clock::period, std::milli>::value,
        "This C++ compiler doesn't provide a precise enough steady_clock."
    );

    mLastQueryTime = std::chrono::steady_clock::now();
}

float TimerSystemStd::GetTimeSinceLastQuery()
{
    std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> timeDiff = currentTime - mLastQueryTime;
    mLastQueryTime = currentTime;
    return timeDiff.count();
}

void TimerSystemStd::WaitFromLastQueryUntil(float deadline)
{
    // We can't pass deadline into a std::chrono::duration<float> and subtract
    // that. This would lead to floating point errors when adding to
    // mLastQueryTime. That again causes this method to wait a bit less than
    // the specified time. The time passed is less than the time of a single
    // simulation step and the physics won't be stepped forward the next time
    // (assuming it is stepped in discrete steps). That again may lead to
    // various issues.
    int milliseconds = ceil(deadline * 1000);
    std::this_thread::sleep_until(mLastQueryTime +
        std::chrono::milliseconds(milliseconds));
}

//void TimerSystemStd::Finalize()
//{
//}
