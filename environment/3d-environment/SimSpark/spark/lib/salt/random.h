/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef SALT_RANDOM_H
#define SALT_RANDOM_H

#include <random>
#include <type_traits>
#include "salt_defines.h"

namespace salt
{

/** A wrapper to a random number generator from the standard library.
 *
 * This class was created so that the underlying generator can be exchanged
 * easily. Making this class a singleton prevents initializing the generator
 * more than once (which can be expensive).
 *
 * The only thing users should do with this class is to set the seed of the
 * generator. To actually create random numbers, use one of the Random
 * Number Generator Classes that map the numbers to a specific distribution
 * and provide an operator()() to access random numbers.
 */
class RandomEngine : public std::mt19937
{
public:
    typedef std::mt19937::result_type result_type;

    static RandomEngine& instance()
    { static RandomEngine the_instance; return the_instance; }
private:
    RandomEngine() : std::mt19937() {}
    RandomEngine(const RandomEngine&) = delete;
};

/** This random number generator should be used to produce
 *  uniformly distributed random numbers.
 */
template<class RealType = double>
class UniformRNG
{
private:
    using DistributionType = std::conditional_t<std::is_integral_v<RealType>, std::uniform_int_distribution<RealType>, std::uniform_real_distribution<RealType>>;

public:
    UniformRNG(RealType min = RealType(0), RealType max = RealType(1))
        : distribution(min, max)
    {}

    RealType operator()()
    {
        return distribution(RandomEngine::instance());
    }

private:
    DistributionType distribution;
};

/** A random number generator producing normally distributed numbers.
 */
template<class RealType = double>
class NormalRNG : public std::normal_distribution<RealType>
{
public:
    NormalRNG(double mean, double sigma = (1))
        : std::normal_distribution<RealType>
    (std::normal_distribution<RealType>(mean, sigma))
    {}

    RealType operator()()
    {
        return std::normal_distribution<RealType>::operator()(RandomEngine::instance());
    }
};

/** A random number generator with an exponential distribution.
 *
 * exponential distribution: p(x) = lambda * exp(-lambda * x)
 */
template<class RealType = double>
class ExponentialRNG : public std::exponential_distribution<RealType>
{
public:
    ExponentialRNG(double lambda = RealType(1))
        : std::exponential_distribution<RealType>
    (std::exponential_distribution<RealType>(lambda))
    {}

    RealType operator()()
    {
        return std::exponential_distribution<RealType>::operator()(RandomEngine::instance());
    }
};

} // namespace salt

#endif // SALT_RANDOM_H
