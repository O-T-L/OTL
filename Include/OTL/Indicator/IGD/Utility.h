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
#include <cmath>
#include <limits>
#include <numeric>

namespace otl
{
namespace indicator
{
namespace igd
{
template <typename _TReal, typename _TIterator>
_TReal Distance2Population(const std::vector<_TReal> &objective, _TIterator begin, _TIterator end)
{
	_TReal minDistance = std::numeric_limits<_TReal>::max();
	for (_TIterator i = begin; i != end; ++i)
	{
		auto &point = **i;
		assert(point.size() == objective.size());
		const _TReal distance = sqrt(std::inner_product(objective.begin(), objective.end(), point.begin(), (_TReal)0, std::plus<_TReal>()
			, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
		if (distance < minDistance)
			minDistance = distance;
	}
	return minDistance;
}
}
}
}
