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
namespace nsga_iii
{
template <typename _TReal>
_TReal Distance(const std::vector<_TReal> &referencePoint, const std::vector<_TReal> &individual)
{
	assert(referencePoint.size() == individual.size());
	_TReal sum = 0;
	for (size_t i = 0; i < referencePoint.size(); ++i)
		sum += individual[i] / referencePoint[i];
	sum = std::fabs(sum - 1);
	_TReal len = 0;
	for (size_t i = 0; i < referencePoint.size(); ++i)
		len += 1 / (referencePoint[i] * referencePoint[i]);
	len = sqrt(len);
	return sum / len;
}

template <typename _TReal>
std::pair<size_t, _TReal> NearestReferencePoint(const std::vector<_TReal> &individual, std::vector<std::vector<_TReal> > &referenceSet)
{
	size_t nearestIndex = 0;
	_TReal minDistance = Distance(referenceSet[nearestIndex], individual);
	for (size_t i = 1; i < referenceSet.size(); ++i)
	{
		const _TReal distance = Distance(referenceSet[i], individual);
		if (distance < minDistance)
		{
			nearestIndex = i;
			minDistance = distance;
		}
	}
	return std::make_pair(nearestIndex, minDistance);
}

template <typename _TIterator, typename _TReal>
std::vector<std::list<std::pair<_TIterator, _TReal> > > Associate(_TIterator begin, _TIterator end, std::vector<std::vector<_TReal> > &referenceSet)
{
	std::vector<std::list<std::pair<_TIterator, _TReal> > > association(referenceSet.size());
	for (_TIterator i = begin; i != end; ++i)
	{
		const std::pair<size_t, _TReal> nearest = NearestReferencePoint((**i).objective_, referenceSet);
		association[nearest.first].push_back(std::make_pair(i, nearest.second));
	}
	return association;
}
}
}
}
