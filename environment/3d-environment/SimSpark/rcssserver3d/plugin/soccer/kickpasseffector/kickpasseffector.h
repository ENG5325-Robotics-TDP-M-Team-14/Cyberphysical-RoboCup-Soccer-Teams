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
#ifndef KICKPASSEFFECTOR_H
#define KICKPASSEFFECTOR_H

#include <ball/ball.h>
#include <ballstateaspect/ballstateaspect.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <zeitgeist/randomserver/randomserver.h>

class GameStateAspect;
class SoccerRuleAspect;

class KickPassEffector : public oxygen::Effector
{
public:
    KickPassEffector();
    virtual ~KickPassEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "kick"; }

    /** constructs an Actionobject, describing a predicate */
    virtual std::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

protected:
    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

private:
    /** tries to realize the kick */
    void ExecuteKick();

    /** Accelerates ball from the agent that called kick pass,
     * considering a final target.
     */
    bool AccelerateBall();

    /** 
     * Chooses the distance considering a normal distribution
     * within a minimum and maximum value. 
     */
    float GenerateNormallyDistributedDistance();

protected:
    /** the reference to the GameState */
    std::shared_ptr<GameStateAspect> mGameState;

    /** a reference to the agent state */
    std::shared_ptr<AgentState> mAgentState;

    /** reference to the soccer rule aspect */
    std::shared_ptr<SoccerRuleAspect> mSoccerRule;

    /** reference to the body node of the ball */
    std::shared_ptr<oxygen::RigidBody> mBallBody;

    /** reference to the BallStateAspect */
    std::shared_ptr<BallStateAspect> mBallStateAspect;
    
    /* random server used for the catch margin and velocity */
    std::shared_ptr<zeitgeist::RandomServer> mRandomServer;

private:
    /** duration of pass mode */
    float mPassModeDuration;

    /** Max distance that the ball will travel after being propelled in kick pass */
    float mMaxPropulsionDistance;
    /** Min distance that the ball will travel after being propelled in kick pass */
    float mMinPropulsionDistance;
    /** A maximum quantity of steps for the ball to be accelerated. This ensures the ball will reach the generated distance, making it safe from the initial agent-ball collision. */    
    int mMaxPropulsionSteps;
    
    /** Counts for how many cycles the ball is being initially propelled by the kick pass */
    int mCurrentPropellingDuration;
    /** Holds the last generated kick pass target*/
    salt::Vector3f mLastGeneratedTarget;

    /** the timestamp when the last realized action was received */
    float mActionTimestamp;

    /** whether this effector is currently active */
    bool mActive;
};

DECLARE_CLASS(KickPassEffector)

#endif 