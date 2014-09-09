#pragma once

#include <cassert>
#include <list>
#include <utility>
#include <limits>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <OTL/Optimizer/SPEA2/Trim.h>

namespace otl
{
namespace optimizer
{
namespace spea2_sde
{
template <typename _TIterator, typename _TReal>
std::pair<_TIterator, _TIterator> LocateMinEdge(_TIterator begin, _TIterator end, const boost::numeric::ublas::matrix<_TReal> &adjacencyMatrix)
{
	_TReal minDistance = std::numeric_limits<_TReal>::max();
	std::pair<_TIterator, _TIterator> minEdge(end, end);
	for (_TIterator individual1 = begin; individual1 != end; ++individual1)
	{
		const size_t index1 = (**individual1).index_;
		for (_TIterator individual2 = begin; individual2 != end; ++individual2)
		{
			if (individual1 != individual2)
			{
				const size_t index2 = (**individual2).index_;
				const _TReal distance1 = adjacencyMatrix(index1, index2);
				const _TReal distance2 = adjacencyMatrix(index2, index1);
				if (distance1 < distance2)
				{
					if (distance1 < minDistance)
					{
						minDistance = distance1;
						minEdge.first = individual1;
						minEdge.second = individual2;
					}
				}
				else
				{
					if (distance2 < minDistance)
					{
						minDistance = distance2;
						minEdge.first = individual2;
						minEdge.second = individual1;
					}
				}
			}
		}
	}
	assert(adjacencyMatrix((**minEdge.first).index_, (**minEdge.second).index_) <= adjacencyMatrix((**minEdge.second).index_, (**minEdge.first).index_));
	return minEdge;
}

template <typename _TReal, typename _TPointer>
void Trim(std::list<_TPointer> &population, const boost::numeric::ublas::matrix<_TReal> &adjacencyMatrix)
{
	auto minEdge = LocateMinEdge(population.begin(), population.end(), adjacencyMatrix);
	population.erase(minEdge.first);
}
}
}
}
