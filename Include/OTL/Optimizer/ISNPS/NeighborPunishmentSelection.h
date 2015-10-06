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
#include <algorithm>

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TPointer, typename _TShouldPunish>
void NeighborPunishment(_TPointer elite, std::list<_TPointer> &punished, std::list<_TPointer> &population, _TShouldPunish shouldPunish)
{
	for (auto i = population.begin(); i != population.end();)
	{
		if (shouldPunish(elite, *i))
		{
			auto punish = i;
			++i;
			punished.splice(punished.end(), population, punish);
		}
		else
			++i;
	}
}

template <typename _TPointer, typename _TIterator, typename _TConvergenceCompator, typename _TShouldPunish>
_TIterator NeighborPunishmentSelection(std::list<_TPointer> &population, _TIterator begin, _TIterator end, _TConvergenceCompator convergenceCompator, _TShouldPunish shouldPunish)
{
	assert(population.size() >= std::distance(begin, end));
	std::list<_TPointer> punished;
	_TIterator dest = begin;
	while (dest != end && !population.empty())
	{
		auto elite = std::min_element(population.begin(), population.end(), convergenceCompator);
		*dest = **elite;
		population.erase(elite);
		NeighborPunishment(&*dest, punished, population, shouldPunish);
		++dest;
	}
	population.splice(population.end(), punished, punished.begin(), punished.end());
	return dest;
}
}
}
}
