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
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace problem
{
namespace wfg
{
namespace shape
{
template <typename _TIteratorAngle, typename _TIteratorObjective, typename _TReal>
void Convex(_TIteratorAngle angleBegin, _TIteratorAngle angleEnd, _TIteratorObjective objectiveBegin, _TIteratorObjective objectiveEnd, const _TReal radius)
{
	assert(std::distance(objectiveBegin, objectiveEnd) > 0);
	{
		_TReal temp = radius;
		for (_TIteratorAngle angle = angleBegin; angle != angleEnd; ++angle)
		{
			assert(0 <= *angle && *angle <= boost::math::constants::pi<_TReal>() / 2);
			temp *= radius - cos(*angle);
		}
		*objectiveBegin = temp;
	}
	for (_TIteratorObjective objective = ++_TIteratorObjective(objectiveBegin); objective != objectiveEnd; ++objective)
	{
		const size_t nObjective = objective - objectiveBegin;
		const size_t index = std::distance(angleBegin, angleEnd) - nObjective;
		_TIteratorAngle _angleEnd = angleBegin + index;
		_TReal temp = radius;
		for (_TIteratorAngle angle = angleBegin; angle != _angleEnd; ++angle)
			temp *= radius - cos(*angle);
		*objective = temp * (radius - sin(*_angleEnd));
	}
}
}
}
}
}
