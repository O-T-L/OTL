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
#include <numeric>

namespace otl
{
namespace utility
{
namespace aggregation
{
template <typename _TReal>
_TReal PBI(const std::vector<_TReal> &weight, const std::vector<_TReal> &direction, const _TReal penalty)
{
	assert(direction.size() == weight.size());
	const _TReal aggregation = fabs(std::inner_product(direction.begin(), direction.end(), weight.begin(), (_TReal)0));

	std::vector<_TReal> line(direction.size());
	for(size_t i = 0; i < line.size(); ++i)
		line[i] = (direction[i] - aggregation * weight[i]);
	const _TReal distance = sqrt(std::inner_product(line.begin(), line.end(), line.begin(), (_TReal)0));
	return aggregation + penalty * distance;
}
}
}
}
