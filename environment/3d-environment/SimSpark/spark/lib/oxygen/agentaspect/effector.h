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
#ifndef OXYGEN_EFFECTOR_H
#define OXYGEN_EFFECTOR_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/gamecontrolserver/baseparser.h>

/**
 * @defgroup effectors Effectors
 * @brief Effectors are used by agents to manipulate the environment.
 *
 * For more details about their implementation please refer to the @ref oxygen::Effector class.
 *
 * @todo Add a more detailled description of the Effectors group...
 */

namespace oxygen
{
class ActionObject;
class AgentAspect;

/**
 * @class Effector
 * @brief Base class for all effectors.
 * @ingroup effectors
 *
 * Effectors are used by agents to manipulate the environemnt.
 * This class serves as a base class for all effectors, providing a generic interface (via the @ref GetActionObject() and @ref Realize() methods) for performing action commands received from agents.
 * The action command from an agent is proivided as @ref Predicate.
 * The @ref GetActionObject() method is used to check and convert the @ref Predicate into an effector specific @ref ActionObject.
 * This @ref ActionObject is then forwarded to the @ref Realize() method and buffered in @ref #mAction for later application.
 * The actual application / realization of the action in the next simulation cycle then typically happens in the @ref PrePhysicsUpdateInternal() method, which uses the buffered action to manipulate the state of the physical simulation accordingly.
 *
 * @todo Document relation of effectors to scene graph.
 *
 * Effectors in general can be enabled or disabled.
 * Only the actions of enabled effectors are actually applied in the next simulation cycle.
 * If an effector is disabled, it has no effect on the next simulation cycle.
 * The @ref Enable() and @ref Disable() methods are used to control the activation state of effectors.
 */
class OXYGEN_API Effector : public BaseNode
{
public:
    /** @brief Default constructor. */
    Effector() : BaseNode()
    {
        disabled = false;
    };

    /** @brief Default destructor. */
    virtual ~Effector() {};

    /** @brief Save the ActionObject for application in the next simulation cycle.
     *
     * @param[in,out] action the action to realize in the next simulation cycle
     * @return @c true if a realization of the provided action is supported by this effector, @c false otherwise
     */
    virtual bool Realize(std::shared_ptr<ActionObject> action);

    /** @brief Retrieve the name of the predicate this effector implements.
     *
     * @return the effector predicate name
     */
    virtual std::string GetPredicate() = 0;

    /** @brief Construct an @ref ActionObject, describing a predicate.
     *
     * @param[in] predicate the effector predicate received from the agent
     * @return An @ref ActionObject describing the action encoded in the predicate
     */
    virtual std::shared_ptr<ActionObject>
    GetActionObject(const Predicate& predicate) = 0;

    /** @brief Enable this effector.
     *
     * Only the actions of enabled effectors are actually applied in the next simulation cycle.
     *
     * @see @ref Disable()
     */
    void Enable();

    /** @brief Disable this effector.
     *
     * Use this method to prevent the application of the current @ref ActionObject in the next simulation cycle.
     *
     * @see @ref Enable()
     */
    void Disable();

protected:
    /** @brief Retrieve the @ref AgentAspect (agent) this effector belongs to.
     *
     * @return the related @ref AgentAspect
     */
    std::shared_ptr<AgentAspect> GetAgentAspect();

    /** @brief The current @ref ActionObject.
     *
     * The current action to an effector is cached here, and then applied / realized in PrePhysicsUpdateInternal().
     */
    std::shared_ptr<ActionObject> mAction;

    /** @brief Flag for enabling/disabling the effector.
     *
     * @c true if effector is disabled, @c false for enabled.
     *
     * Only the actions of enabled effectors are actually applied in the next simulation cycle.
     * (Currently used for disabling hinge joints.)
     *
     * @see @ref Enable()
     * @see @ref Disable()
     */
    bool disabled;
};

DECLARE_ABSTRACTCLASS(Effector)

} // namespace oxygen

#endif //OXYGEN_EFFECTOR_H
