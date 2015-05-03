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
#include <numeric>

namespace otl
{
namespace problem
{
namespace dtlz
{
namespace shape
{
template <typename _TIteratorDecision, typename _TIteratorObjective, typename _TReal>
void Linear(_TIteratorDecision decisionBegin, _TIteratorDecision decisionEnd, _TIteratorObjective objectiveBegin, _TIteratorObjective objectiveEnd, const _TReal distance)
{
	assert(0 < std::distance(decisionBegin, decisionEnd) && std::distance(decisionBegin, decisionEnd) < std::distance(objectiveBegin, objectiveEnd));
	*objectiveBegin = std::accumulate(decisionBegin, decisionEnd, distance, std::multiplies<_TReal>());
	for (_TIteratorObjective objective = ++_TIteratorObjective(objectiveBegin); objective != objectiveEnd; ++objective)
	{
		const size_t nObjective = objective - objectiveBegin;
		const size_t index = std::distance(decisionBegin, decisionEnd) - nObjective;
		_TIteratorDecision _decisionEnd = decisionBegin + index;
		*objective = std::accumulate(decisionBegin, _decisionEnd, distance, std::multiplies<_TReal>()) * (1 - *_decisionEnd);
	}
}
}
}
}
}
