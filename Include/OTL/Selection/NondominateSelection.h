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

#include <list>
#include <cassert>
#include <OTL/Utility/Nondominate.h>

namespace otl
{
namespace selection
{
template <typename _TIndividual, typename _TIterator, typename _TDominate, typename _TSelectNoncritical, typename _TSelectCritical>
_TIterator NondominateSelection(std::list<_TIndividual> &population, _TIterator begin, _TIterator end, _TDominate dominate, _TSelectNoncritical selectNoncritical, _TSelectCritical selectCritical)
{
	assert(population.size() >= std::distance(begin, end));
	_TIterator dest = begin;
	while (!population.empty())
	{
		std::list<_TIndividual> nondominate = otl::utility::ExtractNondominate(population, dominate);
		assert(!nondominate.empty());
		if (std::distance(dest, end) > nondominate.size())
			dest = selectNoncritical(nondominate, dest, end);
		else
		{
			dest = selectCritical(nondominate, dest, end);
			assert(dest == end);
			return dest;
		}
	}
	return dest;
}
}
}
