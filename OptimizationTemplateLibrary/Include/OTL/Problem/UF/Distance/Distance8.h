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
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace problem
{
namespace uf
{
namespace distance
{
template <typename _TReal>
_TReal Distance8(const std::vector<_TReal> decision, const size_t index)
{
	assert(2 <= index && index < decision.size());
	const size_t nDistDecisions = decision.size() - index;
	const _TReal x = decision[0];
	const _TReal y = decision[1];
	_TReal sum = 0;
	for (size_t i = index; i < decision.size(); i += 3)
	{
		const size_t j = i + 1;
		const _TReal temp = decision[i] - 2 * y * sin(2 * boost::math::constants::pi<_TReal>() * x + j * boost::math::constants::pi<_TReal>() / decision.size());
		sum += temp * temp;
	}
	const size_t count = nDistDecisions % 3 ? nDistDecisions / 3 + 1 : nDistDecisions / 3;
	return sum / count;
}
}
}
}
}
