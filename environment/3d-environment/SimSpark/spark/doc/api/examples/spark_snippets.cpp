//! [simple class]
class Simple
{
public:
  Simple();
  virtual ~Simple();
  
  void DoSomething();
  void PrintString(const std::string& s);
  void PrintInt(int i);
  void PrintFloat(float f);
  void PrintBool(bool b);
};
//! [simple class]



//! [simple leaf class step1]
#include <zeitgeist/leaf.h>

class Simple : public zeitgeist::Leaf
{
public:
  Simple();
  virtual ~Simple();
  
  void DoSomething();
  void PrintString(const std::string& s);
  void PrintInt(int i);
  void PrintFloat(float f);
  void PrintBool(bool b);
};
//! [simple leaf class step1]



//! [simple leaf class step2]
#include <zeitgeist/leaf.h>

class Simple : public zeitgeist::Leaf
{
public:
  Simple();
  virtual ~Simple();
  
  void DoSomething();
  void PrintString(const std::string& s);
  void PrintInt(int i);
  void PrintFloat(float f);
  void PrintBool(bool b);
};

DECLARE_CLASS(Simple)
//! [simple leaf class step2]



//! [simple leaf class_c]
#include "simple.h"

using namespace zeitgeist;

void CLASS(Simple)::DefineClass()
{
  DEFINE_BASECLASS(zeitgeist/Leaf)
}
//! [simple leaf class_c]



//! [simple leaf class_c with function]
#include "simple.h"

using namespace zeitgeist;

FUNCTION(doSomething)
{
  if (in.size() == 0)
  {
    Simple *simple = static_cast<Simple*>(obj);
    simple->DoSomething();
  }
}


void CLASS(Simple)::DefineClass()
{
  DEFINE_BASECLASS(zeitgeist/Leaf)
  DEFINE_FUNCTION(doSomething)
}
//! [simple leaf class_c with function]



//! [simple leaf class_c function extension]
#include "simple.h"

using namespace zeitgeist;

FUNCTION(printInt)
{
  if (in.size() == 1)
  {
    Simple *simple = static_cast<Simple*>(obj);
    simple->PrintInt(any_cast<int>(in[0]));
  }
}
//! [simple leaf class_c function extension]



//! [direct class registation]
#include "kerosin.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace zeitgeist;

Kerosin::Kerosin(zeitgeist::Zeitgeist &zg)
{
  zg.GetCore()->RegisterClassObject(new CLASS(SoundServer), "kerosin/");

  zg.GetCore()->RegisterClassObject(new CLASS(InputServer), "kerosin/");

  zg.GetCore()->RegisterClassObject(new CLASS(ImageServer), "kerosin/");

  zg.GetCore()->RegisterClassObject(new CLASS(FontServer), "kerosin/");

  zg.GetCore()->RegisterClassObject(new CLASS(OpenGLServer), "kerosin/");
}
//! [direct class registation]



//! [indirect class registation]
#include "simple.h"
#include "complex.h"
#include <zeitgeist/zeitgeist.h>

ZEITGEIST_EXPORT_BEGIN()
  ZEITGEIST_EXPORT(Simple);
  ZEITGEIST_EXPORT(Complex);
ZEITGEIST_EXPORT_END()
//! [indirect class registation]



//! [run a ruby script]
#include <zeitgeist/zeitgeist.h>

using namespace zeitgeist;

int main()
{
  Zeitgeist  zeitgeist;

  shared_ptr<CoreContext> context = zeitgeist.CreateContext();
  
  shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));
 
  scriptServer->Run("scripttest.rb");
  
  return 0;
}
//! [run a ruby script]



//! [force effector perform]
bool ForceEffector::Perform(std::shared_ptr<BaseNode> &base, float deltaTime)
{
  if (!base) return false;

  // base should be a transform, or some other node, which has a Body-child
  shared_ptr<Body> body = shared_static_cast<Body>(base->GetChildOfClass("Body"));

  if (!body) return false;

  if (mForce.Length() > mMaxForce) mForce = mMaxForce/mForce.Length()*mForce;

  dBodyAddForce(body->GetODEBody(), mForce.x(), mForce.y(), mForce.z());

  mForce.Set(0,0,0);
  return true;
}

void ForceEffector::AddForce(const salt::Vector3f& force)
{
  mForce += force;
}
//! [force effector perform]



//! [control aspect]
shared_ptr<Effector> SurvivalControlAspect::RequestEffectorInternal(shared_ptr<AgentAspect>& agent, const string& effectorName)
{
  // always make sure, we know how to treat an agent class
  if (agent->GetClass()->Supports("SurvivalAgentAspect"))
  {
    if (effectorName == "kerosin/ForceEffector")
      return CreateEffector(effectorName);
  }

  // no valid agentaspect/effector combination
  return shared_ptr<Effector>();
}
//! [control aspect]



//! [agent aspect]
void SurvivalAgentAspect::OnLink()
{
  // locate control aspect
  shared_ptr<SurvivalControlAspect> control = shared_static_cast<SurvivalControlAspect>(GetScene()->GetChildOfClass("SurvivalControlAspect"));
  
  if (!control)
  {
    GetLog()->Error() << "ERROR: Could not locate SurvivalControlAspect..." << endl;
    return;
  }

  // we now have the control aspect

  shared_ptr<AgentAspect> agent = shared_static_cast<AgentAspect>(make_shared(GetSelf()));

  // request some effectors
  mForceEffector = shared_static_cast<ForceEffector>(control->RequestEffector(agent, "kerosin/ForceEffector"));
  
  // request some perceptors
  mVisionPerceptor    = shared_static_cast<VisionPerceptor>(control->RequestPerceptor(agent, "kerosin/VisionPerceptor"));
  mLineSegmentPerceptor  = shared_static_cast<LineSegmentPerceptor>(control->RequestPerceptor(agent, "LineSegmentPerceptor"));
}
//! [agent aspect]


