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
#include <iterator>
#include <numeric>
#include <limits>
#include <boost/numeric/ublas/vector.hpp>

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TReal>
boost::numeric::ublas::vector<_TReal> ComputeDirection(const std::vector<_TReal> &objective, const std::vector<_TReal> &min)
{
	assert(objective.size() == min.size());
	boost::numeric::ublas::vector<_TReal> direction(objective.size());
	for (size_t i = 0; i < direction.size(); ++i)
	{
		direction[i] = objective[i] - min[i];
		assert(direction[i] >= 0);
	}
	return direction;
}

template <typename _TReal>
_TReal ComputeAngleRatio(const boost::numeric::ublas::vector<_TReal> &direction, const _TReal biasFactor)
{
	const _TReal len1 = sqrt((_TReal)direction.size());
	const _TReal len = sqrt(std::inner_product(direction.begin(), direction.end(), direction.begin(), (_TReal)0));
	const _TReal maxAngle = acos(1 / len1);
	_TReal minAngle = maxAngle;
	for (size_t i = 0; i < direction.size(); ++i)
	{
		const _TReal angle = acos(direction[i] / len);
		if (angle < minAngle)
			minAngle = angle;
	}
	assert(0 <= minAngle && minAngle <= maxAngle);
	assert(biasFactor >= 1);
	return pow(1 - minAngle / maxAngle, biasFactor);
}
}
}
}
