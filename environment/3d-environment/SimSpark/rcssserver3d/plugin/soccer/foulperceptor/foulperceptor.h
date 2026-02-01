#ifndef FOULPERCEPTOR_H
#define FOULPERCEPTOR_H

#include <memory>
#include <agentstate/agentstate.h>
#include <oxygen/agentaspect/perceptor.h>
#include <soccerruleaspect/soccerruleaspect.h>
#include <soccertypes.h>

/**
 * @class FoulPerceptor
 *
 * A FoulPerceptor percepts the fouls (including the foul type) called out for an
 * agent since the last time @ref Percept() was called.
 */
class FoulPerceptor : public oxygen::Perceptor
{
public:
    FoulPerceptor();
    virtual ~FoulPerceptor();
    virtual bool Percept(std::shared_ptr<oxygen::PredicateList> predList);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    std::shared_ptr<AgentState> mAgentState;
    std::shared_ptr<SoccerRuleAspect> mSoccerRule;

private:
    int mLastFoulIndex;
};

DECLARE_CLASS(FoulPerceptor)

#endif // FOULPERCEPTOR_H
