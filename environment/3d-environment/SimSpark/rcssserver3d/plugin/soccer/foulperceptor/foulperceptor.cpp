#include "foulperceptor.h"

#include <soccerbase/soccerbase.h>

FoulPerceptor::FoulPerceptor() : oxygen::Perceptor(), mLastFoulIndex(-1)
{
}

FoulPerceptor::~FoulPerceptor()
{
}

void FoulPerceptor::OnLink()
{
    SoccerBase::GetSoccerRuleAspect(*this, mSoccerRule);
    SoccerBase::GetAgentState(*this, mAgentState);
}

void FoulPerceptor::OnUnlink()
{
    mAgentState.reset();
    mSoccerRule.reset();
}

bool FoulPerceptor::Percept(std::shared_ptr<oxygen::PredicateList> predList)
{
    if (mSoccerRule.get() == nullptr || mAgentState == nullptr)
    {
        return false;
    }

    bool davaAvailable = false;
    auto fouls = mSoccerRule->GetFoulsSince(mLastFoulIndex);
    for (auto& foul : fouls)
    {
        if (foul.agent->GetUniformNumber() != mAgentState->GetUniformNumber() ||
            foul.agent->GetTeamIndex() != mAgentState->GetTeamIndex())
        {
            continue;
        }

        davaAvailable = true;
        oxygen::Predicate& predicate = predList->AddPredicate();
        predicate.name = "foul";
        predicate.parameter.AddValue((int) foul.type);

        mLastFoulIndex = foul.index;
    }

    return davaAvailable;
}
