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

namespace otl
{
namespace optimizer
{
namespace spea2_sde
{
template <typename _TReal>
_TReal SDE(const std::vector<_TReal> &objective1, const std::vector<_TReal> &objective2)
{
	assert(objective1.size() == objective2.size());
	_TReal distance = 0;
	for (size_t i = 0; i < objective1.size(); ++i)
	{
		if (objective1[i] < objective2[i])
		{
			const _TReal temp = objective2[i] - objective1[i];
			distance += temp * temp;
		}
	}
	return sqrt(distance);
}
}
}
}
