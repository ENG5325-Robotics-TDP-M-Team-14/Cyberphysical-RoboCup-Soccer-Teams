/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef CATCHEFFECTOR_H
#define CATCHEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/transform.h>
#include <ball/ball.h>
#include <ballstateaspect/ballstateaspect.h>

namespace salt
{
    class AABB2;
}

namespace oxygen
{
    class RigidBody;
    class AgentAspect;
    class SimulationServer;
}

namespace zeitgeist
{
    class RandomServer;
}

class AgentState;

class SoccerRuleAspect;

/**
    With the catch effector, a ball can be artificially caught without hands.
    When activated, the ball will stick to the position of the effector if
    the prerequisites are met.
    The CatchEffector needs to be installed at the surface of a collider
    in order for the catch margin to work as expected.
  */
class CatchEffector : public oxygen::Effector
{
public:
    CatchEffector();
    virtual ~CatchEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return GetName(); }

    /** constructs an Actionobject, describing a predicate */
    virtual std::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the ball body node */
    virtual void OnLink();

    /** remove the reference to the ball body node */
    virtual void OnUnlink();

    /** set the catch margin (within which objects are guaranteed to be catchable) */
    void SetSafeCatchMargin(float margin);

    /** set the catch margin (within which objects may be catchable) */
    void SetMaxCatchMargin(float margin);

    /** set the catch velocity
        (the maximum velocity at which a ball is guaranteed to be caught)
      */
    void SetSafeCatchVelocity(float velocity);

    /** set the catch velocity
        (the maximum velocity at which a ball may be caught)
      */
    void SetMaxCatchVelocity(float velocity);

    /** links another catch effector to this instance */
    void LinkEffector(CatchEffector& other);

    /** sets the pointer to the timestamp when the ball got caught the last time */
    void SetLastCatchTime(std::shared_ptr<float> lastCatchTime);

    /** sets the pointer to the duration representing
        how long the ball has been caught consecutively
      */
    void SetConsecutiveCatchTime(std::shared_ptr<float> consecutiveCatchTime);

protected:
    /** moves the ball to pos setting its linear and angular velocity to 0 */
    void MoveBall(const salt::Vector3f& pos);

     /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    /** reference to the body node of the ball */
    std::shared_ptr<oxygen::RigidBody> mBallBody;
    /** reference to the parent transform node */
    std::shared_ptr<oxygen::Transform> mTransformParent;
    /** reference to the agentstate */
    std::shared_ptr<AgentState> mAgentState;

    /** bounding box for the right penalty area */
    salt::AABB2 mRightPenaltyArea;
    /** bounding box for the left penalty area */
    salt::AABB2 mLeftPenaltyArea;

private:
    /** the margin where objects are guaranteed to be caught */
    float mSafeCatchMargin;
    /** the margin where objects may be be caught */
    float mMaxCatchMargin;
    /** the maximum velocity at which a ball is guaranteed to be caught */
    float mSafeCatchVelocity;
    /** the maximum velocity at which a ball may be be caught */
    float mMaxCatchVelocity;
    /** the maximum catch time in seconds */
    float mCatchTime;
    /** the cooldown time in seconds after which the effector can be used again */
    float mCatchCooldownTime;

    /** radius of the ball */
    float mBallRadius;
    /** timestamp of when the ball was caught the last time */
    std::shared_ptr<float> mLastCatchTime;
    /** how long the ball has been held/caught consecutively */
    std::shared_ptr<float> mConsecutiveCatchTime;
    
    /** a reference to the simulation server */
    std::shared_ptr<oxygen::SimulationServer> mSimulationServer;
    /* random server used for the catch margin and velocity */
    std::shared_ptr<zeitgeist::RandomServer> mRandomServer;
};

DECLARE_CLASS(CatchEffector)

#endif // CATCHEFFECTOR_H
