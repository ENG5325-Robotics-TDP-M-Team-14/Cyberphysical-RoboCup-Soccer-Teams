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
#include "catchaction.h"
#include "catcheffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/randomserver/randomserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/simulationserver/simulationserver.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <ballstateaspect/ballstateaspect.h>
#include <agentstate/agentstate.h>
#include <soccerbase/soccerbase.h>
#include <soccercontrolaspect/soccercontrolaspect.h>
#include <soccerruleaspect/soccerruleaspect.h>

using namespace oxygen;
using namespace salt;
using namespace std;

CatchEffector::CatchEffector()
    : oxygen::Effector(),
      mSafeCatchMargin(0.05),
      mMaxCatchMargin(0.1),
      mSafeCatchVelocity(1.5),
      mMaxCatchVelocity(3.0),
      mCatchTime(6.0),
      mCatchCooldownTime(3.0),
      mBallRadius(0.0)
{
}

CatchEffector::~CatchEffector()
{
}

void
CatchEffector::MoveBall(const Vector3f& pos)
{
    mBallBody->SetPosition(pos);
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
}

void
CatchEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    // this should also include the case when there is no ball
    // (because then there will be no body, neither).
    if (mAction.get() ==0 || mBallBody.get() == 0)
    {
        return;
    }

    if (mTransformParent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) no transform parent node present\n";
        return;
    }

    if (mAgentState.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) no agent state node present\n";
        return;
    }

    std::shared_ptr<CatchAction> catchAction =
        std::dynamic_pointer_cast<CatchAction>(mAction);
    mAction.reset();
    if (catchAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) cannot realize an unknown "
            << "ActionObject\n";
        return;
    }

    // Reset mConsecutiveCatchTime if cooldown time elapsed without being interrupted through catch actions
    float lastCatchTime = mLastCatchTime.get() != 0 ? *mLastCatchTime : 0.0;
    float consecutiveCatchTime = mConsecutiveCatchTime.get() != 0 ? *mConsecutiveCatchTime : 0.0;
    float timeElapsedSinceLastCatch = mSimulationServer->GetTime() - lastCatchTime;
    if (timeElapsedSinceLastCatch > mCatchCooldownTime && mConsecutiveCatchTime.get() != 0) {
        *mConsecutiveCatchTime = 0.0;
    }

    // Check if maximum catch time is exceeded
    if (consecutiveCatchTime > mCatchTime)
    {
        return;
    }

    if (mAgentState->GetUniformNumber() != 1)
    {
        return;
    }

    Vector3f ballPos = mBallBody->GetPosition();
    if ( mAgentState->GetTeamIndex() == TI_LEFT )
    {
        if (! mLeftPenaltyArea.Contains(Vector2f(ballPos[0], ballPos[1])))
        {
            return;
        }
    }
    else
    {
        if (! mRightPenaltyArea.Contains(Vector2f(ballPos[0], ballPos[1])))
        {
            return;
        }
    }

    const salt::Matrix worldTransform = mTransformParent->GetWorldTransform();
    Vector3f ballVec = ballPos - worldTransform.Pos();
    float ballVelocity = mBallBody->GetVelocity().Length();

    float catchMargin = mSafeCatchMargin;
    float catchVelocity = mSafeCatchVelocity;
    if (mRandomServer != 0)
    {
        float additionalMargin = mMaxCatchMargin - mSafeCatchMargin;
        if (additionalMargin > 0.0f) {
            catchMargin += mRandomServer->GetUniformRandom(0.0f, additionalMargin);
        }
        float additionalVelocity = mMaxCatchVelocity - mSafeCatchVelocity;
        if (additionalVelocity > 0.0f)
        {
            catchVelocity += mRandomServer->GetUniformRandom(0.0f, additionalVelocity);
        }
    }

    // the ball can be caught if the distance is
    // less then ball radius + catch margin AND
    // the velocity of the ball is low enough
    if (ballVec.Length() > mBallRadius + catchMargin ||
        ballVelocity > catchVelocity)
    {
        // ball is out of reach or too fast:
        // catch has no effect
        return;
    }

    // Perform actual catching
    ballPos = worldTransform.Pos();
    ballPos += mBallRadius * worldTransform.Forward().Normalized();
    MoveBall(ballPos);

    // Update mConsecutiveCatchTime and mLastCatchTime
    // Only increase mConsecutiveCatchTime if another effector didn't call this in the same cycle already
    if (mConsecutiveCatchTime.get() != 0 && mLastCatchTime.get() != 0 && *mLastCatchTime != mSimulationServer->GetTime())
    {
        *mConsecutiveCatchTime += deltaTime;
    }
    if (mLastCatchTime.get() != 0)
    {
        *mLastCatchTime = mSimulationServer->GetTime();
    }
}

std::shared_ptr<ActionObject>
CatchEffector::GetActionObject(const Predicate& predicate)
{
  do
  {
      if (predicate.name != GetPredicate())
          {
              GetLog()->Error() << "ERROR: (CatchEffector) invalid predicate"
                                << predicate.name << "\n";
              break;
          }

      // construct the CatchAction object
      return std::shared_ptr<CatchAction>(new CatchAction(GetPredicate()));

  } while (0);

  // some error happened
  return std::shared_ptr<ActionObject>();
}

void
CatchEffector::OnLink()
{
    mSimulationServer = std::dynamic_pointer_cast<SimulationServer>
        (GetCore()->Get("/sys/server/simulation"));
    if (mSimulationServer.get() == 0)
    {
        GetLog()->Error() << "(CatchEffector) ERROR: SimulationServer not found\n";
    }

    SoccerBase::GetBallBody(*this,mBallBody);

    std::shared_ptr<SoccerRuleAspect> soccerRule;
    if (SoccerBase::GetSoccerRuleAspect(*this, soccerRule))
    {
        mRightPenaltyArea = soccerRule->GetRightPenaltyArea();
        mLeftPenaltyArea = soccerRule->GetLeftPenaltyArea();
    }

    mTransformParent = FindParentSupportingClass<oxygen::Transform>().lock();
    if (mTransformParent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) no parent node is derived "
            << "from Transform\n";
        return;
    }

    std::shared_ptr<oxygen::SphereCollider> geom;
    if (! SoccerBase::GetBallCollider(*this,geom))
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) ball node has no SphereCollider "
            << "child\n";
    } 
    else
    {
        mBallRadius = geom->GetRadius();
    }

    mRandomServer = static_pointer_cast<zeitgeist::RandomServer>(GetCore()->Get("/sys/server/random"));
    if (mRandomServer == 0)
    {
        GetLog()->Error() << "ERROR: (CatchEffector) no RandomServer node found\n";
    }

    SoccerBase::GetSoccerVar(*this, "CatchTime", mCatchTime);
    SoccerBase::GetSoccerVar(*this, "CatchCooldownTime", mCatchCooldownTime);

    // Assumption: an agent has a single space node that represents the root of the agent
    std::shared_ptr<Space> spaceParent = FindParentSupportingClass<Space>().lock();
    if (spaceParent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) no parent node is derived "
            << "from Space\n";
        return;
    }

    mAgentState = spaceParent->FindChildSupportingClass<AgentState>(true);
    if (mAgentState.get() == 0)
    {
        GetLog()->Error() << "ERROR: (CatchEffector) no AgentState node found\n";
        return;
    }

    // Link to other CatchEffectors belonging to the same agent
    // This allows to limit the catch time for all catch effectors beloging to an agent
    TLeafList catchEffectors;
    spaceParent->GetChildrenSupportingClass("CatchEffector", catchEffectors, true);
    bool foundOther = false;
    for (
        TLeafList::iterator iter = catchEffectors.begin();
        iter != catchEffectors.end();
        ++iter
    )
    {
        std::shared_ptr<CatchEffector> ce = static_pointer_cast<CatchEffector>(*iter);
        if (ce.get() == this)
        {
            // Don't link to this CatchEffector
            continue;
        }
            
        ce->LinkEffector(*this);
        foundOther = true;
        break;
    }

    if (!foundOther) {
        mLastCatchTime.reset(new float(-100000.0));
        mConsecutiveCatchTime.reset(new float(0.0));
    }
}

void
CatchEffector::LinkEffector(CatchEffector& other)
{
    other.SetLastCatchTime(mLastCatchTime);
    other.SetConsecutiveCatchTime(mConsecutiveCatchTime);
}

void
CatchEffector::SetLastCatchTime(std::shared_ptr<float> lastCatchTime)
{
    mLastCatchTime = lastCatchTime;
}

void
CatchEffector::SetConsecutiveCatchTime(std::shared_ptr<float> consecutiveCatchTime)
{
    mConsecutiveCatchTime = consecutiveCatchTime;
}

void
CatchEffector::OnUnlink()
{
    mBallBody.reset();
    mTransformParent.reset();
    mAgentState.reset();
    mLastCatchTime.reset();
    mConsecutiveCatchTime.reset();
    mRandomServer.reset();
    mSimulationServer.reset();
}

void
CatchEffector::SetSafeCatchMargin(float margin)
{
    mSafeCatchMargin = margin;
}

void
CatchEffector::SetMaxCatchMargin(float margin)
{
    mMaxCatchMargin = margin;
}

void
CatchEffector::SetSafeCatchVelocity(float velocity)
{
    mSafeCatchVelocity = velocity;
}

void
CatchEffector::SetMaxCatchVelocity(float velocity)
{
    mMaxCatchVelocity = velocity;
}
