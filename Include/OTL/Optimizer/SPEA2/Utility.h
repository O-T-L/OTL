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
#include <list>

namespace otl
{
namespace optimizer
{
namespace spea2
{
template <typename _TPointer>
std::list<_TPointer> SpliceElites(std::list<_TPointer> &front)
{
	std::list<_TPointer> elites;
	for (auto i = front.begin(); i != front.end();)
	{
		assert((**i).fitness_ >= 0);
		if ((**i).fitness_ < 1)
		{
			auto move = i;
			++i;
			elites.splice(elites.end(), front, move);
		}
		else
			++i;
	}
	return elites;
}

template <typename _TIterator>
void Indexing(_TIterator begin, _TIterator end)
{
	size_t index = 0;
	for (_TIterator individual = begin; individual != end; ++individual)
	{
		(**individual).index_ = index;
		++index;
	}
}
}
}
}
