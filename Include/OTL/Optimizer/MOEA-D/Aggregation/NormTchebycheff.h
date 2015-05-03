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
#include <vector>
#include <limits>

namespace otl
{
namespace optimizer
{
namespace moea_d
{
namespace aggregation
{
template <typename _TReal>
std::vector<_TReal> CalculateScale(const std::vector<std::vector<_TReal> > &referenceObjectiveSet)
{
	assert(!referenceObjectiveSet.empty());
	std::vector<_TReal> scale(referenceObjectiveSet.size());
	for(size_t i = 0; i < scale.size(); ++i)
	{
		const _TReal value = referenceObjectiveSet[0][i];
		_TReal min = value, max = value;
		for(size_t j = 1; j < scale.size(); ++j)
		{
			const _TReal value = referenceObjectiveSet[j][i];
			if(value > max)
				max = value;
			if(value < min)
				min = value;
		}
		scale[i] = max - min;
	}
	return scale;
}

template <typename _TReal>
_TReal NormTchebycheff(const std::vector<_TReal> &weight, const std::vector<_TReal> &direction, const std::vector<std::vector<_TReal> > &referenceObjectiveSet)
{
	assert(direction.size() == weight.size());
	assert(direction.size() == referenceObjectiveSet.size());
	assert(direction.size() == referenceObjectiveSet.size());
	const std::vector<_TReal> scale = CalculateScale(referenceObjectiveSet);
	double maxValue = std::numeric_limits<_TReal>::min();
	for(size_t i = 0; i < direction.size(); ++i)
	{
		if (scale[i] == 0)
			return std::numeric_limits<_TReal>::max();
		const _TReal diff = direction[i] / scale[i];
		const _TReal value = diff * weight[i];
		if(value > maxValue)
			maxValue = value;
	}
	return maxValue;
}
}
}
}
}
