/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include "scene.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

Scene::Scene() : BaseNode(), mModified(false), mModifiedNum(0), mLastCacheUpdate(0), mSpawningParametersLoaded(false)
{
}

Scene::~Scene()
{
}

void Scene::UpdateCacheInternal()
{
    mLastCacheUpdate = mModifiedNum;
}

int Scene::GetLastCacheUpdate()
{
    return mLastCacheUpdate;
}

const salt::Matrix& Scene::GetWorldTransform() const
{
    return mIdentityMatrix;
}

void Scene::SetWorldTransform(const salt::Matrix &/*transform*/)
{
}

void Scene::SetModified(bool modified)
{
    mModified = modified;
    if ( modified ) mModifiedNum++;
}

bool Scene::GetModified()
{
    return mModified;
}

int Scene::GetModifiedNum()
{
    return mModifiedNum;
}

salt::AABB3 Scene::GetSpawningArea()
{
    return mSpawningArea;
}

salt::Vector3f& Scene::GetSpawningOffset()
{
    return mSpawningOffset;
}

salt::Vector3f& Scene::GetNextSpawningPosition()
{
    return mNextSpawningPosition;
}

void Scene::LoadSpawningParameters()
{
    if (!mSpawningParametersLoaded)
    {
        const std::shared_ptr<ScriptServer>& script = GetScript();
        if (script.get() == 0)
        {
            GetLog()->Error() << "(Scene) ERROR: cannot get ScriptServer\n";
            return;
        }

        salt::Vector3f& minVec = mSpawningArea.minVec;
        salt::Vector3f& maxVec = mSpawningArea.maxVec;

        bool success = true;
        success &= GetScript()->GetVariable("Scene.SpawningAreaStartX", minVec[0]);
        success &= GetScript()->GetVariable("Scene.SpawningAreaStartY", minVec[1]);
        success &= GetScript()->GetVariable("Scene.SpawningAreaStartZ", minVec[2]);
        success &= GetScript()->GetVariable("Scene.SpawningAreaStopX", maxVec[0]);
        success &= GetScript()->GetVariable("Scene.SpawningAreaStopY", maxVec[1]);
        success &= GetScript()->GetVariable("Scene.SpawningAreaStopZ", maxVec[2]);
        if (success)
        {
            mSpawningParametersLoaded = true;

            mSpawningOffset[0] = 0.0;
            mSpawningOffset[1] = 0.0;
            mSpawningOffset[2] = 0.0;
            mNextSpawningPosition = minVec;
        }
        else
        {
            GetLog()->Error() << "(Scene) ERROR: unable to get spawning parameters\n";
            return;
        }
    }
}
