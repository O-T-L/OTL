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
@PHDTHESIS{,
  author = {Jason Ramon Schott},
  title = {Fault Tolerant Design Using Single and Multicriteria Genetic Algorithm
	Optimization},
  school = {Massachusetts Institute of Technology. Department of Aeronautics
	and Astronautics},
  year = {1995},
  university = {Massachusetts Institute of Technology}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <numeric>
#include <limits>
#include <OTL/Indicator/Indicator.h>

#undef max

namespace otl
{
namespace indicator
{
template <typename _TReal>
class Spacing : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	Spacing(void);
	~Spacing(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
	static std::vector<TReal> _CalculateDistance(const TFront &front);
	static TReal _CalculateSpacing(const std::vector<TReal> &distance);
};

template <typename _TReal>
Spacing<_TReal>::Spacing(void)
{
}

template <typename _TReal>
Spacing<_TReal>::~Spacing(void)
{
}

template <typename _TReal>
typename Spacing<_TReal>::TMetric Spacing<_TReal>::_DoEvaluate(const TFront &front)
{
	const std::vector<TReal> distanceList = _CalculateDistance(front);
	return _CalculateSpacing(distanceList);
}

template <typename _TReal>
std::vector<_TReal> Spacing<_TReal>::_CalculateDistance(const TFront &front)
{
	std::vector<TReal> distanceList(front.size());
	for (size_t i = 0; i < distanceList.size(); ++i)
		distanceList[i] = std::numeric_limits<TReal>::max();
	for (size_t i = 0; i < front.size(); ++i)
	{
		for (size_t j = 0; j < front.size(); ++j)
		{
			if (i != j)
			{
				const TPoint &point1 = *front[i];
				const TPoint &point2 = *front[j];
				assert(point1.size() == point2.size());
				const TReal newDistance = sqrt(std::inner_product(point1.begin(), point1.end(), point2.begin(), (TReal)0, std::plus<TReal>()
					, [](TReal x, TReal y)->TReal{TReal t = x - y;return t * t;}));
				if (newDistance < distanceList[i])
					distanceList[i] = newDistance;
				if (newDistance < distanceList[j])
					distanceList[j] = newDistance;
			}
		}
	}
	return distanceList;
}

template <typename _TReal>
_TReal Spacing<_TReal>::_CalculateSpacing(const std::vector<TReal> &distance)
{
	const TReal averageDistance = std::accumulate(distance.begin(), distance.end(), (TReal)0) / distance.size();
	TReal sum = 0;
	for (size_t i = 0; i < distance.size(); ++i)
	{
		const TReal temp = averageDistance - distance[i];
		sum += temp * temp;
	}
	return sqrt(sum / (distance.size() - 1));
}
}
}
