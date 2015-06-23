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
#include <random>

namespace otl
{
namespace optimizer
{
namespace nsga_iii
{
template <typename _TReal>
_TReal Distance(const std::vector<_TReal> &referencePoint, const std::vector<_TReal> &objective)
{
	const _TReal factor = std::inner_product(objective.begin(), objective.end(), referencePoint.begin(), (_TReal)0) / std::inner_product(objective.begin(), objective.end(), objective.begin(), (_TReal)0);
	_TReal sum = 0;
	for (size_t i = 0; i < objective.size(); ++i)
	{
		_TReal temp = factor * objective[i] - referencePoint[i];
		sum += temp * temp;
	}
	assert(sum >= 0);
	return sqrt(sum);
}

template <typename _TRandom, typename _TIterator>
_TIterator RandomSelect(_TRandom &random, _TIterator begin, _TIterator end)
{
	assert(begin != end);
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	_TIterator select = begin;
	for (size_t count = dist(random); count; --count)
		++select;
	return select;
}
}
}
}
