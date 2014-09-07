#pragma once

#include <cassert>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>

namespace otl
{
namespace optimizer
{
namespace spea2
{
template <typename _TReal, typename _TIterator, typename _TDistance>
_TReal KthDistance(const size_t k, _TIterator point, _TIterator begin, _TIterator end, _TDistance distance)
{
	assert(std::distance(begin, end) > 0);
	std::vector<_TReal> distanceList(std::distance(begin, end) - 1);
	assert(k < distanceList.size());
	size_t index = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		if (i != point)
		{
			distanceList[index] = distance(*point, *i);
			++index;
		}
	}
	assert(distanceList.size() == index);
	std::partial_sort(distanceList.begin(), distanceList.begin() + k, distanceList.end());
	return distanceList[k - 1];
}

template <typename _TReal, typename _TIterator, typename _TDistance>
_TReal Density(_TIterator point, _TIterator begin, _TIterator end, _TDistance distance)
{
	const size_t k = sqrt((_TReal)std::distance(begin, end));
	const _TReal kthDist = KthDistance<_TReal>(k, point, begin, end, distance);
	return 1 / (kthDist + 2);
}
}
}
}
