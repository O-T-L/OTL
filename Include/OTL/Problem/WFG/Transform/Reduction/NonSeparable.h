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
#include <numeric>
#include "../Utility.h"

namespace otl
{
namespace problem
{
namespace wfg
{
namespace transform
{
namespace reduction
{
template <typename _TReal, typename _TIterator>
_TReal NonSeparable(_TIterator begin, _TIterator end, const size_t degree)
{
	assert(std::distance(begin, end) > 0);
	assert(0 < degree && degree <= std::distance(begin, end));
	assert(std::distance(begin, end) % degree == 0);
	_TReal numerator = 0;
	for(_TIterator decision = begin; decision != end; ++decision)
	{
		const size_t nDecision = decision - begin;
		assert(0 <= *decision && *decision <= 1);
		numerator += *decision;
		for(size_t i = 0; i <= degree - 2; ++i)
		{
			const size_t index = (nDecision + i + 1) % std::distance(begin, end);
			_TIterator _decision = begin + index;
			numerator += std::fabs(*decision - *_decision);
		}
	}
	const _TReal tmp = ceil((_TReal)degree / 2);
	const _TReal denominator = std::distance(begin, end) * tmp * (1 + 2 * degree - 2 * tmp) / degree;
	return Fix<_TReal>(numerator / denominator, 0, 1);
}
}
}
}
}
}
