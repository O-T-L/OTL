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
#include <algorithm>
#include <random>
#include <OTL/Utility/Fix/Truncate.h>

namespace otl
{
namespace crossover
{
namespace real
{
namespace sbx
{
template <typename _TReal>
_TReal CalculateSpreadFactorAttenuation(const _TReal distributionIndex, const _TReal spreadFactor)
{
	assert(distributionIndex >= 0);
	assert(spreadFactor >= 0);
	return 2 - pow(spreadFactor, -(distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateSpreadFactor(const _TReal distributionIndex, const _TReal spreadFactorAttenuation, const _TReal random01)
{
	assert(0 <= random01 && random01 < 1);
	if (random01 < 1. / spreadFactorAttenuation)
		return pow(random01 * spreadFactorAttenuation, 1. / (distributionIndex + 1));
	else
		return pow(1. / (2 - random01 * spreadFactorAttenuation), 1 / (distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateSpreadFactorProbability(const _TReal distributionIndex, const _TReal spreadFactor)
{
	assert(distributionIndex >= 0);
	assert(spreadFactor >= 0);
	if (spreadFactor < 1)
		return (distributionIndex + 1) * pow(spreadFactor, distributionIndex) / 2;
	else
		return (distributionIndex + 1) / pow(spreadFactor, distributionIndex + 2) / 2;
}

template <typename _TReal>
_TReal CalculateAmplificationFactor(const _TReal distributionIndex, const _TReal spreadFactor)
{
	return 2 / CalculateSpreadFactorAttenuation(distributionIndex, spreadFactor);
}

template <typename _TRandom, typename _TReal>
void BoundedCrossover(_TRandom &random, const _TReal distributionIndex, const _TReal parent1, const _TReal parent2, _TReal &child1, _TReal &child2, const _TReal lower, const _TReal upper)
{
	static std::uniform_real_distribution<_TReal> dist(0, 1);
	assert(lower < upper);
	const _TReal distance = std::fabs(parent1 - parent2);
	if (distance == 0)
	{
		child1 = parent1;
		child2 = parent2;
		return;
	}
	const _TReal spreadFactorLower = 1 + 2 * (std::min(parent1, parent2) - lower) / distance;
	const _TReal spreadFactorUpper = 1 + 2 * (upper - std::max(parent1, parent2)) / distance;
	assert(spreadFactorLower >= 0);
	assert(spreadFactorUpper >= 0);
	const _TReal spreadFactorAttenuationLower = CalculateSpreadFactorAttenuation(distributionIndex, spreadFactorLower);
	const _TReal spreadFactorAttenuationUpper = CalculateSpreadFactorAttenuation(distributionIndex, spreadFactorUpper);
	const _TReal random01 = dist(random);
	assert(0 <= random01 && random01 < 1);
	const _TReal spreadFactor1 = CalculateSpreadFactor(distributionIndex, spreadFactorAttenuationLower, random01);
	const _TReal spreadFactor2 = CalculateSpreadFactor(distributionIndex, spreadFactorAttenuationUpper, random01);
	const _TReal middle = (parent1 + parent2) / 2;
	const _TReal halfDistance = distance / 2;
	child1 = otl::utility::fix::Truncate(middle - spreadFactor1 * halfDistance, lower, upper);
	child2 = otl::utility::fix::Truncate(middle + spreadFactor2 * halfDistance, lower, upper);
}
}
}
}
}
