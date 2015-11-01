/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cassert>
#include <cmath>
#include <random>
#include <OTL/Utility/Fix/Truncate.h>

namespace otl
{
namespace mutation
{
namespace real
{
namespace pm
{
template <typename _TReal>
_TReal CalculatePerturbanceFactor(const _TReal distributionIndex, const _TReal perturbanceFactorLower, const _TReal perturbanceFactorUpper, const _TReal random01)
{
	assert(-1 <= perturbanceFactorLower && perturbanceFactorLower <= 0);
	assert(0 <= perturbanceFactorUpper && perturbanceFactorUpper <= 1);
	if (random01 < 0.5)
	{
		const _TReal temp = pow(1 + perturbanceFactorLower, distributionIndex + 1);
		const _TReal perturbanceFactor = pow(2 * random01 + (1 - 2 * random01) * temp, 1 / (distributionIndex + 1)) - 1;
		assert(perturbanceFactor <= 0);
		return perturbanceFactor;
	}
	else
	{
		const _TReal temp = pow(1 - perturbanceFactorUpper, distributionIndex + 1);
		const _TReal perturbanceFactor = 1 - pow(2 * (1 - random01) + (2 * random01 - 1) * temp, 1 / (distributionIndex + 1));
		assert(0 <= perturbanceFactor);
		return perturbanceFactor;
	}
}

template <typename _TReal>
_TReal CalculatePerturbanceFactorProbability(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	return (distributionIndex + 1) * pow(1 - std::fabs(perturbanceFactor), distributionIndex) / 2;
}

template <typename _TReal>
_TReal CalculateAmplificationLower(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	assert(distributionIndex >= 0);
	assert(-1 <= perturbanceFactor && perturbanceFactor <= 0);
	return 2 / (1 - pow(1 + perturbanceFactor, distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateAmplificationUpper(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	assert(distributionIndex >= 0);
	assert(0 <= perturbanceFactor && perturbanceFactor <= 1);
	return 2 / (1 - pow(1 - perturbanceFactor, distributionIndex + 1));
}

template <typename _TRandom, typename _TReal>
_TReal BoundedMutate(_TRandom &random, const _TReal distributionIndex, const _TReal coding, const _TReal lower, const _TReal upper)
{
	static std::uniform_real_distribution<_TReal> dist(0, 1);
	assert(lower < upper);
	const _TReal maxDistance = upper - lower;
	assert(maxDistance > 0);
	const _TReal perturbanceFactorLower = (lower - coding) / maxDistance;
	const _TReal perturbanceFactorUpper = (upper - coding) / maxDistance;
	const _TReal random01 = dist(random);
	assert(0 <= random01 && random01 < 1);
	const _TReal perturbanceFactor = CalculatePerturbanceFactor(distributionIndex, perturbanceFactorLower, perturbanceFactorUpper, random01);
	return otl::utility::fix::Truncate(coding + perturbanceFactor * maxDistance, lower, upper);
}
}
}
}
}
