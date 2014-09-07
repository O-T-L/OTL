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

/*
@ARTICLE{,
  author = {Lamjed Ben Said and Slim Bechikh and Khaled Ghédira},
  title = {The r-Dominance: A New Dominance Relation for Interactive Evolutionary
	Multicriteria Decision Making},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2010},
  volume = {14},
  pages = {801-818},
  number = {5},
  month = {October},
  doi = {10.1109/TEVC.2010.2041060}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <numeric>
#include <OTL/Utility/Relation/Pareto.h>

namespace otl
{
namespace optimizer
{
namespace r_nsga_ii
{
template <typename _TReal, typename _TIterator>
_TReal CalculateDistRange(const std::vector<_TReal> &referencePoint, _TIterator begin, _TIterator end)
{
	_TReal minDist = sqrt(std::inner_product(referencePoint.begin(), referencePoint.end(), (**begin).objective_.begin(), (_TReal)0, std::plus<_TReal>()
		, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
	_TReal maxDist = minDist;
	for (_TIterator i = ++_TIterator(begin); i != end; ++i)
	{
		const _TReal dist = sqrt(std::inner_product(referencePoint.begin(), referencePoint.end(), (**i).objective_.begin(), (_TReal)0, std::plus<_TReal>()
			, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
		if (dist < minDist)
			minDist = dist;
		if (dist > maxDist)
			maxDist = dist;
	}
	return maxDist - minDist;
}

template <typename _TReal>
bool R_Dominance(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2, const std::vector<_TReal> &referencePoint, const _TReal threshold, const _TReal distRange)
{
	assert(point1.size() == point2.size());
	assert(point1.size() == referencePoint.size());
	assert(0 <= threshold && threshold <= 1);
	assert(distRange >= 0);
	if (otl::utility::relation::Dominate(point1, point2))
		return true;
	else if (otl::utility::relation::Dominate(point2, point1))
		return false;
	else
	{
		const _TReal dist1 = sqrt(std::inner_product(referencePoint.begin(), referencePoint.end(), point1.begin(), (_TReal)0, std::plus<_TReal>()
			, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
		const _TReal dist2 = sqrt(std::inner_product(referencePoint.begin(), referencePoint.end(), point2.begin(), (_TReal)0, std::plus<_TReal>()
			, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
		return dist1 - dist2 / distRange;
	}
}
}
}
}
