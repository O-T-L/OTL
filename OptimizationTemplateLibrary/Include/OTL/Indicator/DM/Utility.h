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
namespace dm
{
template <typename _TReal>
std::vector<std::pair<_TReal, _TReal> > ExpandHalfBox(const std::vector<std::pair<_TReal, _TReal> > &boundary, const std::vector<size_t> &division)
{
	assert(division.size() == boundary.size());
	std::vector<std::pair<_TReal, _TReal> > expandedBoundary(boundary.size());
	for (size_t i = 0; i < boundary.size(); ++i)
	{
		assert(division[i] > 0);
		assert(boundary[i].first <= boundary[i].second);
		const _TReal move = (boundary[i].second - boundary[i].first) / division[i] / 2;
		expandedBoundary[i].first = boundary[i].first - move;
		expandedBoundary[i].second = boundary[i].second + move;
	}
	return expandedBoundary;
}
}
}
}
