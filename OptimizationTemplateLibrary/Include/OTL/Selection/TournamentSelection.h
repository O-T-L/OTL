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
#include <random>

namespace otl
{
namespace selection
{
template <typename _TRandom, typename _TSrcIterator, typename _TDestIterator, typename _TCompete>
_TDestIterator TournamentSelection(_TRandom &random, _TSrcIterator srcBegin, _TSrcIterator srcEnd, _TDestIterator destBegin, _TDestIterator destEnd, _TCompete compete, const size_t competitors = 2)
{
	typedef typename std::iterator_traits<_TSrcIterator>::value_type _TPointer;
	_TDestIterator dest = destBegin;
	for (_TSrcIterator src = srcEnd; dest != destEnd; ++dest)
	{
		std::vector<_TPointer> competition(competitors);
		for (size_t i = 0; i < competition.size(); ++i)
		{
			if (src == srcEnd)
			{
				std::random_shuffle(srcBegin, srcEnd, [&random](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(random);});
				src = srcBegin;
			}
			competition[i] = *src;
			++src;
		}
		*dest = compete(competition);
	}
	return dest;
}
}
}
