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
#include <limits>
#include <algorithm>
#include <iterator>

#undef max

namespace otl
{
namespace optimizer
{
namespace nsga_ii
{
template <typename _TIterator, typename _TReal>
void CrowdingDistanceAssignment(_TIterator begin, _TIterator end, const _TReal extremeDistance)
{
	assert(extremeDistance >= 0);
	assert(begin != end);

	for (_TIterator i = begin; i != end; ++i)
		(**i).crowdingDistance_ = 0;
	const size_t nObjectives = (**begin).objective_.size();
	for (size_t objective = 0; objective < nObjectives; ++objective)
	{
		typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
		std::sort(begin, end
			, [objective](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[objective] < individual2->objective_[objective];}
		);
		_TIterator last = end - 1;
		const _TReal range = (**last).objective_[objective] - (**begin).objective_[objective];
		if (range > 0)
		{
			for (_TIterator i = begin + 1; i != last; ++i)
			{
				assert((**(i - 1)).objective_[objective] <= (**i).objective_[objective] && (**i).objective_[objective] <= (**(i + 1)).objective_[objective]);
				(**i).crowdingDistance_ += ((**(i + 1)).objective_[objective] - (**(i - 1)).objective_[objective]) / range;
			}
			(**begin).crowdingDistance_ = extremeDistance;
			(**last).crowdingDistance_ = extremeDistance;
		}
		else
		{
			for (_TIterator i = begin; i != end; ++i)
				(**i).crowdingDistance_ = extremeDistance;
			return;
		}
	}
}

template <typename _TReal, typename _TIterator>
void CrowdingDistanceAssignment(_TIterator begin, _TIterator end)
{
	CrowdingDistanceAssignment(begin, end, std::numeric_limits<_TReal>::max());
}
}
}
}
