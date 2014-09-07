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

namespace otl
{
namespace problem
{
namespace dtlz
{
namespace shape
{
template <typename _TReal>
void ConvertConvex(std::vector<_TReal> &objective)
{
	assert(objective.size() > 1);
	for (size_t i = 0; i < objective.size() - 1; ++i)
		objective[i] = pow(objective[i], 4);
	objective.back() = pow(objective.back(), 2);
}
}
}
}
}
