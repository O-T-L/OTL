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
#include <utility>

namespace otl
{
namespace optimizer
{
namespace ibea
{
template <typename _TReal, typename _TIterator, typename _TFetcher>
std::vector<std::pair<_TReal, _TReal> > CalculateBoundary(_TIterator begin, _TIterator end, _TFetcher fetcher)
{
	assert(begin != end);
	std::vector<std::pair<_TReal, _TReal> > boundary(fetcher(*begin).size());
	for (size_t axis = 0; axis < boundary.size(); ++axis)
	{
		std::pair<_TReal, _TReal> &range = boundary[axis];
		range.first = fetcher(*begin)[axis];
		range.second = range.first;
		for (_TIterator i = ++_TIterator(begin); i != end; ++i)
		{
			const _TReal coordinate = fetcher(*i)[axis];
			if (coordinate < range.first)
				range.first = coordinate;
			if (coordinate > range.second)
				range.second = coordinate;
		}
		range.second += 1;
	}
	return boundary;
}
}
}
}
