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
#include "kickpassaction.h"
#include "kickpasseffector.h"
#include <algorithm>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <soccerruleaspect/soccerruleaspect.h>

using namespace oxygen;
using namespace salt;
using namespace std;

KickPassEffector::KickPassEffector() : oxygen::Effector(),
    mPassModeDuration(4.0),
    mMaxPropulsionDistance(5.0),
    mMinPropulsionDistance(3.0),
    mMaxPropulsionSteps(5),
    mCurrentPropellingDuration(0)
{
}

KickPassEffector::~KickPassEffector()
{
}

void
KickPassEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (
        (mGameState.get() == 0) ||
        (mSoccerRule.get() == 0) ||
        (mBallBody.get() == 0) ||
        (mBallStateAspect.get() == 0) ||
        (mAgentState.get() == 0) ||
        (mRandomServer.get() == 0)
        )
    {
        return;
    }

    if (mActive)
    {
        // Effector was already active before and is not finished yet
        mAction.reset();
        ExecuteKick();
        return;
    }

    if (mAction.get() == 0)
    {
        return;
    }

    std::shared_ptr<KickPassAction> passAction =
        std::dynamic_pointer_cast<KickPassAction>(mAction);

    mAction.reset();

    if (passAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickPassEffector) cannot realize an unknown ActionObject\n";
        return;
    }

    TTeamIndex team = mAgentState->GetTeamIndex();
    int unum = mAgentState->GetUniformNumber();
    if (!mSoccerRule->CanActivatePassMode(unum, team) || mSoccerRule->IsKickPassActive())
    {
        return;
    }
    mActive = true;
    mSoccerRule->StartKickPass(team);
    mActionTimestamp = mGameState->GetTime();

    ExecuteKick();
}

void
KickPassEffector::ExecuteKick()
{
    const TTime now = mGameState->GetTime();
    const bool maxDurationExceeded = mActionTimestamp > now + mPassModeDuration;
    const bool invalidGameMode = mGameState->GetPlayMode() != PM_PlayOn;
    if (maxDurationExceeded || invalidGameMode)
    {
        // Cancel kick
        mActive = false;
        mSoccerRule->EndKickPass();
        mCurrentPropellingDuration = 0;
        return;
    }

    TTeamIndex team = mAgentState->GetTeamIndex();
    mGameState->SetLastTimeInPassMode(team, now);
    mSoccerRule->SetPlayerUNumTouchedBallSincePassMode(team, -1);
    mSoccerRule->SetMulitpleTeammatesTouchedBallSincePassMode(team, false);
    mGameState->SetPassModeClearedToScore(team, false);

    // Cancel out affects of pass mode for opponent
    mGameState->SetLastTimeInPassMode(SoccerBase::OpponentTeam(team), -1000);

    std::shared_ptr<AgentAspect> collidingAgent;
    TTime collisionTime;
    if (mBallStateAspect->GetLastCollidingAgent(collidingAgent, collisionTime))
    {
        // Sanity check: does this effector belong to the colliding agent?
        std::shared_ptr<AgentState> collidingAgentState = collidingAgent->FindChildSupportingClass<AgentState>(true);
        if (collidingAgentState.get() == 0
            || collidingAgentState->GetUniformNumber() != mAgentState->GetUniformNumber()
            || collidingAgentState->GetTeamIndex() != mAgentState->GetTeamIndex())
        {
            if (mCurrentPropellingDuration > 0)
            {
                // Another agent collided after propelling the ball
                // -> Stop propelling the ball.
                mActive = false;
                mSoccerRule->EndKickPass();
                mCurrentPropellingDuration = 0;
            }
            return;
        }

        if (collisionTime >= mActionTimestamp)
        {
            // Propelling for more than one cycle prevents that another possible collision might interfere on the desired distance.
            bool isFinishedPropelling = mCurrentPropellingDuration >= mMaxPropulsionSteps;

            if (isFinishedPropelling)
            {
                mActive = false;
                mSoccerRule->EndKickPass();
                mCurrentPropellingDuration = 0;
                mLastGeneratedTarget.Set(0, 0, 0);
            }
            else
            {
                // If it is the first propelling cycle, final target generates normally distributed distance.
                if (mCurrentPropellingDuration == 0)
                {
                    std::shared_ptr<RigidBody> body;
                    SoccerBase::GetAgentBody(collidingAgent, body);

                    if (body.get() != 0)
                    {
                        Vector3f displacement = (body->GetRotation() * Vector2f(0, 1) * GenerateNormallyDistributedDistance());
                        mLastGeneratedTarget = mBallBody->GetPosition() + displacement;
                    }
                }

                // Also stop if the agent did not collide anymore with the ball in the cycle.
                bool agentCurrentlyColliding = collisionTime == now;
                bool ballOverAccelerated = !AccelerateBall();
                if (ballOverAccelerated || !agentCurrentlyColliding)
                {
                    mCurrentPropellingDuration = mMaxPropulsionSteps; // Triggers finishing condition
                }
                else
                {
                    mCurrentPropellingDuration++;
                }
            }
        }
    }
}

bool
KickPassEffector::AccelerateBall()
{
    Vector3f currentNeededDisplacement = (mLastGeneratedTarget - mBallBody->GetPosition());

    double distance = currentNeededDisplacement.Length();

    const float calculatedSpeed = distance * 1.5;

    double currentBallSpeed = mBallBody->GetVelocity().Length();

    // If the robot collision was enough to over-accelerate the ball (e.g. a kick)
    // just finishes pass mode without any propelling.
    if (currentBallSpeed >= calculatedSpeed)
    {
        return false;
    }

    Vector3f propulsionVelocity = currentNeededDisplacement.Normalized() * calculatedSpeed;
    propulsionVelocity.Set(propulsionVelocity.Get(0), propulsionVelocity.Get(1), 0); // Z-axis is ignored so the result is a rolling ball.

    mBallBody->SetAngularVelocity(Vector3f(0, 0, 0)); // Making sure that the angular velocity resulted from the collision doesn't affect it.
    mBallBody->SetVelocity(propulsionVelocity);

    return true;
}

float
KickPassEffector::GenerateNormallyDistributedDistance()
{
    const float minMeters = mMinPropulsionDistance;
    const float maxMeters = mMaxPropulsionDistance;

    const float mean = (minMeters + maxMeters) / 2.0;
    const float range = maxMeters - minMeters;

    // The calculated standard deviation will consider the statistics Empirical Rule,
    // where values within 3 standard deviations from the mean will result in 99.73% of all the possible values.
    const float stdDeviation = range / 6.0;

    const float distance = mRandomServer->GetNormalRandom(mean, stdDeviation);

    //It kind of messes with the normal distribution concept but treats rare and very out of range values.
    return std::clamp<float>(distance, minMeters, maxMeters);
}

std::shared_ptr<ActionObject>
KickPassEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (KickPassEffector) invalid predicate"
                        << predicate.name << "\n";
      return std::shared_ptr<ActionObject>();
    }

  return std::shared_ptr<ActionObject>(new KickPassAction(GetPredicate()));
}

void
KickPassEffector::OnLink()
{
    mRandomServer = std::static_pointer_cast<zeitgeist::RandomServer>(GetCore()->Get("/sys/server/random"));
    if (mRandomServer.get() == 0)
    {
        GetLog()->Error() << "ERROR: (KickPassEffector) no RandomServer node found\n";
    }

    SoccerBase::GetGameState(*this, mGameState);
    SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetSoccerRuleAspect(*this, mSoccerRule);
    SoccerBase::GetBallBody(*this, mBallBody);
    mBallStateAspect = std::dynamic_pointer_cast<BallStateAspect>
        (SoccerBase::GetControlAspect(*this, "BallStateAspect"));

    SoccerBase::GetSoccerVar(*this, "PassModeDuration", mPassModeDuration);
    SoccerBase::GetSoccerVar(*this, "KickPassMaxPropulsionDistance", mMaxPropulsionDistance);
    SoccerBase::GetSoccerVar(*this, "KickPassMinPropulsionDistance", mMinPropulsionDistance);
    SoccerBase::GetSoccerVar(*this, "KickPassMaxPropulsionSteps", mMaxPropulsionSteps);
}

void
KickPassEffector::OnUnlink()
{
    if (mSoccerRule->IsKickPassActive() && mActive)
    {
        mActive = false;
        mSoccerRule->EndKickPass();
    }

    mGameState.reset();
    mAgentState.reset();
    mSoccerRule.reset();
    mBallBody.reset();
    mBallStateAspect.reset();
}

