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
#include <utility>

namespace otl
{
namespace optimizer
{
namespace ibea
{
/*
 * calculates the hypervolume of that portion of the objective space that is dominated by individual a but not by individual b
 */
template <typename _TReal>
_TReal CalculateHypervolume(const size_t axis, const std::vector<std::pair<_TReal, _TReal> > &boundary, const std::vector<_TReal> &point1, const std::vector<_TReal> &point2)
{
	const std::pair<_TReal, _TReal> &minMax = boundary[axis];
	assert(minMax.first < minMax.second);
	const _TReal upper = minMax.second;
	const _TReal range = upper - boundary[axis].first;
	const _TReal p1 = point1[axis];
	const _TReal p2 = point2[axis];
	if (!axis)
	{
		if (p1 < p2)
			return (p2 - p1) / range;
		else
			return 0;
	}
	_TReal volume = 0;
	const size_t axisNext = axis - 1;
	if (p1 < p2)
	{
		std::vector<_TReal> _point2 = point2;
		_point2[axisNext] = boundary[axisNext].second;
		volume = CalculateHypervolume(axisNext, boundary, point1, _point2) * (p2 - p1) / range;
		volume += CalculateHypervolume(axisNext, boundary, point1, point2) * (upper - p2) / range;
	}
	else
		volume = CalculateHypervolume(axisNext, boundary, point1, point2) * (upper - p2) / range;
	return volume;
}
}
}
}

