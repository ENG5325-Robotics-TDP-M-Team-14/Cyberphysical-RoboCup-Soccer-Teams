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
#ifndef OXYGEN_PERCEPTOR_H
#define OXYGEN_PERCEPTOR_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/gamecontrolserver/baseparser.h>

/**
 * @defgroup perceptors Perceptors
 * @brief Perceptors are used by agents to sense information from the environment.
 *
 * For more details about their implementation please refer to the @ref oxygen::Perceptor class.
 *
 * @todo Add a more detailled description of the Perceptors group...
 */

namespace oxygen
{

/**
 * @class Perceptor
 * @brief Base class for all perceptors.
 * @ingroup perceptors
 *
 * Perceptors are used by agents to sense information from the environment.
 * This class serves as a base class for all perceptors, providing a generic interface (via the @ref Percept() method) for collecting perceptor information in a @ref PredicateList.
 *
 * @todo Document relation of perceptors to scene graph.
 *
 * Depending on the actual perceptor, sensor information might be available more or less frequent.
 * To address this issue, each perceptor provides its own perception interval in which new sensor information is generated.
 * The @ref GetInterval() and @ref SetInterval() methods are used to access and change the interval cycle of a perceptor.
 */
class OXYGEN_API Perceptor : public oxygen::BaseNode
{
public:
    /** @brief Default Constructor. */
    Perceptor();

    /** @brief Default Destructor. */
    virtual ~Perceptor() {};

    /** @brief Trigger perception event.
     *
     * This is called by agents to trigger the percept event implemented by the perceptor.
     * The perceptor can return data through the @ref PredicateList which is passed as a parameter.
     *
     * @param[in,out] predList the predicate list for storing the newly created perception
     * @return @c true if valid data is available and @c false otherwise
     */
    virtual bool Percept(std::shared_ptr<PredicateList> predList) = 0;

    /** @brief Set / change the predicate name.
     *
     * (used for example for debugging purposes)
     *
     * @param[in] name the new name of the predicate for the perceptor
     */
    void SetPredicateName(const std::string& name);

    /** @brief Retrieve the interval cycle of the perceptor.
     *
     * @return the perceptor interval cycle
     * @see @ref SetInterval()
     */
    unsigned int GetInterval() const;

    /** @brief Set the interval cycle of the perceptor.
     *
     * @param[in] i the perceptor interval cycle
     * @see @ref GetInterval()
     */
    void SetInterval(unsigned int i);

protected:
    /** @brief The predicate name of the perceptor. */
    std::string mPredicateName;

    /** @brief The interval cycle of the perceptor. */
    unsigned int mInterval;
};

DECLARE_ABSTRACTCLASS(Perceptor)

} // namespace oxygen

#endif //OXYGEN_PERCEPTOR_H
