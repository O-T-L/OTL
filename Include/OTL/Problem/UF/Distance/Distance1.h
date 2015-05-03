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
_TReal Distance1(const std::vector<_TReal> decision, const size_t index)
{
	assert(1 <= index && index < decision.size());
	const size_t nDistDecisions = decision.size() - index;
	const _TReal x = decision[0];
	_TReal sum = 0;
	for (size_t i = index; i < decision.size(); i += 2)
	{
		const size_t j = i + 1;
		const _TReal temp = decision[i] - sin(6 * boost::math::constants::pi<_TReal>() * x + j * boost::math::constants::pi<_TReal>() / decision.size());
		sum += temp * temp;
	}
	const size_t count = nDistDecisions % 2 ? nDistDecisions / 2 + 1 : nDistDecisions / 2;
	return sum / count;
}
}
}
}
}
