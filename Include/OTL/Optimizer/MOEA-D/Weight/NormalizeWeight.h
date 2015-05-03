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

#include <cstddef>
#include <vector>
#include <numeric>

namespace otl
{
namespace optimizer
{
namespace moea_d
{
namespace weight
{
template <typename _TReal>
void NormalizeWeight(std::vector<_TReal> &weight)
{
	const _TReal length = sqrt(std::inner_product(weight.begin(), weight.end(), weight.begin(), (_TReal)0));
	for(size_t i = 0; i < weight.size(); ++i)
		weight[i] = weight[i] / length;
}
}
}
}
}
