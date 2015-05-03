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
#include <algorithm>
#include <iterator>

namespace otl
{
namespace optimizer
{
namespace hype
{
template <typename _TReal, typename _TIterator>
std::vector<_TReal> FindLower(_TIterator begin, _TIterator end)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	assert(begin != end);
	std::vector<_TReal> lower((**begin).objective_.size());
	for (size_t i = 0; i < lower.size(); ++i)
		lower[i] = (**std::min_element(begin, end, [i](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[i] < individual2->objective_[i];})).objective_[i];
	return lower;
}

template <typename _TReal, typename _TIterator>
std::vector<_TReal> FindUpper(_TIterator begin, _TIterator end)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	assert(begin != end);
	std::vector<_TReal> upper((**begin).objective_.size());
	for (size_t i = 0; i < upper.size(); ++i)
		upper[i] = (**std::min_element(begin, end, [i](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[i] > individual2->objective_[i];})).objective_[i];
	return upper;
}

template <typename _TReal>
std::vector<_TReal> CalculateReferencePoint(const std::vector<_TReal> &lower, const std::vector<_TReal> &upper, const _TReal expand)
{
	assert(lower.size() == upper.size());
	std::vector<_TReal> referencePoint(lower.size());
	for (size_t i = 0; i < referencePoint.size(); ++i)
	{
		assert(lower[i] <= upper[i]);
		referencePoint[i] = upper[i] + (upper[i] - lower[i]) * expand;
	}
	return referencePoint;
}
}
}
}
