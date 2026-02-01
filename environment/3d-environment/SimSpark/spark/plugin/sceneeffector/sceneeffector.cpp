/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#include "sceneeffector.h"
#include "sceneaction.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;

SceneEffector::SceneEffector() : oxygen::Effector()
{
}

SceneEffector::~SceneEffector()
{
}

void SceneEffector::OnLink()
{
    // Load spawning parameters
    std::shared_ptr<AgentAspect> aspect = GetAgentAspect();
    if (aspect.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: cannot get AgentAspect\n";
            return;
        }
    std::shared_ptr<Scene> scene = aspect->FindParentSupportingClass<Scene>().lock();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(SceneEffector) ERROR: can't get scene parent node.\n";
            return;
        }
    scene->LoadSpawningParameters();
}

void SceneEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if ( mAction.get() == 0 )
        return;

    std::shared_ptr<SceneAction> sceneAction =
        std::dynamic_pointer_cast<SceneAction>(mAction);
    mAction.reset();

    if (sceneAction.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: cannot realize "
                << "an unknown ActionObject\n";
            return;
        }

    std::shared_ptr<AgentAspect> aspect =GetAgentAspect();

    if (aspect.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: cannot get AgentAspect\n";
            return;
        }

    aspect->ImportScene(sceneAction->GetScene(),
        sceneAction->GetSceneParameters());

    // Update the cache and hierarchy of the scene
    // in order to correctly calculate the bounding boxes
    std::shared_ptr<Scene> scene = aspect->FindParentSupportingClass<Scene>().lock();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(SceneEffector) ERROR: can't get scene parent node.\n";
            return;
        }
    scene->UpdateCache();
    scene->UpdateHierarchy();

    std::shared_ptr<Space> ownSpace = aspect->FindChildSupportingClass<Space>(true);
    if (!ownSpace)
        {
            GetLog()->Error() << "(SceneEffector) ERROR: can't get space child node of agent.\n";
            return;
        }
    salt::AABB3 ownBoundingBox = GetAgentBoundingBox(ownSpace);

    // Get bounding boxes of other agents
    Leaf::TLeafList agentAspects;
    scene->ListChildrenSupportingClass<AgentAspect>(agentAspects, false);
    list<salt::AABB3> otherBoundingBoxes;
    for (TLeafList::iterator i = agentAspects.begin(); i != agentAspects.end(); ++i)
        {
            std::shared_ptr<AgentAspect> otherAspect = std::dynamic_pointer_cast<AgentAspect>(*i);
            std::shared_ptr<Space> otherSpace = aspect->FindChildSupportingClass<Space>(true);
            if (!otherSpace)
                {
                    // No space node found
                    // Most likely, the agent just didn't import a scene graph yet
                    // Skip this agent since it's irrelevant for the collision detection
                    continue;
                }
            otherBoundingBoxes.push_back(GetAgentBoundingBox(otherSpace));
        }

    // Update spawningOffset
    salt::Vector3f& spawningOffset = scene->GetSpawningOffset();
    spawningOffset[0] = ownBoundingBox.GetWidth();
    spawningOffset[1] = fmax(spawningOffset[1], ownBoundingBox.GetHeight());
    spawningOffset[2] = fmax(spawningOffset[2], ownBoundingBox.GetDepth());

    // Get list of rigid bodies to move
    Leaf::TLeafList rigidBodyList;
    aspect->ListChildrenSupportingClass<RigidBody>(rigidBodyList, true);
    if (rigidBodyList.size() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: agent aspect doesn't have children of type RigidBody\n";
            return;
        }

    // Determine body node
    std::shared_ptr<RigidBody> body = aspect->FindChildSupportingClass<RigidBody>(true);
    if (body.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: " << aspect->GetName()
                << " node has no Body child\n";
            return;
        }
    
    // Try to move the agent and prevent a collision with other agents
    // Stop repositioning after 10 failed repositioning attempts
    int tries = 0;
    bool intersectsWithOtherAgent;
    salt::Vector3f& nextSpawningPosition = scene->GetNextSpawningPosition();
    do {
        // Position before moving
        const salt::Vector3f agentPos = body->GetPosition();

        // move all child bodies
        for (TLeafList::iterator i = rigidBodyList.begin(); i != rigidBodyList.end(); ++i)
            {
                std::shared_ptr<RigidBody> childBody = std::dynamic_pointer_cast<RigidBody>(*i);

                salt::Vector3f childPos = childBody->GetPosition();
                childBody->SetPosition(nextSpawningPosition + (childPos-agentPos));
                childBody->SetVelocity(salt::Vector3f(0,0,0));
                childBody->SetAngularVelocity(salt::Vector3f(0,0,0));
            }

        // Update own bounding box for collision detection
        ownBoundingBox.Translate(body->GetPosition() - agentPos);
        
        IncrementNextSpawningPosition(scene);

        // Check if the agent intersects with another agent
        intersectsWithOtherAgent = false;
        for (
            list<salt::AABB3>::iterator i = otherBoundingBoxes.begin();
            i != otherBoundingBoxes.end();
            ++i
        )
            {
                if (ownBoundingBox.Intersects(*i)) {
                    intersectsWithOtherAgent = true;
                    break;
                }
            }

        tries++;
    } while(intersectsWithOtherAgent && tries < 10);
}

salt::AABB3 SceneEffector::GetAgentBoundingBox(std::shared_ptr<Space> space) {
    // We assume that all nodes relevant for calculating the bounding box
    // are encapsulated within a space node. If this assumption isn't true for other
    // domains, we need to adjust this.

    // The space node itself is (at least in the RoboCup soccer domain) always located at (0.0, 0.0, 0.0).
    // We have to do a manual calculation to get the correct bounding box.

    Leaf::TLeafList baseNodes;
    space->ListChildrenSupportingClass<BaseNode>(baseNodes, true);
    if (baseNodes.empty())
        {
            GetLog()->Warning()
                    << "(GetAgentBoundingBox) WARNING: space object doesn't have any"
                    << " children of type BaseNode.\n";
        }
    
    salt::AABB3 boundingBox;
    for (TLeafList::iterator i = baseNodes.begin(); i != baseNodes.end(); ++i)
    {
        std::shared_ptr<BaseNode> node = std::static_pointer_cast<BaseNode>(*i);
        boundingBox.Encapsulate(node->GetWorldBoundingBox());
    }

    return boundingBox;
}

void SceneEffector::IncrementNextSpawningPosition(std::shared_ptr<Scene> scene) {
    salt::AABB3 spawningArea = scene->GetSpawningArea();
    salt::Vector3f& spawningOffset = scene->GetSpawningOffset();
    salt::Vector3f& nextSpawningPosition = scene->GetNextSpawningPosition();
    // Add some safety distance to avoid agents touching each other
    const float safetyDistance = 0.1f;

    nextSpawningPosition[0] += spawningOffset[0] + safetyDistance;
    if (!spawningArea.Contains(nextSpawningPosition))
        {
            // Reset x, increment y
            nextSpawningPosition[0] = spawningArea.minVec[0];
            nextSpawningPosition[1] += spawningOffset[1] + safetyDistance;
            spawningOffset[1] = 0.0;
            if (!spawningArea.Contains(nextSpawningPosition))
                {
                    // Reset y, increment z
                    nextSpawningPosition[1] = spawningArea.minVec[1];
                    nextSpawningPosition[2] += spawningOffset[2] + safetyDistance;
                    spawningOffset[2] = 0.0;
                    if (!spawningArea.Contains(nextSpawningPosition))
                        {
                            // Reset z (reset to the start of the spawning area)
                            nextSpawningPosition[2] = spawningArea.minVec[2];
                        }
                }
        }
}

std::shared_ptr<ActionObject>
SceneEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
        {
            GetLog()->Error() << "(SceneEffector) ERROR: invalid predicate"
                              << predicate.name << "\n";
            return std::shared_ptr<ActionObject>();
        }

    string scene;
    if (! predicate.GetValue(predicate.begin(), scene))
        {
            GetLog()->Error()
                << "ERROR: (SceneEffector) scene filename expected\n";
            return std::shared_ptr<ActionObject>();
        };

    std::shared_ptr<ParameterList> parameters(
        new ParameterList(predicate.parameter));
    parameters->Pop_Front();

    return std::shared_ptr<ActionObject>(
        new SceneAction(GetPredicate(), scene, parameters));
}
