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
#include <list>

namespace otl
{
namespace optimizer
{
namespace hype
{
template <typename _TReal, typename _TIterator>
std::vector<_TReal> FindLower(_TIterator begin, _TIterator end)
{
	assert(begin != end);
	std::vector<_TReal> lower((**begin).objective_.size());
	for (size_t obj = 0; obj < lower.size(); ++obj)
	{
		lower[obj] = (**begin).objective_[obj];
		for (_TIterator i = ++_TIterator(begin); i != end; ++i)
		{
			assert((**i).objective_.size() == lower.size());
			const _TReal coordinate = (**i).objective_[obj];
			if (coordinate < lower[obj])
				lower[obj] = coordinate;
		}
		lower[obj] -= 1;
	}
	return lower;
}

template <typename _TReal, typename _TIterator>
std::vector<_TReal> FindUpper(_TIterator begin, _TIterator end)
{
	assert(begin != end);
	std::vector<_TReal> upper((**begin).objective_.size());
	for (size_t obj = 0; obj < upper.size(); ++obj)
	{
		upper[obj] = (**begin).objective_[obj];
		for (_TIterator i = ++_TIterator(begin); i != end; ++i)
		{
			assert((**i).objective_.size() == upper.size());
			const _TReal coordinate = (**i).objective_[obj];
			if (upper[obj] < coordinate)
				upper[obj] = coordinate;
		}
		upper[obj] += 1;
	}
	return upper;
}
}
}
}
